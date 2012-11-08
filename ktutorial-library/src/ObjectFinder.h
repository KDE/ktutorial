/***************************************************************************
 *   Copyright (C) 2012 by Daniel Calviño Sánchez <danxuliu@gmail.com>     *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; If not, see <http://www.gnu.org/licenses/>.  *
 ***************************************************************************/

#ifndef KTUTORIAL_OBJECTFINDER_H
#define KTUTORIAL_OBJECTFINDER_H

#include <QtCore/QObject>
#include <QtCore/QRegExp>

#include "ktutorial_export.h"

namespace ktutorial {

/**
 * Helper class to find objects.
 * This class is not intended to be used directly. Instead, use
 * KTutorial::findObject(const QString&).
 */
class KTUTORIAL_EXPORT ObjectFinder: public QObject {
Q_OBJECT
public:
    
    /**
     * Creates a new ObjectFinder with the given parent.
     * 
     * @param parent The parent object.
     */
    ObjectFinder(QObject* parent = 0);

    /**
     * Returns the object with the specified name, if any.
     * Objects are searched in the children of the given base object.
     * 
     * For details please refer to KTutorial::findObject(const QString&)
     * documentation.
     *
     * @param name The name of the object to find.
     * @param baseObject The base object to search from.
     * @return The object with the specified name, or null if there is none.
     * @see KTutorial::findObject(const QString&)
     */
    template <typename T>
    T findObject(const QString& name, const QObject* baseObject) const {
        QList<T> candidateObjects;
        findObjects<T>(name, baseObject, candidateObjects);
        
        if (candidateObjects.isEmpty()) {
            return 0;
        }
        
        if (candidateObjects.count() == 1) {
            return candidateObjects.first();
        }
        
        return getBestMatch(name, candidateObjects);
    }

private:

    /**
     * Adds to the foundObjects list the objects with the specified name that
     * are descendant of the given ancestor, if any.
     * The name of the objects can contain ancestor names separated by "/". Note
     * that ancestors are not necessarily direct parents.
     *
     * @param name The name of the objects to find.
     * @param ancestor The ancestor to look the objects in.
     * @param foundObjects The list to add to the objects with the specified
     *        name to.
     */
    template <typename T>
    void findObjects(const QString& name, const QObject* ancestor,
                     QList<T>& foundObjects) const {
        if (name.isEmpty() || ancestor == 0) {
            return;
        }

        if (name.indexOf('/') == -1) {
            foundObjects.append(ancestor->findChildren<T>(name));
            return;
        }

        QRegExp slashPattern("/+");
        QString ancestorName = name.left(name.indexOf(slashPattern));
        QString descendantName = name.mid(ancestorName.length() +
                                          slashPattern.matchedLength());

        QList<QObject*> namedAncestors =
                                ancestor->findChildren<QObject*>(ancestorName);
        foreach (QObject* namedAncestor, namedAncestors) {
            findObjects<T>(descendantName, namedAncestor, foundObjects);
        }
    }

    /**
     * Resolves the ambiguity between several objects that match the given name.
     * The ambiguity resolving rules are those specified in
     * findObject(const QString&).
     * 
     * @param name The name of the object to find.
     * @param candidateObjects A list with objects that match the given name.
     * @return The object that matches the best the given name.
     */
    template <typename T>
    T getBestMatch(const QString& name, QList<T> candidateObjects) const {
        QList< QList<QObject*> > objectPaths = getObjectPaths(candidateObjects);
        
        QList<QObject*> bestMatches = getBestMatches(name, objectPaths);
        
        //Should not happen, but just in case
        if (bestMatches.isEmpty()) {
            return 0;
        }
        
        return static_cast<T>(bestMatches[0]);
    }

    /**
     * Returns a list with the paths to the given objects.
     * Each path is a list that contains the object and all its ancestors. The
     * first object in the list is the more distant ancestor, and the last
     * object is the object itself.
     * 
     * @param objects The objects to get their paths.
     * @return A list with the paths to the given objects.
     */
    template <typename T>
    QList< QList<QObject*> > getObjectPaths(const QList<T> objects) const {
        QList< QList<QObject*> > objectPaths;

        foreach (T candidateObject, objects) {
            QList<QObject*> objectPath;

            QObject* ancestor = candidateObject;
            while (ancestor) {
                objectPath.prepend(ancestor);
                ancestor = ancestor->parent();
            }

            objectPaths.append(objectPath);
        }

        return objectPaths;
    }

    /**
     * Gets the objects from the given object paths that match the best the
     * given name.
     * The name can contain ancestor names. The ambiguity resolving rules are
     * applied recursively for each component of the name, so the object paths
     * used to find each component are the ones filtered with the name of its
     * ancestor.
     * 
     * @param name The name of the object to get.
     * @param objectPaths The paths to get the object from.
     * @return The list of objects that match the best the given name.
     */
    QList<QObject*> getBestMatches(const QString& name,
                           QList< QList<QObject*> > objectPaths) const;

    /**
     * Returns the object paths that contain a descendant of the base object
     * with the given name.
     * If direct children are found, their path is used. If not, if descendants
     * without named objects between them and the base object are found, their
     * path is used. If not, the path of the shallower descendants is used.
     * The name must be a single object name, without any ancestor name.
     * The returned paths are trimmed to make the object with the given name the
     * new base object of the path.
     * 
     * @param name The name of the descendant to find.
     * @param objectPaths The paths to search the object in.
     * @return The filtered and trimmed object paths.
     */
    QList< QList<QObject*> > filterObjectPaths(const QString& name,
                            const QList< QList<QObject*> >& objectPaths) const;

    /**
     * Returns the object paths that contain a direct child from the base object
     * with the given name.
     * The name must be a single object name, without any ancestor name.
     * The returned paths are trimmed to make the object with the given name the
     * new base object of the path.
     * 
     * @param name The name of the direct child to find.
     * @param objectPaths The paths to search the object in.
     * @return The filtered and trimmed object paths.
     */
    QList< QList<QObject*> > filterDirectChildren(const QString& name,
                            const QList< QList<QObject*> >& objectPaths) const;

    /**
     * Returns the object paths that contain a descendant from the base object
     * with the given name.
     * All the objects between the base object and the descendant with the given
     * name must have no name.
     * If there is more than one descendant with the given name, only the
     * shallower ones are taken into account.
     * The name must be a single object name, without any ancestor name.
     * The returned paths are trimmed to make the object with the given name the
     * new base object of the path.
     * 
     * @param name The name of the descendant to find.
     * @param objectPaths The paths to search the object in.
     * @return The filtered and trimmed object paths.
     */
    QList< QList<QObject*> > filterNestedChildrenWithUnnamedAncestors(
        const QString& name, const QList< QList<QObject*> >& objectPaths) const;    

    /**
     * Returns the object paths that contain a descendant from the base object
     * with the given name.
     * If there is more than one descendant with the given name, only the
     * shallower ones are taken into account.
     * The name must be a single object name, without any ancestor name.
     * The returned paths are trimmed to make the object with the given name the
     * new base object of the path.
     * 
     * @param name The name of the descendant to find.
     * @param objectPaths The paths to search the object in.
     * @return The filtered and trimmed object paths.
     */
    QList< QList<QObject*> > filterNestedChildren(const QString& name,
                            const QList< QList<QObject*> >& objectPaths) const;

};

}

#endif
