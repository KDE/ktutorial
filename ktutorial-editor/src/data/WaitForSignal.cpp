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

#include "WaitForSignal.h"

//public:

WaitForSignal::WaitForSignal(QObject* parent): WaitFor(parent) {
}

WaitFor* WaitForSignal::clone() const {
    WaitForSignal* cloned = new WaitForSignal();
    cloned->setEmitterName(mEmitterName);
    cloned->setSignalName(mSignalName);

    return cloned;
}

bool WaitForSignal::equals(const WaitFor& waitFor) const {
    if (!qobject_cast<const WaitForSignal*>(&waitFor)) {
        return false;
    }

    const WaitForSignal* waitForSignal =
                                    static_cast<const WaitForSignal*>(&waitFor);
    if (waitForSignal->emitterName() != mEmitterName) {
        return false;
    }

    if (waitForSignal->signalName() != mSignalName) {
        return false;
    }

    return true;
}

QString WaitForSignal::emitterName() const {
    return mEmitterName;
}

void WaitForSignal::setEmitterName(const QString& emitterName) {
    mEmitterName = emitterName;

    emit dataChanged(this);
}

QString WaitForSignal::signalName() const {
    return mSignalName;
}

void WaitForSignal::setSignalName(const QString& signalName) {
    mSignalName = signalName;

    emit dataChanged(this);
}
