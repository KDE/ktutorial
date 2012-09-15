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

#include "WaitForWindow.h"
#include "KTutorial.h"
#include "common/WindowVisibilitySpy.h"

using ktutorial::common::WindowVisibilitySpy;

namespace ktutorial {

//public:

WaitForWindow::WaitForWindow(): WaitFor(),
    mConditionMet(false) {

    WindowVisibilitySpy* spy = new WindowVisibilitySpy(this);
    spy->addWidgetToSpy(KTutorial::self()->mainApplicationWindow());
    connect(spy, SIGNAL(windowShown(QWidget*)),
            this, SLOT(checkWindowShown(QWidget*)));
}

WaitForWindow::WaitForWindow(const QString& objectName): WaitFor(),
    mConditionMet(false) {

    WindowVisibilitySpy* spy = new WindowVisibilitySpy(this);
    spy->addWidgetToSpy(KTutorial::self()->mainApplicationWindow());
    connect(spy, SIGNAL(windowShown(QWidget*)),
            this, SLOT(checkWindowShown(QWidget*)));

    setWindowObjectName(objectName);
}

void WaitForWindow::setWindowObjectName(const QString& objectName) {
    mWindowObjectName = objectName;
}

bool WaitForWindow::conditionMet() const {
    return mConditionMet;
}

void WaitForWindow::setActive(bool active) {
    WaitFor::setActive(active);

    if (active) {
        mConditionMet = false;
    }
}

//private slots:

void WaitForWindow::checkWindowShown(QWidget* window) {
    if (!isActive()) {
        return;
    }

    if (window->objectName() != mWindowObjectName) {
        return;
    }

    mConditionMet = true;
    emit waitEnded(this);
}

}

#include "WaitForWindow.moc"
