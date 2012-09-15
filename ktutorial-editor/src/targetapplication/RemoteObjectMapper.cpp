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

#include "RemoteObjectMapper.h"

#include <QHash>

#include "RemoteClass.h"
#include "RemoteObject.h"

//public:

RemoteObjectMapper::RemoteObjectMapper(const QString& service):
    mService(service) {
}

RemoteObjectMapper::~RemoteObjectMapper() {
    qDeleteAll(mRemoteObjects);
    qDeleteAll(mRemoteClasses);
}

RemoteObject* RemoteObjectMapper::remoteObject(int objectId) {
    if (objectId == 0) {
        return 0;
    }

    if (mRemoteObjects.contains(objectId)) {
        return mRemoteObjects.value(objectId);
    }

    RemoteObject* remoteObject = new RemoteObject(mService, this, objectId);
    mRemoteObjects.insert(objectId, remoteObject);

    return remoteObject;
}

RemoteClass* RemoteObjectMapper::remoteClass(const QString& className) {
    if (mRemoteClasses.contains(className)) {
        return mRemoteClasses.value(className);
    }

    RemoteClass* remoteClass = new RemoteClass(mService, this, className);
    mRemoteClasses.insert(className, remoteClass);

    return remoteClass;
}
