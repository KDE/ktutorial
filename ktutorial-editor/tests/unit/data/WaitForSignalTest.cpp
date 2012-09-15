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

#include "WaitForSignal.h"

class WaitForSignalTest: public QObject {
Q_OBJECT

private slots:

    void initTestCase();

    void testConstructor();

    void testClone();

    void testEquals();

    void testSetEmitterName();

    void testSetSignalName();

private:

    int mWaitForStarType;

    void assertWaitForSignal(const QSignalSpy& spy, int index,
                             WaitFor* waitFor);

};

class StubWaitFor: public WaitFor {
Q_OBJECT
public:

    int mValue;

    StubWaitFor(int value = 0): mValue(value) {
    }

    virtual WaitFor* clone() const {
        return new StubWaitFor(mValue);
    }

    virtual bool equals(const WaitFor& waitFor) const {
        if (!qobject_cast<const StubWaitFor*>(&waitFor)) {
            return false;
        }

        return mValue == static_cast<const StubWaitFor*>(&waitFor)->mValue;
    }
};

void WaitForSignalTest::initTestCase() {
    //WaitFor* must be registered in order to be used with QSignalSpy
    mWaitForStarType = qRegisterMetaType<WaitFor*>("WaitFor*");
}

void WaitForSignalTest::testConstructor() {
    QObject parent;
    WaitForSignal* waitForSignal = new WaitForSignal(&parent);

    QCOMPARE(waitForSignal->parent(), &parent);
}

void WaitForSignalTest::testClone() {
    WaitForSignal waitForSignal;
    waitForSignal.setEmitterName("The emitter name");
    waitForSignal.setSignalName("The signal name");

    WaitForSignal* cloned = static_cast<WaitForSignal*>(waitForSignal.clone());

    QVERIFY(cloned != &waitForSignal);
    QCOMPARE(cloned->emitterName(), waitForSignal.emitterName());
    QCOMPARE(cloned->signalName(), waitForSignal.signalName());
    delete cloned;
}

void WaitForSignalTest::testEquals() {
    WaitForSignal waitForSignal1;
    waitForSignal1.setEmitterName("The emitter name");
    waitForSignal1.setSignalName("The signal name");
    WaitForSignal waitForSignal2;

    QCOMPARE(waitForSignal1 == waitForSignal2, false);
    QCOMPARE(waitForSignal2 == waitForSignal1, false);

    waitForSignal2.setEmitterName("The emitter name");
    waitForSignal2.setSignalName("The signal name");

    QCOMPARE(waitForSignal1 == waitForSignal2, true);
    QCOMPARE(waitForSignal2 == waitForSignal1, true);

    StubWaitFor stubWaitFor;

    QCOMPARE(waitForSignal1 == stubWaitFor, false);
}

void WaitForSignalTest::testSetEmitterName() {
    WaitForSignal waitForSignal;

    QSignalSpy dataChangedSpy(&waitForSignal, SIGNAL(dataChanged(WaitFor*)));

    waitForSignal.setEmitterName("The emitter name");

    QCOMPARE(waitForSignal.emitterName(), QString("The emitter name"));
    QCOMPARE(dataChangedSpy.count(), 1);
    assertWaitForSignal(dataChangedSpy, 0, &waitForSignal);
}

void WaitForSignalTest::testSetSignalName() {
    WaitForSignal waitForSignal;

    QSignalSpy dataChangedSpy(&waitForSignal, SIGNAL(dataChanged(WaitFor*)));

    waitForSignal.setSignalName("The signal name");

    QCOMPARE(waitForSignal.signalName(), QString("The signal name"));
    QCOMPARE(dataChangedSpy.count(), 1);
    assertWaitForSignal(dataChangedSpy, 0, &waitForSignal);
}

//WaitFor* must be declared as a metatype to be used in qvariant_cast
Q_DECLARE_METATYPE(WaitFor*);

/////////////////////////////////// Helpers ////////////////////////////////////

void WaitForSignalTest::assertWaitForSignal(const QSignalSpy& spy, int index,
                                            WaitFor* waitFor) {
    QCOMPARE(spy.at(index).count(), 1);

    QVariant argument = spy.at(index).at(0);
    QCOMPARE(argument.userType(), mWaitForStarType);
    QCOMPARE(qvariant_cast<WaitFor*>(argument), waitFor);
}

QTEST_MAIN(WaitForSignalTest)

#include "WaitForSignalTest.moc"
