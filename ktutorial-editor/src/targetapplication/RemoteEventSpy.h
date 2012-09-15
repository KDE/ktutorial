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

#ifndef REMOTEEVENTSPY_H
#define REMOTEEVENTSPY_H

#include <QObject>

class RemoteObject;
class RemoteObjectMapper;

/**
 * Proxy for the remote EventSpy exposed by KTutorial editor support module in
 * KTutorial library.
 * RemoteEventSpy represents a remote EventSpy exposed through DBus. Its purpose
 * is provide an API to use the remote EventSpy like any other local object,
 * hiding the DBus complexity behind it.
 *
 * RemoteEventSpy handles the eventReceived signal emitted by
 * "org.kde.ktutorial.EventSpy" interface in the DBus service specified in the
 * constructor and emits an equivalent signal replacing the object id with a
 * RemoteObject proxy.
 */
class RemoteEventSpy: public QObject {
Q_OBJECT
public:

    /**
     * Creates a new RemoteEventSpy to represent a remote EventSpy in the given
     * DBus service.
     *
     * @param service The DBus service name.
     * @param mapper The RemoteObjectMapper to get RemoteObjects from.
     */
    RemoteEventSpy(const QString& service, RemoteObjectMapper* mapper);

Q_SIGNALS:

    /**
     * Emitted when the remote object receives an event.
     *
     * @param remoteObject The proxy for the real remote object.
     * @param eventType The type of the event received.
     */
    void eventReceived(RemoteObject* remoteObject, const QString& eventType);

private:

    /**
     * The mapper that associates a RemoteObject with its object id.
     */
    RemoteObjectMapper* mMapper;

private Q_SLOTS:

    /**
     * Handles an event reception notified by the EventSpy.
     *
     * @param objectId The id of the remote object that received the event.
     * @param eventType The type of the event received.
     */
    void handleEventReceived(int objectId, const QString& eventType);

};

#endif
