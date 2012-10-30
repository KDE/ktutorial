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

#include <QTest>

#include "RemoteEditorSupport.h"

#include <QSignalSpy>
#include <QtDBus/QtDBus>

#include "RemoteClassStubs.h"
#include "RemoteEventSpy.h"
#include "RemoteObject.h"
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

class RemoteEditorSupportTest: public QObject {
Q_OBJECT

private slots:

    void init();
    void cleanup();

    void testMainWindowSpy();
    void testMainWindowTwice();
    void testMainWindowWhenRemoteEditorSupportIsNotAvailable();

    void testFindObject();
    void testFindObjectWhenRemoteEditorSupportIsNotAvailable();

    void testHighlight();
    void testHighlightWhenRemoteEditorSupportIsNotAvailable();

    void testStopHighlighting();
    void testStopHighlightingWhenRemoteEditorSupportIsNotAvailable();

    void testEnableEventSpy();
    void testEnableEventSpyTwice();
    void testEnableEventSpyWhenRemoteEditorSupportIsNotAvailable();

    void testDisableEventSpy();
    void testDisableEventSpyAfterSeveralEnableEventSpyCalls();
    void testDisableEventSpyTwice();
    void testDisableEventSpyWhenRemoteEditorSupportIsNotAvailable();

    void testTestScriptedTutorial();
    void testTestScriptedTutorialWithStepId();
    void testTestScriptedTutorialWhenRemoteEditorSupportIsNotAvailable();
    void testTestScriptedTutorialWithStepIdWhenRemoteEditorSupportIsNotAvailable();

private:

    StubEditorSupport* mEditorSupport;
    StubObjectRegister* mObjectRegister;

};

void RemoteEditorSupportTest::init() {
    QVERIFY(QDBusConnection::sessionBus().isConnected());

    mEditorSupport = new StubEditorSupport();
    QDBusConnection::sessionBus().registerObject("/ktutorial",
                            mEditorSupport, QDBusConnection::ExportAllSlots);

    mObjectRegister = new StubObjectRegister();
    QDBusConnection::sessionBus().registerObject("/ktutorial/ObjectRegister",
                            mObjectRegister, QDBusConnection::ExportAdaptors);
}

void RemoteEditorSupportTest::cleanup() {
    QDBusConnection::sessionBus().unregisterObject("/ktutorial");
    delete mEditorSupport;

    QDBusConnection::sessionBus().unregisterObject("/ktutorial/ObjectRegister");
    delete mObjectRegister;
}

void RemoteEditorSupportTest::testMainWindowSpy() {
    RemoteObjectMapper mapper(QDBusConnection::sessionBus().baseService());
    RemoteEditorSupport remoteEditorSupport(
                        QDBusConnection::sessionBus().baseService(), &mapper);

    RemoteObject* mainWindow = remoteEditorSupport.mainWindow();

    QVERIFY(mainWindow);
    QCOMPARE(mainWindow->name(), QString("The object name 42"));
}

void RemoteEditorSupportTest::testMainWindowTwice() {
    RemoteObjectMapper mapper(QDBusConnection::sessionBus().baseService());
    RemoteEditorSupport remoteEditorSupport(
                        QDBusConnection::sessionBus().baseService(), &mapper);

    RemoteObject* mainWindow1 = remoteEditorSupport.mainWindow();
    RemoteObject* mainWindow2 = remoteEditorSupport.mainWindow();

    QVERIFY(mainWindow1);
    QVERIFY(mainWindow2);
    QCOMPARE(mainWindow1, mainWindow2);
}

void RemoteEditorSupportTest::
                        testMainWindowWhenRemoteEditorSupportIsNotAvailable() {
    RemoteObjectMapper mapper(QDBusConnection::sessionBus().baseService());
    RemoteEditorSupport remoteEditorSupport(
                        QDBusConnection::sessionBus().baseService(), &mapper);

    QDBusConnection::sessionBus().unregisterObject("/ktutorial");

    EXPECT_EXCEPTION(remoteEditorSupport.mainWindow(), DBusException);
}

