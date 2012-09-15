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

#define protected public
#define private public
#include "TargetApplication.h"
#undef private
#undef protected

#include <QApplication>

#include <KProcess>

#include "RemoteEditorSupport.h"
#include "RemoteObject.h"

class TargetApplicationTest: public QObject {
Q_OBJECT

private slots:

    void initTestCase();

    void testSelf();

    void testConstructor();

    void testSetTargetApplicationFilePath();
    void testSetTargetApplicationFilePathAbsoluteWithRelativeComponents();
    void testSetTargetApplicationFilePathRelative();

    void testStart();
    void testStartApplicationWithDelayedRegister();
    void testStartAlreadyStarted();
    void testStartAfterSettingAgainTargetApplicationFilePath();
    void testStartAfterChangingTargetApplicationFilePath();
    void testStartAfterApplicationKilled();
    void testStartApplicationFilePathNotSet();
    void testStartInvalidApplicationFilePath();
    void testStartApplicationNotUsingKTutorial();

    void testApplicationStopped();
    void testApplicationStoppedBeforeBeingSuccessfullyStarted();
    void testApplicationNotUsingKTutorialStoppedBeforeCheckTimeout();

private:

    QString mTargetApplicationStubPath;

    bool waitForSignalCount(const QSignalSpy* spy, int count,
                            int timeout) const;

};

void TargetApplicationTest::initTestCase() {
    mTargetApplicationStubPath = QApplication::applicationDirPath() +
                                                    "/TargetApplicationStub";
}

void TargetApplicationTest::testSelf() {
    TargetApplication* instance1 = TargetApplication::self();
    TargetApplication* instance2 = TargetApplication::self();

    QVERIFY(instance1);
    QVERIFY(instance2);
    QVERIFY(instance1 == instance2);
}

void TargetApplicationTest::testConstructor() {
    TargetApplication targetApplication;

    QCOMPARE(targetApplication.targetApplicationFilePath(), QString(""));
    QCOMPARE(targetApplication.remoteEditorSupport(), (RemoteEditorSupport*)0);
}

void TargetApplicationTest::testSetTargetApplicationFilePath() {
    TargetApplication targetApplication;
    targetApplication.setTargetApplicationFilePath("/usr/bin/dummy");

    QCOMPARE(targetApplication.targetApplicationFilePath(),
             QString("/usr/bin/dummy"));
}

void TargetApplicationTest::
            testSetTargetApplicationFilePathAbsoluteWithRelativeComponents() {
    TargetApplication targetApplication;
    targetApplication.setTargetApplicationFilePath("/usr/../bin//./dummy");

    QCOMPARE(targetApplication.targetApplicationFilePath(),
             QString("/bin/dummy"));
}

void TargetApplicationTest::testSetTargetApplicationFilePathRelative() {
    TargetApplication targetApplication;
    targetApplication.setTargetApplicationFilePath("A relative path");

    QCOMPARE(targetApplication.targetApplicationFilePath(), QString(""));
}

void TargetApplicationTest::testStart() {
    TargetApplication targetApplication;
    targetApplication.setTargetApplicationFilePath(mTargetApplicationStubPath);

    QSignalSpy startedSpy(&targetApplication, SIGNAL(started()));
    targetApplication.start();

    QVERIFY(waitForSignalCount(&startedSpy, 1, 10000));
    QVERIFY(targetApplication.remoteEditorSupport());
    QCOMPARE(targetApplication.remoteEditorSupport()->mainWindow()->name(),
             QString("The object name 42"));
}

void TargetApplicationTest::testStartApplicationWithDelayedRegister() {
    TargetApplication targetApplication;
    targetApplication.setTargetApplicationFilePath(
                        QApplication::applicationDirPath() +
                                "/TargetApplicationStubWithDelayedRegister");

    QSignalSpy startedSpy(&targetApplication, SIGNAL(started()));
    targetApplication.start();

    QVERIFY(waitForSignalCount(&startedSpy, 1, 10000));
    QVERIFY(targetApplication.remoteEditorSupport());
    QCOMPARE(targetApplication.remoteEditorSupport()->mainWindow()->name(),
             QString("The object name 42"));
}

