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

#include <QTest>

#include "Tutorial.h"

#include <QSignalSpy>

#include "Step.h"

class TutorialTest: public QObject {
Q_OBJECT

private slots:

    void initTestCase();

    void testConstructor();

    void testId();
    void testIdWithEmptyName();
    void testIdWithSpaceName();

    void testSetName();

    void testSetDescription();

    void testSetLicenseText();

    void testSetCustomSetupCode();

    void testSetCustomTearDownCode();

    void testAddStep();
    void testAddStepAtIndex();
    void testRemoveStep();

private:

    int mStepStarType;

    void assertStepAddedSignal(const QSignalSpy& spy, int index, Step* step,
                               int stepIndex);
    void assertStepRemovedSignal(const QSignalSpy& spy, int index, Step* step);

};

void TutorialTest::initTestCase() {
    //Step* must be registered in order to be used with QSignalSpy
    mStepStarType = qRegisterMetaType<Step*>("Step*");
}

void TutorialTest::testConstructor() {
    QObject parent;
    Tutorial* tutorial = new Tutorial(&parent);

    QCOMPARE(tutorial->parent(), &parent);
}

void TutorialTest::testId() {
    Tutorial tutorial;
    tutorial.setName("ThE name of a tutoRial");

    QCOMPARE(tutorial.id(), QString("thENameOfATutoRial"));
}

void TutorialTest::testIdWithEmptyName() {
    Tutorial tutorial;
    tutorial.setName("");

    QCOMPARE(tutorial.id(), QString(""));
}

void TutorialTest::testIdWithSpaceName() {
    Tutorial tutorial;
    tutorial.setName("   ");

    QCOMPARE(tutorial.id(), QString(""));
}

//Tutorial* must be declared as a metatype to be used in qvariant_cast
Q_DECLARE_METATYPE(Tutorial*);

void TutorialTest::testSetName() {
    Tutorial tutorial;

    //Tutorial* must be registered in order to be used with QSignalSpy
    int tutorialStarType = qRegisterMetaType<Tutorial*>("Tutorial*");
    QSignalSpy dataChangedSpy(&tutorial, SIGNAL(dataChanged(Tutorial*)));

    tutorial.setName("The name");

    QCOMPARE(tutorial.name(), QString("The name"));
    QCOMPARE(dataChangedSpy.count(), 1);
    QVariant argument = dataChangedSpy.at(0).at(0);
    QCOMPARE(argument.userType(), tutorialStarType);
    QCOMPARE(qvariant_cast<Tutorial*>(argument), &tutorial);
}

void TutorialTest::testSetDescription() {
    Tutorial tutorial;

    //Tutorial* must be registered in order to be used with QSignalSpy
    int tutorialStarType = qRegisterMetaType<Tutorial*>("Tutorial*");
    QSignalSpy dataChangedSpy(&tutorial, SIGNAL(dataChanged(Tutorial*)));

    tutorial.setDescription("The description");

    QCOMPARE(tutorial.description(), QString("The description"));
    QCOMPARE(dataChangedSpy.count(), 1);
    QVariant argument = dataChangedSpy.at(0).at(0);
    QCOMPARE(argument.userType(), tutorialStarType);
    QCOMPARE(qvariant_cast<Tutorial*>(argument), &tutorial);
}

void TutorialTest::testSetLicenseText() {
    Tutorial tutorial;

    //Tutorial* must be registered in order to be used with QSignalSpy
    int tutorialStarType = qRegisterMetaType<Tutorial*>("Tutorial*");
    QSignalSpy dataChangedSpy(&tutorial, SIGNAL(dataChanged(Tutorial*)));

    tutorial.setLicenseText("The license text");

    QCOMPARE(tutorial.licenseText(), QString("The license text"));
    QCOMPARE(dataChangedSpy.count(), 1);
    QVariant argument = dataChangedSpy.at(0).at(0);
    QCOMPARE(argument.userType(), tutorialStarType);
    QCOMPARE(qvariant_cast<Tutorial*>(argument), &tutorial);
}

void TutorialTest::testSetCustomSetupCode() {
    Tutorial tutorial;

    //Tutorial* must be registered in order to be used with QSignalSpy
    int tutorialStarType = qRegisterMetaType<Tutorial*>("Tutorial*");
    QSignalSpy dataChangedSpy(&tutorial, SIGNAL(dataChanged(Tutorial*)));

    tutorial.setCustomSetupCode("The setup code");

    QCOMPARE(tutorial.customSetupCode(), QString("The setup code"));
    QCOMPARE(dataChangedSpy.count(), 1);
    QVariant argument = dataChangedSpy.at(0).at(0);
    QCOMPARE(argument.userType(), tutorialStarType);
    QCOMPARE(qvariant_cast<Tutorial*>(argument), &tutorial);
}