void RemoteEditorSupportTest::testFindObject() {
    RemoteObjectMapper mapper(QDBusConnection::sessionBus().baseService());
    RemoteEditorSupport remoteEditorSupport(
                        QDBusConnection::sessionBus().baseService(), &mapper);

    RemoteObject* object = remoteEditorSupport.mainWindow()->children()[3];

    QCOMPARE(remoteEditorSupport.findObject("The object name 423"), object);
}

void RemoteEditorSupportTest::
testFindObjectWhenRemoteEditorSupportIsNotAvailable() {
    RemoteObjectMapper mapper(QDBusConnection::sessionBus().baseService());
    RemoteEditorSupport remoteEditorSupport(
                        QDBusConnection::sessionBus().baseService(), &mapper);

    QDBusConnection::sessionBus().unregisterObject("/ktutorial");

    EXPECT_EXCEPTION(remoteEditorSupport.findObject("The object name 423"),
                     DBusException);
}

void RemoteEditorSupportTest::testHighlight() {
    RemoteObjectMapper mapper(QDBusConnection::sessionBus().baseService());
    RemoteEditorSupport remoteEditorSupport(
                        QDBusConnection::sessionBus().baseService(), &mapper);

    RemoteObject* mainWindow = remoteEditorSupport.mainWindow();

    remoteEditorSupport.highlight(mainWindow);

    QCOMPARE(mEditorSupport->mHighlightRemoteWidgetIds.count(), 1);
    QCOMPARE(mEditorSupport->mHighlightRemoteWidgetIds[0],
             mainWindow->objectId());
}

void RemoteEditorSupportTest::
                        testHighlightWhenRemoteEditorSupportIsNotAvailable() {
    RemoteObjectMapper mapper(QDBusConnection::sessionBus().baseService());
    RemoteEditorSupport remoteEditorSupport(
                        QDBusConnection::sessionBus().baseService(), &mapper);

    RemoteObject* mainWindow = remoteEditorSupport.mainWindow();

    QDBusConnection::sessionBus().unregisterObject("/ktutorial");

    EXPECT_EXCEPTION(remoteEditorSupport.highlight(mainWindow),
                     DBusException);
}

void RemoteEditorSupportTest::testStopHighlighting() {
    RemoteObjectMapper mapper(QDBusConnection::sessionBus().baseService());
    RemoteEditorSupport remoteEditorSupport(
                        QDBusConnection::sessionBus().baseService(), &mapper);

    RemoteObject* mainWindow = remoteEditorSupport.mainWindow();

    remoteEditorSupport.stopHighlighting(mainWindow);

    QCOMPARE(mEditorSupport->mStopHighlightingRemoteWidgetIds.count(), 1);
    QCOMPARE(mEditorSupport->mStopHighlightingRemoteWidgetIds[0],
             mainWindow->objectId());
}

void RemoteEditorSupportTest::
                testStopHighlightingWhenRemoteEditorSupportIsNotAvailable() {
    RemoteObjectMapper mapper(QDBusConnection::sessionBus().baseService());
    RemoteEditorSupport remoteEditorSupport(
                        QDBusConnection::sessionBus().baseService(), &mapper);

    RemoteObject* mainWindow = remoteEditorSupport.mainWindow();

    QDBusConnection::sessionBus().unregisterObject("/ktutorial");

    EXPECT_EXCEPTION(remoteEditorSupport.stopHighlighting(mainWindow),
                     DBusException);
}

//RemoteObject* must be declared as a metatype to be used in qvariant_cast
Q_DECLARE_METATYPE(RemoteObject*);

