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

#include "Step.h"

#include <QSignalSpy>

#include "Reaction.h"

class StepTest: public QObject {
Q_OBJECT

private slots:

    void initTestCase();

    void testConstructor();

    void testSetId();

    void testSetText();

    void testSetCustomSetupCode();

    void testSetCustomTearDownCode();

    void testAddReaction();
    void testAddReactionAtIndex();
    void testRemoveReaction();

private:

    int mReactionStarType;

    void assertReactionAddedSignal(const QSignalSpy& spy, int index,
                                   Reaction* reaction, int reactionIndex) const;
    void assertReactionRemovedSignal(const QSignalSpy& spy, int index,
                                     Reaction* reaction) const;

};

void StepTest::initTestCase() {
    //Reaction* must be registered in order to be used with QSignalSpy
    mReactionStarType = qRegisterMetaType<Reaction*>("Reaction*");
}

void StepTest::testConstructor() {
    QObject parent;
    Step* step = new Step(&parent);

    QCOMPARE(step->parent(), &parent);
}

//Step* must be declared as a metatype to be used in qvariant_cast
Q_DECLARE_METATYPE(Step*);

void StepTest::testSetId() {
    Step step;

    //Step* must be registered in order to be used with QSignalSpy
    int stepStarType = qRegisterMetaType<Step*>("Step*");
    QSignalSpy dataChangedSpy(&step, SIGNAL(dataChanged(Step*)));

    step.setId("The id");

    QCOMPARE(step.id(), QString("The id"));
    QCOMPARE(dataChangedSpy.count(), 1);
    QVariant argument = dataChangedSpy.at(0).at(0);
    QCOMPARE(argument.userType(), stepStarType);
    QCOMPARE(qvariant_cast<Step*>(argument), &step);
}

void StepTest::testSetText() {
    Step step;

    //Step* must be registered in order to be used with QSignalSpy
    int stepStarType = qRegisterMetaType<Step*>("Step*");
    QSignalSpy dataChangedSpy(&step, SIGNAL(dataChanged(Step*)));

    step.setText("The text");

    QCOMPARE(step.text(), QString("The text"));
    QCOMPARE(dataChangedSpy.count(), 1);
    QVariant argument = dataChangedSpy.at(0).at(0);
    QCOMPARE(argument.userType(), stepStarType);
    QCOMPARE(qvariant_cast<Step*>(argument), &step);
}

void StepTest::testSetCustomSetupCode() {
    Step step;

    //Step* must be registered in order to be used with QSignalSpy
    int stepStarType = qRegisterMetaType<Step*>("Step*");
    QSignalSpy dataChangedSpy(&step, SIGNAL(dataChanged(Step*)));

    step.setCustomSetupCode("The setup code");

    QCOMPARE(step.customSetupCode(), QString("The setup code"));
    QCOMPARE(dataChangedSpy.count(), 1);
    QVariant argument = dataChangedSpy.at(0).at(0);
    QCOMPARE(argument.userType(), stepStarType);
    QCOMPARE(qvariant_cast<Step*>(argument), &step);
}

void StepTest::testSetCustomTearDownCode() {
    Step step;

    //Step* must be registered in order to be used with QSignalSpy
    int stepStarType = qRegisterMetaType<Step*>("Step*");
    QSignalSpy dataChangedSpy(&step, SIGNAL(dataChanged(Step*)));

    step.setCustomTearDownCode("The tear down code");

    QCOMPARE(step.customTearDownCode(), QString("The tear down code"));
    QCOMPARE(dataChangedSpy.count(), 1);
    QVariant argument = dataChangedSpy.at(0).at(0);
    QCOMPARE(argument.userType(), stepStarType);
    QCOMPARE(qvariant_cast<Step*>(argument), &step);
}