void TutorialTest::testSetCustomTearDownCode() {
    Tutorial tutorial;

    //Tutorial* must be registered in order to be used with QSignalSpy
    int tutorialStarType = qRegisterMetaType<Tutorial*>("Tutorial*");
    QSignalSpy dataChangedSpy(&tutorial, SIGNAL(dataChanged(Tutorial*)));

    tutorial.setCustomTearDownCode("The tear down code");

    QCOMPARE(tutorial.customTearDownCode(), QString("The tear down code"));
    QCOMPARE(dataChangedSpy.count(), 1);
    QVariant argument = dataChangedSpy.at(0).at(0);
    QCOMPARE(argument.userType(), tutorialStarType);
    QCOMPARE(qvariant_cast<Tutorial*>(argument), &tutorial);
}

//Step* must be declared as a metatype to be used in qvariant_cast
Q_DECLARE_METATYPE(Step*);

void TutorialTest::testAddStep() {
    Tutorial tutorial;
    Step* step1 = new Step();
    Step* step2 = new Step();
    Step* step3 = new Step();

    QSignalSpy stepAddedSpy(&tutorial, SIGNAL(stepAdded(Step*,int)));

    tutorial.addStep(step1);
    tutorial.addStep(step2);
    tutorial.addStep(step3);

    QCOMPARE(tutorial.steps().count(), 3);
    QCOMPARE(tutorial.steps()[0], step1);
    QCOMPARE(tutorial.steps()[1], step2);
    QCOMPARE(tutorial.steps()[2], step3);
    QCOMPARE(stepAddedSpy.count(), 3);
    assertStepAddedSignal(stepAddedSpy, 0, step1, 0);
    assertStepAddedSignal(stepAddedSpy, 1, step2, 1);
    assertStepAddedSignal(stepAddedSpy, 2, step3, 2);
}

void TutorialTest::testAddStepAtIndex() {
    Tutorial tutorial;
    Step* step1 = new Step();
    Step* step2 = new Step();
    Step* step3 = new Step();

    QSignalSpy stepAddedSpy(&tutorial, SIGNAL(stepAdded(Step*,int)));

    tutorial.addStep(step2, 0);
    tutorial.addStep(step1, 0);
    tutorial.addStep(step3, 2);

    QCOMPARE(tutorial.steps().count(), 3);
    QCOMPARE(tutorial.steps()[0], step1);
    QCOMPARE(tutorial.steps()[1], step2);
    QCOMPARE(tutorial.steps()[2], step3);
    QCOMPARE(stepAddedSpy.count(), 3);
    assertStepAddedSignal(stepAddedSpy, 0, step2, 0);
    assertStepAddedSignal(stepAddedSpy, 1, step1, 0);
    assertStepAddedSignal(stepAddedSpy, 2, step3, 2);
}

void TutorialTest::testRemoveStep() {
    Tutorial tutorial;

    //They will be removed and not deleted by the Tutorial, so they are created
    //in stack
    Step step1;
    Step step2;
    Step step3;

    tutorial.addStep(&step1);
    tutorial.addStep(&step2);
    tutorial.addStep(&step3);

    QSignalSpy stepRemovedSpy(&tutorial, SIGNAL(stepRemoved(Step*)));

    tutorial.removeStep(&step2);

    QCOMPARE(tutorial.steps().count(), 2);
    QCOMPARE(tutorial.steps()[0], &step1);
    QCOMPARE(tutorial.steps()[1], &step3);
    QCOMPARE(stepRemovedSpy.count(), 1);
    assertStepRemovedSignal(stepRemovedSpy, 0, &step2);

    tutorial.removeStep(&step1);
    tutorial.removeStep(&step3);

    QCOMPARE(tutorial.steps().count(), 0);
    QCOMPARE(stepRemovedSpy.count(), 3);
    assertStepRemovedSignal(stepRemovedSpy, 1, &step1);
    assertStepRemovedSignal(stepRemovedSpy, 2, &step3);
}

/////////////////////////////////// Helpers ////////////////////////////////////

void TutorialTest::assertStepAddedSignal(const QSignalSpy& spy, int index,
                                         Step* step, int stepIndex) {
    QCOMPARE(spy.at(index).count(), 2);

    QVariant argument = spy.at(index).at(0);
    QCOMPARE(argument.userType(), mStepStarType);
    QCOMPARE(qvariant_cast<Step*>(argument), step);
    argument = spy.at(index).at(1);
    QCOMPARE(argument.type(), QVariant::Int);
    QCOMPARE(argument.toInt(), stepIndex);
}

void TutorialTest::assertStepRemovedSignal(const QSignalSpy& spy, int index,
                                           Step* step) {
    QCOMPARE(spy.at(index).count(), 1);

    QVariant argument = spy.at(index).at(0);
    QCOMPARE(argument.userType(), mStepStarType);
    QCOMPARE(qvariant_cast<Step*>(argument), step);
}

QTEST_MAIN(TutorialTest)

#include "TutorialTest.moc"