void RemoteEditorSupportTest::testEnableEventSpy() {
    RemoteObjectMapper mapper(QDBusConnection::sessionBus().baseService());
    RemoteEditorSupport remoteEditorSupport(
                        QDBusConnection::sessionBus().baseService(), &mapper);

    RemoteEventSpy* remoteEventSpy = remoteEditorSupport.enableEventSpy();

    QVERIFY(remoteEventSpy);

    //RemoteObject* must be registered in order to be used with QSignalSpy
    int remoteObjectStarType =
                            qRegisterMetaType<RemoteObject*>("RemoteObject*");
    QSignalSpy eventReceivedSpy(remoteEventSpy,
                                SIGNAL(eventReceived(RemoteObject*, QString)));

    mEditorSupport->mEventSpy->emitEventReceived(42, "Close");

    //Give D-Bus time to deliver the signal
    QTest::qWait(100);

    QCOMPARE(mEditorSupport->mEnableEventSpyCount, 1);
    QCOMPARE(eventReceivedSpy.count(), 1);
    QVariant argument = eventReceivedSpy.at(0).at(0);
    QCOMPARE(argument.userType(), remoteObjectStarType);
    QCOMPARE(qvariant_cast<RemoteObject*>(argument), mapper.remoteObject(42));
    QCOMPARE(mapper.remoteObject(42)->name(), QString("The object name 42"));
    argument = eventReceivedSpy.at(0).at(1);
    QCOMPARE(argument.type(), QVariant::String);
    QCOMPARE(argument.toString(), QString("Close"));
}

void RemoteEditorSupportTest::testEnableEventSpyTwice() {
    RemoteObjectMapper mapper(QDBusConnection::sessionBus().baseService());
    RemoteEditorSupport remoteEditorSupport(
                        QDBusConnection::sessionBus().baseService(), &mapper);

    RemoteEventSpy* remoteEventSpy1 = remoteEditorSupport.enableEventSpy();
    RemoteEventSpy* remoteEventSpy2 = remoteEditorSupport.enableEventSpy();

    QVERIFY(remoteEventSpy1);
    QVERIFY(remoteEventSpy2);
    QCOMPARE(remoteEventSpy1, remoteEventSpy2);
    QCOMPARE(mEditorSupport->mEnableEventSpyCount, 1);
}

void RemoteEditorSupportTest::
                    testEnableEventSpyWhenRemoteEditorSupportIsNotAvailable() {
    RemoteObjectMapper mapper(QDBusConnection::sessionBus().baseService());
    RemoteEditorSupport remoteEditorSupport(
                        QDBusConnection::sessionBus().baseService(), &mapper);

    QDBusConnection::sessionBus().unregisterObject("/ktutorial");

    EXPECT_EXCEPTION(remoteEditorSupport.enableEventSpy(), DBusException);
}

void RemoteEditorSupportTest::testDisableEventSpy() {
    RemoteObjectMapper mapper(QDBusConnection::sessionBus().baseService());
    RemoteEditorSupport remoteEditorSupport(
                        QDBusConnection::sessionBus().baseService(), &mapper);

    RemoteEventSpy* remoteEventSpy = remoteEditorSupport.enableEventSpy();

    QVERIFY(remoteEventSpy);

    //RemoteObject* must be registered in order to be used with QSignalSpy
    qRegisterMetaType<RemoteObject*>("RemoteObject*");
    QSignalSpy eventReceivedSpy(remoteEventSpy,
                                SIGNAL(eventReceived(RemoteObject*, QString)));

    remoteEditorSupport.disableEventSpy();

    mEditorSupport->mEventSpy->emitEventReceived(42, "Close");

    //Give D-Bus time to deliver the signal (although nothing should be
    //delivered)
    QTest::qWait(100);

    QCOMPARE(mEditorSupport->mDisableEventSpyCount, 1);
    QCOMPARE(eventReceivedSpy.count(), 0);
}

void RemoteEditorSupportTest::
                        testDisableEventSpyAfterSeveralEnableEventSpyCalls() {
    RemoteObjectMapper mapper(QDBusConnection::sessionBus().baseService());
    RemoteEditorSupport remoteEditorSupport(
                        QDBusConnection::sessionBus().baseService(), &mapper);

    RemoteEventSpy* remoteEventSpy = remoteEditorSupport.enableEventSpy();
    remoteEditorSupport.enableEventSpy();
    remoteEditorSupport.enableEventSpy();

    QVERIFY(remoteEventSpy);

    remoteEditorSupport.disableEventSpy();
    remoteEditorSupport.disableEventSpy();

    QCOMPARE(mEditorSupport->mDisableEventSpyCount, 0);

    remoteEditorSupport.enableEventSpy();
    remoteEditorSupport.disableEventSpy();

    QCOMPARE(mEditorSupport->mDisableEventSpyCount, 0);

    remoteEditorSupport.disableEventSpy();

    QCOMPARE(mEditorSupport->mDisableEventSpyCount, 1);
}

