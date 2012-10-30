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

#include <kdebug.h>

#include "Step.h"
#include "TutorialInformation.h"

namespace ktutorial {

//public:

Tutorial::~Tutorial() {
    delete mTutorialInformation;
}

void Tutorial::addStep(Step* step) {
    if (mSteps.contains(step->id())) {
        kWarning() << "Step with id " << step->id() << " already added in tutorial "
                   << mTutorialInformation->id();
        return;
    }

    step->setParent(this);

    mSteps.insert(step->id(), step);

    connect(step, SIGNAL(nextStepRequested(QString)),
            this, SLOT(nextStep(QString)));
}

void Tutorial::start() {
    setup();

    if (!mSteps.contains("start")) {
        kError() << "No start step found in tutorial " << mTutorialInformation->id();
        finish();
        return;
    }

    nextStep("start");
}

void Tutorial::nextStep(const QString& id) {
    if (!mSteps.contains(id)) {
        kError() << "No step " << id << " found in tutorial " << mTutorialInformation->id();
        return;
    }

    nextStep(mSteps.value(id));
}

void Tutorial::nextStep(Step* step) {
    mQueuedSteps.append(step);

    if (mQueuedSteps.count() > 1) {
        //Nested call to nextStep(Step*) (that is, something called by
        //nextStep(Step*) caused the method to be called again before its
        //previous execution ended). Once that previous call continues its
        //execution it will change to the next queued step.
        return;
    }

    while (mQueuedSteps.count() > 0) {
        changeToStep(mQueuedSteps[0]);
        mQueuedSteps.removeFirst();
    }
}

//public slots:

void Tutorial::finish() {
    if (mCurrentStep != 0) {
        mCurrentStep->setActive(false);
        mCurrentStep = 0;
    }

    tearDown();

    emit finished(this);
}

//private:

void Tutorial::changeToStep(Step* step) {
    if (mSteps.key(step).isEmpty()) {
        kError() << "Activate step " << step->id()
                 << " which doesn't belong to tutorial "
                 << mTutorialInformation->id();
        return;
    }

    if (mCurrentStep != 0) {
        mCurrentStep->setActive(false);
    }

    //TODO remove
    kDebug() << "Next step: " << step->id() << endl;

    mCurrentStep = step;
    mCurrentStep->setActive(true);

    emit stepActivated(step);
}

}
