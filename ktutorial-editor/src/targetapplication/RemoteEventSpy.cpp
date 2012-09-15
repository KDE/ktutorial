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

#include "RemoteEventSpy.h"

#include <QDBusInterface>

#include "RemoteObjectMapper.h"

//public:

RemoteEventSpy::RemoteEventSpy(const QString& service,
                               RemoteObjectMapper* mapper): QObject(),
    mMapper(mapper) {

    //RemoteEventSpy class can not inherit from QDBusInterface as that breaks
    //the "magic" done by QDbusInterface (it redefines qt_metacall and things
    //like that) and signals can not be connected so easily
    QDBusInterface* interface = new QDBusInterface(
                service, "/ktutorial/EventSpy", "org.kde.ktutorial.EventSpy",
                QDBusConnection::sessionBus(), this);
    connect(interface, SIGNAL(eventReceived(int,QString)),
            this, SLOT(handleEventReceived(int,QString)));
}

//private:

void RemoteEventSpy::handleEventReceived(int objectId,
                                         const QString& eventType) {
    emit eventReceived(mMapper->remoteObject(objectId), eventType);
}
