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

#ifndef KTUTORIAL_EDITORSUPPORT_EVENTSPY_H
#define KTUTORIAL_EDITORSUPPORT_EVENTSPY_H

#include <QObject>

namespace ktutorial {
namespace editorsupport {

/**
 * Spy to know all the events received in an object hierarchy.
 * EventSpy emitts a signal whenever an event is received in any of the spied
 * objects or its children (recursively). Even children added to a spied object
 * after it was added are spied.
 */
class EventSpy: public QObject {
Q_OBJECT
public:

    /**
     * Creates a new EventSpy with the given parent.
     *
     * @param parent The parent QObject.
     */
    explicit EventSpy(QObject* parent = 0);

    /**
     * Add object and all its children to spy.
     *
     * @param object The object to spy.
     */
    void addObjectToSpy(QObject* object);

Q_SIGNALS:

    /**
     * Emitted when an event is received in any of the spied objects or their
     * children.
     *
     * @param object The object that received the event.
     * @param event The event received.
     */
    void eventReceived(QObject* object, QEvent* event);

protected:

    /**
     * Filters the events received in the spied object hierarchies.
     * A eventReceived(QObject*, QEvent*) is emitted for each event.
     *
     * @param object The object that received the event.
     * @param event The event received.
     * @return False, the let events be handled as necessary.
     */
    virtual bool eventFilter(QObject* object, QEvent* event);

};

}
}

#endif
