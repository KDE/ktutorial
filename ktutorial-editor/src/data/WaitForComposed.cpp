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

#include "WaitForComposed.h"

//public:

WaitForComposed::WaitForComposed(QObject* parent):
        WaitFor(parent),
    mCompositionType(And) {
}

WaitForComposed::~WaitForComposed() {
    qDeleteAll(mWaitFors);
}

WaitFor* WaitForComposed::clone() const {
    WaitForComposed* cloned = new WaitForComposed();
    cloned->setCompositionType(mCompositionType);

    foreach (WaitFor* waitFor, mWaitFors) {
        cloned->addWaitFor(waitFor->clone());
    }

    return cloned;
}

bool WaitForComposed::equals(const WaitFor& waitFor) const {
    if (!qobject_cast<const WaitForComposed*>(&waitFor)) {
        return false;
    }

    const WaitForComposed* waitForComposed =
                                static_cast<const WaitForComposed*>(&waitFor);
    if (waitForComposed->compositionType() != mCompositionType) {
        return false;
    }

    if (waitForComposed->waitFors().count() != mWaitFors.count()) {
        return false;
    }

    for (int i=0; i<mWaitFors.count(); ++i) {
        if (*waitForComposed->waitFors()[i] != *mWaitFors[i]) {
            return false;
        }
    }

    return true;
}

WaitForComposed::CompositionType WaitForComposed::compositionType() const {
    return mCompositionType;
}

void WaitForComposed::setCompositionType(CompositionType compositionType) {
    mCompositionType = compositionType;

    emit dataChanged(this);
}

void WaitForComposed::addWaitFor(WaitFor* waitFor) {
    Q_ASSERT(!mWaitFors.contains(waitFor));

    mWaitFors.append(waitFor);

    emit waitForAdded(waitFor);
}

QList<WaitFor*> WaitForComposed::waitFors() const {
    return mWaitFors;
}

void WaitForComposed::removeWaitFor(WaitFor* waitFor) {
    Q_ASSERT(mWaitFors.contains(waitFor));

    mWaitFors.removeOne(waitFor);

    emit waitForRemoved(waitFor);
}
