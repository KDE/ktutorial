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

#include "WaitForEvent.h"

class WaitForEventTest: public QObject {
Q_OBJECT

private slots:

    void initTestCase();

    void testConstructor();

    void testClone();

    void testEquals();

    void testSetReceiverName();

    void testSetEventName();

private:

    int mWaitForStarType;

    void assertWaitForEvent(const QSignalSpy& spy, int index,
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

void WaitForEventTest::initTestCase() {
    //WaitFor* must be registered in order to be used with QSignalSpy
    mWaitForStarType = qRegisterMetaType<WaitFor*>("WaitFor*");
}

void WaitForEventTest::testConstructor() {
    QObject parent;
    WaitForEvent* waitForEvent = new WaitForEvent(&parent);

    QCOMPARE(waitForEvent->parent(), &parent);
}

void WaitForEventTest::testClone() {
    WaitForEvent waitForEvent;
    waitForEvent.setReceiverName("The receiver name");
    waitForEvent.setEventName("The event name");

    WaitForEvent* cloned = static_cast<WaitForEvent*>(waitForEvent.clone());

    QVERIFY(cloned != &waitForEvent);
    QCOMPARE(cloned->receiverName(), waitForEvent.receiverName());
    QCOMPARE(cloned->eventName(), waitForEvent.eventName());
    delete cloned;
}

void WaitForEventTest::testEquals() {
    WaitForEvent waitForEvent1;
    waitForEvent1.setReceiverName("The receiver name");
    waitForEvent1.setEventName("The event name");
    WaitForEvent waitForEvent2;

    QCOMPARE(waitForEvent1 == waitForEvent2, false);
    QCOMPARE(waitForEvent2 == waitForEvent1, false);

    waitForEvent2.setReceiverName("The receiver name");
    waitForEvent2.setEventName("The event name");

    QCOMPARE(waitForEvent1 == waitForEvent2, true);
    QCOMPARE(waitForEvent2 == waitForEvent1, true);

    StubWaitFor stubWaitFor;

    QCOMPARE(waitForEvent1 == stubWaitFor, false);
}

void WaitForEventTest::testSetReceiverName() {
    WaitForEvent waitForEvent;

    QSignalSpy dataChangedSpy(&waitForEvent, SIGNAL(dataChanged(WaitFor*)));

    waitForEvent.setReceiverName("The receiver name");

    QCOMPARE(waitForEvent.receiverName(), QString("The receiver name"));
    QCOMPARE(dataChangedSpy.count(), 1);
    assertWaitForEvent(dataChangedSpy, 0, &waitForEvent);
}

void WaitForEventTest::testSetEventName() {
    WaitForEvent waitForEvent;

    QSignalSpy dataChangedSpy(&waitForEvent, SIGNAL(dataChanged(WaitFor*)));

    waitForEvent.setEventName("The event name");

    QCOMPARE(waitForEvent.eventName(), QString("The event name"));
    QCOMPARE(dataChangedSpy.count(), 1);
    assertWaitForEvent(dataChangedSpy, 0, &waitForEvent);
}

//WaitFor* must be declared as a metatype to be used in qvariant_cast
Q_DECLARE_METATYPE(WaitFor*);

/////////////////////////////////// Helpers ////////////////////////////////////

void WaitForEventTest::assertWaitForEvent(const QSignalSpy& spy, int index,
                                            WaitFor* waitFor) {
    QCOMPARE(spy.at(index).count(), 1);

    QVariant argument = spy.at(index).at(0);
    QCOMPARE(argument.userType(), mWaitForStarType);
    QCOMPARE(qvariant_cast<WaitFor*>(argument), waitFor);
}

QTEST_MAIN(WaitForEventTest)

#include "WaitForEventTest.moc"
