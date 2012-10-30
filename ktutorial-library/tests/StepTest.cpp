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

#include <QTest>

#include <QSignalSpy>

#define protected public
#define private public
#include "Step.h"
#undef private
#undef protected

#include "Option.h"
#include "WaitForSignal.h"

namespace ktutorial {

class StepTest: public QObject {
Q_OBJECT

public slots:

    void dummySlot() {
        mDummySlotCallCount++;
    }

    void anotherDummySlot() {
        mAnotherDummySlotCallCount++;
    }

signals:

    void dummySignal();

    void anotherDummySignal();

    void thirdDummySignal();

    void fourthDummySignal();

private:

    int mDummySlotCallCount;
    int mAnotherDummySlotCallCount;

    void assertStepId(const QSignalSpy& spy, int index, const QString& stepId);

private slots:

    void init() {
        mDummySlotCallCount = 0;
        mAnotherDummySlotCallCount = 0;
    }

    void testConstructor();

    void testSetText();

    void testSetActiveTrue();
    void testSetActiveFalse();

    void testAddOption();
    void testAddOptionWithoutSlotMacro();
    void testAddOptionAssociatedToStepId();
    void testAddOptionSeveralOptions();
    void testAddOptionDuringSetup();
    void testAddOptionNormalAndDuringSetup();
    void testAddOptionTwice();
    void testAddOptionDifferentOptionsWithSameName();

    void testAddWaitFor();
    void testAddWaitForWithoutSlotMacro();
    void testAddWaitForAssociatedToStepId();
    void testAddWaitForSeveralWaitFors();
    void testAddWaitForDuringSetup();
    void testAddWaitForNormalAndDuringSetup();
    void testAddWaitForTwice();

    void testRemoveOption();
    void testRemoveOptionAssociatedToStepId();
    void testRemoveOptionSeveralOptions();

    void testRemoveWaitFor();
    void testRemoveWaitForAssociatedToStepId();
    void testRemoveWaitForSeveralWaitFors();

};

class InspectedStep: public Step {
public:

    int mSetupCount;
    int mTearDownCount;

