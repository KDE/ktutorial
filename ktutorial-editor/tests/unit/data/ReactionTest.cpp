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

#include <QtTest>

#include "Reaction.h"
#include "WaitFor.h"

class ReactionTest: public QObject {
Q_OBJECT

private slots:

    void initTestCase();

    void testConstructor();

    void testSetTriggerType();

    void testSetOptionName();

    void testSetWaitFor();

    void testSetResponseType();

    void testSetNextStepId();

    void testSetCustomCode();

private:

    int mReactionStarType;

    void assertDataChanged(const QSignalSpy& spy, int index,
                           Reaction* reaction) const;

};

class MockWaitFor: public WaitFor {
Q_OBJECT
public:

    WaitFor* clone() const {
        return 0;
    }

    bool equals(const WaitFor& waitFor) const {
        Q_UNUSED(waitFor);
        return false;
    }

    void emitDataChanged() {
        emit dataChanged(this);
    }

};

void ReactionTest::initTestCase() {
    //Reaction* must be registered in order to be used with QSignalSpy
    mReactionStarType = qRegisterMetaType<Reaction*>("Reaction*");
}

void ReactionTest::testConstructor() {
    QObject parent;
    Reaction* reaction = new Reaction(&parent);

    QCOMPARE(reaction->parent(), &parent);
    QCOMPARE(reaction->triggerType(), Reaction::OptionSelected);
    QCOMPARE(reaction->responseType(), Reaction::NextStep);
    QCOMPARE(reaction->waitFor(), (WaitFor*)0);
}

void ReactionTest::testSetTriggerType() {
    Reaction reaction;

    QSignalSpy dataChangedSpy(&reaction, SIGNAL(dataChanged(Reaction*)));

    reaction.setTriggerType(Reaction::ConditionMet);

    QCOMPARE(reaction.triggerType(), Reaction::ConditionMet);
    QCOMPARE(dataChangedSpy.count(), 1);
    assertDataChanged(dataChangedSpy, 0, &reaction);
}

void ReactionTest::testSetOptionName() {
    Reaction reaction;

    QSignalSpy dataChangedSpy(&reaction, SIGNAL(dataChanged(Reaction*)));

    reaction.setOptionName("The option name");

    QCOMPARE(reaction.optionName(), QString("The option name"));
    QCOMPARE(dataChangedSpy.count(), 1);
    assertDataChanged(dataChangedSpy, 0, &reaction);
}

void ReactionTest::testSetWaitFor() {
    Reaction reaction;

    QSignalSpy dataChangedSpy(&reaction, SIGNAL(dataChanged(Reaction*)));

    MockWaitFor* waitFor = new MockWaitFor();
    reaction.setWaitFor(waitFor);

    QCOMPARE(reaction.waitFor(), waitFor);
    QCOMPARE(dataChangedSpy.count(), 1);
    assertDataChanged(dataChangedSpy, 0, &reaction);
}

void ReactionTest::testSetResponseType() {
    Reaction reaction;

    QSignalSpy dataChangedSpy(&reaction, SIGNAL(dataChanged(Reaction*)));

    reaction.setResponseType(Reaction::CustomCode);

    QCOMPARE(reaction.responseType(), Reaction::CustomCode);
    QCOMPARE(dataChangedSpy.count(), 1);
    assertDataChanged(dataChangedSpy, 0, &reaction);
}

void ReactionTest::testSetNextStepId() {
    Reaction reaction;

    QSignalSpy dataChangedSpy(&reaction, SIGNAL(dataChanged(Reaction*)));

    reaction.setNextStepId("The id");

    QCOMPARE(reaction.nextStepId(), QString("The id"));
    QCOMPARE(dataChangedSpy.count(), 1);
    assertDataChanged(dataChangedSpy, 0, &reaction);
}

void ReactionTest::testSetCustomCode() {
    Reaction reaction;

    QSignalSpy dataChangedSpy(&reaction, SIGNAL(dataChanged(Reaction*)));

    reaction.setCustomCode("The custom code");

    QCOMPARE(reaction.customCode(), QString("The custom code"));
    QCOMPARE(dataChangedSpy.count(), 1);
    assertDataChanged(dataChangedSpy, 0, &reaction);
}

/////////////////////////////////// Helpers ////////////////////////////////////

//Reaction* must be declared as a metatype to be used in qvariant_cast
Q_DECLARE_METATYPE(Reaction*);

void ReactionTest::assertDataChanged(const QSignalSpy& spy, int index,
                                     Reaction* reaction) const {
    QCOMPARE(spy.at(index).count(), 1);

    QVariant argument = spy.at(index).at(0);
    QCOMPARE(argument.userType(), mReactionStarType);
    QCOMPARE(qvariant_cast<Reaction*>(argument), reaction);
}

QTEST_MAIN(ReactionTest)

#include "ReactionTest.moc"