void StepTest::testAddReaction() {
    Step step;
    Reaction* reaction1 = new Reaction();
    Reaction* reaction2 = new Reaction();
    Reaction* reaction3 = new Reaction();

    QSignalSpy reactionAddedSpy(&step, SIGNAL(reactionAdded(Reaction*, int)));

    step.addReaction(reaction1);
    step.addReaction(reaction2);
    step.addReaction(reaction3);

    QCOMPARE(step.reactions().count(), 3);
    QCOMPARE(step.reactions()[0], reaction1);
    QCOMPARE(step.reactions()[1], reaction2);
    QCOMPARE(step.reactions()[2], reaction3);
    QCOMPARE(reactionAddedSpy.count(), 3);
    assertReactionAddedSignal(reactionAddedSpy, 0, reaction1, 0);
    assertReactionAddedSignal(reactionAddedSpy, 1, reaction2, 1);
    assertReactionAddedSignal(reactionAddedSpy, 2, reaction3, 2);
}

void StepTest::testAddReactionAtIndex() {
    Step step;
    Reaction* reaction1 = new Reaction();
    Reaction* reaction2 = new Reaction();
    Reaction* reaction3 = new Reaction();

    QSignalSpy reactionAddedSpy(&step, SIGNAL(reactionAdded(Reaction*, int)));

    step.addReaction(reaction2, 0);
    step.addReaction(reaction1, 0);
    step.addReaction(reaction3, 2);

    QCOMPARE(step.reactions().count(), 3);
    QCOMPARE(step.reactions()[0], reaction1);
    QCOMPARE(step.reactions()[1], reaction2);
    QCOMPARE(step.reactions()[2], reaction3);
    QCOMPARE(reactionAddedSpy.count(), 3);
    assertReactionAddedSignal(reactionAddedSpy, 0, reaction2, 0);
    assertReactionAddedSignal(reactionAddedSpy, 1, reaction1, 0);
    assertReactionAddedSignal(reactionAddedSpy, 2, reaction3, 2);
}

void StepTest::testRemoveReaction() {
    Step step;

    //They will be removed and not deleted by the Step, so they are created in
    //stack
    Reaction reaction1;
    Reaction reaction2;
    Reaction reaction3;

    step.addReaction(&reaction1);
    step.addReaction(&reaction2);
    step.addReaction(&reaction3);

    QSignalSpy reactionRemovedSpy(&step, SIGNAL(reactionRemoved(Reaction*)));

    step.removeReaction(&reaction2);

    QCOMPARE(step.reactions().count(), 2);
    QCOMPARE(step.reactions()[0], &reaction1);
    QCOMPARE(step.reactions()[1], &reaction3);
    QCOMPARE(reactionRemovedSpy.count(), 1);
    assertReactionRemovedSignal(reactionRemovedSpy, 0, &reaction2);

    step.removeReaction(&reaction1);
    step.removeReaction(&reaction3);

    QCOMPARE(step.reactions().count(), 0);
    QCOMPARE(reactionRemovedSpy.count(), 3);
    assertReactionRemovedSignal(reactionRemovedSpy, 1, &reaction1);
    assertReactionRemovedSignal(reactionRemovedSpy, 2, &reaction3);
}

/////////////////////////////////// Helpers ////////////////////////////////////

//Reaction* must be declared as a metatype to be used in qvariant_cast
Q_DECLARE_METATYPE(Reaction*);

void StepTest::assertReactionAddedSignal(const QSignalSpy& spy, int index,
                                         Reaction* reaction,
                                         int reactionIndex) const {
    QCOMPARE(spy.at(index).count(), 2);

    QVariant argument = spy.at(index).at(0);
    QCOMPARE(argument.userType(), mReactionStarType);
    QCOMPARE(qvariant_cast<Reaction*>(argument), reaction);
    argument = spy.at(index).at(1);
    QCOMPARE(argument.type(), QVariant::Int);
    QCOMPARE(argument.toInt(), reactionIndex);
}

void StepTest::assertReactionRemovedSignal(const QSignalSpy& spy, int index,
                                           Reaction* reaction) const {
    QCOMPARE(spy.at(index).count(), 1);

    QVariant argument = spy.at(index).at(0);
    QCOMPARE(argument.userType(), mReactionStarType);
    QCOMPARE(qvariant_cast<Reaction*>(argument), reaction);
}

QTEST_MAIN(StepTest)

#include "StepTest.moc"
