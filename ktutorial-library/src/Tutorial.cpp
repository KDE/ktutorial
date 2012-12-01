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

#include "Tutorial.h"
#include "Tutorial_p.h"

#include <kdebug.h>

#include "Step.h"
#include "TutorialInformation.h"

namespace ktutorial {
extern int debugArea();
}

namespace ktutorial {

//public:

Tutorial::Tutorial(TutorialInformation* tutorialInformation): QObject(),
    d(new TutorialPrivate()) {
    d->mTutorialInformation = tutorialInformation;
    d->mCurrentStep = 0;
}

Tutorial::~Tutorial() {
    delete d->mTutorialInformation;
    delete d;
}

TutorialInformation* Tutorial::tutorialInformation() const {
    return d->mTutorialInformation;
}

void Tutorial::addStep(Step* step) {
    if (d->mSteps.contains(step->id())) {
        kWarning(debugArea()) << "Step with id" << step->id()
                              << "already added in tutorial"
                              << d->mTutorialInformation->id();
        return;
    }

    step->setParent(this);

    d->mSteps.insert(step->id(), step);

    connect(step, SIGNAL(nextStepRequested(QString)),
            this, SLOT(nextStep(QString)));
}

void Tutorial::start() {
    setup();

    if (!d->mSteps.contains("start")) {
        kError(debugArea()) << "No start step found in tutorial"
                            << d->mTutorialInformation->id();
        finish();
        return;
    }

    nextStep("start");
}

void Tutorial::nextStep(const QString& id) {
    if (!d->mSteps.contains(id)) {
        kError(debugArea()) << "No step" << id << "found in tutorial"
                            << d->mTutorialInformation->id();
        return;
    }

    nextStep(d->mSteps.value(id));
}

void Tutorial::nextStep(Step* step) {
    d->mQueuedSteps.append(step);

    if (d->mQueuedSteps.count() > 1) {
        //Nested call to nextStep(Step*) (that is, something called by
        //nextStep(Step*) caused the method to be called again before its
        //previous execution ended). Once that previous call continues its
        //execution it will change to the next queued step.
        return;
    }

    while (d->mQueuedSteps.count() > 0) {
        changeToStep(d->mQueuedSteps[0]);
        d->mQueuedSteps.removeFirst();
    }
}

//public slots:

void Tutorial::finish() {
    if (d->mCurrentStep != 0) {
        d->mCurrentStep->setActive(false);
        d->mCurrentStep = 0;
    }

    tearDown();

    emit finished(this);
}

//protected:

void Tutorial::setTutorialInformation(
                                    TutorialInformation* tutorialInformation) {
    d->mTutorialInformation = tutorialInformation;
}

void Tutorial::setup() {
}

void Tutorial::tearDown() {
}

//private:

void Tutorial::changeToStep(Step* step) {
    if (d->mSteps.key(step).isEmpty()) {
        kError(debugArea()) << "Activate step" << step->id()
                            << "which doesn't belong to tutorial"
                            << d->mTutorialInformation->id();
        return;
    }

    if (d->mCurrentStep != 0) {
        d->mCurrentStep->setActive(false);
    }

    kDebug(debugArea()) << "Next step:" << step->id();

    d->mCurrentStep = step;
    d->mCurrentStep->setActive(true);

    emit stepActivated(step);
}

}
