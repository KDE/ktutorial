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

#include <QDialog>
#include <QSignalSpy>
#include <QTimer>

#include <KXmlGuiWindow>

#define protected public
#define private public
#include "WaitForWindow.h"
#undef private
#undef protected

#include "KTutorial.h"

//WaitFor* must be declared as a metatype to be used in qvariant_cast
Q_DECLARE_METATYPE(ktutorial::WaitFor*);

namespace ktutorial {

class WaitForWindowTest: public QObject {
Q_OBJECT
private slots:

    void initTestCase();

    void testConstructor();
    void testConstructorDefault();

    void testSetActive();

    void testWaitEnded();
    void testWaitEndedByModalDialog();
    void testWaitEndedWithDefaultConstructor();
    void testWaitEndedNotActive();

private:

    KXmlGuiWindow* mMainWindow;

    void queueAssertConditionMet(WaitForWindow* waitForWindow,
                                 QSignalSpy* waitEndedSpy,
                                 int timeToWait);

};

void WaitForWindowTest::initTestCase() {
    //The name of the application must be set to avoid failing an assert in
    //QDBusConnection::registerObject (dbus/qdbusconnection.cpp:697) that checks
    //for a proper object path. The path given is based on the application name,
    //and the application name has to be set to be a valid path.
    qApp->setApplicationName("WaitForWindowTest");

    mMainWindow = new KXmlGuiWindow();
    KTutorial::self()->setup(mMainWindow);
}

void WaitForWindowTest::testConstructor() {
    WaitForWindow waitForWindow("theName");

    QVERIFY(!waitForWindow.isActive());
    QVERIFY(!waitForWindow.conditionMet());
}

void WaitForWindowTest::testConstructorDefault() {
    WaitForWindow waitForWindow;
    waitForWindow.setWindowObjectName("theName");

    QVERIFY(!waitForWindow.isActive());
    QVERIFY(!waitForWindow.conditionMet());
}

void WaitForWindowTest::testSetActive() {
    WaitForWindow waitForWindow("theName");
    waitForWindow.mConditionMet = true;

    waitForWindow.setActive(true);

    QVERIFY(waitForWindow.isActive());
    QVERIFY(!waitForWindow.conditionMet());
}

void WaitForWindowTest::testWaitEnded() {
    WaitForWindow waitForWindow("theName");
    waitForWindow.setActive(true);

    //WaitFor* must be registered in order to be used with QSignalSpy
    int waitForStarType = qRegisterMetaType<WaitFor*>("WaitFor*");
    QSignalSpy waitEndedSpy(&waitForWindow, SIGNAL(waitEnded(WaitFor*)));

    QWidget* otherWindow = new QWidget(mMainWindow);
    otherWindow->setObjectName("otherName");
    otherWindow->setWindowFlags(Qt::Window);
    otherWindow->show();

    otherWindow->deleteLater();

    QVERIFY(!waitForWindow.conditionMet());
    QCOMPARE(waitEndedSpy.count(), 0);

    QWidget* window = new QWidget(mMainWindow);
    window->setObjectName("theName");
    window->setWindowFlags(Qt::Window);
    window->show();

    window->deleteLater();

    QVERIFY(waitForWindow.conditionMet());
    QCOMPARE(waitEndedSpy.count(), 1);
    QVariant argument = waitEndedSpy.at(0).at(0);
    QCOMPARE(argument.userType(), waitForStarType);
    QCOMPARE(qvariant_cast<WaitFor*>(argument), &waitForWindow);
}

void WaitForWindowTest::testWaitEndedByModalDialog() {
    WaitForWindow waitForWindow("theName");
    waitForWindow.setActive(true);

    //WaitFor* must be registered in order to be used with QSignalSpy
    int waitForStarType = qRegisterMetaType<WaitFor*>("WaitFor*");
    QSignalSpy waitEndedSpy(&waitForWindow, SIGNAL(waitEnded(WaitFor*)));

    QDialog* modalDialog = new QDialog(mMainWindow); //krazy:exclude=qclasses
    modalDialog->setObjectName("theName");

    QTimer timerAccept;
    timerAccept.setSingleShot(true);
    timerAccept.setInterval(1500);
    connect(&timerAccept, SIGNAL(timeout()), modalDialog, SLOT(accept()));

    //Check that the condition was met before closing the modal dialog to ensure
    //that the processing of events or signals is not halted due to being modal.
    queueAssertConditionMet(&waitForWindow, &waitEndedSpy, 500);

    timerAccept.start();
    modalDialog->exec(); //krazy:exclude=crashy

    modalDialog->deleteLater();

    QVERIFY(waitForWindow.conditionMet());
    QCOMPARE(waitEndedSpy.count(), 1);
    QVariant argument = waitEndedSpy.at(0).at(0);
    QCOMPARE(argument.userType(), waitForStarType);
    QCOMPARE(qvariant_cast<WaitFor*>(argument), &waitForWindow);
}

void WaitForWindowTest::testWaitEndedWithDefaultConstructor() {
    WaitForWindow waitForWindow;
    waitForWindow.setWindowObjectName("theName");
    waitForWindow.setActive(true);

    //WaitFor* must be registered in order to be used with QSignalSpy
    int waitForStarType = qRegisterMetaType<WaitFor*>("WaitFor*");
    QSignalSpy waitEndedSpy(&waitForWindow, SIGNAL(waitEnded(WaitFor*)));

    QWidget* otherWindow = new QWidget(mMainWindow);
    otherWindow->setObjectName("otherName");
    otherWindow->setWindowFlags(Qt::Window);
    otherWindow->show();

    otherWindow->deleteLater();

    QVERIFY(!waitForWindow.conditionMet());
    QCOMPARE(waitEndedSpy.count(), 0);

    QWidget* window = new QWidget(mMainWindow);
    window->setObjectName("theName");
    window->setWindowFlags(Qt::Window);
    window->show();

    window->deleteLater();

    QVERIFY(waitForWindow.conditionMet());
    QCOMPARE(waitEndedSpy.count(), 1);
    QVariant argument = waitEndedSpy.at(0).at(0);
    QCOMPARE(argument.userType(), waitForStarType);
    QCOMPARE(qvariant_cast<WaitFor*>(argument), &waitForWindow);
}

void WaitForWindowTest::testWaitEndedNotActive() {
    WaitForWindow waitForWindow("theName");

    qRegisterMetaType<WaitFor*>("WaitFor*");
    QSignalSpy waitEndedSpy(&waitForWindow, SIGNAL(waitEnded(WaitFor*)));

    QWidget* window = new QWidget(mMainWindow);
    window->setObjectName("theName");
    window->setWindowFlags(Qt::Window);
    window->show();

    window->deleteLater();

    QVERIFY(!waitForWindow.conditionMet());
    QCOMPARE(waitEndedSpy.count(), 0);
}

/////////////////////////////////// Helpers ////////////////////////////////////

//Modal dialogs don't return to the test code until they are closed. Thus, the
//actions or asserts to be performed while a modal dialog is being shown (like
//checking if a wait for condition was met) must be "queued".
class QueuedActionsHelper: public QObject {
Q_OBJECT
public:

    QueuedActionsHelper(QObject* parent = 0): QObject(parent) {
    }

    void setWaitForWindow(WaitForWindow* waitForWindow) {
        mWaitForWindow = waitForWindow;
    }

    void setWaitEndedSpy(QSignalSpy* waitEndedSpy) {
        mWaitEndedSpy = waitEndedSpy;
    }

public slots:

    void assertConditionMet() {
        QVERIFY(mWaitForWindow->conditionMet());
        QCOMPARE(mWaitEndedSpy->count(), 1);
    }

private:

    WaitForWindow* mWaitForWindow;
    QSignalSpy* mWaitEndedSpy;

};

void WaitForWindowTest::queueAssertConditionMet(WaitForWindow* waitForWindow,
                                                QSignalSpy* waitEndedSpy,
                                                int timeToWait) {
    QueuedActionsHelper* helper = new QueuedActionsHelper(this);
    helper->setWaitForWindow(waitForWindow);
    helper->setWaitEndedSpy(waitEndedSpy);
    QTimer::singleShot(timeToWait, helper, SLOT(assertConditionMet()));
}

}

QTEST_MAIN(ktutorial::WaitForWindowTest)

#include "WaitForWindowTest.moc"
