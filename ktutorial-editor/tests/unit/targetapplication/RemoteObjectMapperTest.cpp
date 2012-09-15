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

#include "RemoteObjectMapper.h"

#include <QtDBus/QtDBus>

#include "RemoteClass.h"
#include "RemoteClassStubs.h"
#include "RemoteObject.h"

class RemoteObjectMapperTest: public QObject {
Q_OBJECT

private slots:

    void init();
    void cleanup();

    void testRemoteObject();
    void testRemoteObjectSeveralIds();
    void testRemoteObjectTwice();
    void testRemoteNullObject();

    void testRemoteClass();
    void testRemoteClassSeveralIds();
    void testRemoteClassTwice();

private:

    StubObjectRegister* mObjectRegister;

};

void RemoteObjectMapperTest::init() {
    QVERIFY(QDBusConnection::sessionBus().isConnected());

    mObjectRegister = new StubObjectRegister();
    QDBusConnection::sessionBus().registerObject("/ktutorial/ObjectRegister",
                            mObjectRegister, QDBusConnection::ExportAdaptors);
}

void RemoteObjectMapperTest::cleanup() {
    QDBusConnection::sessionBus().unregisterObject("/ktutorial/ObjectRegister");
    delete mObjectRegister;
}

void RemoteObjectMapperTest::testRemoteObject() {
    RemoteObjectMapper mapper(QDBusConnection::sessionBus().baseService());

    RemoteObject* remoteObject = mapper.remoteObject(42);

    QVERIFY(remoteObject);
    QCOMPARE(remoteObject->name(), QString("The object name 42"));
}

void RemoteObjectMapperTest::testRemoteObjectSeveralIds() {
    RemoteObjectMapper mapper(QDBusConnection::sessionBus().baseService());

    RemoteObject* remoteObject1 = mapper.remoteObject(1);
    RemoteObject* remoteObject2 = mapper.remoteObject(2);
    RemoteObject* remoteObject3 = mapper.remoteObject(3);

    QVERIFY(remoteObject1);
    QVERIFY(remoteObject2);
    QVERIFY(remoteObject3);
    QVERIFY(remoteObject1 != remoteObject2);
    QVERIFY(remoteObject1 != remoteObject3);
    QVERIFY(remoteObject2 != remoteObject3);
    QCOMPARE(remoteObject1->name(), QString("The object name 1"));
    QCOMPARE(remoteObject2->name(), QString("The object name 2"));
    QCOMPARE(remoteObject3->name(), QString("The object name 3"));
}

void RemoteObjectMapperTest::testRemoteObjectTwice() {
    RemoteObjectMapper mapper(QDBusConnection::sessionBus().baseService());

    RemoteObject* remoteObject1 = mapper.remoteObject(42);
    RemoteObject* remoteObject2 = mapper.remoteObject(42);

    QVERIFY(remoteObject1);
    QVERIFY(remoteObject2);
    QVERIFY(remoteObject1 == remoteObject2);
    QCOMPARE(remoteObject1->name(), QString("The object name 42"));
}

void RemoteObjectMapperTest::testRemoteNullObject() {
    RemoteObjectMapper mapper(QDBusConnection::sessionBus().baseService());

    RemoteObject* remoteObject = mapper.remoteObject(0);

    QVERIFY(!remoteObject);
}

void RemoteObjectMapperTest::testRemoteClass() {
    RemoteObjectMapper mapper(QDBusConnection::sessionBus().baseService());

    RemoteClass* remoteClass = mapper.remoteClass("Class");

    QVERIFY(remoteClass);
    QCOMPARE(remoteClass->className(), QString("Class"));
}

void RemoteObjectMapperTest::testRemoteClassSeveralIds() {
    RemoteObjectMapper mapper(QDBusConnection::sessionBus().baseService());

    RemoteClass* remoteClass1 = mapper.remoteClass("Class1");
    RemoteClass* remoteClass2 = mapper.remoteClass("Class2");
    RemoteClass* remoteClass3 = mapper.remoteClass("Class3");

    QVERIFY(remoteClass1);
    QVERIFY(remoteClass2);
    QVERIFY(remoteClass3);
    QVERIFY(remoteClass1 != remoteClass2);
    QVERIFY(remoteClass1 != remoteClass3);
    QVERIFY(remoteClass2 != remoteClass3);
    QCOMPARE(remoteClass1->className(), QString("Class1"));
    QCOMPARE(remoteClass2->className(), QString("Class2"));
    QCOMPARE(remoteClass3->className(), QString("Class3"));
}

void RemoteObjectMapperTest::testRemoteClassTwice() {
    RemoteObjectMapper mapper(QDBusConnection::sessionBus().baseService());

    RemoteClass* remoteClass1 = mapper.remoteClass("Class");
    RemoteClass* remoteClass2 = mapper.remoteClass("Class");

    QVERIFY(remoteClass1);
    QVERIFY(remoteClass2);
    QVERIFY(remoteClass1 == remoteClass2);
    QCOMPARE(remoteClass1->className(), QString("Class"));
}

QTEST_MAIN(RemoteObjectMapperTest)

#include "RemoteObjectMapperTest.moc"
