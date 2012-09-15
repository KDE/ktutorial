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

#include <QtTest>
#include <qtest_kde.h>

#include <KTemporaryFile>
#include <kross/core/action.h>
#include <kross/core/manager.h>

#define protected public
#define private public
#include "ScriptedTutorial.h"
#include "../Step.h"
#undef private
#undef protected

#include "ScriptedStep.h"
#include "../KTutorial.h"
#include "../TutorialInformation.h"

using ktutorial::scripting::ScriptedStep;

namespace ktutorial {
namespace scripting {

//Not a unit test class, but more an integration test to check that scripted
//tutorials work as expected
class ScriptingTest: public QObject {
Q_OBJECT
Q_PROPERTY(int intProperty READ intProperty NOTIFY intPropertyChanged)
public:

    int intProperty() const {
        return mIntProperty;
    }

public slots:

    bool booleanValue() {
        return mBooleanValue;
    }

    void setBooleanValue(bool booleanValue) {
        mBooleanValue = booleanValue;
    }

    void dummySlot() {
        mDummySlotCallCount++;
    }

signals:

    void dummySignal();
    void otherDummySignal();
    void anotherDummySignal();

    void intPropertyChanged();

private slots:

    void initTestCase();
    void init();
    void cleanup();

    void setTutorialInformation();

    void basicStep();
    void stepWithCustomSetup();

    void stepsWithOptions();

    void stepsWithWaitFors();

    void fullTutorial();

private:

    bool mBooleanValue;

    int mIntProperty;

    int mDummySlotCallCount;

    KTemporaryFile* mTemporaryFile;

