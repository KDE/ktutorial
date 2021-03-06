/***************************************************************************
 *   Copyright (C) 2008 by Daniel Calviño Sánchez <danxuliu@gmail.com>     *
 *   Copyright (C) 2009 by Daniel Calviño Sánchez <danxuliu@gmail.com>     *
 *   Copyright (C) 2010 by Daniel Calviño Sánchez <danxuliu@gmail.com>     *
 *   Copyright (C) 2011 by Daniel Calviño Sánchez <danxuliu@gmail.com>     *
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
#include "WaitForComposed_p.h"

namespace ktutorial {

//public:

WaitForComposed::~WaitForComposed() {
    delete d;
}

void WaitForComposed::setActive(bool active) {
    WaitFor::setActive(active);

    QListIterator<WaitFor*> it(d->mWaitFors);
    while (it.hasNext()) {
        it.next()->setActive(active);
    }
}

void WaitForComposed::add(WaitFor* waitFor) {
    if (d->mWaitFors.contains(waitFor)) {
        return;
    }

    waitFor->setParent(this);
    d->mWaitFors.append(waitFor);

    connect(waitFor, SIGNAL(waitEnded(WaitFor*)),
            this, SLOT(childWaitEnd(WaitFor*)));
}

//public slots:

void WaitForComposed::childWaitEnd(WaitFor* waitFor) {
    Q_UNUSED(waitFor);

    Q_ASSERT(d->mWaitFors.contains(waitFor));

    if (!isActive()) {
        return;
    }

    if (conditionMet()) {
        emit waitEnded(this);
    }
}

//protected:

WaitForComposed::WaitForComposed(): WaitFor(),
    d(new WaitForComposedPrivate()) {
}

QList<WaitFor*>& WaitForComposed::waitFors() const {
    return d->mWaitFors;
}

}
