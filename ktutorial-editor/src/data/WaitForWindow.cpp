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

//public:

WaitForWindow::WaitForWindow(QObject* parent): WaitFor(parent) {
}

WaitFor* WaitForWindow::clone() const {
    WaitForWindow* cloned = new WaitForWindow();
    cloned->setWindowObjectName(mWindowObjectName);

    return cloned;
}

bool WaitForWindow::equals(const WaitFor& waitFor) const {
    if (!qobject_cast<const WaitForWindow*>(&waitFor)) {
        return false;
    }

    const WaitForWindow* waitForWindow =
                                    static_cast<const WaitForWindow*>(&waitFor);
    if (waitForWindow->windowObjectName() != mWindowObjectName) {
        return false;
    }

    return true;
}

QString WaitForWindow::windowObjectName() const {
    return mWindowObjectName;
}

void WaitForWindow::setWindowObjectName(const QString& windowObjectName) {
    mWindowObjectName = windowObjectName;

    emit dataChanged(this);
}
