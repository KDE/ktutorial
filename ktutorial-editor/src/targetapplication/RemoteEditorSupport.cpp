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

#include "RemoteEditorSupport.h"

#include <QDBusReply>

#include "RemoteEventSpy.h"
#include "RemoteObject.h"
#include "RemoteObjectMapper.h"

//public:

RemoteEditorSupport::RemoteEditorSupport(const QString& service,
                                         RemoteObjectMapper* mapper):
        QDBusAbstractInterface(service, "/ktutorial",
                               "org.kde.ktutorial.EditorSupport",
                               QDBusConnection::sessionBus(), 0),
    mMapper(mapper),
    mRemoteEventSpy(0),
    mNumberOfPendingEnableEventSpyCalls(0) {
}

RemoteEditorSupport::~RemoteEditorSupport() {
    delete mRemoteEventSpy;
}

RemoteObject* RemoteEditorSupport::mainWindow() throw (DBusException) {
    QDBusReply<int> reply = call("mainWindowObjectId");
    if (!reply.isValid()) {
        throw DBusException(reply.error().message());
    }

    return mMapper->remoteObject(reply.value());
}

RemoteObject* RemoteEditorSupport::findObject(const QString& name)
                                                        throw (DBusException) {
    QDBusReply<int> reply = call("findObject", name);
    if (!reply.isValid()) {
        throw DBusException(reply.error().message());
    }

    return mMapper->remoteObject(reply.value());
}

void RemoteEditorSupport::highlight(RemoteObject* remoteWidget)
                                                        throw (DBusException) {
    QDBusReply<void> reply = call("highlight", remoteWidget->objectId());
    if (!reply.isValid()) {
        throw DBusException(reply.error().message());
    }
}

void RemoteEditorSupport::stopHighlighting(RemoteObject* remoteWidget)
                                                        throw (DBusException) {
    QDBusReply<void> reply = call("stopHighlighting", remoteWidget->objectId());
    if (!reply.isValid()) {
        throw DBusException(reply.error().message());
    }
}

RemoteEventSpy* RemoteEditorSupport::enableEventSpy() throw (DBusException) {
    if (mRemoteEventSpy) {
        mNumberOfPendingEnableEventSpyCalls++;
        return mRemoteEventSpy;
    }

    QDBusReply<void> reply = call("enableEventSpy");
    if (!reply.isValid()) {
        throw DBusException(reply.error().message());
    }

    mRemoteEventSpy = new RemoteEventSpy(service(), mMapper);
    mNumberOfPendingEnableEventSpyCalls = 1;
    return mRemoteEventSpy;
}

void RemoteEditorSupport::disableEventSpy() throw (DBusException) {
    if (!mRemoteEventSpy) {
        return;
    }

    mNumberOfPendingEnableEventSpyCalls--;
    if (mNumberOfPendingEnableEventSpyCalls > 0) {
        return;
    }

    QDBusReply<void> reply = call("disableEventSpy");
    if (!reply.isValid()) {
        throw DBusException(reply.error().message());
    }

    delete mRemoteEventSpy;
    mRemoteEventSpy = 0;
}

void RemoteEditorSupport::testScriptedTutorial(const QString& filename,
                                               const QString& stepId)
                                                        throw (DBusException) {
    QDBusReply<void> reply;
    if (stepId.isEmpty()) {
        reply = call("testScriptedTutorial", filename);
    } else {
        reply = call("testScriptedTutorial", filename, stepId);
    }

    if (!reply.isValid()) {
        throw DBusException(reply.error().message());
    }
}
