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

#include "ObjectFinder.h"

namespace ktutorial {

//private:

QList<QObject*> ObjectFinder::getBestMatches(const QString& name,
                                QList< QList<QObject*> > objectPaths) const {
    if (name.isEmpty() || objectPaths.isEmpty()) {
        return QList<QObject*>();
    }

    if (name.indexOf('/') == -1) {
        QList< QList<QObject*> > filteredObjectPaths =
                                        filterObjectPaths(name, objectPaths);

        QList<QObject*> filteredObjects;
        foreach (QList<QObject*> filteredObjectPath, filteredObjectPaths) {
            filteredObjects.append(filteredObjectPath.first());
        }

        return filteredObjects;
    }

    QRegExp slashPattern("/+");
    QString ancestorName = name.left(name.indexOf(slashPattern));
    QString descendantName = name.mid(ancestorName.length() +
                                        slashPattern.matchedLength());

    return getBestMatches(descendantName, filterObjectPaths(ancestorName,
                                                            objectPaths));
}

QList< QList<QObject*> > ObjectFinder::filterObjectPaths(const QString& name,
                            const QList< QList<QObject*> >& objectPaths) const {
    QList< QList<QObject*> > filteredPaths = filterDirectChildren(name,
                                                                  objectPaths);
    if (filteredPaths.size() > 0) {
        return filteredPaths;
    }
    
    filteredPaths = filterNestedChildrenWithUnnamedAncestors(name, objectPaths);
    if (filteredPaths.size() > 0) {
        return filteredPaths;
    }

    return filterNestedChildren(name, objectPaths);
}

QList< QList<QObject*> > ObjectFinder::filterDirectChildren(const QString& name, 
                            const QList< QList<QObject*> >& objectPaths) const {
    QList< QList<QObject*> > filteredPaths;
    
    foreach (QList<QObject*> objectPath, objectPaths) {
        if (objectPath.size() >= 2 && objectPath[1]->objectName() == name) {
            objectPath.removeAt(0);
            filteredPaths.append(objectPath);
        }
    }
    
    return filteredPaths;
}

QList< QList<QObject*> > ObjectFinder::filterNestedChildrenWithUnnamedAncestors(
                            const QString& name, 
                            const QList< QList<QObject*> >& objectPaths) const {
    QList< QList<QObject*> > candidatePaths;

    //No need to use std::numeric_limits, as there would never be a 100000
    //levels deep object.
    int minimumNumberOfUnnamedAncestors = 100000;
    foreach (QList<QObject*> objectPath, objectPaths) {
        objectPath.removeAt(0);

        int unnamedAncestorCount = 0;
        while (objectPath.size() > unnamedAncestorCount &&
               objectPath[unnamedAncestorCount]->objectName().isEmpty()) {
            unnamedAncestorCount++;
        }

        if (unnamedAncestorCount > 0 &&
            objectPath.size() > unnamedAncestorCount &&
            objectPath[unnamedAncestorCount]->objectName() == name) {
            candidatePaths.append(objectPath);

            if (unnamedAncestorCount < minimumNumberOfUnnamedAncestors) {
                minimumNumberOfUnnamedAncestors = unnamedAncestorCount;
            }
        }
    }

    QList< QList<QObject*> > filteredPaths;

    foreach (QList<QObject*> candidatePath, candidatePaths) {
        if (candidatePath[minimumNumberOfUnnamedAncestors]->objectName() == 
                                                                        name) {
            for (int i=0; i<minimumNumberOfUnnamedAncestors; ++i) {
                candidatePath.removeAt(0);
            }
            filteredPaths.append(candidatePath);
        }
    }

    return filteredPaths;
}

QList< QList<QObject*> > ObjectFinder::filterNestedChildren(const QString& name, 
                            const QList< QList<QObject*> >& objectPaths) const {
    QList< QList<QObject*> > candidatePaths;

    //No need to use std::numeric_limits, as there would never be a 100000
    //levels deep object.
    int minimumNumberOfAncestors = 100000;
    foreach (QList<QObject*> objectPath, objectPaths) {
        objectPath.removeAt(0);

        int ancestorCount = 0;
        while (objectPath.size() > ancestorCount &&
               objectPath[ancestorCount]->objectName() != name) {
            ancestorCount++;
        }

        if (ancestorCount > 0 && objectPath.size() > ancestorCount &&
            objectPath[ancestorCount]->objectName() == name) {
            candidatePaths.append(objectPath);

            if (ancestorCount < minimumNumberOfAncestors) {
                minimumNumberOfAncestors = ancestorCount;
            }
        }
    }

    QList< QList<QObject*> > filteredPaths;

    foreach (QList<QObject*> candidatePath, candidatePaths) {
        if (candidatePath[minimumNumberOfAncestors]->objectName() == name) {
            for (int i=0; i<minimumNumberOfAncestors; ++i) {
                candidatePath.removeAt(0);
            }
            filteredPaths.append(candidatePath);
        }
    }
    
    return filteredPaths;
}

}
