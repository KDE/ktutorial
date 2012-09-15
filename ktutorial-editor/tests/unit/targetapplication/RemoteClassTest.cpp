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

#include "RemoteClass.h"

#include <QtDBus/QtDBus>

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

class RemoteClassTest: public QObject {
Q_OBJECT

private slots:

    void init();
    void cleanup();

    void testClassName();

    void testSuperClass();
    void testSuperClassWhenRemoteClassIsNotAvailable();

    void testPropertyList();
    void testPropertyListWhenRemoteClassIsNotAvailable();

    void testSignalList();
    void testSignalListWhenRemoteClassIsNotAvailable();

private:

    StubObjectRegister* mObjectRegister;

    QString mService;
    RemoteObjectMapper* mMapper;

};

void RemoteClassTest::init() {
    QVERIFY(QDBusConnection::sessionBus().isConnected());

    mObjectRegister = new StubObjectRegister();
    QDBusConnection::sessionBus().registerObject("/ktutorial/ObjectRegister",
                            mObjectRegister, QDBusConnection::ExportAdaptors);

    mService = QDBusConnection::sessionBus().baseService();
    mMapper = new RemoteObjectMapper(mService);
}

void RemoteClassTest::cleanup() {
    delete mMapper;

    QDBusConnection::sessionBus().unregisterObject("/ktutorial/ObjectRegister");
    delete mObjectRegister;
}

void RemoteClassTest::testClassName() {
    RemoteClass remoteClass(mService, mMapper, "The class name");

    QCOMPARE(remoteClass.className(), QString("The class name"));
}

void RemoteClassTest::testSuperClass() {
    RemoteClass remoteClass(mService, mMapper, "ChildClass");

    QCOMPARE(remoteClass.superClass()->className(), QString("Class"));
    QCOMPARE(remoteClass.superClass()->superClass(), (RemoteClass*)0);
}

void RemoteClassTest::testSuperClassWhenRemoteClassIsNotAvailable() {
    RemoteClass remoteClass(mService, mMapper, "Class");

    QDBusConnection::sessionBus().unregisterObject("/ktutorial/ObjectRegister");

    EXPECT_EXCEPTION(remoteClass.superClass(), DBusException);
}

void RemoteClassTest::testPropertyList() {
    RemoteClass remoteClass(mService, mMapper, "Class");

    QStringList propertyList = remoteClass.propertyList();
    QCOMPARE(propertyList.count(), 3);
    QCOMPARE(propertyList[0], QString("ClassProperty0"));
    QCOMPARE(propertyList[1], QString("ClassProperty1"));
    QCOMPARE(propertyList[2], QString("ClassProperty2"));
}

void RemoteClassTest::testPropertyListWhenRemoteClassIsNotAvailable() {
    RemoteClass remoteClass(mService, mMapper, "Class");

    QDBusConnection::sessionBus().unregisterObject("/ktutorial/ObjectRegister");

    EXPECT_EXCEPTION(remoteClass.propertyList(), DBusException);
}

void RemoteClassTest::testSignalList() {
    RemoteClass remoteClass(mService, mMapper, "Class");

    QStringList signalList = remoteClass.signalList();
    QCOMPARE(signalList.count(), 3);
    QCOMPARE(signalList[0], QString("ClassSignal0()"));
    QCOMPARE(signalList[1], QString("ClassSignal1()"));
    QCOMPARE(signalList[2], QString("ClassSignal2()"));
}

void RemoteClassTest::testSignalListWhenRemoteClassIsNotAvailable() {
    RemoteClass remoteClass(mService, mMapper, "Class");

    QDBusConnection::sessionBus().unregisterObject("/ktutorial/ObjectRegister");

    EXPECT_EXCEPTION(remoteClass.signalList(), DBusException);
}

QTEST_MAIN(RemoteClassTest)

#include "RemoteClassTest.moc"