void TargetApplicationTest::testStartAlreadyStarted() {
    TargetApplication targetApplication;
    targetApplication.setTargetApplicationFilePath(mTargetApplicationStubPath);

    QSignalSpy startedSpy(&targetApplication, SIGNAL(started()));
    targetApplication.start();
    QVERIFY(waitForSignalCount(&startedSpy, 1, 10000));

    RemoteEditorSupport* oldRemoteEditorSupport =
                                        targetApplication.remoteEditorSupport();

    targetApplication.start();

    //Give time to ensure that "started" signal is not sent again
    QVERIFY(!waitForSignalCount(&startedSpy, 2, 1000));
    QCOMPARE(targetApplication.remoteEditorSupport(), oldRemoteEditorSupport);
    QCOMPARE(targetApplication.remoteEditorSupport()->mainWindow()->name(),
             QString("The object name 42"));
}

void TargetApplicationTest::
                        testStartAfterSettingAgainTargetApplicationFilePath() {
    TargetApplication targetApplication;
    targetApplication.setTargetApplicationFilePath(mTargetApplicationStubPath);

    QSignalSpy startedSpy(&targetApplication, SIGNAL(started()));
    targetApplication.start();
    QVERIFY(waitForSignalCount(&startedSpy, 1, 10000));

    RemoteEditorSupport* oldRemoteEditorSupport =
                                        targetApplication.remoteEditorSupport();

    targetApplication.setTargetApplicationFilePath("");
    targetApplication.setTargetApplicationFilePath(mTargetApplicationStubPath);

    targetApplication.start();

    //Give time to ensure that "started" signal is not sent again
    QVERIFY(!waitForSignalCount(&startedSpy, 2, 1000));
    QCOMPARE(targetApplication.remoteEditorSupport(), oldRemoteEditorSupport);
    QCOMPARE(targetApplication.remoteEditorSupport()->mainWindow()->name(),
             QString("The object name 42"));
}

//TargetApplication::Error must be declared as a metatype to be used in
//qvariant_cast
Q_DECLARE_METATYPE(TargetApplication::Error)

void TargetApplicationTest::testStartAfterChangingTargetApplicationFilePath() {
    TargetApplication targetApplication;
    targetApplication.setTargetApplicationFilePath(mTargetApplicationStubPath);

    QSignalSpy startedSpy(&targetApplication, SIGNAL(started()));
    targetApplication.start();
    QVERIFY(waitForSignalCount(&startedSpy, 1, 10000));

    QVERIFY(targetApplication.remoteEditorSupport());

    targetApplication.setTargetApplicationFilePath("/other/executable");

    QSignalSpy finishedSpy(&targetApplication, SIGNAL(finished()));
    //TargetApplication::Error must be registered in order to be used with
    //QSignalSpy
    qRegisterMetaType<TargetApplication::Error>("TargetApplication::Error");
    QSignalSpy startFailedSpy(&targetApplication,
                              SIGNAL(startFailed(TargetApplication::Error)));
    targetApplication.start();

    QVERIFY(waitForSignalCount(&finishedSpy, 1, 10000));
    QVERIFY(waitForSignalCount(&startFailedSpy, 1, 10000));
    QVariant argument = startFailedSpy.at(0).at(0);
    QCOMPARE(qvariant_cast<TargetApplication::Error>(argument),
             TargetApplication::InvalidPath);
    //Give time to ensure that "started" signal is not sent again
    QVERIFY(!waitForSignalCount(&startedSpy, 2, 1000));
    QVERIFY(!targetApplication.remoteEditorSupport());
}

void TargetApplicationTest::testStartAfterApplicationKilled() {
    TargetApplication targetApplication;
    targetApplication.setTargetApplicationFilePath(mTargetApplicationStubPath);

    QSignalSpy startedSpy(&targetApplication, SIGNAL(started()));
    targetApplication.start();
    QVERIFY(waitForSignalCount(&startedSpy, 1, 10000));

    RemoteEditorSupport* oldRemoteEditorSupport =
                                        targetApplication.remoteEditorSupport();

    QSignalSpy finishedSpy(&targetApplication, SIGNAL(finished()));
    targetApplication.mProcess->kill();
    QVERIFY(waitForSignalCount(&finishedSpy, 1, 10000));

    targetApplication.start();

    QVERIFY(waitForSignalCount(&startedSpy, 2, 10000));
    QVERIFY(targetApplication.remoteEditorSupport());
    QVERIFY(targetApplication.remoteEditorSupport() != oldRemoteEditorSupport);
    QCOMPARE(targetApplication.remoteEditorSupport()->mainWindow()->name(),
             QString("The object name 42"));
}

