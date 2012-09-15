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

#include "RemoteObject.h"

#include <QtDBus/QtDBus>

#include "RemoteObjectMapper.h"

//public:

RemoteObject::RemoteObject(const QString& service, RemoteObjectMapper* mapper,
                           int objectId):
        QDBusAbstractInterface(service, "/ktutorial/ObjectRegister",
                               "org.kde.ktutorial.ObjectRegister",
                               QDBusConnection::sessionBus(), 0),
    mMapper(mapper),
    mObjectId(objectId) {
}

int RemoteObject::objectId() const {
    return mObjectId;
}

QString RemoteObject::name() throw (DBusException) {
    QDBusReply<QString> reply = call("objectName", mObjectId);
    if (!reply.isValid()) {
        throw DBusException(reply.error().message());
    }

    return reply.value();
}

RemoteClass* RemoteObject::remoteClass() throw (DBusException) {
    QDBusReply<QString> reply = call("className", mObjectId);
    if (!reply.isValid()) {
        throw DBusException(reply.error().message());
    }

    return mMapper->remoteClass(reply.value());
}

Q_DECLARE_METATYPE(QList<int>)

QList<RemoteObject*> RemoteObject::children() throw (DBusException) {
    qDBusRegisterMetaType< QList<int> >();

    QDBusReply< QList<int> > reply = call("childObjectIds", mObjectId);
    if (!reply.isValid()) {
        throw DBusException(reply.error().message());
    }

    QList<RemoteObject*> children;
    foreach (int childObjectId, reply.value()) {
        children.append(mMapper->remoteObject(childObjectId));
    }

    return children;
}
