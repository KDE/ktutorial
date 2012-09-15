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

#include "RemoteEventSpy.h"

#include <QtDBus/QtDBus>

#include "RemoteClassStubs.h"
#include "RemoteObject.h"
#include "RemoteObjectMapper.h"

class RemoteEventSpyTest: public QObject {
Q_OBJECT

private slots:

    void init();
    void cleanup();

    void testEventReceived();

private:

    StubEventSpy* mEventSpy;
    StubObjectRegister* mObjectRegister;

};

void RemoteEventSpyTest::init() {
    QVERIFY(QDBusConnection::sessionBus().isConnected());

    mEventSpy = new StubEventSpy();
    QDBusConnection::sessionBus().registerObject("/ktutorial/EventSpy",
                            mEventSpy, QDBusConnection::ExportAllSignals);

    mObjectRegister = new StubObjectRegister();
    QDBusConnection::sessionBus().registerObject("/ktutorial/ObjectRegister",
                            mObjectRegister, QDBusConnection::ExportAdaptors);
}

void RemoteEventSpyTest::cleanup() {
    QDBusConnection::sessionBus().unregisterObject("/ktutorial/EventSpy");
    delete mEventSpy;

    QDBusConnection::sessionBus().unregisterObject("/ktutorial/ObjectRegister");
    delete mObjectRegister;
}

//RemoteObject* must be declared as a metatype to be used in qvariant_cast
Q_DECLARE_METATYPE(RemoteObject*);

void RemoteEventSpyTest::testEventReceived() {
    RemoteObjectMapper mapper(QDBusConnection::sessionBus().baseService());
    RemoteEventSpy remoteEventSpy(QDBusConnection::sessionBus().baseService(),
                                  &mapper);

    //RemoteObject* must be registered in order to be used with QSignalSpy
    int remoteObjectStarType =
                            qRegisterMetaType<RemoteObject*>("RemoteObject*");
    QSignalSpy eventReceivedSpy(&remoteEventSpy,
                                SIGNAL(eventReceived(RemoteObject*, QString)));

    mEventSpy->emitEventReceived(42, "Close");

    //Give D-Bus time to deliver the signal
    QTest::qWait(100);

    QCOMPARE(eventReceivedSpy.count(), 1);
    QVariant argument = eventReceivedSpy.at(0).at(0);
    QCOMPARE(argument.userType(), remoteObjectStarType);
    QCOMPARE(qvariant_cast<RemoteObject*>(argument), mapper.remoteObject(42));
    QCOMPARE(mapper.remoteObject(42)->name(), QString("The object name 42"));
    argument = eventReceivedSpy.at(0).at(1);
    QCOMPARE(argument.type(), QVariant::String);
    QCOMPARE(argument.toString(), QString("Close"));
}

QTEST_MAIN(RemoteEventSpyTest)

#include "RemoteEventSpyTest.moc"
