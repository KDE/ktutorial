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

#ifndef REMOTEOBJECTMAPPER_H
#define REMOTEOBJECTMAPPER_H

#include <QHash>

class RemoteClass;
class RemoteObject;

/**
 * Map to get RemoteObjects and RemoteClasses from their objectId or class name.
 * The RemoteObjectMapper should be used to get all the RemoteObjects and
 * RemoteClasses for its DBus service. It creates a new RemoteObject when there
 * is no RemoteObject for the given id, or returns the previosly created one,
 * depending on the case. The same behavior is shown for RemoteClasses.
 *
 * The RemoteObjectMapper also has ownership of the RemoteObjects and
 * RemoteClasses, so they are deleted when the mapper is destroyed.
 */
class RemoteObjectMapper {
public:

    /**
     * Creates a new RemoteObjectMapper for the given DBus service name.
     *
     * @param service The DBus service name of the remote objects and classes.
     */
    RemoteObjectMapper(const QString& service);

    /**
     * Destroys this RemoteObjectMapper.
     * All the mapped RemoteObjects and RemoteClasses are also destroyed.
     */
    virtual ~RemoteObjectMapper();

    /**
     * Returns the RemoteObject associated with the given object id.
     * The RemoteObject is destroyed when this RemoteObjectMapper is destroyed,
     * so consider using QPointer to store it.
     * The object id 0 is a special case, and it is associated to the null
     * object; a null pointer is returned in this case.
     *
     * @param objectId The id of the remote object.
     * @return The RemoteObject.
     */
    RemoteObject* remoteObject(int objectId);

    /**
     * Returns the RemoteClass associated with the given class name.
     * The RemoteClass is destroyed when this RemoteObjectMapper is destroyed,
     * so consider using QPointer to store it.
     *
     * @param className The name of the remote class.
     * @return The RemoteClass.
     */
    RemoteClass* remoteClass(const QString& className);

private:

    /**
     * The DBus service name of the remote objects.
     */
    QString mService;

    /**
     * All the RemoteObjects already requested.
     */
    QHash<int, RemoteObject*> mRemoteObjects;

    /**
     * All the RemoteClasses already requested.
     */
    QHash<QString, RemoteClass*> mRemoteClasses;

};

#endif
