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

#include <QApplication>
#include <QSignalSpy>

#include "EventSpy.h"

//QEvent* must be declared as a metatype to be used in qvariant_cast
Q_DECLARE_METATYPE(QEvent*);

namespace ktutorial {
namespace editorsupport {

class EventSpyTest: public QObject {
Q_OBJECT

private slots:

    void initTestCase();

    void testConstructor();

    void testEventReceived();
    void testEventReceivedInChildObject();
    void testEventReceivedInChildObjectAddedAfterSpyingStart();
    void testEventReceivedSeveralObjects();

private:

    int mEventStarType;

    void assertEventReceivedSignal(const QSignalSpy& spy, int index,
                                   QObject* object, QEvent* event) const;

};

void EventSpyTest::initTestCase() {
    //QEvent* must be registered in order to be used with QSignalSpy
    mEventStarType = qRegisterMetaType<QEvent*>("QEvent*");
}

void EventSpyTest::testConstructor() {
    QObject parent;
    EventSpy* spy = new EventSpy(&parent);

    QCOMPARE(spy->parent(), &parent);
}

void EventSpyTest::testEventReceived() {
    EventSpy spy;
    QObject spiedObject;
    spy.addObjectToSpy(&spiedObject);

    QSignalSpy eventEmittedSpy(&spy, SIGNAL(eventReceived(QObject*,QEvent*)));

    //Send an event not managed by QObject to avoid messing up its internal
    //state
    QEvent event(QEvent::Show);
    QApplication::sendEvent(&spiedObject, &event);

    QCOMPARE(eventEmittedSpy.count(), 1);
    assertEventReceivedSignal(eventEmittedSpy, 0, &spiedObject, &event);
}

void EventSpyTest::testEventReceivedInChildObject() {
    EventSpy spy;
    QObject spiedObject;
    QObject* childObject = new QObject(&spiedObject);
    spy.addObjectToSpy(&spiedObject);

    QSignalSpy eventEmittedSpy(&spy, SIGNAL(eventReceived(QObject*,QEvent*)));

    //Send an event not managed by QObject to avoid messing up its internal
    //state
    QEvent event(QEvent::Show);
    QApplication::sendEvent(childObject, &event);

    QCOMPARE(eventEmittedSpy.count(), 1);
    assertEventReceivedSignal(eventEmittedSpy, 0, childObject, &event);
}

void EventSpyTest::testEventReceivedInChildObjectAddedAfterSpyingStart() {
    EventSpy spy;
    QObject spiedObject;
    spy.addObjectToSpy(&spiedObject);

    QSignalSpy eventEmittedSpy(&spy, SIGNAL(eventReceived(QObject*,QEvent*)));

    QObject* childObject = new QObject(&spiedObject);

    //Send an event not managed by QObject to avoid messing up its internal
    //state
    QEvent event(QEvent::Show);
    QApplication::sendEvent(childObject, &event);

    QCOMPARE(eventEmittedSpy.count(), 2);
    //First event is a ChildAdded emitted when the childObject was added. The
    //event is automatically destroyed afterwards, so it can not be checked here
    assertEventReceivedSignal(eventEmittedSpy, 1, childObject, &event);
}

void EventSpyTest::testEventReceivedSeveralObjects() {
    EventSpy spy;
    QObject spiedObject1;
    spy.addObjectToSpy(&spiedObject1);
    QObject spiedObject2;
    spy.addObjectToSpy(&spiedObject2);

    QSignalSpy eventEmittedSpy(&spy, SIGNAL(eventReceived(QObject*,QEvent*)));

    //Send an event not managed by QObject to avoid messing up its internal
    //state
    QEvent event1(QEvent::Show);
    QApplication::sendEvent(&spiedObject1, &event1);
    QEvent event2(QEvent::Show);
    QApplication::sendEvent(&spiedObject2, &event2);

    QCOMPARE(eventEmittedSpy.count(), 2);
    assertEventReceivedSignal(eventEmittedSpy, 0, &spiedObject1, &event1);
    assertEventReceivedSignal(eventEmittedSpy, 1, &spiedObject2, &event2);
}

/////////////////////////////////Helpers////////////////////////////////////////

void EventSpyTest::assertEventReceivedSignal(const QSignalSpy& spy, int index,
                                             QObject* object,
                                             QEvent* event) const {
    QCOMPARE(spy.at(index).count(), 2);

    QVariant argument = spy.at(index).at(0);
    QCOMPARE(argument.userType(), (int)QMetaType::QObjectStar);
    QCOMPARE(qvariant_cast<QObject*>(argument), object);

    argument = spy.at(index).at(1);
    QCOMPARE(argument.userType(), mEventStarType);
    QCOMPARE(qvariant_cast<QEvent*>(argument), event);
}

}
}

QTEST_MAIN(ktutorial::editorsupport::EventSpyTest)

#include "EventSpyTest.moc"
