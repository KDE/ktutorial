/***************************************************************************
 *   Copyright (C) 2008 by Daniel Calviño Sánchez <danxuliu@gmail.com>     *
 *   Copyright (C) 2009 by Daniel Calviño Sánchez <danxuliu@gmail.com>     *
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

#include <KDebug>

namespace ktutorial {
extern int debugArea();
}

namespace ktutorial {

//public:

WaitForSignal::WaitForSignal(): WaitFor(),
    mConditionMet(false) {
}

WaitForSignal::WaitForSignal(QObject* sender, const QString& signal): WaitFor(),
    mConditionMet(false) {
    setSignal(sender, signal);
}

void WaitForSignal::setSignal(QObject* sender, const QString& signal) {
    if (!sender) {
        kWarning(debugArea()) << "The object that emits the signal" << signal
                              << "to wait for is null!";
        return;
    }

    QString signalName = signal;
    if (!signalName.startsWith('2')) {
        signalName = QString("2%1").arg(signal);
    }

    connect(sender, signalName.toLatin1(), this, SLOT(signalWaitEnd()));
}

bool WaitForSignal::conditionMet() const {
    return mConditionMet;
}

void WaitForSignal::setActive(bool active) {
    WaitFor::setActive(active);

    if (active) {
        mConditionMet = false;
    }
}

//public slots:

void WaitForSignal::signalWaitEnd() {
    if (!isActive()) {
        return;
    }

    mConditionMet = true;
    emit waitEnded(this);
}

}
