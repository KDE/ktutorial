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

#ifndef KTUTORIAL_WAITFOREVENT_H
#define KTUTORIAL_WAITFOREVENT_H

#include <QEvent>

#include "ktutorial_export.h"

#include "WaitFor.h"

namespace ktutorial {

/**
 * Waits for an event of some specific type to be sent to an object.
 * When an event of the expected type is sent and the WaitForEvent is active,
 * the wait ends.
 *
 * Note that if the event is sent while the WaitFor isn't active, it won't
 * be registered and the condition won't be met. In order to met the condition,
 * the event must be sent while the WaitForEvent is active.
 *
 * In some cases, just waiting for an event of some type isn't enough. For
 * example, you may want to end the waiting if a QEvent::MouseButtonPress is
 * sent, but only if the button pressed is the left. To do this, you can create
 * a subclass of WaitForEvent and redefine handleEvent(QEvent*) method.
 * \code
 * void WaitForEventSubclass::handleEvent(QEvent* event) {
 *     QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(event);
 *
 *     if (mouseEvent->button() == Qt::LeftButton) {
 *         WaitForEvent::handleEvent(event);
 *     }
 * }
 * \endcode
 */
class KTUTORIAL_EXPORT WaitForEvent: public WaitFor {
Q_OBJECT
public:

    /**
     * Creates a new WaitForEvent.
     * This constructor is needed to dynamically create WaitForEvent objects in
     * scripts using ScriptingModule::newWaitFor(const QString&). Method
     * setEvent(QObject*, const QString&) must be called to finish setting up
     * the object. For C++ tutorials, use
     * WaitForEvent(QObject*, QEvent::Type) constructor instead of this one.
     */
    Q_INVOKABLE WaitForEvent();

    /**
     * Creates a new WaitForEvent.
     *
     * @param object The object to watch.
     * @param type The type of the event to wait for.
     */
    WaitForEvent(QObject* object, QEvent::Type type);

    /**
     * Sets the event to wait for.
     * Note that the QEvent::Type has to be passed as a string. For example, to
     * wait for a QEvent::Close you have to pass "Close" as the second
     * parameter.
     * This method can be invoked from a script.
     *
     * In fact, you should only invoke this method from a script, and only once,
     * to set up the object. For C++ tutorials, use
     * WaitForEvent(QObject*, QEvent::Type) constructor when creating this
     * WaitForEvent.
     *
     * The type is passed as a string with its name instead of using a pure
     * QEvent::Type due to a current limitation of Kross (enumeration in QObject
     * classes seem supported. However, QEvent is not a QObject but a Q_GADGET).
     *
     * @param object The object to watch.
     * @param typeName The name of the event type to wait for.
     */
    Q_INVOKABLE void setEvent(QObject* object, const QString& typeName);

    /**
     * Inspects the events sent to the watched object.
     * If the event has the type expected by this WaitFor it is handled by
     * handleEvent(QEvent*) which, by default, ends the waiting.
     *
     * @param object The object that the event was sent to.
     * @param event The event sent.
     * @return False, so the event can be handled further.
     */
    virtual bool eventFilter(QObject* object, QEvent* event);

    /**
     * Returns true if the event was received while active, false otherwise.
     *
     * @return True if the event was received while active, false otherwise.
     */
    virtual bool conditionMet() const;

    /**
     * Sets this WaitForEvent active or inactive.
     * Activating it resets its condition.
     *
     * @param active True to set it active, false otherwise.
     */
    virtual void setActive(bool active);

protected:

    /**
     * Handles the event ending the wait (if this WaitForEvent is active).
     * WaitForEvent subclasses can redefine this method if just receiving an
     * event of the type expected isn't enough to end the waiting and some
     * special check has to be done.
     *
     * The event is assured to have the type expected by this WaitForEvent and
     * to be sent to the watched object.
     *
     * @param event The event.
     */
    virtual void handleEvent(QEvent* event);

private:

    /**
     * The watched object.
     */
    QObject* mObject;

    /**
     * The type of event expected.
     */
    QEvent::Type mEventType;

    /**
     * Whether the event was received when active or not.
     */
    bool mConditionMet;

};

}

#endif
