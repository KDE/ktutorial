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

#include "WaitForNot.h"

//public:

WaitForNot::WaitForNot(QObject* parent):
        WaitFor(parent),
    mNegatedWaitFor(0) {
}

WaitForNot::~WaitForNot() {
    delete mNegatedWaitFor;
}

WaitFor* WaitForNot::clone() const {
    WaitForNot* cloned = new WaitForNot();
    if (mNegatedWaitFor) {
        cloned->setNegatedWaitFor(mNegatedWaitFor->clone());
    }

    return cloned;
}

bool WaitForNot::equals(const WaitFor& waitFor) const {
    if (!qobject_cast<const WaitForNot*>(&waitFor)) {
        return false;
    }

    const WaitForNot* waitForNot = static_cast<const WaitForNot*>(&waitFor);
    if (waitForNot->negatedWaitFor() == 0 && mNegatedWaitFor == 0) {
        return true;
    }

    if (waitForNot->negatedWaitFor() == 0 || mNegatedWaitFor == 0) {
        return false;
    }

    if (*waitForNot->negatedWaitFor() != *mNegatedWaitFor) {
        return false;
    }

    return true;
}

WaitFor* WaitForNot::negatedWaitFor() const {
    return mNegatedWaitFor;
}

void WaitForNot::setNegatedWaitFor(WaitFor* waitFor) {
    mNegatedWaitFor = waitFor;

    emit dataChanged(this);
}
