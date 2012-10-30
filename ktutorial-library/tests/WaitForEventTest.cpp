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

#include <QSignalSpy>

#define protected public
#define private public
#include "WaitForEvent.h"
#undef private
#undef protected

//WaitFor* must be declared as a metatype to be used in qvariant_cast
Q_DECLARE_METATYPE(ktutorial::WaitFor*);

namespace ktutorial {

class WaitForEventTest: public QObject {
Q_OBJECT
private slots:

    void testConstructor();
    void testConstructorWithNullObject();
    void testConstructorDefault();
    void testConstructorDefaultWithNullObject();

    void testSetActive();

    void testWaitEnded();
    void testWaitEndedWithDefaultConstructor();
    void testWaitEndedNotActive();

    void testHandleEvent();
    void testHandleEventNotActive();
    void testHandleEventEventTypeNotExpected();

    void testEventSentAfterWaitForDestruction();

};

class WaitForEventWithCustomHandling: public WaitForEvent {
public:

    QEvent::Type mType;
    int mHandledEventsCount;

    WaitForEventWithCustomHandling(QObject* object, QEvent::Type type): 
            WaitForEvent(object, type),
        mType(type),
        mHandledEventsCount(0) {
    }

protected:

    virtual void handleEvent(QEvent* event) {
        QCOMPARE(event->type(), mType);

        mHandledEventsCount++;
    }

};

void WaitForEventTest::testConstructor() {
    WaitForEvent waitForEvent(this, QEvent::ChildAdded);

    QVERIFY(!waitForEvent.isActive());
    QVERIFY(!waitForEvent.conditionMet());
}

void WaitForEventTest::testConstructorWithNullObject() {
    WaitForEvent waitForEvent(0, QEvent::ChildAdded);

    QVERIFY(!waitForEvent.isActive());
    QVERIFY(!waitForEvent.conditionMet());
}

void WaitForEventTest::testConstructorDefault() {
    WaitForEvent waitForEvent;
    waitForEvent.setEvent(this, "ChildAdded");

    QVERIFY(!waitForEvent.isActive());
    QVERIFY(!waitForEvent.conditionMet());
}

void WaitForEventTest::testConstructorDefaultWithNullObject() {
    WaitForEvent waitForEvent;
    waitForEvent.setEvent(0, "ChildAdded");

    QVERIFY(!waitForEvent.isActive());
    QVERIFY(!waitForEvent.conditionMet());
}

void WaitForEventTest::testSetActive() {
    WaitForEvent waitForEvent(this, QEvent::ChildAdded);
    waitForEvent.mConditionMet = true;

    waitForEvent.setActive(true);

    QVERIFY(waitForEvent.isActive());
    QVERIFY(!waitForEvent.conditionMet());
}

void WaitForEventTest::testWaitEnded() {
    QObject parentObject;

    WaitForEvent waitForEvent(&parentObject, QEvent::ChildAdded);
    waitForEvent.setActive(true);

    //WaitFor* must be registered in order to be used with QSignalSpy
    int waitForStarType = qRegisterMetaType<WaitFor*>("WaitFor*");
    QSignalSpy waitEndedSpy(&waitForEvent, SIGNAL(waitEnded(WaitFor*)));

    QObject* childObject = new QObject();
    childObject->setParent(&parentObject);

    QVERIFY(waitForEvent.conditionMet());
    QCOMPARE(waitEndedSpy.count(), 1);
    QVariant argument = waitEndedSpy.at(0).at(0);
    QCOMPARE(argument.userType(), waitForStarType);
    QCOMPARE(qvariant_cast<WaitFor*>(argument), &waitForEvent);
}

void WaitForEventTest::testWaitEndedWithDefaultConstructor() {
    QObject parentObject;

    WaitForEvent waitForEvent;
    waitForEvent.setEvent(&parentObject, "ChildAdded");
    waitForEvent.setActive(true);

    //WaitFor* must be registered in order to be used with QSignalSpy
    int waitForStarType = qRegisterMetaType<WaitFor*>("WaitFor*");
    QSignalSpy waitEndedSpy(&waitForEvent, SIGNAL(waitEnded(WaitFor*)));

    QObject* childObject = new QObject();
    childObject->setParent(&parentObject);

    QVERIFY(waitForEvent.conditionMet());
    QCOMPARE(waitEndedSpy.count(), 1);
    QVariant argument = waitEndedSpy.at(0).at(0);
    QCOMPARE(argument.userType(), waitForStarType);
    QCOMPARE(qvariant_cast<WaitFor*>(argument), &waitForEvent);
}

void WaitForEventTest::testWaitEndedNotActive() {
    QObject parentObject;

    WaitForEvent waitForEvent(&parentObject, QEvent::ChildAdded);

    qRegisterMetaType<WaitFor*>("WaitFor*");
    QSignalSpy waitEndedSpy(&waitForEvent, SIGNAL(waitEnded(WaitFor*)));

    QObject* childObject = new QObject();
    childObject->setParent(&parentObject);

    QVERIFY(!waitForEvent.conditionMet());
    QCOMPARE(waitEndedSpy.count(), 0);
}

void WaitForEventTest::testHandleEvent() {
    QObject parentObject;

    WaitForEventWithCustomHandling waitForEvent(&parentObject,
                                                QEvent::ChildAdded);
    waitForEvent.setActive(true);

    QObject* childObject = new QObject();
    childObject->setParent(&parentObject);

    QCOMPARE(waitForEvent.mHandledEventsCount, 1);
}

void WaitForEventTest::testHandleEventNotActive() {
    QObject parentObject;

    WaitForEventWithCustomHandling waitForEvent(&parentObject,
                                                QEvent::ChildAdded);

    QObject* childObject = new QObject();
    childObject->setParent(&parentObject);

    QCOMPARE(waitForEvent.mHandledEventsCount, 0);
}

void WaitForEventTest::testHandleEventEventTypeNotExpected() {
    QObject parentObject;

    WaitForEventWithCustomHandling waitForEvent(&parentObject,
                                                QEvent::ChildRemoved);
    waitForEvent.setActive(true);

    QObject* childObject = new QObject();
    childObject->setParent(&parentObject);

    QCOMPARE(waitForEvent.mHandledEventsCount, 0);

    childObject->setParent(0);

    QCOMPARE(waitForEvent.mHandledEventsCount, 1);

    childObject->setParent(&parentObject);

    QCOMPARE(waitForEvent.mHandledEventsCount, 1);
}

void WaitForEventTest::testEventSentAfterWaitForDestruction() {
    QObject parentObject;

    WaitForEvent* waitForEvent = new WaitForEvent(&parentObject,
                                                  QEvent::ChildAdded);
    waitForEvent->setActive(true);

    QObject* childObject = new QObject();
    childObject->setParent(&parentObject);

    delete waitForEvent;

    QObject* anotherChildObject = new QObject();
    anotherChildObject->setParent(&parentObject);

    //Nothing is checked explicitly. Implicitly, it is checked if deleting the
    //event filter object and then sending another event crashes the application
}

}

QTEST_MAIN(ktutorial::WaitForEventTest)

#include "WaitForEventTest.moc"
