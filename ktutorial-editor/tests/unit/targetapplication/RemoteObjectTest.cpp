/***************************************************************************
 *   Copyright (C) 2010 by Daniel Calviño Sánchez <danxuliu@gmail.com>     *
 *   Copyright (C) 2011 by Daniel Calviño Sánchez <danxuliu@gmail.com>     *
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

#include "RemoteObject.h"

#include <QtDBus/QtDBus>

#include "RemoteClass.h"
#include "RemoteClassStubs.h"
#include "RemoteObjectMapper.h"

#define EXPECT_EXCEPTION(statement, exception) \
do {\
    try {\
        statement;\
        QFAIL("Expected " #exception " not thrown");\
    } catch (exception e) {\
    } catch (Exception e) {\
        QFAIL("Expected " #exception " not thrown");\
    }\
} while (0)

class RemoteObjectTest: public QObject {
Q_OBJECT

private slots:

    void init();
    void cleanup();

    void testObjectId();

    void testName();
    void testNameWhenRemoteObjectIsNotAvailable();

    void testRemoteClass();
    void testRemoteClassWhenRemoteObjectIsNotAvailable();

    void testChildren();
    void testChildrenWhenRemoteObjectIsNotAvailable();

private:

    StubObjectRegister* mObjectRegister;

    QString mService;
    RemoteObjectMapper* mMapper;

};

void RemoteObjectTest::init() {
    QVERIFY(QDBusConnection::sessionBus().isConnected());

    mObjectRegister = new StubObjectRegister();
    QDBusConnection::sessionBus().registerObject("/ktutorial/ObjectRegister",
                            mObjectRegister, QDBusConnection::ExportAdaptors);

    mService = QDBusConnection::sessionBus().baseService();
    mMapper = new RemoteObjectMapper(mService);
}

void RemoteObjectTest::cleanup() {
    delete mMapper;

    QDBusConnection::sessionBus().unregisterObject("/ktutorial/ObjectRegister");
    delete mObjectRegister;
}

void RemoteObjectTest::testObjectId() {
    RemoteObject remoteObject(mService, mMapper, 42);

    QCOMPARE(remoteObject.objectId(), 42);
}

void RemoteObjectTest::testName() {
    RemoteObject remoteObject(mService, mMapper, 42);

    QCOMPARE(remoteObject.name(), QString("The object name 42"));
}

void RemoteObjectTest::testNameWhenRemoteObjectIsNotAvailable() {
    RemoteObject remoteObject(mService, mMapper, 42);

    QDBusConnection::sessionBus().unregisterObject("/ktutorial/ObjectRegister");

    EXPECT_EXCEPTION(remoteObject.name(), DBusException);
}

void RemoteObjectTest::testRemoteClass() {
    RemoteObject remoteObject(mService, mMapper, 42);

    QCOMPARE(remoteObject.remoteClass()->className(),
             QString("The class name 42"));
}

void RemoteObjectTest::testRemoteClassWhenRemoteObjectIsNotAvailable() {
    RemoteObject remoteObject(mService, mMapper, 42);

    QDBusConnection::sessionBus().unregisterObject("/ktutorial/ObjectRegister");

    EXPECT_EXCEPTION(remoteObject.remoteClass(), DBusException);
}

void RemoteObjectTest::testChildren() {
    RemoteObject remoteObject(mService, mMapper, 42);

    QList<RemoteObject*> children = remoteObject.children();
    QCOMPARE(children.count(), 9);
    QCOMPARE(children[0]->objectId(), 420);
    QCOMPARE(children[1]->objectId(), 421);
    QCOMPARE(children[2]->objectId(), 422);
    QCOMPARE(children[3]->objectId(), 423);
    QCOMPARE(children[4]->objectId(), 5);
    QCOMPARE(children[5]->objectId(), 6);
    QCOMPARE(children[6]->objectId(), 7);
    QCOMPARE(children[7]->objectId(), 8);
    QCOMPARE(children[8]->objectId(), 9);
}

void RemoteObjectTest::testChildrenWhenRemoteObjectIsNotAvailable() {
    RemoteObject remoteObject(mService, mMapper, 42);

    QDBusConnection::sessionBus().unregisterObject("/ktutorial/ObjectRegister");

    EXPECT_EXCEPTION(remoteObject.children(), DBusException);
}

QTEST_MAIN(RemoteObjectTest)

#include "RemoteObjectTest.moc"