    InspectedStep(const QString& id): Step(id) {
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

class StepWithWaitForAddedInSetup: public Step {
public:

    StepTest* mStepTest;
    WaitFor* mWaitFor;

    StepWithWaitForAddedInSetup(const QString& id, StepTest* stepTest):
            Step(id),
        mStepTest(stepTest),
        mWaitFor(0) {
    }

protected:

    virtual void setup() {
        mWaitFor = new WaitForSignal(mStepTest, SIGNAL(dummySignal()));
        addWaitFor(mWaitFor, mStepTest, SLOT(dummySlot()));
    }

};

class StepWithOptionAddedInSetup: public Step {
public:

    StepTest* mStepTest;
    Option* mOption;

    StepWithOptionAddedInSetup(const QString& id, StepTest* stepTest):
            Step(id),
        mStepTest(stepTest),
        mOption(0) {
    }

protected:

    virtual void setup() {
        mOption = new Option("Bathe your iguana");
        addOption(mOption, mStepTest, SLOT(dummySlot()));
    }

};

void StepTest::testConstructor() {
    InspectedStep step("doSomethingConstructive");

    QCOMPARE(step.id(), QString("doSomethingConstructive"));
    QCOMPARE(step.options().size(), 0);
    QCOMPARE(step.text(), QString(""));
    QVERIFY(!step.isActive());
    QCOMPARE(step.mSetupCount, 0);
    QCOMPARE(step.mTearDownCount, 0);
}

void StepTest::testSetText() {
    Step step("doSomethingConstructive");

    step.setText("OK, you beat the game, now turn off your computer and do something constructive");

    QCOMPARE(step.text(), QString("OK, you beat the game, now turn off your computer and do something constructive"));
}

void StepTest::testSetActiveTrue() {
    InspectedStep step("doSomethingConstructive");

    step.setActive(true);

    QVERIFY(step.isActive());
    QCOMPARE(step.mSetupCount, 1);
    QCOMPARE(step.mTearDownCount, 0);
}

void StepTest::testSetActiveFalse() {
    InspectedStep step("doSomethingConstructive");

    step.setActive(false);

    QVERIFY(!step.isActive());
    QCOMPARE(step.mSetupCount, 0);
    QCOMPARE(step.mTearDownCount, 1);
}

void StepTest::testAddOption() {
    Step step("doSomethingConstructive");

    Option* option1 = new Option("Bathe your iguana");

    step.addOption(option1, this, SLOT(dummySlot()));
    connect(this, SIGNAL(dummySignal()), option1, SIGNAL(selected()));

    QCOMPARE(option1->parent(), &step);
    QCOMPARE(step.options().count(), 1);
    QVERIFY(step.options().contains(option1));
    QCOMPARE(mDummySlotCallCount, 0);

    emit dummySignal();
    QCOMPARE(mDummySlotCallCount, 0);

    step.setActive(true);
    emit dummySignal();
    QCOMPARE(mDummySlotCallCount, 1);
}

void StepTest::testAddOptionWithoutSlotMacro() {
    Step step("doSomethingConstructive");

    Option* option1 = new Option("Bathe your iguana");

    step.addOption(option1, this, "dummySlot()");
    connect(this, SIGNAL(dummySignal()), option1, SIGNAL(selected()));

    QCOMPARE(option1->parent(), &step);
    QCOMPARE(step.options().count(), 1);
    QVERIFY(step.options().contains(option1));
    QCOMPARE(mDummySlotCallCount, 0);

    emit dummySignal();
    QCOMPARE(mDummySlotCallCount, 0);

    step.setActive(true);
    emit dummySignal();
    QCOMPARE(mDummySlotCallCount, 1);
}

void StepTest::testAddOptionAssociatedToStepId() {
    Step step("doSomethingConstructive");

    Option* option1 = new Option("Bathe your iguana");

    step.addOption(option1, "batheYourIguanaStep");
    connect(this, SIGNAL(dummySignal()), option1, SIGNAL(selected()));

    QSignalSpy nextStepRequestedSpy(&step, SIGNAL(nextStepRequested(QString)));

    QCOMPARE(option1->parent(), &step);
    QCOMPARE(step.options().count(), 1);
    QVERIFY(step.options().contains(option1));
    QCOMPARE(nextStepRequestedSpy.count(), 0);

    emit dummySignal();
    QCOMPARE(nextStepRequestedSpy.count(), 0);

    step.setActive(true);
    emit dummySignal();
    QCOMPARE(nextStepRequestedSpy.count(), 1);
    assertStepId(nextStepRequestedSpy, 0, "batheYourIguanaStep");
}

void StepTest::testAddOptionSeveralOptions() {
    Step step("doSomethingConstructive");

    Option* option1 = new Option("Bathe your iguana");
    step.addOption(option1, this, SLOT(dummySlot()));
    connect(this, SIGNAL(dummySignal()), option1, SIGNAL(selected()));

    Option* option2 = new Option("Feed a toucan");
    step.addOption(option2, this, SLOT(dummySlot()));
    connect(this, SIGNAL(anotherDummySignal()), option2, SIGNAL(selected()));

    Option* option3 = new Option("Walk the platypus");
    step.addOption(option3, this, SLOT(dummySlot()));
    connect(this, SIGNAL(dummySignal()), option3, SIGNAL(selected()));

    Option* option4 = new Option("Lull the penguin");
    step.addOption(option4, "lullThePenguinStep");
    connect(this, SIGNAL(anotherDummySignal()), option4, SIGNAL(selected()));

    Option* option5 = new Option("Pamper the Tasmanian devil");
    step.addOption(option5, "pamperTheTasmanianDevilStep");
    connect(this, SIGNAL(dummySignal()), option5, SIGNAL(selected()));

    QSignalSpy nextStepRequestedSpy(&step, SIGNAL(nextStepRequested(QString)));

    QCOMPARE(option1->parent(), &step);
    QCOMPARE(option2->parent(), &step);
    QCOMPARE(option3->parent(), &step);
    QCOMPARE(option4->parent(), &step);
    QCOMPARE(option5->parent(), &step);
    QCOMPARE(step.options().count(), 5);
    QVERIFY(step.options().contains(option1));
    QVERIFY(step.options().contains(option2));
    QVERIFY(step.options().contains(option3));
    QVERIFY(step.options().contains(option4));
    QVERIFY(step.options().contains(option5));
    QCOMPARE(mDummySlotCallCount, 0);
    QCOMPARE(nextStepRequestedSpy.count(), 0);

    emit dummySignal();
    emit anotherDummySignal();
    QCOMPARE(mDummySlotCallCount, 0);
    QCOMPARE(nextStepRequestedSpy.count(), 0);

    step.setActive(true);
    emit anotherDummySignal();
    QCOMPARE(mDummySlotCallCount, 1);
    QCOMPARE(nextStepRequestedSpy.count(), 1);
    assertStepId(nextStepRequestedSpy, 0, "lullThePenguinStep");

    emit dummySignal();
    QCOMPARE(mDummySlotCallCount, 3);
    QCOMPARE(nextStepRequestedSpy.count(), 2);
    assertStepId(nextStepRequestedSpy, 1, "pamperTheTasmanianDevilStep");
}

void StepTest::testAddOptionDuringSetup() {
    StepWithOptionAddedInSetup step("doSomethingConstructive", this);

    step.setActive(true);

    connect(this, SIGNAL(dummySignal()), step.mOption, SIGNAL(selected()));

    QCOMPARE(step.mOption->parent(), &step);
    QCOMPARE(step.options().count(), 1);
    QVERIFY(step.options().contains(step.mOption));
    QCOMPARE(mDummySlotCallCount, 0);

    emit dummySignal();
    QCOMPARE(mDummySlotCallCount, 1);

    QSignalSpy destroyedSpy(step.mOption, SIGNAL(destroyed(QObject*)));

    step.setActive(false);

    QCOMPARE(step.options().count(), 0);
    QCOMPARE(step.mWaitsForToBeDeletedInTearDown.count(), 0);
    QCOMPARE(step.mOptionsToBeDeletedInTearDown.count(), 0);
    QCOMPARE(destroyedSpy.count(), 1);
}

void StepTest::testAddOptionNormalAndDuringSetup() {
    StepWithOptionAddedInSetup step("doSomethingConstructive", this);

    Option* option1 = new Option("Feed a toucan");

    step.addOption(option1, this, SLOT(anotherDummySlot()));
    connect(this, SIGNAL(anotherDummySignal()), option1, SIGNAL(selected()));

    step.setActive(true);

    connect(this, SIGNAL(dummySignal()), step.mOption, SIGNAL(selected()));

    emit dummySignal();
    QCOMPARE(mDummySlotCallCount, 1);

    emit anotherDummySignal();
    QCOMPARE(mAnotherDummySlotCallCount, 1);

    QSignalSpy destroyedSpy(step.mOption, SIGNAL(destroyed(QObject*)));

    step.setActive(false);

    QCOMPARE(step.options().count(), 1);
    QVERIFY(step.options().contains(option1));
    QCOMPARE(step.mWaitsForToBeDeletedInTearDown.count(), 0);
    QCOMPARE(step.mOptionsToBeDeletedInTearDown.count(), 0);
    QCOMPARE(destroyedSpy.count(), 1);
}

void StepTest::testAddOptionTwice() {
    Step step("doSomethingConstructive");

    Option* option1 = new Option("Bathe your iguana");
    step.addOption(option1, this, SLOT(dummySlot()));
    connect(this, SIGNAL(dummySignal()), option1, SIGNAL(selected()));

    Option* option2 = new Option("Feed a toucan");
    step.addOption(option2, "feedAToucanStep");
    connect(this, SIGNAL(dummySignal()), option2, SIGNAL(selected()));

    step.addOption(option1, this, SLOT(anotherDummySlot()));
    step.addOption(option2, "feedAPigeonStep");

    QCOMPARE(option1->parent(), &step);
    QCOMPARE(option2->parent(), &step);
    QCOMPARE(step.options().count(), 2);
    QVERIFY(step.options().contains(option1));
    QVERIFY(step.options().contains(option2));

    QSignalSpy nextStepRequestedSpy(&step, SIGNAL(nextStepRequested(QString)));

    step.setActive(true);
    emit dummySignal();
    QCOMPARE(mDummySlotCallCount, 1);
    QCOMPARE(mAnotherDummySlotCallCount, 0);
    QCOMPARE(nextStepRequestedSpy.count(), 1);
    assertStepId(nextStepRequestedSpy, 0, "feedAToucanStep");
}

void StepTest::testAddOptionDifferentOptionsWithSameName() {
    Step step("doSomethingConstructive");

    Option* option1 = new Option("Bathe your iguana");
    step.addOption(option1, this, SLOT(dummySlot()));
    connect(this, SIGNAL(dummySignal()), option1, SIGNAL(selected()));

    Option* option2 = new Option("Feed a toucan");
    step.addOption(option2, "feedAToucanStep");
    connect(this, SIGNAL(dummySignal()), option2, SIGNAL(selected()));

    //They will not be added and thus not deleted by parent Step, so they are
    //created in stack
    Option option3("Bathe your iguana");
    Option option4("Feed a toucan");

    step.addOption(&option3, this, SLOT(anotherDummySlot()));
    connect(this, SIGNAL(dummySignal()), &option3, SIGNAL(selected()));

    step.addOption(&option4, "feedAToucanStep2");
    connect(this, SIGNAL(dummySignal()), &option4, SIGNAL(selected()));

    QCOMPARE(option1->parent(), &step);
    QCOMPARE(option2->parent(), &step);
    QCOMPARE(option3.parent(), (QObject*)0);
    QCOMPARE(option4.parent(), (QObject*)0);
    QCOMPARE(step.options().count(), 2);
    QVERIFY(step.options().contains(option1));
    QVERIFY(step.options().contains(option2));

    QSignalSpy nextStepRequestedSpy(&step, SIGNAL(nextStepRequested(QString)));

    step.setActive(true);
    emit dummySignal();
    QCOMPARE(mDummySlotCallCount, 1);
    QCOMPARE(mAnotherDummySlotCallCount, 0);
    QCOMPARE(nextStepRequestedSpy.count(), 1);
    assertStepId(nextStepRequestedSpy, 0, "feedAToucanStep");
}

void StepTest::testAddWaitFor() {
    Step step("doSomethingConstructive");

    WaitFor* waitFor1 = new WaitForSignal(this, SIGNAL(dummySignal()));

    step.addWaitFor(waitFor1, this, SLOT(dummySlot()));

    QCOMPARE(waitFor1->parent(), &step);
    QCOMPARE(step.mWaitsFor.count(), 1);
    QVERIFY(step.mWaitsFor.contains(waitFor1));
    QCOMPARE(mDummySlotCallCount, 0);

    emit dummySignal();
    QCOMPARE(mDummySlotCallCount, 0);

    step.setActive(true);
    emit dummySignal();
    QCOMPARE(mDummySlotCallCount, 1);
}

void StepTest::testAddWaitForWithoutSlotMacro() {
    Step step("doSomethingConstructive");

    WaitFor* waitFor1 = new WaitForSignal(this, SIGNAL(dummySignal()));

    step.addWaitFor(waitFor1, this, "dummySlot()");

    QCOMPARE(waitFor1->parent(), &step);
    QCOMPARE(step.mWaitsFor.count(), 1);
    QVERIFY(step.mWaitsFor.contains(waitFor1));
    QCOMPARE(mDummySlotCallCount, 0);

    emit dummySignal();
    QCOMPARE(mDummySlotCallCount, 0);

    step.setActive(true);
    emit dummySignal();
    QCOMPARE(mDummySlotCallCount, 1);
}

void StepTest::testAddWaitForAssociatedToStepId() {
    Step step("doSomethingConstructive");

    WaitFor* waitFor1 = new WaitForSignal(this, SIGNAL(dummySignal()));

    step.addWaitFor(waitFor1, "batheYourIguanaStep");

    QSignalSpy nextStepRequestedSpy(&step, SIGNAL(nextStepRequested(QString)));

    QCOMPARE(waitFor1->parent(), &step);
    QCOMPARE(step.mWaitsFor.count(), 1);
    QVERIFY(step.mWaitsFor.contains(waitFor1));
    QCOMPARE(nextStepRequestedSpy.count(), 0);

    emit dummySignal();
    QCOMPARE(nextStepRequestedSpy.count(), 0);

    step.setActive(true);
    emit dummySignal();
    QCOMPARE(nextStepRequestedSpy.count(), 1);
    assertStepId(nextStepRequestedSpy, 0, "batheYourIguanaStep");
}

void StepTest::testAddWaitForSeveralWaitFors() {
    Step step("doSomethingConstructive");

    WaitFor* waitFor1 = new WaitForSignal(this, SIGNAL(dummySignal()));
    step.addWaitFor(waitFor1, this, SLOT(dummySlot()));

    WaitFor* waitFor2 = new WaitForSignal(this, SIGNAL(anotherDummySignal()));
    step.addWaitFor(waitFor2, this, SLOT(dummySlot()));

    WaitFor* waitFor3 = new WaitForSignal(this, SIGNAL(thirdDummySignal()));
    step.addWaitFor(waitFor3, "batheYourIguanaStep");

    WaitFor* waitFor4 = new WaitForSignal(this, SIGNAL(fourthDummySignal()));
    step.addWaitFor(waitFor4, "feedAToucanStep");

    QSignalSpy nextStepRequestedSpy(&step, SIGNAL(nextStepRequested(QString)));

    QCOMPARE(waitFor1->parent(), &step);
    QCOMPARE(waitFor2->parent(), &step);
    QCOMPARE(waitFor3->parent(), &step);
    QCOMPARE(waitFor4->parent(), &step);
    QCOMPARE(step.mWaitsFor.count(), 4);
    QVERIFY(step.mWaitsFor.contains(waitFor1));
    QVERIFY(step.mWaitsFor.contains(waitFor2));
    QVERIFY(step.mWaitsFor.contains(waitFor3));
    QVERIFY(step.mWaitsFor.contains(waitFor4));
    QCOMPARE(mDummySlotCallCount, 0);
    QCOMPARE(nextStepRequestedSpy.count(), 0);

    emit dummySignal();
    emit anotherDummySignal();
    emit thirdDummySignal();
    emit fourthDummySignal();
    QCOMPARE(mDummySlotCallCount, 0);
    QCOMPARE(nextStepRequestedSpy.count(), 0);

    step.setActive(true);
    emit anotherDummySignal();
    QCOMPARE(mDummySlotCallCount, 1);
    QCOMPARE(nextStepRequestedSpy.count(), 0);

    emit dummySignal();
    QCOMPARE(mDummySlotCallCount, 2);
    QCOMPARE(nextStepRequestedSpy.count(), 0);

    emit thirdDummySignal();
    QCOMPARE(mDummySlotCallCount, 2);
    QCOMPARE(nextStepRequestedSpy.count(), 1);
    assertStepId(nextStepRequestedSpy, 0, "batheYourIguanaStep");

    emit fourthDummySignal();
    QCOMPARE(mDummySlotCallCount, 2);
    QCOMPARE(nextStepRequestedSpy.count(), 2);
    assertStepId(nextStepRequestedSpy, 1, "feedAToucanStep");
}

void StepTest::testAddWaitForDuringSetup() {
    StepWithWaitForAddedInSetup step("doSomethingConstructive", this);

    step.setActive(true);

    QCOMPARE(step.mWaitFor->parent(), &step);
    QCOMPARE(step.mWaitsFor.count(), 1);
    QVERIFY(step.mWaitsFor.contains(step.mWaitFor));
    QCOMPARE(mDummySlotCallCount, 0);

    emit dummySignal();
    QCOMPARE(mDummySlotCallCount, 1);

    QSignalSpy destroyedSpy(step.mWaitFor, SIGNAL(destroyed(QObject*)));

    step.setActive(false);

    QCOMPARE(step.mWaitsFor.count(), 0);
    QCOMPARE(step.mWaitsForToBeDeletedInTearDown.count(), 0);
    QCOMPARE(step.mOptionsToBeDeletedInTearDown.count(), 0);
    QCOMPARE(destroyedSpy.count(), 1);
}

void StepTest::testAddWaitForNormalAndDuringSetup() {
    StepWithWaitForAddedInSetup step("doSomethingConstructive", this);

    WaitFor* waitFor1 = new WaitForSignal(this, SIGNAL(anotherDummySignal()));

    step.addWaitFor(waitFor1, this, SLOT(anotherDummySlot()));

    step.setActive(true);

    emit dummySignal();
    QCOMPARE(mDummySlotCallCount, 1);

    emit anotherDummySignal();
    QCOMPARE(mAnotherDummySlotCallCount, 1);

    QSignalSpy destroyedSpy(step.mWaitFor, SIGNAL(destroyed(QObject*)));

    step.setActive(false);

    QCOMPARE(step.mWaitsFor.count(), 1);
    QVERIFY(step.mWaitsFor.contains(waitFor1));
    QCOMPARE(step.mWaitsForToBeDeletedInTearDown.count(), 0);
    QCOMPARE(step.mOptionsToBeDeletedInTearDown.count(), 0);
    QCOMPARE(destroyedSpy.count(), 1);
}

void StepTest::testAddWaitForTwice() {
    Step step("doSomethingConstructive");

    WaitFor* waitFor1 = new WaitForSignal(this, SIGNAL(dummySignal()));
    step.addWaitFor(waitFor1, this, SLOT(dummySlot()));

    WaitFor* waitFor2 = new WaitForSignal(this, SIGNAL(anotherDummySignal()));
    step.addWaitFor(waitFor2, "batheYourIguanaStep");

    step.addWaitFor(waitFor1, this, SLOT(anotherDummySlot()));
    step.addWaitFor(waitFor2, "batheYourChameleonStep");

    QCOMPARE(waitFor1->parent(), &step);
    QCOMPARE(waitFor2->parent(), &step);
    QCOMPARE(step.mWaitsFor.count(), 2);
    QVERIFY(step.mWaitsFor.contains(waitFor1));
    QVERIFY(step.mWaitsFor.contains(waitFor2));

    step.setActive(true);
    emit dummySignal();
    QCOMPARE(mDummySlotCallCount, 1);
    QCOMPARE(mAnotherDummySlotCallCount, 0);

    QSignalSpy nextStepRequestedSpy(&step, SIGNAL(nextStepRequested(QString)));

    emit anotherDummySignal();
    QCOMPARE(nextStepRequestedSpy.count(), 1);
    assertStepId(nextStepRequestedSpy, 0, "batheYourIguanaStep");
}

void StepTest::testRemoveOption() {
    Step step("doSomethingConstructive");

    Option* option1 = new Option("Bathe your iguana");
    step.addOption(option1, this, SLOT(dummySlot()));
    connect(this, SIGNAL(dummySignal()), option1, SIGNAL(selected()));

    //It will be removed and not deleted by parent Step, so it is created in 
    //stack
    Option option2("Feed a toucan");
    step.addOption(&option2, this, SLOT(anotherDummySlot()));
    connect(this, SIGNAL(dummySignal()), &option2, SIGNAL(selected()));

    step.removeOption(&option2);

    QCOMPARE(option1->parent(), &step);
    QCOMPARE(option2.parent(), (QObject*)0);
    QCOMPARE(step.options().count(), 1);
    QVERIFY(step.options().contains(option1));
    QVERIFY(!step.options().contains(&option2));
    QCOMPARE(mDummySlotCallCount, 0);
    QCOMPARE(mAnotherDummySlotCallCount, 0);

    step.setActive(true);
    emit dummySignal();
    QCOMPARE(mDummySlotCallCount, 1);
    QCOMPARE(mAnotherDummySlotCallCount, 0);
}

void StepTest::testRemoveOptionAssociatedToStepId() {
    Step step("doSomethingConstructive");

    Option* option1 = new Option("Bathe your iguana");
    step.addOption(option1, "batheYourIguanaStep");
    connect(this, SIGNAL(dummySignal()), option1, SIGNAL(selected()));

    //It will be removed and not deleted by parent Step, so it is created in
    //stack
    Option option2("Feed a toucan");
    step.addOption(&option2, "feedAToucanStep");
    connect(this, SIGNAL(dummySignal()), &option2, SIGNAL(selected()));

    step.removeOption(&option2);

    QSignalSpy nextStepRequestedSpy(&step, SIGNAL(nextStepRequested(QString)));

    QCOMPARE(option1->parent(), &step);
    QCOMPARE(option2.parent(), (QObject*)0);
    QCOMPARE(step.options().count(), 1);
    QVERIFY(step.options().contains(option1));
    QVERIFY(!step.options().contains(&option2));
    QCOMPARE(nextStepRequestedSpy.count(), 0);

    step.setActive(true);
    emit dummySignal();
    QCOMPARE(nextStepRequestedSpy.count(), 1);
    assertStepId(nextStepRequestedSpy, 0, "batheYourIguanaStep");
}

void StepTest::testRemoveOptionSeveralOptions() {
    Step step("doSomethingConstructive");

    //They will be removed and not deleted by parent Step, so they are created
    //in stack
    Option option1("Bathe your iguana");
    step.addOption(&option1, this, SLOT(anotherDummySlot()));
    connect(this, SIGNAL(dummySignal()), &option1, SIGNAL(selected()));

    Option option2("Feed a toucan");
    step.addOption(&option2, this, SLOT(dummySlot()));
    connect(this, SIGNAL(dummySignal()), &option2, SIGNAL(selected()));

    Option option3("Walk the platypus");
    step.addOption(&option3, this, SLOT(anotherDummySlot()));
    connect(this, SIGNAL(dummySignal()), &option3, SIGNAL(selected()));

    Option option4("Lull the penguin");
    step.addOption(&option4, "lullThePenguinStep");
    connect(this, SIGNAL(dummySignal()), &option4, SIGNAL(selected()));

    Option option5("Pamper the Tasmanian Devil");
    step.addOption(&option5, "pamperTheTasmanianDevilStep");
    connect(this, SIGNAL(dummySignal()), &option5, SIGNAL(selected()));

    QSignalSpy nextStepRequestedSpy(&step, SIGNAL(nextStepRequested(QString)));

    step.removeOption(&option1);
    step.removeOption(&option3);
    step.removeOption(&option5);

    QCOMPARE(option1.parent(), (QObject*)0);
    QCOMPARE(option2.parent(), &step);
    QCOMPARE(option3.parent(), (QObject*)0);
    QCOMPARE(option4.parent(), &step);
    QCOMPARE(option5.parent(), (QObject*)0);
    QCOMPARE(step.options().count(), 2);
    QVERIFY(step.options().contains(&option2));
    QVERIFY(step.options().contains(&option4));
    QVERIFY(!step.options().contains(&option1));
    QVERIFY(!step.options().contains(&option3));
    QVERIFY(!step.options().contains(&option5));
    QCOMPARE(mDummySlotCallCount, 0);
    QCOMPARE(mAnotherDummySlotCallCount, 0);
    QCOMPARE(nextStepRequestedSpy.count(), 0);

    step.setActive(true);
    emit dummySignal();
    QCOMPARE(mDummySlotCallCount, 1);
    QCOMPARE(mAnotherDummySlotCallCount, 0);
    QCOMPARE(nextStepRequestedSpy.count(), 1);
    assertStepId(nextStepRequestedSpy, 0, "lullThePenguinStep");

    step.removeOption(&option2);

    QCOMPARE(option2.parent(), (QObject*)0);
    QCOMPARE(step.options().count(), 1);
    QVERIFY(step.options().contains(&option4));
    QVERIFY(!step.options().contains(&option2));

    emit dummySignal();
    QCOMPARE(mDummySlotCallCount, 1);
    QCOMPARE(mAnotherDummySlotCallCount, 0);
    QCOMPARE(nextStepRequestedSpy.count(), 2);
    assertStepId(nextStepRequestedSpy, 1, "lullThePenguinStep");

    step.removeOption(&option4);

    QCOMPARE(option4.parent(), (QObject*)0);
    QCOMPARE(step.options().count(), 0);
    QVERIFY(!step.options().contains(&option4));

    emit dummySignal();
    QCOMPARE(mDummySlotCallCount, 1);
    QCOMPARE(mAnotherDummySlotCallCount, 0);
    QCOMPARE(nextStepRequestedSpy.count(), 2);
}

void StepTest::testRemoveWaitFor() {
    Step step("doSomethingConstructive");

    WaitForSignal* waitFor1 = new WaitForSignal(this, SIGNAL(dummySignal()));
    step.addWaitFor(waitFor1, this, SLOT(dummySlot()));

    //It will be removed and not deleted by parent Step, so it is created in
    //stack
    WaitForSignal waitFor2(this, SIGNAL(anotherDummySignal()));
    step.addWaitFor(&waitFor2, this, SLOT(anotherDummySlot()));

    step.setActive(true);

    step.removeWaitFor(&waitFor2);

    QCOMPARE(waitFor1->parent(), &step);
    QCOMPARE(waitFor2.parent(), (QObject*)0);
    QCOMPARE(step.mWaitsFor.count(), 1);
    QVERIFY(step.mWaitsFor.contains(waitFor1));
    QVERIFY(!step.mWaitsFor.contains(&waitFor2));
    QVERIFY(waitFor1->isActive());
    QVERIFY(!waitFor2.isActive());
    QCOMPARE(mDummySlotCallCount, 0);
    QCOMPARE(mAnotherDummySlotCallCount, 0);

    waitFor2.setActive(true);
    emit dummySignal();
    emit anotherDummySignal();
    QCOMPARE(mDummySlotCallCount, 1);
    QCOMPARE(mAnotherDummySlotCallCount, 0);
}

void StepTest::testRemoveWaitForAssociatedToStepId() {
    Step step("doSomethingConstructive");

    WaitForSignal* waitFor1 = new WaitForSignal(this, SIGNAL(dummySignal()));
    step.addWaitFor(waitFor1, "batheYourIguanaStep");

    //It will be removed and not deleted by parent Step, so it is created in
    //stack
    WaitForSignal waitFor2(this, SIGNAL(anotherDummySignal()));
    step.addWaitFor(&waitFor2, "feedAToucanStep");

    QSignalSpy nextStepRequestedSpy(&step, SIGNAL(nextStepRequested(QString)));

    step.setActive(true);

    step.removeWaitFor(&waitFor2);

    QCOMPARE(waitFor1->parent(), &step);
    QCOMPARE(waitFor2.parent(), (QObject*)0);
    QCOMPARE(step.mWaitsFor.count(), 1);
    QVERIFY(step.mWaitsFor.contains(waitFor1));
    QVERIFY(!step.mWaitsFor.contains(&waitFor2));
    QVERIFY(waitFor1->isActive());
    QVERIFY(!waitFor2.isActive());
    QCOMPARE(nextStepRequestedSpy.count(), 0);

    waitFor2.setActive(true);
    emit dummySignal();
    emit anotherDummySignal();
    QCOMPARE(nextStepRequestedSpy.count(), 1);
    assertStepId(nextStepRequestedSpy, 0, "batheYourIguanaStep");
}

void StepTest::testRemoveWaitForSeveralWaitFors() {
    Step step("doSomethingConstructive");

    //They will be removed and not deleted by parent Step, so they are created
    //in stack
    WaitForSignal waitFor1(this, SIGNAL(dummySignal()));
    step.addWaitFor(&waitFor1, this, SLOT(anotherDummySlot()));

    WaitForSignal waitFor2(this, SIGNAL(anotherDummySignal()));
    step.addWaitFor(&waitFor2, this, SLOT(dummySlot()));

    WaitForSignal waitFor3(this, SIGNAL(thirdDummySignal()));
    step.addWaitFor(&waitFor3, "batheYourIguanaStep");

    WaitForSignal waitFor4(this, SIGNAL(fourthDummySignal()));
    step.addWaitFor(&waitFor4, "feedAToucanStep");

    QSignalSpy nextStepRequestedSpy(&step, SIGNAL(nextStepRequested(QString)));

    step.setActive(true);

    step.removeWaitFor(&waitFor1);
    step.removeWaitFor(&waitFor3);

    QCOMPARE(waitFor1.parent(), (QObject*)0);
    QCOMPARE(waitFor2.parent(), &step);
    QCOMPARE(waitFor3.parent(), (QObject*)0);
    QCOMPARE(waitFor4.parent(), &step);
    QCOMPARE(step.mWaitsFor.count(), 2);
    QVERIFY(step.mWaitsFor.contains(&waitFor2));
    QVERIFY(step.mWaitsFor.contains(&waitFor4));
    QVERIFY(!step.mWaitsFor.contains(&waitFor1));
    QVERIFY(!step.mWaitsFor.contains(&waitFor3));
    QVERIFY(!waitFor1.isActive());
    QVERIFY(waitFor2.isActive());
    QVERIFY(!waitFor3.isActive());
    QVERIFY(waitFor4.isActive());
    QCOMPARE(mDummySlotCallCount, 0);
    QCOMPARE(mAnotherDummySlotCallCount, 0);
    QCOMPARE(nextStepRequestedSpy.count(), 0);

    step.setActive(true);
    waitFor1.setActive(true);
    waitFor3.setActive(true);
    emit dummySignal();
    emit anotherDummySignal();
    emit thirdDummySignal();
    emit fourthDummySignal();
    QCOMPARE(mDummySlotCallCount, 1);
    QCOMPARE(mAnotherDummySlotCallCount, 0);
    QCOMPARE(nextStepRequestedSpy.count(), 1);
    assertStepId(nextStepRequestedSpy, 0, "feedAToucanStep");

    step.removeWaitFor(&waitFor2);

    QCOMPARE(waitFor2.parent(), (QObject*)0);
    QCOMPARE(step.mWaitsFor.count(), 1);
    QVERIFY(step.mWaitsFor.contains(&waitFor4));
    QVERIFY(!step.mWaitsFor.contains(&waitFor2));
    QVERIFY(!waitFor2.isActive());

    waitFor2.setActive(true);
    emit dummySignal();
    QCOMPARE(mDummySlotCallCount, 1);

    step.removeWaitFor(&waitFor4);

    QCOMPARE(waitFor4.parent(), (QObject*)0);
    QCOMPARE(step.mWaitsFor.count(), 0);
    QVERIFY(!step.mWaitsFor.contains(&waitFor4));
    QVERIFY(!waitFor4.isActive());

    waitFor4.setActive(true);
    emit fourthDummySignal();
    QCOMPARE(nextStepRequestedSpy.count(), 1);
}

/////////////////////////////////// Helpers ////////////////////////////////////

void StepTest::assertStepId(const QSignalSpy& spy, int index,
                            const QString& stepId) {
    QVariant argument = spy.at(index).at(0);
    QCOMPARE(argument.type(), QVariant::String);
    QCOMPARE(argument.toString(), stepId);
}

}

QTEST_MAIN(ktutorial::StepTest)

#include "StepTest.moc"