void TargetApplicationTest::testStartApplicationFilePathNotSet() {
    TargetApplication targetApplication;
    targetApplication.setTargetApplicationFilePath("");

    //TargetApplication::Error must be registered in order to be used with
    //QSignalSpy
    qRegisterMetaType<TargetApplication::Error>("TargetApplication::Error");
    QSignalSpy startFailedSpy(&targetApplication,
                              SIGNAL(startFailed(TargetApplication::Error)));
    QSignalSpy startedSpy(&targetApplication, SIGNAL(started()));
    QSignalSpy finishedSpy(&targetApplication, SIGNAL(finished()));

    targetApplication.start();

    QVERIFY(waitForSignalCount(&startFailedSpy, 1, 10000));
    QVariant argument = startFailedSpy.at(0).at(0);
    QCOMPARE(qvariant_cast<TargetApplication::Error>(argument),
             TargetApplication::InvalidPath);
    QCOMPARE(startedSpy.count(), 0);
    QCOMPARE(finishedSpy.count(), 0);
}

void TargetApplicationTest::testStartInvalidApplicationFilePath() {
    TargetApplication targetApplication;
    targetApplication.setTargetApplicationFilePath("/some/invalid/file/path");

    //TargetApplication::Error must be registered in order to be used with
    //QSignalSpy
    qRegisterMetaType<TargetApplication::Error>("TargetApplication::Error");
    QSignalSpy startFailedSpy(&targetApplication,
                              SIGNAL(startFailed(TargetApplication::Error)));
    QSignalSpy startedSpy(&targetApplication, SIGNAL(started()));
    QSignalSpy finishedSpy(&targetApplication, SIGNAL(finished()));

    targetApplication.start();

    QVERIFY(waitForSignalCount(&startFailedSpy, 1, 10000));
    QVariant argument = startFailedSpy.at(0).at(0);
    QCOMPARE(qvariant_cast<TargetApplication::Error>(argument),
             TargetApplication::InvalidPath);
    QCOMPARE(startedSpy.count(), 0);
    QCOMPARE(finishedSpy.count(), 0);
}

void TargetApplicationTest::testStartApplicationNotUsingKTutorial() {
    TargetApplication targetApplication;
    targetApplication.setTargetApplicationFilePath(
                    QApplication::applicationDirPath() + "/DummyApplication");

    //TargetApplication::Error must be registered in order to be used with
    //QSignalSpy
    qRegisterMetaType<TargetApplication::Error>("TargetApplication::Error");
    QSignalSpy startFailedSpy(&targetApplication,
                              SIGNAL(startFailed(TargetApplication::Error)));
    QSignalSpy startedSpy(&targetApplication, SIGNAL(started()));
    QSignalSpy finishedSpy(&targetApplication, SIGNAL(finished()));

    targetApplication.start();

    //Time out higher than the internal TargetApplication time out
    QVERIFY(waitForSignalCount(&startFailedSpy, 1, 10000));
    QVariant argument = startFailedSpy.at(0).at(0);
    QCOMPARE(qvariant_cast<TargetApplication::Error>(argument),
             TargetApplication::InvalidApplication);
    QCOMPARE(startedSpy.count(), 0);
    QCOMPARE(finishedSpy.count(), 0);
}

void TargetApplicationTest::testApplicationStopped() {
    TargetApplication targetApplication;
    targetApplication.setTargetApplicationFilePath(mTargetApplicationStubPath);

    QSignalSpy startedSpy(&targetApplication, SIGNAL(started()));
    targetApplication.start();
    QVERIFY(waitForSignalCount(&startedSpy, 1, 10000));

    QSignalSpy finishedSpy(&targetApplication, SIGNAL(finished()));
    targetApplication.mProcess->kill();

    QVERIFY(waitForSignalCount(&finishedSpy, 1, 10000));
    QVERIFY(!targetApplication.remoteEditorSupport());
}

