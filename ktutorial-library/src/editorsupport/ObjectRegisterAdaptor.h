/***************************************************************************
 *   Copyright (C) 2010 by Daniel Calviño Sánchez <danxuliu@gmail.com>     *
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

#ifndef KTUTORIAL_EDITORSUPPORT_OBJECTREGISTERADAPTOR_H
#define KTUTORIAL_EDITORSUPPORT_OBJECTREGISTERADAPTOR_H

#include <QDBusAbstractAdaptor>

namespace ktutorial {
namespace editorsupport {
class ObjectRegister;
}
}

namespace ktutorial {
namespace editorsupport {

/**
 * Adaptor to expose an ObjectRegister through DBus.
 * It provides methods to get information about the registered QObjects.
 *
 * @see EditorSupport
 */
class ObjectRegisterAdaptor: public QDBusAbstractAdaptor {
Q_OBJECT
Q_CLASSINFO("D-Bus Interface", "org.kde.ktutorial.ObjectRegister")
public:

    /**
     * Creates a new ObjectRegisterAdaptor for the given ObjectRegister.
     *
     * @param objectRegister The ObjectRegister to adapt.
     */
    explicit ObjectRegisterAdaptor(ObjectRegister* objectRegister);

public Q_SLOTS:

    /**
     * Returns the name of the object with the given id.
     * If the id is not registered, an empty string is returned.
     *
     * @param objectId The id of the object.
     * @return The name of the object.
     */
    QString objectName(int objectId) const;

    /**
     * Returns the class name of the object with the given id.
     * If the id is not registered, an empty string is returned.
     *
     * @param objectId The id of the object.
     * @return The class name of the object.
     */
    QString className(int objectId) const;

    /**
     * Returns a list with the ids of the child objects of the object with the
     * given id.
     * If the id is not registered, an empty list is returned.
     *
     * @param objectId The id of the object.
     * @return The ids of the child objects.
     */
    QList<int> childObjectIds(int objectId) const;

    /**
     * Removes all the entries in the ObjectRegister.
     */
    void clear();

private:

    /**
     * The ObjectRegister to adapt.
     */
    ObjectRegister* mObjectRegister;

};

}
}

#endif
