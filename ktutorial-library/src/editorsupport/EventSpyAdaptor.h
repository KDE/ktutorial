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

#ifndef KTUTORIAL_EDITORSUPPORT_EVENTSPYADAPTOR_H
#define KTUTORIAL_EDITORSUPPORT_EVENTSPYADAPTOR_H

#include <QDBusAbstractAdaptor>

namespace ktutorial {
namespace editorsupport {
class EventSpy;
class ObjectRegister;
}
}

namespace ktutorial {
namespace editorsupport {

/**
 * Adaptor to expose an EventSpy through DBus.
 *
 * @see EditorSupport
 */
class EventSpyAdaptor: public QDBusAbstractAdaptor {
Q_OBJECT
Q_CLASSINFO("D-Bus Interface", "org.kde.ktutorial.EventSpy")
public:

    /**
     * Creates a new EventSpyAdaptor for the given EventSpy.
     *
     * @param eventSpy The EventSpy to adapt.
     * @param objectRegister The object register to get the object ids from.
     */
    explicit EventSpyAdaptor(EventSpy* eventSpy,
                             ObjectRegister* objectRegister);

Q_SIGNALS:

    /**
     * Emitted when an event is received in any of the spied objects or their
     * children.
     *
     * @param objectId The id of the object that received the event.
     * @param eventType The type of the event received.
     */
    void eventReceived(int objectId, const QString& eventType);

private:

    /**
     * The register to associate objects with their id.
     */
    ObjectRegister* mObjectRegister;

private Q_SLOTS:

    /**
     * Adapts the eventReceived(QObject*, QEvent*) sent by the EventSpy to be
     * sent as eventReceived(int, QString).
     *
     * @param object The object that received the event.
     * @param event The event received.
     */
    void handleEventReceived(QObject* object, QEvent* event);

};

}
}

#endif
