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

#include "TestTutorialActions.h"

#include <KAction>
#include <KActionCollection>
#include <KLocalizedString>

#include "KTutorialEditor.h"
#include "TutorialTester.h"
#include "data/Step.h"

//public:

TestTutorialActions::TestTutorialActions(KTutorialEditor* tutorialEditor):
        QObject(tutorialEditor),
    mTutorialEditor(tutorialEditor),
    mCurrentStep(0) {

    setupActions();
}

//public slots:

void TestTutorialActions::selectStep(Step* step) {
    if (mCurrentStep) {
        disconnect(mCurrentStep, SIGNAL(dataChanged(Step*)),
                   this, SLOT(updateTestTutorialFromCurrentStepActionState()));
    }

    mCurrentStep = step;

    if (mCurrentStep) {
        connect(mCurrentStep, SIGNAL(dataChanged(Step*)),
                this, SLOT(updateTestTutorialFromCurrentStepActionState()));
    }

    updateTestTutorialFromCurrentStepActionState();
}

//private:

void TestTutorialActions::setupActions() {
    KActionCollection* actionCollection = mTutorialEditor->actionCollection();

    KAction* action = new KAction(this);
    action = new KAction(this);
    action->setText(i18nc("@action", "Test tutorial"));
    action->setStatusTip(i18nc("@info:status", "Starts the tutorial in the "
"target application."));
    action->setIcon(KIcon("document-preview"));
    action->setEnabled(true);
    actionCollection->addAction("testTutorial", action);
    connect(action, SIGNAL(triggered(bool)),
            this, SLOT(testTutorial()));

    action = new KAction(this);
    action->setText(i18nc("@action", "Test tutorial from current step"));
    action->setStatusTip(i18nc("@info:status", "Starts the tutorial in the "
"target application from the current step."));
    action->setIcon(KIcon("document-preview"));
    action->setEnabled(false);
    actionCollection->addAction("testTutorialFromCurrentStep", action);
    connect(action, SIGNAL(triggered(bool)),
            this, SLOT(testTutorialFromCurrentStep()));
}

//private slots:

void TestTutorialActions::updateTestTutorialFromCurrentStepActionState() {
    KActionCollection* actionCollection = mTutorialEditor->actionCollection();
    QAction* action = actionCollection->action("testTutorialFromCurrentStep");

    if (mCurrentStep && !mCurrentStep->id().isEmpty()) {
        action->setEnabled(true);
    } else {
        action->setEnabled(false);
    }
}

void TestTutorialActions::testTutorial() {
    TutorialTester* tutorialTester = new TutorialTester(mTutorialEditor);
    tutorialTester->testTutorial();
}

void TestTutorialActions::testTutorialFromCurrentStep() {
    Q_ASSERT(mCurrentStep);

    TutorialTester* tutorialTester = new TutorialTester(mTutorialEditor);
    tutorialTester->setStepToTestFrom(mCurrentStep->id());
    tutorialTester->testTutorial();
}
