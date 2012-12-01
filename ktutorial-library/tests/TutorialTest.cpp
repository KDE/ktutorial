/***************************************************************************
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

#include <QTest>
#include <qtest_kde.h>

#define protected public
#define private public
#include "Tutorial.h"
#undef private
#undef protected

#include "Tutorial_p.h"

#include "Step.h"
#include "TutorialInformation.h"

//Step* must be declared as a metatype to be used in qvariant_cast
Q_DECLARE_METATYPE(ktutorial::Step*);

//Tutorial* must be declared as a metatype to be used in qvariant_cast
Q_DECLARE_METATYPE(ktutorial::Tutorial*);

namespace ktutorial {

class TutorialTest: public QObject {
Q_OBJECT
public slots:

    void assertStepActive(Step* step) {
        QVERIFY(step->isActive());
    }

private slots:

    void testConstructor();
    void testConstructorWithoutTutorialInformation();

    void testAddStep();
    void testAddStepSeveralSteps();
    void testAddStepTwice();
    void testAddStepDifferentStepWithSameId();

    void testStart();
    void testStartNoStartStep();

    void testNextStepId();
    void testNextStepIdRequestedFromStep();
    void testNextStepIdWithInvalidId();

    void testNextStepStep();
    void testNextStepStepWithInvalidStep();

    void testNextStepFromStepSetup();

    void testFinish();

};

class StepToRequestNextStep: public Step {
public:

    StepToRequestNextStep(const QString& id): Step(id) {
    }

    void emitNextStepRequested(const QString& nextStepId) {
        emit nextStepRequested(nextStepId);
    }

};

class StepToSetNextStepInSetup: public Step {
public:

    Tutorial* mTutorial;
    Step* mNextStep;
    bool mDuringSetup;

    StepToSetNextStepInSetup(const QString& id): Step(id),
        mTutorial(0),
        mNextStep(0),
        mDuringSetup(false) {
    }

protected:

    void setup() {
        mDuringSetup = true;
        mTutorial->nextStep(mNextStep);
        mDuringSetup = false;
    }

};

class StepToAssertNoRecursiveStepActivation: public Step {
public:

    StepToSetNextStepInSetup* mPreviousStep;

    StepToAssertNoRecursiveStepActivation(const QString& id): Step(id),
        mPreviousStep(0) {
    }

protected:

    void setup() {
        QVERIFY(!mPreviousStep->mDuringSetup);
    }

};

class MockTutorial: public Tutorial {
public:

    int mSetupCount;
    int mTearDownCount;

    MockTutorial(TutorialInformation* tutorialInformation):
        Tutorial(tutorialInformation) {
        mSetupCount = 0;
        mTearDownCount = 0;
    }

protected:

    virtual void setup() {
        mSetupCount++;
    }

    virtual void tearDown() {
        mTearDownCount++;
    }

};

void TutorialTest::testConstructor() {
    TutorialInformation* tutorialInformation =
                                    new TutorialInformation("pearlOrientation");
    Tutorial tutorial(tutorialInformation);

    QCOMPARE(tutorial.tutorialInformation(), tutorialInformation);
    QCOMPARE(tutorial.d->mCurrentStep, (Step*)0);
}

void TutorialTest::testConstructorWithoutTutorialInformation() {
    Tutorial tutorial(0);
    TutorialInformation* tutorialInformation =
                                    new TutorialInformation("pearlOrientation");
    tutorial.setTutorialInformation(tutorialInformation);

    QCOMPARE(tutorial.tutorialInformation(), tutorialInformation);
    QCOMPARE(tutorial.d->mCurrentStep, (Step*)0);
}

void TutorialTest::testAddStep() {
    Tutorial tutorial(new TutorialInformation("pearlOrientation"));

    Step* step1 = new Step("record");

    tutorial.addStep(step1);

    QCOMPARE(step1->parent(), &tutorial);
    QCOMPARE(tutorial.d->mSteps.size(), 1);
    QCOMPARE(tutorial.d->mSteps.value("record"), step1);
    QCOMPARE(tutorial.d->mCurrentStep, (Step*)0);
}

void TutorialTest::testAddStepSeveralSteps() {
    Tutorial tutorial(new TutorialInformation("pearlOrientation"));

    Step* step1 = new Step("record");
    tutorial.addStep(step1);

    Step* step2 = new Step("roll");
    tutorial.addStep(step2);

    Step* step3 = new Step("send");
    tutorial.addStep(step3);

    QCOMPARE(step1->parent(), &tutorial);
    QCOMPARE(step2->parent(), &tutorial);
    QCOMPARE(step3->parent(), &tutorial);
    QCOMPARE(tutorial.d->mSteps.size(), 3);
    QCOMPARE(tutorial.d->mSteps.value("record"), step1);
    QCOMPARE(tutorial.d->mSteps.value("roll"), step2);
    QCOMPARE(tutorial.d->mSteps.value("send"), step3);
    QCOMPARE(tutorial.d->mCurrentStep, (Step*)0);
}

void TutorialTest::testAddStepTwice() {
    Tutorial tutorial(new TutorialInformation("pearlOrientation"));

    Step* step1 = new Step("record");
    tutorial.addStep(step1);

    Step* step2 = new Step("roll");
    tutorial.addStep(step2);

    Step* step3 = new Step("send");
    tutorial.addStep(step3);

    tutorial.addStep(step1);

    QCOMPARE(step1->parent(), &tutorial);
    QCOMPARE(step2->parent(), &tutorial);
    QCOMPARE(step3->parent(), &tutorial);
    QCOMPARE(tutorial.d->mSteps.size(), 3);
    QCOMPARE(tutorial.d->mSteps.value("record"), step1);
    QCOMPARE(tutorial.d->mSteps.value("roll"), step2);
    QCOMPARE(tutorial.d->mSteps.value("send"), step3);
    QCOMPARE(tutorial.d->mCurrentStep, (Step*)0);
}

void TutorialTest::testAddStepDifferentStepWithSameId() {
    Tutorial tutorial(new TutorialInformation("pearlOrientation"));

    Step* step1 = new Step("record");
    tutorial.addStep(step1);

    Step* step2 = new Step("roll");
    tutorial.addStep(step2);

    Step* step3 = new Step("send");
    tutorial.addStep(step3);
  
    //It will not be added and thus not deleted by parent Tutorial, so it is 
    //created in stack
    Step step4("roll");
    tutorial.addStep(&step4);

    QCOMPARE(step1->parent(), &tutorial);
    QCOMPARE(step2->parent(), &tutorial);
    QCOMPARE(step3->parent(), &tutorial);
    QCOMPARE(tutorial.d->mSteps.size(), 3);
    QCOMPARE(tutorial.d->mSteps.value("record"), step1);
    QCOMPARE(tutorial.d->mSteps.value("roll"), step2);
    QCOMPARE(tutorial.d->mSteps.value("send"), step3);
    QCOMPARE(tutorial.d->mCurrentStep, (Step*)0);
}

void TutorialTest::testStart() {
    MockTutorial tutorial(new TutorialInformation("pearlOrientation"));

    Step* stepStart = new Step("start");
    tutorial.addStep(stepStart);

    tutorial.addStep(new Step("record"));
    tutorial.addStep(new Step("roll"));
    tutorial.addStep(new Step("send"));

    //Step* must be registered in order to be used with QSignalSpy
    int stepStarType = qRegisterMetaType<Step*>("Step*");
    QSignalSpy stepActivatedSpy(&tutorial, SIGNAL(stepActivated(Step*)));

    //Tutorial* must be registered in order to be used with QSignalSpy
    qRegisterMetaType<Tutorial*>("Tutorial*");
    QSignalSpy finishedSpy(&tutorial, SIGNAL(finished(Tutorial*)));

    tutorial.start();

    QCOMPARE(tutorial.mSetupCount, 1);
    QCOMPARE(stepActivatedSpy.count(), 1);
    QVariant argument = stepActivatedSpy.at(0).at(0);
    QCOMPARE(argument.userType(), stepStarType);
    QCOMPARE(qvariant_cast<Step*>(argument), stepStart);
    QCOMPARE(tutorial.d->mCurrentStep, stepStart);
    QVERIFY(stepStart->isActive());
    QCOMPARE(finishedSpy.count(), 0);
    QCOMPARE(tutorial.mTearDownCount, 0);
}

void TutorialTest::testStartNoStartStep() {
    MockTutorial tutorial(new TutorialInformation("pearlOrientation"));

    tutorial.addStep(new Step("record"));
    tutorial.addStep(new Step("roll"));
    tutorial.addStep(new Step("send"));

    //Step* must be registered in order to be used with QSignalSpy
    qRegisterMetaType<Step*>("Step*");
    QSignalSpy stepActivatedSpy(&tutorial, SIGNAL(stepActivated(Step*)));

    //Tutorial* must be registered in order to be used with QSignalSpy
    int tutorialStarType = qRegisterMetaType<Tutorial*>("Tutorial*");
    QSignalSpy finishedSpy(&tutorial, SIGNAL(finished(Tutorial*)));

    tutorial.start();

    QCOMPARE(tutorial.mSetupCount, 1);
    QCOMPARE(stepActivatedSpy.count(), 0);
    QCOMPARE(tutorial.d->mCurrentStep, (Step*)0);
    QCOMPARE(finishedSpy.count(), 1);
    QVariant argument = finishedSpy.at(0).at(0);
    QCOMPARE(argument.userType(), tutorialStarType);
    QCOMPARE(qvariant_cast<Tutorial*>(argument), &tutorial);
    QCOMPARE(tutorial.mTearDownCount, 1);
}

void TutorialTest::testNextStepId() {
    Tutorial tutorial(new TutorialInformation("pearlOrientation"));

    Step* stepStart = new Step("start");
    tutorial.addStep(stepStart);

    Step* step1 = new Step("record");
    tutorial.addStep(step1);

    tutorial.addStep(new Step("roll"));
    tutorial.addStep(new Step("send"));

    tutorial.start();

    //Step* must be registered in order to be used with QSignalSpy
    int stepStarType = qRegisterMetaType<Step*>("Step*");
    QSignalSpy stepActivatedSpy(&tutorial, SIGNAL(stepActivated(Step*)));

    //Ensure that the step is already active when the signal is emitted
    connect(&tutorial, SIGNAL(stepActivated(Step*)),
            this, SLOT(assertStepActive(Step*)));

    tutorial.nextStep("record");

    QCOMPARE(stepActivatedSpy.count(), 1);
    QVariant argument = stepActivatedSpy.at(0).at(0);
    QCOMPARE(argument.userType(), stepStarType);
    QCOMPARE(qvariant_cast<Step*>(argument), step1);
    QCOMPARE(tutorial.d->mCurrentStep, step1);
    QVERIFY(!stepStart->isActive());
    QVERIFY(step1->isActive());
}

void TutorialTest::testNextStepIdRequestedFromStep() {
    Tutorial tutorial(new TutorialInformation("pearlOrientation"));

    StepToRequestNextStep* stepStart = new StepToRequestNextStep("start");
    tutorial.addStep(stepStart);

    Step* step1 = new Step("record");
    tutorial.addStep(step1);

    tutorial.addStep(new Step("roll"));
    tutorial.addStep(new Step("send"));

    tutorial.start();

    //Step* must be registered in order to be used with QSignalSpy
    int stepStarType = qRegisterMetaType<Step*>("Step*");
    QSignalSpy stepActivatedSpy(&tutorial, SIGNAL(stepActivated(Step*)));

    //Ensure that the step is already active when the signal is emitted
    connect(&tutorial, SIGNAL(stepActivated(Step*)),
            this, SLOT(assertStepActive(Step*)));

    stepStart->emitNextStepRequested("record");

    QCOMPARE(stepActivatedSpy.count(), 1);
    QVariant argument = stepActivatedSpy.at(0).at(0);
    QCOMPARE(argument.userType(), stepStarType);
    QCOMPARE(qvariant_cast<Step*>(argument), step1);
    QCOMPARE(tutorial.d->mCurrentStep, step1);
    QVERIFY(!stepStart->isActive());
    QVERIFY(step1->isActive());
}

void TutorialTest::testNextStepIdWithInvalidId() {
    Tutorial tutorial(new TutorialInformation("pearlOrientation"));

    Step* stepStart = new Step("start");
    tutorial.addStep(stepStart);

    Step* step1 = new Step("record");
    tutorial.addStep(step1);

    tutorial.addStep(new Step("roll"));
    tutorial.addStep(new Step("send"));

    tutorial.start();
    tutorial.nextStep("record");

    //Step* must be registered in order to be used with QSignalSpy
    qRegisterMetaType<Step*>("Step*");
    QSignalSpy stepActivatedSpy(&tutorial, SIGNAL(stepActivated(Step*)));

    tutorial.nextStep("idNotAdded");

    QCOMPARE(stepActivatedSpy.count(), 0);
    QCOMPARE(tutorial.d->mCurrentStep, step1);
    QVERIFY(!stepStart->isActive());
    QVERIFY(step1->isActive());
}

void TutorialTest::testNextStepStep() {
    Tutorial tutorial(new TutorialInformation("pearlOrientation"));

    Step* stepStart = new Step("start");
    tutorial.addStep(stepStart);

    Step* step1 = new Step("record");
    tutorial.addStep(step1);

    tutorial.addStep(new Step("roll"));
    tutorial.addStep(new Step("send"));

    tutorial.start();

    //Step* must be registered in order to be used with QSignalSpy
    int stepStarType = qRegisterMetaType<Step*>("Step*");
    QSignalSpy stepActivatedSpy(&tutorial, SIGNAL(stepActivated(Step*)));

    //Ensure that the step is already active when the signal is emitted
    connect(&tutorial, SIGNAL(stepActivated(Step*)),
            this, SLOT(assertStepActive(Step*)));

    tutorial.nextStep(step1);

    QCOMPARE(stepActivatedSpy.count(), 1);
    QVariant argument = stepActivatedSpy.at(0).at(0);
    QCOMPARE(argument.userType(), stepStarType);
    QCOMPARE(qvariant_cast<Step*>(argument), step1);
    QCOMPARE(tutorial.d->mCurrentStep, step1);
    QVERIFY(!stepStart->isActive());
    QVERIFY(step1->isActive());
}

void TutorialTest::testNextStepStepWithInvalidStep() {
    Tutorial tutorial(new TutorialInformation("pearlOrientation"));

    Step* stepStart = new Step("start");
    tutorial.addStep(stepStart);

    Step* step1 = new Step("record");
    tutorial.addStep(step1);

    tutorial.addStep(new Step("roll"));
    tutorial.addStep(new Step("send"));

    tutorial.start();
    tutorial.nextStep("record");

    //Step* must be registered in order to be used with QSignalSpy
    qRegisterMetaType<Step*>("Step*");
    QSignalSpy stepActivatedSpy(&tutorial, SIGNAL(stepActivated(Step*)));
 
    Step stepNotAdded("id");
    tutorial.nextStep(&stepNotAdded);

    QCOMPARE(stepActivatedSpy.count(), 0);
    QCOMPARE(tutorial.d->mCurrentStep, step1);
    QVERIFY(!stepStart->isActive());
    QVERIFY(step1->isActive());
}

//Ensure that if some step calls Tutorial::nextStep from its setup (that is,
//when the step is being activated), the next step is not activated until the
//activation of the previous one ended (that is, steps are not activated
//recursively, but sequentially).
void TutorialTest::testNextStepFromStepSetup() {
    Tutorial tutorial(new TutorialInformation("pearlOrientation"));

    Step* stepStart = new Step("start");
    tutorial.addStep(stepStart);

    StepToSetNextStepInSetup* step1 = new StepToSetNextStepInSetup("record");
    tutorial.addStep(step1);

    StepToAssertNoRecursiveStepActivation* step2 =
                            new StepToAssertNoRecursiveStepActivation("roll");
    tutorial.addStep(step2);

    step1->mTutorial = &tutorial;
    step1->mNextStep = step2;
    step2->mPreviousStep = step1;

    tutorial.addStep(new Step("send"));

    tutorial.start();

    //Step* must be registered in order to be used with QSignalSpy
    int stepStarType = qRegisterMetaType<Step*>("Step*");
    QSignalSpy stepActivatedSpy(&tutorial, SIGNAL(stepActivated(Step*)));

    //Ensure that the step is already active when the signal is emitted
    connect(&tutorial, SIGNAL(stepActivated(Step*)),
            this, SLOT(assertStepActive(Step*)));

    tutorial.nextStep(step1);

    QCOMPARE(stepActivatedSpy.count(), 2);
    QVariant argument = stepActivatedSpy.at(0).at(0);
    QCOMPARE(argument.userType(), stepStarType);
    QCOMPARE(qvariant_cast<Step*>(argument), step1);
    argument = stepActivatedSpy.at(1).at(0);
    QCOMPARE(argument.userType(), stepStarType);
    QCOMPARE(qvariant_cast<Step*>(argument), step2);
    QCOMPARE(tutorial.d->mCurrentStep, step2);
    QVERIFY(!stepStart->isActive());
    QVERIFY(!step1->isActive());
    QVERIFY(step2->isActive());
}

void TutorialTest::testFinish() {
    MockTutorial tutorial(new TutorialInformation("pearlOrientation"));

    Step* stepStart = new Step("start");
    tutorial.addStep(stepStart);

    //Tutorial* must be registered in order to be used with QSignalSpy
    int tutorialStarType = qRegisterMetaType<Tutorial*>("Tutorial*");
    QSignalSpy finishedSpy(&tutorial, SIGNAL(finished(Tutorial*)));

    tutorial.start();
    tutorial.finish();

    QCOMPARE(tutorial.d->mCurrentStep, (Step*)0);
    QVERIFY(!stepStart->isActive());
    QCOMPARE(finishedSpy.count(), 1);
    QVariant argument = finishedSpy.at(0).at(0);
    QCOMPARE(argument.userType(), tutorialStarType);
    QCOMPARE(qvariant_cast<Tutorial*>(argument), &tutorial);
    QCOMPARE(tutorial.mTearDownCount, 1);
}

}

QTEST_MAIN(ktutorial::TutorialTest)

#include "TutorialTest.moc"