void RemoteEditorSupportTest::testDisableEventSpyTwice() {
    RemoteObjectMapper mapper(QDBusConnection::sessionBus().baseService());
    RemoteEditorSupport remoteEditorSupport(
                        QDBusConnection::sessionBus().baseService(), &mapper);

    RemoteEventSpy* remoteEventSpy = remoteEditorSupport.enableEventSpy();

    QVERIFY(remoteEventSpy);

    remoteEditorSupport.disableEventSpy();

    QCOMPARE(mEditorSupport->mDisableEventSpyCount, 1);

    remoteEditorSupport.disableEventSpy();

    QCOMPARE(mEditorSupport->mDisableEventSpyCount, 1);

    QVERIFY(remoteEditorSupport.enableEventSpy());
}

void RemoteEditorSupportTest::
                    testDisableEventSpyWhenRemoteEditorSupportIsNotAvailable() {
    RemoteObjectMapper mapper(QDBusConnection::sessionBus().baseService());
    RemoteEditorSupport remoteEditorSupport(
                        QDBusConnection::sessionBus().baseService(), &mapper);

    remoteEditorSupport.enableEventSpy();

    QDBusConnection::sessionBus().unregisterObject("/ktutorial");

    EXPECT_EXCEPTION(remoteEditorSupport.disableEventSpy(), DBusException);
}

void RemoteEditorSupportTest::testTestScriptedTutorial() {
    RemoteObjectMapper mapper(QDBusConnection::sessionBus().baseService());
    RemoteEditorSupport remoteEditorSupport(
                        QDBusConnection::sessionBus().baseService(), &mapper);

    remoteEditorSupport.testScriptedTutorial("/some/file");

    QCOMPARE(mEditorSupport->mTestScriptedTutorialFilenames.count(), 1);
    QCOMPARE(mEditorSupport->mTestScriptedTutorialFilenames[0],
             QString("/some/file"));
    QCOMPARE(mEditorSupport->mTestScriptedTutorialStepIds.count(), 0);
}

void RemoteEditorSupportTest::testTestScriptedTutorialWithStepId() {
    RemoteObjectMapper mapper(QDBusConnection::sessionBus().baseService());
    RemoteEditorSupport remoteEditorSupport(
                        QDBusConnection::sessionBus().baseService(), &mapper);

    remoteEditorSupport.testScriptedTutorial("/some/file", "some step id");

    QCOMPARE(mEditorSupport->mTestScriptedTutorialFilenames.count(), 1);
    QCOMPARE(mEditorSupport->mTestScriptedTutorialFilenames[0],
             QString("/some/file"));
    QCOMPARE(mEditorSupport->mTestScriptedTutorialStepIds.count(), 1);
    QCOMPARE(mEditorSupport->mTestScriptedTutorialStepIds[0],
             QString("some step id"));
}

void RemoteEditorSupportTest::
            testTestScriptedTutorialWhenRemoteEditorSupportIsNotAvailable() {
    RemoteObjectMapper mapper(QDBusConnection::sessionBus().baseService());
    RemoteEditorSupport remoteEditorSupport(
                        QDBusConnection::sessionBus().baseService(), &mapper);

    QDBusConnection::sessionBus().unregisterObject("/ktutorial");

    EXPECT_EXCEPTION(remoteEditorSupport.testScriptedTutorial("/some/file"),
                     DBusException);
}

void RemoteEditorSupportTest::
    testTestScriptedTutorialWithStepIdWhenRemoteEditorSupportIsNotAvailable() {
    RemoteObjectMapper mapper(QDBusConnection::sessionBus().baseService());
    RemoteEditorSupport remoteEditorSupport(
                        QDBusConnection::sessionBus().baseService(), &mapper);

    QDBusConnection::sessionBus().unregisterObject("/ktutorial");

    EXPECT_EXCEPTION(remoteEditorSupport.testScriptedTutorial("/some/file",
                                                              "some step id"),
                     DBusException);
}

QTEST_MAIN(RemoteEditorSupportTest)

#include "RemoteEditorSupportTest.moc"
