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

#ifndef KTUTORIAL_COMMON_WINDOWVISIBILITYSPY_H
#define KTUTORIAL_COMMON_WINDOWVISIBILITYSPY_H

#include <QObject>

namespace ktutorial {
namespace common {

/**
 * Spy to know when windows from a widget hierarchy are shown or hidden.
 * WindowVisibilitySpy emitts windowShown(QWidget*) whenever any of the spied
 * widgets or its child widgets (recursively) are shown as a window. The signal
 * windowHidden(QWidget*) is emitted when any of them is hidden. The signals are
 * emitted for windows but also for dialogs.
 *
 * Children added to a spied widget after it was added to the
 * WindowVisibilitySpy are also automatically spied.
 */
class WindowVisibilitySpy: public QObject {
Q_OBJECT
public:

    /**
     * Creates a new WindowVisibilitySpy with the given parent.
     *
     * @param parent The parent QObject.
     */
    explicit WindowVisibilitySpy(QObject* parent = 0);

    /**
     * Add widget and all its child widgets to spy.
     *
     * @param widget The widget to spy.
     */
    void addWidgetToSpy(QWidget* widget);

Q_SIGNALS:

    /**
     * Emitted when any of the spied widgets or its children is shown as a
     * window.
     *
     * @param window The widget shown.
     */
    void windowShown(QWidget* window);

    /**
     * Emitted when any of the spied widgets or its children is hidden (and it
     * was a window).
     *
     * @param window The widget hidden.
     */
    void windowHidden(QWidget* window);

protected:

    /**
     * Filters the events received in the spied widget hierarchies.
     * A windowShown(QWidget*) is emitted when a window is shown, and a
     * windowHidden(QWidget*) is emitted when a window is hidden.
     *
     * @param object The widget that received the event.
     * @param event The event received.
     * @return False, to let the events be handled as necessary.
     */
    virtual bool eventFilter(QObject* object, QEvent* event);

};

}
}

#endif
