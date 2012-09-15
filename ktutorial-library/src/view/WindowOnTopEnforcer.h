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

#ifndef KTUTORIAL_VIEW_WINDOWONTOPENFORCER_H
#define KTUTORIAL_VIEW_WINDOWONTOPENFORCER_H

#include <QObject>
#include <QStack>

namespace ktutorial {
namespace common {
class WindowVisibilitySpy;
}
}

namespace ktutorial {
namespace view {

/**
 * Utility class to avoid windows being blocked by modal widgets.
 * When a modal widget is shown, the widget to keep on top is reparented to the
 * shown widget (if necessary). When the modal widget is hidden, the widget is
 * reparented to its first previous ancestor still visible.
 */
class WindowOnTopEnforcer: public QObject {
Q_OBJECT
public:

    /**
     * Creates a new WindowOnTopEnforcer with the widget to keep on top.
     * The widget to keep on top is set as the parent of the
     * WindowOnTopEnforcer.
     *
     * @param widget The widget to keep on top.
     */
    explicit WindowOnTopEnforcer(QWidget* widget);

    /**
     * Sets the base window to spy for its children dialogs.
     *
     * @param baseWindow The window to spy.
     */
    void setBaseWindow(QWidget* baseWindow);

private:

    /**
     * The widget to keep on top.
     */
    QWidget* mWidgetToKeepOnTop;

    /**
     * A stack with the parents of the widget to keep on top.
     * It is used to restore the previous parent when the latest one is hidden.
     * It always contains, at least, the base window as its first item.
     */
    QStack<QWidget*> mParentStack;

    /**
     * Checks whether the given object is ancestor of the given child object.
     *
     * @param object The object to check if it is the ancestor.
     * @param childObject The child object.
     * @return True if it is ancestor, false otherwise.
     */
    bool isAncestorOf(QObject* object, QObject* childObject) const;

    /**
     * Reparents the window to the given parent, preserving its window flags (as
     * they are cleared when a new parent is set).
     *
     * @param window The window to reparent.
     * @param parent The new parent.
     */
    void reparentWindowTo(QWidget* window, QWidget* parent) const;

private Q_SLOTS:

    /**
     * Reparents the widget to keep on top to the window if it is the top most
     * modal widget.
     * If the window is modal but is parent of the top most modal widget the
     * window is just inserted at the appropriate place in the stack.
     * If the window is modal but it is already in the stack nothing is done.
     *
     * @param window The window that has been shown.
     */
    void handleWindowShown(QWidget* window);

    /**
     * Reparents the widget to keep on top to its previous parent if the hidden
     * window is the top most modal widget.
     * If the window is modal but is parent of the top most modal widget the
     * window is just removed from the stack.
     * If the window is modal but it is not part of the stack nothing is done.
     *
     * @param window The window that has been hidden.
     */
    void handleWindowHidden(QWidget* window);

};

}
}

#endif
