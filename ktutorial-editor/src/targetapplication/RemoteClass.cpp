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

#include "RemoteClass.h"

#include <QtDBus/QtDBus>

#include "RemoteObjectMapper.h"

//public:

RemoteClass::RemoteClass(const QString& service, RemoteObjectMapper* mapper,
                         const QString& className):
        QDBusAbstractInterface(service, "/ktutorial/ObjectRegister",
                               "org.kde.ktutorial.ClassRegister",
                               QDBusConnection::sessionBus(), 0),
    mMapper(mapper),
    mClassName(className) {
}

QString RemoteClass::className() const {
    return mClassName;
}

RemoteClass* RemoteClass::superClass() throw (DBusException) {
    QDBusReply<QString> reply = call("superClass", mClassName);
    if (!reply.isValid()) {
        throw DBusException(reply.error().message());
    }

    if (reply.value().isEmpty()) {
        return 0;
    }
 
    return mMapper->remoteClass(reply.value());
}

QStringList RemoteClass::propertyList() throw (DBusException) {
    QDBusReply<QStringList> reply = call("propertyList", mClassName);
    if (!reply.isValid()) {
        throw DBusException(reply.error().message());
    }

    return reply.value();
}

QStringList RemoteClass::signalList() throw (DBusException) {
    QDBusReply<QStringList> reply = call("signalList", mClassName);
    if (!reply.isValid()) {
        throw DBusException(reply.error().message());
    }

    return reply.value();
}
