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

#include <QtTest>

#include "WaitForStepActivation.h"

#include "Step.h"
#include "Tutorial.h"

//WaitFor* must be declared as a metatype to be used in qvariant_cast
Q_DECLARE_METATYPE(ktutorial::WaitFor*);

namespace ktutorial {

class WaitForStepActivationTest: public QObject {
Q_OBJECT

public slots:

    void assertConditionMet(WaitFor* waitForToAssertItsCondition) {
        QVERIFY(waitForToAssertItsCondition);
        QVERIFY(waitForToAssertItsCondition->conditionMet());
    }

private slots:

    void testConstructor();
    void testConstructorWithNullTutorial();
    void testConstructorWithNullStep();
    void testConstructorDefault();
    void testConstructorDefaultWithNullTutorial();
    void testConstructorDefaultWithNullStep();

    void testStepActivation();
    void testStepActivationWhenWaitForIsInactive();

};

class InspectedTutorial: public Tutorial {
public:

    int mstepActivatedConnectionCount;

    InspectedTutorial(TutorialInformation* tutorialInformation):
                                                Tutorial(tutorialInformation),
        mstepActivatedConnectionCount(0) {
    }

protected:

    void connectNotify(const char* signal) {
        if (QLatin1String(signal) == SIGNAL(stepActivated(Step*))) {
            mstepActivatedConnectionCount++;
        }
    }

};

void WaitForStepActivationTest::testConstructor() {
    InspectedTutorial tutorial(0);
    Step step("stepName");
    WaitForStepActivation waitForStepActivation(&tutorial, &step);

    QVERIFY(!waitForStepActivation.isActive());
    QVERIFY(!waitForStepActivation.conditionMet());
    QCOMPARE(tutorial.mstepActivatedConnectionCount, 1);
}

void WaitForStepActivationTest::testConstructorWithNullTutorial() {
    Step step("stepName");
    WaitForStepActivation waitForStepActivation(0, &step);

    QVERIFY(!waitForStepActivation.isActive());
    QVERIFY(!waitForStepActivation.conditionMet());
}

void WaitForStepActivationTest::testConstructorWithNullStep() {
    InspectedTutorial tutorial(0);
    WaitForStepActivation waitForStepActivation(&tutorial, 0);

    QVERIFY(!waitForStepActivation.isActive());
    QVERIFY(!waitForStepActivation.conditionMet());
    QCOMPARE(tutorial.mstepActivatedConnectionCount, 0);
}

void WaitForStepActivationTest::testConstructorDefault() {
    InspectedTutorial tutorial(0);
    Step step("stepName");
    WaitForStepActivation waitForStepActivation;
    waitForStepActivation.setStep(&tutorial, &step);

    QVERIFY(!waitForStepActivation.isActive());
    QVERIFY(!waitForStepActivation.conditionMet());
    QCOMPARE(tutorial.mstepActivatedConnectionCount, 1);
}

void WaitForStepActivationTest::testConstructorDefaultWithNullTutorial() {
    Step step("stepName");
    WaitForStepActivation waitForStepActivation;
    waitForStepActivation.setStep(0, &step);

    QVERIFY(!waitForStepActivation.isActive());
    QVERIFY(!waitForStepActivation.conditionMet());
}

void WaitForStepActivationTest::testConstructorDefaultWithNullStep() {
    InspectedTutorial tutorial(0);
    WaitForStepActivation waitForStepActivation;
    waitForStepActivation.setStep(&tutorial, 0);

    QVERIFY(!waitForStepActivation.isActive());
    QVERIFY(!waitForStepActivation.conditionMet());
    QCOMPARE(tutorial.mstepActivatedConnectionCount, 0);
}

void WaitForStepActivationTest::testStepActivation() {
    InspectedTutorial tutorial(0);
    Step* startStep = new Step("start");
    tutorial.addStep(startStep);

    WaitForStepActivation waitForStepActivation(&tutorial, startStep);
    waitForStepActivation.setActive(true);

    QVERIFY(!waitForStepActivation.conditionMet());

    //WaitFor* must be registered in order to be used with QSignalSpy
    int waitForStarType = qRegisterMetaType<WaitFor*>("WaitFor*");
    QSignalSpy waitEndedSpy(&waitForStepActivation,
                            SIGNAL(waitEnded(WaitFor*)));

    connect(&waitForStepActivation, SIGNAL(waitEnded(WaitFor*)),
            this, SLOT(assertConditionMet(WaitFor*)));

    tutorial.start();

    QVERIFY(!waitForStepActivation.conditionMet());
    QCOMPARE(waitEndedSpy.count(), 1);
    QVariant argument = waitEndedSpy.at(0).at(0);
    QCOMPARE(argument.userType(), waitForStarType);
    QCOMPARE(qvariant_cast<WaitFor*>(argument), &waitForStepActivation);
}

//Should not happen, as WaitForStepActivation is meant to be used only with the
//step that contains the WaitFor... but just in case ;)
void WaitForStepActivationTest::testStepActivationWhenWaitForIsInactive() {
    InspectedTutorial tutorial(0);
    Step* startStep = new Step("start");
    tutorial.addStep(startStep);

    WaitForStepActivation waitForStepActivation(&tutorial, startStep);

    //WaitFor* must be registered in order to be used with QSignalSpy
    qRegisterMetaType<WaitFor*>("WaitFor*");
    QSignalSpy waitEndedSpy(&waitForStepActivation,
                            SIGNAL(waitEnded(WaitFor*)));

    tutorial.start();

    QVERIFY(!waitForStepActivation.conditionMet());
    QCOMPARE(waitEndedSpy.count(), 0);
}

}

QTEST_MAIN(ktutorial::WaitForStepActivationTest)

#include "WaitForStepActivationTest.moc"
