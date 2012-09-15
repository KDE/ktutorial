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

#ifndef REMOTEOBJECT_H
#define REMOTEOBJECT_H

#include <QDBusAbstractInterface>

#include "DBusException.h"

class RemoteClass;
class RemoteObjectMapper;

/**
 * Proxy for remote objects exposed by KTutorial editor support module in
 * KTutorial library.
 * RemoteObject represents a remote object exposed by ObjectRegister in
 * KTutorial library through DBus. Its purpose is provide an API to use the
 * remote object like any other local object, hiding the DBus complexity behind
 * it.
 *
 * To get the data, RemoteObject makes DBus calls to the
 * "org.kde.ktutorial.ObjectRegistry" interface in the DBus service specified in
 * the constructor.
 *
 * Although the idea is let other objects use it like a local object, it has to
 * communicate with the remote ObjectRegistry through DBus anyway, so the
 * methods may throw a DBusException if something goes wrong.
 */
class RemoteObject: public QDBusAbstractInterface {
Q_OBJECT
public:

    /**
     * Creates a new RemoteObject to represent the remote object with the given
     * id in the given DBus service name.
     *
     * @param service The DBus service name.
     * @param mapper The RemoteObjectMapper to get RemoteObjects from.
     * @param objectId The id of the remote object.
     */
    RemoteObject(const QString& service, RemoteObjectMapper* mapper,
                 int objectId);

    /**
     * Returns the id of the remote object.
     *
     * @return The id of the remote object.
     */
    int objectId() const;

    /**
     * Returns the object name.
     *
     * @return The object name.
     * @throws DBusException If a DBus error happens.
     */
    QString name() throw (DBusException);

    /**
     * Returns the remote class.
     *
     * @return The remote class.
     * @throws DBusException If a DBus error happens.
     */
    RemoteClass* remoteClass() throw (DBusException);

    /**
     * Returns a list with the RemoteObjects that represent the children of this
     * remote object.
     *
     * @return The child remote objects.
     * @throws DBusException If a DBus error happens.
     */
    QList<RemoteObject*> children() throw (DBusException);

private:

    /**
     * The mapper that associates a RemoteObject with its object id.
     */
    RemoteObjectMapper* mMapper;

    /**
     * The id of the remote object.
     */
    int mObjectId;

};

#endif
