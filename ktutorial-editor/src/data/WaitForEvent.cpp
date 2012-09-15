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

#include "WaitForEvent.h"

//public:

WaitForEvent::WaitForEvent(QObject* parent): WaitFor(parent) {
}

WaitFor* WaitForEvent::clone() const {
    WaitForEvent* cloned = new WaitForEvent();
    cloned->setReceiverName(mReceiverName);
    cloned->setEventName(mEventName);

    return cloned;
}

bool WaitForEvent::equals(const WaitFor& waitFor) const {
    if (!qobject_cast<const WaitForEvent*>(&waitFor)) {
        return false;
    }

    const WaitForEvent* waitForEvent =
                                    static_cast<const WaitForEvent*>(&waitFor);
    if (waitForEvent->receiverName() != mReceiverName) {
        return false;
    }

    if (waitForEvent->eventName() != mEventName) {
        return false;
    }

    return true;
}

QString WaitForEvent::receiverName() const {
    return mReceiverName;
}

void WaitForEvent::setReceiverName(const QString& receiverName) {
    mReceiverName = receiverName;

    emit dataChanged(this);
}

QString WaitForEvent::eventName() const {
    return mEventName;
}

void WaitForEvent::setEventName(const QString& eventName) {
    mEventName = eventName;

    emit dataChanged(this);
}
