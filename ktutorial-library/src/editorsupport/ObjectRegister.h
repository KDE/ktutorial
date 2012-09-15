/***************************************************************************
 *   Copyright (C) 2010 by Daniel Calviño Sánchez <danxuliu@gmail.com>     *
 *   Copyright (C) 2011 by Daniel Calviño Sánchez <danxuliu@gmail.com>     *
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

#ifndef KTUTORIAL_EDITORSUPPORT_OBJECTREGISTER_H
#define KTUTORIAL_EDITORSUPPORT_OBJECTREGISTER_H

#include <QHash>
#include <QObject>

namespace ktutorial {
namespace editorsupport {

/**
 * Register to associate objects with an id.
 * The id for an object is assigned after the first time the id is requested
 * for that object, and are valid until the register is cleared or the object is
 * destroyed (if an object is destroyed it is automatically removed from the
 * register). The null object is a special case, and its id is always 0.
 *
 * When an object is registered, its QMetaObject and the QMetaObject of all its
 * super classes are also registered. The QMetaObject can be got using the class
 * name since then until the register is cleared.
 *
 * Its purpose is assign QObjects an id to allow the remote KTutorial editor to
 * refer to the objects in the target application.
 */
class ObjectRegister: public QObject {
Q_OBJECT
public:

    /**
     * Creates a new ObjectRegister with the given parent.
     *
     * @param parent The parent QObject.
     */
    explicit ObjectRegister(QObject* parent = 0);

    /**
     * Returns the id assigned to the object.
     *
     * @param object The object to get its id.
     * @return The id assigned to the object.
     */
    int idForObject(QObject* object);

    /**
     * Returns object associated to the id.
     *
     * @param objectId The id to get its associated object.
     * @return The object associated to the id.
     */
    QObject* objectForId(int objectId);

    /**
     * Returns the meta object with the given class name.
     *
     * @param className The class name to get its meta object.
     * @return The meta object with the given class name.
     */
    const QMetaObject* metaObjectForClassName(const QString& className) const;

    /**
     * Removes all the entries in this ObjectRegister.
     */
    void clear();

    /**
     * Registers a meta object and the meta objects of all its super classes.
     *
     * @param metaObject The meta object to register.
     */
    void registerMetaObject(const QMetaObject* metaObject);

private:

    /**
     * The next id to assign.
     */
    int mNextId;

    /**
     * The registered ids mapped by associated object.
     */
    QHash<QObject*, int> mRegisteredIds;

    /**
     * The registered objects mapped by id.
     */
    QHash<int, QObject*> mRegisteredObjects;

    /**
     * The registered meta objects mapped by their class name.
     */
    QHash<QString, const QMetaObject*> mRegisteredMetaObjects;

private Q_SLOTS:

    /**
     * Deregisters an object automatically when it is destroyed.
     *
     * @param object The object to deregister.
     */
    void deregister(QObject* object);

};

}
}

#endif
