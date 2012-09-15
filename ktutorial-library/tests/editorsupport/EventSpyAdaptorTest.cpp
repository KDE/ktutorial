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

#include <QApplication>

#include "EventSpyAdaptor.h"
#include "EventSpy.h"
#include "ObjectRegister.h"

namespace ktutorial {
namespace editorsupport {

class EventSpyAdaptorTest: public QObject {
Q_OBJECT

private slots:

    void testConstructor();

    void testEventReceived();

};

void EventSpyAdaptorTest::testConstructor() {
    EventSpy spy;
    ObjectRegister objectRegister;
    EventSpyAdaptor* adaptor = new EventSpyAdaptor(&spy, &objectRegister);

    QCOMPARE(adaptor->parent(), &spy);
}

void EventSpyAdaptorTest::testEventReceived() {
    EventSpy spy;
    QObject spiedObject;
    spy.addObjectToSpy(&spiedObject);
    ObjectRegister objectRegister;
    EventSpyAdaptor* adaptor = new EventSpyAdaptor(&spy, &objectRegister);

    QSignalSpy eventEmittedSpy(adaptor, SIGNAL(eventReceived(int,QString)));

    //Send an event not managed by QObject to avoid messing up its internal
    //state
    QEvent event(QEvent::Show);
    QApplication::sendEvent(&spiedObject, &event);

    QCOMPARE(eventEmittedSpy.count(), 1);
    QVariant argument = eventEmittedSpy.at(0).at(0);
    QCOMPARE(argument.type(), QVariant::Int);
    QCOMPARE(argument.toInt(), objectRegister.idForObject(&spiedObject));
    argument = eventEmittedSpy.at(0).at(1);
    QCOMPARE(argument.type(), QVariant::String);
    QCOMPARE(argument.toString(), QString("Show"));
}

}
}

QTEST_MAIN(ktutorial::editorsupport::EventSpyAdaptorTest)

#include "EventSpyAdaptorTest.moc"
