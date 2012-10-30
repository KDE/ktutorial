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

#include "WindowOnTopEnforcer.h"

#include <QWidget>

#include "../common/WindowVisibilitySpy.h"

using ktutorial::common::WindowVisibilitySpy;

namespace ktutorial {
namespace view {

//public:

WindowOnTopEnforcer::WindowOnTopEnforcer(QWidget* widget): QObject(widget),
    mWidgetToKeepOnTop(widget) {

    Q_ASSERT(widget);

    mParentStack.push(mWidgetToKeepOnTop->parentWidget());
}

void WindowOnTopEnforcer::setBaseWindow(QWidget* baseWindow) {
    Q_ASSERT(baseWindow);

    WindowVisibilitySpy* spy = new WindowVisibilitySpy(this);
    spy->addWidgetToSpy(baseWindow);
    connect(spy, SIGNAL(windowShown(QWidget*)),
            this, SLOT(handleWindowShown(QWidget*)));
    connect(spy, SIGNAL(windowHidden(QWidget*)),
            this, SLOT(handleWindowHidden(QWidget*)));
}

//private:

bool WindowOnTopEnforcer::isAncestorOf(QObject* object,
                                       QObject* childObject) const {
    if (childObject->parent() == 0) {
        return false;
    }

    if (childObject->parent() == object) {
        return true;
    }

    return isAncestorOf(object, childObject->parent());
}

void WindowOnTopEnforcer::reparentWindowTo(QWidget* window,
                                           QWidget* parent) const {
    //When a widget is reparented it is hidden and its window flags are cleared,
    //so they must be restored and the widget shown again
    Qt::WindowFlags flags = window->windowFlags();
    window->setParent(parent);
    window->setWindowFlags(flags);
    window->show();
}

//private slots:

void WindowOnTopEnforcer::handleWindowShown(QWidget* window) {
    Q_ASSERT(window);

    if (!window->isModal() || mParentStack.contains(window)) {
        return;
    }

    //If the modal window shown is ancestor of any of the modal windows already
    //shown just insert it at the appropriate place in the parent stack.
    for (int i=1; i<mParentStack.size(); ++i) {
        QWidget* widgetInStack = mParentStack[i];

        if (!isAncestorOf(widgetInStack, window) &&
                isAncestorOf(window, widgetInStack)) {
            mParentStack.insert(i, window);
            return;
        }
    }

    mParentStack.push(window);

    reparentWindowTo(mWidgetToKeepOnTop, window);
}

void WindowOnTopEnforcer::handleWindowHidden(QWidget* window) {
    Q_ASSERT(window);

    if (!window->isModal() || !mParentStack.contains(window)) {
        return;
    }

    if (window != mParentStack.top()) {
        mParentStack.remove(mParentStack.indexOf(window));
        return;
    }

    mParentStack.pop();

    reparentWindowTo(mWidgetToKeepOnTop, mParentStack.top());
}

}
}