void TargetApplicationTest::
                        testApplicationStoppedBeforeBeingSuccessfullyStarted() {
    TargetApplication targetApplication;
    targetApplication.setTargetApplicationFilePath(mTargetApplicationStubPath);

    //TargetApplication::Error must be registered in order to be used with
    //QSignalSpy
    qRegisterMetaType<TargetApplication::Error>("TargetApplication::Error");
    QSignalSpy startFailedSpy(&targetApplication,
                              SIGNAL(startFailed(TargetApplication::Error)));
    QSignalSpy startedSpy(&targetApplication, SIGNAL(started()));
    QSignalSpy finishedSpy(&targetApplication, SIGNAL(finished()));

    targetApplication.start();

    //Give the application some time to start before killing it, but not enough
    //time to start successfully
    QTest::qWait(5);

    targetApplication.mProcess->kill();

    QVERIFY(waitForSignalCount(&startFailedSpy, 1, 10000));
    QVariant argument = startFailedSpy.at(0).at(0);
    QCOMPARE(qvariant_cast<TargetApplication::Error>(argument),
             TargetApplication::InvalidApplication);
    QVERIFY(!targetApplication.remoteEditorSupport());

    //Keep waiting until the timeout expires to ensure that no other signal is
    //emitted or the process is tried to be killed again
    QTest::qWait(3000);

    QCOMPARE(startedSpy.count(), 0);
    QCOMPARE(startFailedSpy.count(), 1);
    QCOMPARE(finishedSpy.count(), 0);
}

void TargetApplicationTest::
                testApplicationNotUsingKTutorialStoppedBeforeCheckTimeout() {
    TargetApplication targetApplication;
    targetApplication.setTargetApplicationFilePath(
                    QApplication::applicationDirPath() + "/DummyApplication");

    //TargetApplication::Error must be registered in order to be used with
    //QSignalSpy
    qRegisterMetaType<TargetApplication::Error>("TargetApplication::Error");
    QSignalSpy startFailedSpy(&targetApplication,
                              SIGNAL(startFailed(TargetApplication::Error)));
    QSignalSpy startedSpy(&targetApplication, SIGNAL(started()));
    QSignalSpy finishedSpy(&targetApplication, SIGNAL(finished()));

    targetApplication.start();

    //Give the application some time to start before killing it, but not enough
    //time to realize that it does not use KTutorial
    QTest::qWait(1000);

    targetApplication.mProcess->kill();

    QVERIFY(waitForSignalCount(&startFailedSpy, 1, 10000));
    QVariant argument = startFailedSpy.at(0).at(0);
    QCOMPARE(qvariant_cast<TargetApplication::Error>(argument),
             TargetApplication::InvalidApplication);
    QVERIFY(!targetApplication.remoteEditorSupport());

    //Keep waiting until the timeout expires to ensure that no other signal is
    //emitted or the process is tried to be killed again
    QTest::qWait(3000);

    QCOMPARE(startedSpy.count(), 0);
    QCOMPARE(startFailedSpy.count(), 1);
    QCOMPARE(finishedSpy.count(), 0);
}

/////////////////////////////////// Helpers ////////////////////////////////////

template <typename Class>
bool waitFor(Class &object, bool (Class::*condition)() const, int timeout) {
    QElapsedTimer timer;
    timer.start();
    do {
        QTest::qWait(100);
    } while (!(object.*condition)() && timer.elapsed() < timeout);

    if (timer.elapsed() >= timeout) {
        return false;
    }

    return true;    
}

class SignalCountCondition {
public:
    const QSignalSpy* mSpy;
    int mCount;

    bool condition() const {
        return mSpy->count() == mCount;
    }
};

bool TargetApplicationTest::waitForSignalCount(const QSignalSpy* spy, int count,
                                               int timeout) const {
    SignalCountCondition helper;
    helper.mSpy = spy;
    helper.mCount = count;
    return waitFor(helper, &SignalCountCondition::condition, timeout);
}

QTEST_MAIN(TargetApplicationTest)

#include "TargetApplicationTest.moc"