    QObject* getObject(ScriptedTutorial* scriptedTutorial,
                       const QString& function);

};

void ScriptingTest::initTestCase() {
    Kross::Manager::self().addObject(this, "testObject");
}

void ScriptingTest::init() {
    mBooleanValue = false;
    mIntProperty = 0;
    mDummySlotCallCount = 0;

    mTemporaryFile = new KTemporaryFile();
    mTemporaryFile->setSuffix(".js");
    mTemporaryFile->open();
}

void ScriptingTest::cleanup() {
    delete mTemporaryFile;
}

void ScriptingTest::setTutorialInformation() {
    QTextStream out(mTemporaryFile);
    out << "tutorial.tutorialInformationAsObject().setName(\
\"Test tutorial\");\n";
    out << "tutorial.tutorialInformationAsObject().setDescription(\
\"A tutorial to test scripting\");\n";
    out.flush();

    ScriptedTutorial scriptedTutorial(mTemporaryFile->fileName());

    QVERIFY(scriptedTutorial.isValid());

    QCOMPARE(scriptedTutorial.tutorialInformation()->id(),
             mTemporaryFile->fileName());
    QCOMPARE(scriptedTutorial.tutorialInformation()->name(),
             QString("Test tutorial"));
    QCOMPARE(scriptedTutorial.tutorialInformation()->description(),
             QString("A tutorial to test scripting"));
}

void ScriptingTest::basicStep() {
    QTextStream out(mTemporaryFile);
    out << "startStep = ktutorial.newStep(\"start\");\n";
    out << "startStep.setText(\"The start step\");\n";
    out << "tutorial.addStep(startStep);\n";
    out.flush();

    ScriptedTutorial scriptedTutorial(mTemporaryFile->fileName());

    QVERIFY(scriptedTutorial.isValid());

    scriptedTutorial.start();

    QCOMPARE(scriptedTutorial.mCurrentStep->id(), QString("start"));
    QCOMPARE(scriptedTutorial.mCurrentStep->text(), QString("The start step"));
}

void ScriptingTest::stepWithCustomSetup() {
    QTextStream out(mTemporaryFile);
    out << "function startStepSetup(step) {\n";
    out << "    waitFor = ktutorial.newWaitFor(\"WaitForSignal\");\n";
    out << "    waitFor.setSignal(testObject, \"dummySignal()\");\n";
    out << "    step.addWaitFor(waitFor, self, \"setFirstStep()\");\n";
    out << "}\n";
    out << "function startStepTearDown(step) {\n";
    out << "    testObject.setBooleanValue(true);\n";
    out << "}\n";

    out << "function setFirstStep() { tutorial.nextStep(\"first\"); }\n";

    out << "startStep = ktutorial.newStep(\"start\");\n";
    out << "connect(startStep, \"setup(QObject*)\",\n"
        << "        this, \"startStepSetup(QObject*)\");\n";
    out << "connect(startStep, \"tearDown(QObject*)\",\n"
        << "        this, \"startStepTearDown(QObject*)\");\n";
    out << "tutorial.addStep(startStep);\n";

    out << "tutorial.addStep(ktutorial.newStep(\"first\"));\n";
    out.flush();

    ScriptedTutorial scriptedTutorial(mTemporaryFile->fileName());

    QVERIFY(scriptedTutorial.isValid());

    scriptedTutorial.start();
    Step* startStep = scriptedTutorial.mCurrentStep;

    QCOMPARE(startStep->mWaitsFor.size(), 1);

    emit dummySignal();

    QCOMPARE(scriptedTutorial.mCurrentStep->id(), QString("first"));
    QCOMPARE(startStep->mWaitsFor.size(), 0);
    QVERIFY(mBooleanValue);
}

void ScriptingTest::stepsWithOptions() {
    QTextStream out(mTemporaryFile);
    out << "option0 = ktutorial.newOption(\"Go to start step\");\n";
    out << "function getOption0() { return option0; }\n";
    out << "option1 = ktutorial.newOption(\"Go to first step\");\n";
    out << "function getOption1() { return option1; }\n";
    out << "option2 = ktutorial.newOption(\"Go to second step\");\n";
    out << "function getOption2() { return option2; }\n";

    out << "startStep = ktutorial.newStep(\"start\");\n";
    out << "tutorial.addStep(startStep);\n";
    out << "firstStep = ktutorial.newStep(\"first\");\n";
    out << "tutorial.addStep(firstStep);\n";
    out << "secondStep = ktutorial.newStep(\"second\");\n";
    out << "tutorial.addStep(secondStep);\n";

    out << "function option0Selected() { tutorial.nextStep(\"start\"); }\n";
    out << "firstStep.addOption(option0, self, \"option0Selected()\");\n";

    out << "function option1Selected() { tutorial.nextStep(\"first\"); }\n";
    out << "startStep.addOption(option1, self, \"option1Selected()\");\n";

    out << "startStep.addOption(option2, \"second\");\n";
    out.flush();

    ScriptedTutorial scriptedTutorial(mTemporaryFile->fileName());

    QVERIFY(scriptedTutorial.isValid());

    scriptedTutorial.start();

    QObject* option1 = getObject(&scriptedTutorial, "getOption1");
    connect(this, SIGNAL(dummySignal()), option1, SIGNAL(selected()));
    emit dummySignal();

    QCOMPARE(scriptedTutorial.mCurrentStep->id(), QString("first"));

    QObject* option0 = getObject(&scriptedTutorial, "getOption0");
    //The same signal can be used, as the options belong to different steps
    connect(this, SIGNAL(dummySignal()), option0, SIGNAL(selected()));
    emit dummySignal();

    QCOMPARE(scriptedTutorial.mCurrentStep->id(), QString("start"));

    QObject* option2 = getObject(&scriptedTutorial, "getOption2");
    connect(this, SIGNAL(otherDummySignal()), option2, SIGNAL(selected()));
    emit otherDummySignal();

    QCOMPARE(scriptedTutorial.mCurrentStep->id(), QString("second"));
}

void ScriptingTest::stepsWithWaitFors() {
    QTextStream out(mTemporaryFile);
    out << "startStep = ktutorial.newStep(\"start\");\n";
    out << "tutorial.addStep(startStep);\n";
    out << "firstStep = ktutorial.newStep(\"first\");\n";
    out << "tutorial.addStep(firstStep);\n";
    out << "secondStep = ktutorial.newStep(\"second\");\n";
    out << "tutorial.addStep(secondStep);\n";

    out << "waitForDummy = ktutorial.newWaitFor(\"WaitForSignal\");\n";
    out << "waitForDummy.setSignal(testObject, \"dummySignal()\");\n";
    out << "waitForOtherDummy = ktutorial.newWaitFor(\"WaitForSignal\");\n";
    out << "waitForOtherDummy.setSignal(testObject, \"otherDummySignal()\");\n";
    out << "waitForAnotherDummy = ktutorial.newWaitFor(\"WaitForSignal\");\n";
    out << "waitForAnotherDummy.setSignal(testObject, \"anotherDummySignal()\");\n";

    out << "startStep.addWaitFor(waitForDummy, testObject, \"dummySlot()\");\n";

    out << "function checkEnded() {\n";
    out << "    if (testObject.booleanValue()) {\n";
    out << "        tutorial.nextStep(\"first\");\n";
    out << "    }\n";
    out << "};\n";
    out << "startStep.addWaitFor(waitForOtherDummy, self, \"checkEnded()\");\n";

    out << "firstStep.addWaitFor(waitForAnotherDummy, \"second\");\n";
    out.flush();

    ScriptedTutorial scriptedTutorial(mTemporaryFile->fileName());

    QVERIFY(scriptedTutorial.isValid());

    scriptedTutorial.start();

    QCOMPARE(scriptedTutorial.mCurrentStep->id(), QString("start"));
    QCOMPARE(mDummySlotCallCount, 0);

    emit dummySignal();

    QCOMPARE(scriptedTutorial.mCurrentStep->id(), QString("start"));
    QCOMPARE(mDummySlotCallCount, 1);

    emit otherDummySignal();

    QCOMPARE(scriptedTutorial.mCurrentStep->id(), QString("start"));
    QCOMPARE(mDummySlotCallCount, 1);

    mBooleanValue = true;
    emit otherDummySignal();

    QCOMPARE(scriptedTutorial.mCurrentStep->id(), QString("first"));
    QCOMPARE(mDummySlotCallCount, 1);

    emit anotherDummySignal();

    QCOMPARE(scriptedTutorial.mCurrentStep->id(), QString("second"));
}

void ScriptingTest::fullTutorial() {
    QTextStream out(mTemporaryFile);
    out << "tutorial.tutorialInformationAsObject().setName(\
\"Test tutorial\");\n";
    out << "tutorial.tutorialInformationAsObject().setDescription(\
\"A tutorial to test scripting\");\n";

    out << "option1 = ktutorial.newOption(\"First option\");\n";
    out << "function getOption1() { return option1; }\n";
    out << "option2 = ktutorial.newOption(\"Second option\");\n";
    out << "function getOption2() { return option2; }\n";

    out << "startStep = ktutorial.newStep(\"start\");\n";
    out << "tutorial.addStep(startStep);\n";
    out << "firstStep = ktutorial.newStep(\"first\");\n";
    out << "tutorial.addStep(firstStep);\n";
    out << "secondStep = ktutorial.newStep(\"second\");\n";
    out << "tutorial.addStep(secondStep);\n";
    out << "thirdStep = ktutorial.newStep(\"third\");\n";
    out << "tutorial.addStep(thirdStep);\n";
    out << "fourthStep = ktutorial.newStep(\"fourth\");\n";
    out << "tutorial.addStep(fourthStep);\n";
    out << "fifthStep = ktutorial.newStep(\"fifth\");\n";
    out << "tutorial.addStep(fifthStep);\n";
    out << "endStep = ktutorial.newStep(\"end\");\n";
    out << "endStep.setText(\"The tutorial has ended.\");\n";
    out << "tutorial.addStep(endStep);\n";

    out << "function option1Selected() { tutorial.nextStep(\"first\"); }\n";
    out << "startStep.addOption(option1, self, \"option1Selected()\");\n";

    out << "function option2Selected() { tutorial.nextStep(\"second\"); }\n";
    out << "firstStep.addOption(option2, self, \"option2Selected()\");\n";

    out << "waitForDummy = ktutorial.newWaitFor(\"WaitForSignal\");\n";
    out << "waitForDummy.setSignal(testObject, \"dummySignal()\");\n";
    out << "waitForOtherDummy = ktutorial.newWaitFor(\"WaitForSignal\");\n";
    out << "waitForOtherDummy.setSignal(testObject, \"otherDummySignal()\");\n";
    out << "waitForNotOther = ktutorial.newWaitFor(\"WaitForNot\");\n";
    out << "waitForNotOther.setNegatedWaitFor(waitForOtherDummy);\n";
    out << "waitForOr = ktutorial.newWaitFor(\"WaitForOr\");\n";
    out << "waitForOr.add(waitForDummy);\n";
    out << "waitForOr.add(waitForNotOther);\n";

    out << "waitForAnotherDummy = ktutorial.newWaitFor(\"WaitForSignal\");\n";
    out << "waitForAnotherDummy.setSignal(testObject, \
\"anotherDummySignal()\");\n";
    out << "waitForAnd = ktutorial.newWaitFor(\"WaitForAnd\");\n";
    out << "waitForAnd.add(waitForAnotherDummy);\n";
    out << "waitForAnd.add(waitForOr);\n";

    out << "function checkThird() {\n";
    out << "    if (testObject.booleanValue()) {\n";
    out << "        tutorial.nextStep(\"third\");\n";
    out << "    }\n";
    out << "};\n";
    out << "secondStep.addWaitFor(waitForAnd, self, \"checkThird\");\n";

    out << "waitForChildAddedEvent = ktutorial.newWaitFor(\"WaitForEvent\");\n";
    out << "waitForChildAddedEvent.setEvent(testObject, \"ChildAdded\");\n";

    out << "thirdStep.addWaitFor(waitForChildAddedEvent, \"fourth\");\n";

    out << "waitForIntProperty = ktutorial.newWaitFor(\"WaitForProperty\");\n";
    out << "waitForIntProperty.setProperty(testObject, \"intProperty\", \
\"42\");\n";

    out << "fourthStep.addWaitFor(waitForIntProperty, \"fifth\");\n";

    out << "function fifthStepSetup(step) {\n";
    out << "    waitForStepActivation = ktutorial.newWaitFor(\
\"WaitForStepActivation\");\n";
    out << "    waitForStepActivation.setStep(tutorial, step);\n";

    out << "    waitForIntProperty = ktutorial.newWaitFor(\
\"WaitForProperty\");\n";
    out << "    waitForIntProperty.setProperty(testObject, \"intProperty\", \
\"42\");\n";

    out << "    waitForAnd = ktutorial.newWaitFor(\"WaitForAnd\");\n";
    out << "    waitForAnd.add(waitForStepActivation);\n";
    out << "    waitForAnd.add(waitForIntProperty);\n";
    out << "    step.addWaitFor(waitForAnd, \"end\");\n";
    out << "}\n";

    out << "connect(fifthStep, \"setup(QObject*)\",\n"
        << "        this, \"fifthStepSetup(QObject*)\");\n";

    out.flush();

    ScriptedTutorial scriptedTutorial(mTemporaryFile->fileName());

    QVERIFY(scriptedTutorial.isValid());

    scriptedTutorial.start();

    QCOMPARE(scriptedTutorial.tutorialInformation()->id(),
             mTemporaryFile->fileName());
    QCOMPARE(scriptedTutorial.tutorialInformation()->name(),
             QString("Test tutorial"));
    QCOMPARE(scriptedTutorial.tutorialInformation()->description(),
             QString("A tutorial to test scripting"));

    QObject* option1 = getObject(&scriptedTutorial, "getOption1");
    connect(this, SIGNAL(dummySignal()), option1, SIGNAL(selected()));
    emit dummySignal();

    QCOMPARE(scriptedTutorial.mCurrentStep->id(), QString("first"));

    QObject* option2 = getObject(&scriptedTutorial, "getOption2");
    //The same signal can be used, as the options belong to different steps
    connect(this, SIGNAL(dummySignal()), option2, SIGNAL(selected()));
    emit dummySignal();

    QCOMPARE(scriptedTutorial.mCurrentStep->id(), QString("second"));

    emit dummySignal();

    QCOMPARE(scriptedTutorial.mCurrentStep->id(), QString("second"));

    mBooleanValue = true;
    emit otherDummySignal();

    QCOMPARE(scriptedTutorial.mCurrentStep->id(), QString("second"));

    emit anotherDummySignal();

    QCOMPARE(scriptedTutorial.mCurrentStep->id(), QString("third"));

    QObject* childObject = new QObject();
    childObject->setParent(this);

    QCOMPARE(scriptedTutorial.mCurrentStep->id(), QString("fourth"));

    mIntProperty = 42;
    emit intPropertyChanged();

    //Fifth step changes to end step when activated

    QCOMPARE(scriptedTutorial.mCurrentStep->id(), QString("end"));
    QCOMPARE(scriptedTutorial.mCurrentStep->text(),
             QString("The tutorial has ended."));
}

/////////////////////////////////////Helpers////////////////////////////////////

QObject* ScriptingTest::getObject(ScriptedTutorial* scriptedTutorial,
                                  const QString& function) {
    QVariant variant = scriptedTutorial->mScriptAction->callFunction(function);
    return qvariant_cast<QObject*>(variant);
}

}
}

QTEST_MAIN(ktutorial::scripting::ScriptingTest)

#include "ScriptingTest.moc"
