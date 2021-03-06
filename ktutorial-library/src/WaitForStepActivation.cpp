/***************************************************************************
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

#include "WaitForStepActivation.h"
#include "WaitForStepActivation_p.h"

#include <QEvent>

#include <KDebug>

#include "Tutorial.h"

namespace ktutorial {
extern int debugArea();
}

namespace ktutorial {

//public:

WaitForStepActivation::WaitForStepActivation(): WaitFor(),
    d(new WaitForStepActivationPrivate()) {
    d->mDuringStepActivation = false;
}

WaitForStepActivation::WaitForStepActivation(const Tutorial* tutorial,
                                             const Step* step): WaitFor(),
    d(new WaitForStepActivationPrivate()) {
    d->mDuringStepActivation = false;
    setStep(tutorial, step);
}

WaitForStepActivation::~WaitForStepActivation() {
    delete d;
}

void WaitForStepActivation::setStep(const Tutorial* tutorial,
                                    const Step* step) {
    if (!tutorial) {
        kWarning(debugArea()) << "The tutorial that contains the step to wait"
                              << "for its activation is null!";
        return;
    }

    if (!step) {
        kWarning(debugArea()) << "The step to wait for its activation is null!";
        return;
    }

    connect(tutorial, SIGNAL(stepActivated(Step*)),
            this, SLOT(checkStepActivatedToEndTheWait(Step*)));
}

bool WaitForStepActivation::conditionMet() const {
    return d->mDuringStepActivation;
}

//private slots:

void WaitForStepActivation::checkStepActivatedToEndTheWait(Step* step) {
    Q_UNUSED(step);

    if (!isActive()) {
        return;
    }

    d->mDuringStepActivation = true;
    emit waitEnded(this);
    d->mDuringStepActivation = false;
}

}
