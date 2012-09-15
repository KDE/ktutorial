/***************************************************************************
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

#include "RemoteObjectNameRegister.h"

#include <KProcess>

#include "../targetapplication/RemoteEditorSupport.h"
#include "../targetapplication/RemoteObject.h"
#define protected public
#define private public
#include "../targetapplication/TargetApplication.h"
#undef private
#undef protected

class RemoteObjectNameRegisterTest: public QObject {
Q_OBJECT

private slots:

    void init();
    void cleanup();

    void testConstructor();

    void testTargetApplicationStartedAfterRegister();
    void testTargetApplicationStopped();
    void testTargetApplicationStoppedBeforeFinishingNameUpdate();

    void testAddingOrRemovingRemoteObjects();

    void testFindRemoteObject();
    void testFindRemoteObjectWithParentName();
    void testFindRemoteObjectWithGrandparentName();
    void testFindRemoteObjectWithParentAndGrandparentNames();
    void testFindRemoteObjectWithAmbiguousNameAndUniqueParent();
    void testFindRemoteObjectWithAmbiguousNameAndDuplicatedParent();
    void testFindRemoteObjectWithAmbiguousNameAndEmptyParent();
    void testFindRemoteObjectWithUnknownName();

    void testUniqueName();
    void testUniqueNameWhenUniqueParent();
    void testUniqueNameWhenUniqueGrandparentAndEmptyParent();
    void testUniqueNameWhenUniqueGrandparentAndDuplicatedParent();
    void testUniqueNameWhenUniqueUnionOfGrandparentAndParent();
    void testUniqueNameWhenAmbiguousNameWithUniqueParent();
    void testUniqueNameWhenAmbiguousNameWithDuplicatedParent();
    void testUniqueNameWhenAmbiguousNameWithEmptyParent();
    void testUniqueNameWhenEmptyName();
    void testUniqueNameWhenNameNotUnique();

    void testBestNames();
    void testBestNamesWithSingleRemoteObject();

private:

    QString mPath;

    bool waitForTargetApplicationToStart(int timeout) const;
    bool waitForTargetApplicationToStop(int timeout) const;

    bool waitForNamesToBeRegistered(
                    const RemoteObjectNameRegister& remoteObjectNameRegister,
                    int timeout) const;

    void assertNames(const QStringList& names) const;

};

void RemoteObjectNameRegisterTest::init() {
    mPath = QApplication::applicationDirPath() +
                                "/../targetapplication/TargetApplicationStub";

    //Avoid signals from previous tests to be delivered to the next ones
    //setting a new TargetApplication
    delete TargetApplication::sSelf;
    TargetApplication::sSelf = new TargetApplication();
}

void RemoteObjectNameRegisterTest::cleanup() {
    delete TargetApplication::sSelf;
    TargetApplication::sSelf = 0;
}

void RemoteObjectNameRegisterTest::testConstructor() {
    TargetApplication::self()->setTargetApplicationFilePath(mPath);
    TargetApplication::self()->start();

    QVERIFY(waitForTargetApplicationToStart(10000));

    RemoteObjectNameRegister remoteObjectNameRegister;

    QVERIFY(remoteObjectNameRegister.isBeingUpdated());
    QVERIFY(waitForNamesToBeRegistered(remoteObjectNameRegister, 10000));

    assertNames(remoteObjectNameRegister.names());
}

void RemoteObjectNameRegisterTest::testTargetApplicationStartedAfterRegister() {
    RemoteObjectNameRegister remoteObjectNameRegister;
    QSignalSpy nameAddedSpy(&remoteObjectNameRegister,
                            SIGNAL(nameAdded(QString)));
    QSignalSpy nameRemovedSpy(&remoteObjectNameRegister,
                              SIGNAL(nameRemoved(QString)));
    QSignalSpy nameUpdateStartedSpy(&remoteObjectNameRegister,
                                    SIGNAL(nameUpdateStarted()));
    QSignalSpy nameUpdateFinishedSpy(&remoteObjectNameRegister,
                                     SIGNAL(nameUpdateFinished()));

    TargetApplication::self()->setTargetApplicationFilePath(mPath);
    TargetApplication::self()->start();

    QVERIFY(waitForTargetApplicationToStart(10000));

    QVERIFY(remoteObjectNameRegister.isBeingUpdated());

    QCOMPARE(nameUpdateStartedSpy.count(), 1);
    QCOMPARE(nameUpdateFinishedSpy.count(), 0);

    QVERIFY(waitForNamesToBeRegistered(remoteObjectNameRegister, 10000));

    QCOMPARE(nameUpdateStartedSpy.count(), 1);
    QCOMPARE(nameUpdateFinishedSpy.count(), 1);

    assertNames(remoteObjectNameRegister.names());
    QCOMPARE(nameAddedSpy.count(), 102);
    QCOMPARE(nameAddedSpy.at(0).at(0).toString(),
             QString("The object name 42"));
    QCOMPARE(nameAddedSpy.at(1).at(0).toString(),
             QString("The object name 420"));
    QCOMPARE(nameAddedSpy.at(5).at(0).toString(),
             QString("Duplicated grandparent"));
    QCOMPARE(nameAddedSpy.at(6).at(0).toString(),
             QString("The object name 50"));
    QCOMPARE(nameAddedSpy.at(7).at(0).toString(),
             QString("Duplicated object"));
    QCOMPARE(nameAddedSpy.at(11).at(0).toString(),
             QString("The object name 51"));
    QCOMPARE(nameAddedSpy.at(26).at(0).toString(),
             QString("Duplicated grandparent"));
    QCOMPARE(nameAddedSpy.at(27).at(0).toString(),
             QString("Duplicated parent"));
    QCOMPARE(nameAddedSpy.at(28).at(0).toString(),
             QString("Duplicated object"));
    QCOMPARE(nameAddedSpy.at(68).at(0).toString(),
             QString("The object name 8"));
    QCOMPARE(nameAddedSpy.at(69).at(0).toString(),
             QString("Duplicated object"));
    QCOMPARE(nameAddedSpy.at(70).at(0).toString(),
             QString("The object name 801"));
    QCOMPARE(nameAddedSpy.at(71).at(0).toString(),
             QString("The object name 803"));
    QCOMPARE(nameRemovedSpy.count(), 0);
    QCOMPARE(nameUpdateStartedSpy.count(), 1);
    QCOMPARE(nameUpdateFinishedSpy.count(), 1);
}

void RemoteObjectNameRegisterTest::testTargetApplicationStopped() {
    TargetApplication::self()->setTargetApplicationFilePath(mPath);
    TargetApplication::self()->start();

    QVERIFY(waitForTargetApplicationToStart(10000));

    RemoteObjectNameRegister remoteObjectNameRegister;

    QVERIFY(waitForNamesToBeRegistered(remoteObjectNameRegister, 10000));

    QSignalSpy nameAddedSpy(&remoteObjectNameRegister,
                            SIGNAL(nameAdded(QString)));
    QSignalSpy nameRemovedSpy(&remoteObjectNameRegister,
                              SIGNAL(nameAdded(QString)));
    QSignalSpy nameUpdateStartedSpy(&remoteObjectNameRegister,
                                    SIGNAL(nameUpdateStarted()));
    QSignalSpy nameUpdateFinishedSpy(&remoteObjectNameRegister,
                                     SIGNAL(nameUpdateFinished()));

    TargetApplication::self()->mProcess->kill();

    QVERIFY(waitForTargetApplicationToStop(10000));

    QVERIFY(remoteObjectNameRegister.names().isEmpty());
    QCOMPARE(nameAddedSpy.count(), 0);
    QCOMPARE(nameRemovedSpy.count(), 0);
    QCOMPARE(nameUpdateStartedSpy.count(), 0);
    QCOMPARE(nameUpdateFinishedSpy.count(), 0);
}

void RemoteObjectNameRegisterTest::
                    testTargetApplicationStoppedBeforeFinishingNameUpdate() {
    TargetApplication::self()->setTargetApplicationFilePath(mPath);
    TargetApplication::self()->start();

    QVERIFY(waitForTargetApplicationToStart(10000));

    RemoteObjectNameRegister remoteObjectNameRegister;

    QSignalSpy nameUpdateFinishedSpy(&remoteObjectNameRegister,
                                     SIGNAL(nameUpdateFinished()));

    TargetApplication::self()->mProcess->kill();

    //Process deleteLater events for the RemoteObjects
    QApplication::processEvents(QEventLoop::DeferredDeletion);

    //Ensure that there are still names pending to be updated
    QVERIFY(remoteObjectNameRegister.isBeingUpdated());

    QVERIFY(waitForTargetApplicationToStop(10000));

    //If the pending RemoteObjects are not stored in the
    //RemoteObjectNameRegister using a guarded pointer, getting the names to
    //register them after the target application was killed would lead to a
    //crash
    QVERIFY(waitForNamesToBeRegistered(remoteObjectNameRegister, 10000));

    QVERIFY(remoteObjectNameRegister.names().isEmpty());
    QCOMPARE(nameUpdateFinishedSpy.count(), 1);
}

void RemoteObjectNameRegisterTest::testAddingOrRemovingRemoteObjects() {
    QSKIP("Unfortunately, testing if the signals are emitted when an object is "
          "added or removed in the target application is too burdensome so the "
          "test must be done manually", SkipAll);
}

void RemoteObjectNameRegisterTest::testFindRemoteObject() {
    TargetApplication::self()->setTargetApplicationFilePath(mPath);
    TargetApplication::self()->start();

    QVERIFY(waitForTargetApplicationToStart(10000));

    RemoteObjectNameRegister remoteObjectNameRegister;

    QVERIFY(waitForNamesToBeRegistered(remoteObjectNameRegister, 10000));

    RemoteObject* mainWindow =
                TargetApplication::self()->remoteEditorSupport()->mainWindow();

    QCOMPARE(remoteObjectNameRegister.findRemoteObject("The object name 423"),
             mainWindow->children()[3]);
}

void RemoteObjectNameRegisterTest::testFindRemoteObjectWithParentName() {
    TargetApplication::self()->setTargetApplicationFilePath(mPath);
    TargetApplication::self()->start();

    QVERIFY(waitForTargetApplicationToStart(10000));

    RemoteObjectNameRegister remoteObjectNameRegister;

    QVERIFY(waitForNamesToBeRegistered(remoteObjectNameRegister, 10000));

    RemoteObject* mainWindow =
                TargetApplication::self()->remoteEditorSupport()->mainWindow();

    QCOMPARE(remoteObjectNameRegister.findRemoteObject(
                                        "The object name 50/Duplicated object"),
             mainWindow->children()[4]->children()[0]->children()[0]);
}

void RemoteObjectNameRegisterTest::testFindRemoteObjectWithGrandparentName() {
    TargetApplication::self()->setTargetApplicationFilePath(mPath);
    TargetApplication::self()->start();

    QVERIFY(waitForTargetApplicationToStart(10000));

    RemoteObjectNameRegister remoteObjectNameRegister;

    QVERIFY(waitForNamesToBeRegistered(remoteObjectNameRegister, 10000));

    RemoteObject* mainWindow =
                TargetApplication::self()->remoteEditorSupport()->mainWindow();

    QCOMPARE(remoteObjectNameRegister.findRemoteObject(
                                        "The object name 7/Duplicated object"),
             mainWindow->children()[6]->children()[0]->children()[0]);
}

void RemoteObjectNameRegisterTest::
                        testFindRemoteObjectWithParentAndGrandparentNames() {
    TargetApplication::self()->setTargetApplicationFilePath(mPath);
    TargetApplication::self()->start();

    QVERIFY(waitForTargetApplicationToStart(10000));

    RemoteObjectNameRegister remoteObjectNameRegister;

    QVERIFY(waitForNamesToBeRegistered(remoteObjectNameRegister, 10000));

    RemoteObject* mainWindow =
                TargetApplication::self()->remoteEditorSupport()->mainWindow();

    QCOMPARE(remoteObjectNameRegister.findRemoteObject(
                "Duplicated grandparent/Duplicated parent/Duplicated object"),
             mainWindow->children()[5]->children()[0]->children()[0]);
}

void RemoteObjectNameRegisterTest::
                        testFindRemoteObjectWithAmbiguousNameAndUniqueParent() {
    TargetApplication::self()->setTargetApplicationFilePath(mPath);
    TargetApplication::self()->start();

    QVERIFY(waitForTargetApplicationToStart(10000));

    RemoteObjectNameRegister remoteObjectNameRegister;

    QVERIFY(waitForNamesToBeRegistered(remoteObjectNameRegister, 10000));

    RemoteObject* mainWindow =
                TargetApplication::self()->remoteEditorSupport()->mainWindow();

    QCOMPARE(remoteObjectNameRegister.findRemoteObject(
                                        "The object name 90/Duplicated object"),
             mainWindow->children()[8]->children()[0]->children()[0]);
}

void RemoteObjectNameRegisterTest::
                    testFindRemoteObjectWithAmbiguousNameAndDuplicatedParent() {
    TargetApplication::self()->setTargetApplicationFilePath(mPath);
    TargetApplication::self()->start();

    QVERIFY(waitForTargetApplicationToStart(10000));

    RemoteObjectNameRegister remoteObjectNameRegister;

    QVERIFY(waitForNamesToBeRegistered(remoteObjectNameRegister, 10000));

    RemoteObject* mainWindow =
                TargetApplication::self()->remoteEditorSupport()->mainWindow();

    QCOMPARE(remoteObjectNameRegister.findRemoteObject(
        "Duplicated grandparent/Another duplicated parent/Duplicated object"),
             mainWindow->children()[8]->children()[1]->children()[0]);
}

void RemoteObjectNameRegisterTest::
                        testFindRemoteObjectWithAmbiguousNameAndEmptyParent() {
    TargetApplication::self()->setTargetApplicationFilePath(mPath);
    TargetApplication::self()->start();

    QVERIFY(waitForTargetApplicationToStart(10000));

    RemoteObjectNameRegister remoteObjectNameRegister;

    QVERIFY(waitForNamesToBeRegistered(remoteObjectNameRegister, 10000));

    RemoteObject* mainWindow =
                TargetApplication::self()->remoteEditorSupport()->mainWindow();

    QCOMPARE(remoteObjectNameRegister.findRemoteObject(
                                    "Duplicated grandparent/Duplicated object"),
             mainWindow->children()[8]->children()[3]->children()[0]);
}

void RemoteObjectNameRegisterTest::testFindRemoteObjectWithUnknownName() {
    TargetApplication::self()->setTargetApplicationFilePath(mPath);
    TargetApplication::self()->start();

    QVERIFY(waitForTargetApplicationToStart(10000));

    RemoteObjectNameRegister remoteObjectNameRegister;

    QVERIFY(waitForNamesToBeRegistered(remoteObjectNameRegister, 10000));

    QCOMPARE(remoteObjectNameRegister.findRemoteObject("The object name 108"),
             (RemoteObject*)0);
}

void RemoteObjectNameRegisterTest::testUniqueName() {
    TargetApplication::self()->setTargetApplicationFilePath(mPath);
    TargetApplication::self()->start();

    QVERIFY(waitForTargetApplicationToStart(10000));

    RemoteObjectNameRegister remoteObjectNameRegister;

    QVERIFY(waitForNamesToBeRegistered(remoteObjectNameRegister, 10000));

    RemoteObject* mainWindow =
                TargetApplication::self()->remoteEditorSupport()->mainWindow();
    RemoteObject* remoteObject =
                        mainWindow->children()[4]->children()[0]->children()[1];

    QCOMPARE(remoteObjectNameRegister.uniqueName(remoteObject),
             QString("The object name 501"));
}

void RemoteObjectNameRegisterTest::testUniqueNameWhenUniqueParent() {
    TargetApplication::self()->setTargetApplicationFilePath(mPath);
    TargetApplication::self()->start();

    QVERIFY(waitForTargetApplicationToStart(10000));

    RemoteObjectNameRegister remoteObjectNameRegister;

    QVERIFY(waitForNamesToBeRegistered(remoteObjectNameRegister, 10000));

    RemoteObject* mainWindow =
                TargetApplication::self()->remoteEditorSupport()->mainWindow();
    RemoteObject* remoteObject =
                        mainWindow->children()[4]->children()[0]->children()[0];

    QCOMPARE(remoteObjectNameRegister.uniqueName(remoteObject),
             QString("The object name 50/Duplicated object"));
}

void RemoteObjectNameRegisterTest::
                        testUniqueNameWhenUniqueGrandparentAndEmptyParent() {
    TargetApplication::self()->setTargetApplicationFilePath(mPath);
    TargetApplication::self()->start();

    QVERIFY(waitForTargetApplicationToStart(10000));

    RemoteObjectNameRegister remoteObjectNameRegister;

    QVERIFY(waitForNamesToBeRegistered(remoteObjectNameRegister, 10000));

    RemoteObject* mainWindow =
                TargetApplication::self()->remoteEditorSupport()->mainWindow();
    RemoteObject* remoteObject =
                        mainWindow->children()[7]->children()[0]->children()[0];

    QCOMPARE(remoteObjectNameRegister.uniqueName(remoteObject),
             QString("The object name 8/Duplicated object"));
}

void RemoteObjectNameRegisterTest::
                    testUniqueNameWhenUniqueGrandparentAndDuplicatedParent() {
    TargetApplication::self()->setTargetApplicationFilePath(mPath);
    TargetApplication::self()->start();

    QVERIFY(waitForTargetApplicationToStart(10000));

    RemoteObjectNameRegister remoteObjectNameRegister;

    QVERIFY(waitForNamesToBeRegistered(remoteObjectNameRegister, 10000));

    RemoteObject* mainWindow =
                TargetApplication::self()->remoteEditorSupport()->mainWindow();
    RemoteObject* remoteObject =
                        mainWindow->children()[6]->children()[0]->children()[0];

    QCOMPARE(remoteObjectNameRegister.uniqueName(remoteObject),
             QString("The object name 7/Duplicated object"));
}

void RemoteObjectNameRegisterTest::
                        testUniqueNameWhenUniqueUnionOfGrandparentAndParent() {
    TargetApplication::self()->setTargetApplicationFilePath(mPath);
    TargetApplication::self()->start();

    QVERIFY(waitForTargetApplicationToStart(10000));

    RemoteObjectNameRegister remoteObjectNameRegister;

    QVERIFY(waitForNamesToBeRegistered(remoteObjectNameRegister, 10000));

    RemoteObject* mainWindow =
                TargetApplication::self()->remoteEditorSupport()->mainWindow();
    RemoteObject* remoteObject =
                        mainWindow->children()[5]->children()[0]->children()[0];

    QCOMPARE(remoteObjectNameRegister.uniqueName(remoteObject),
             QString("Duplicated grandparent/Duplicated parent/"
                     "Duplicated object"));
}

void RemoteObjectNameRegisterTest::
                            testUniqueNameWhenAmbiguousNameWithUniqueParent() {
    TargetApplication::self()->setTargetApplicationFilePath(mPath);
    TargetApplication::self()->start();

    QVERIFY(waitForTargetApplicationToStart(10000));

    RemoteObjectNameRegister remoteObjectNameRegister;

    QVERIFY(waitForNamesToBeRegistered(remoteObjectNameRegister, 10000));

    RemoteObject* mainWindow =
                TargetApplication::self()->remoteEditorSupport()->mainWindow();
    RemoteObject* remoteObject =
                        mainWindow->children()[8]->children()[0]->children()[0];

    QCOMPARE(remoteObjectNameRegister.uniqueName(remoteObject),
             QString("The object name 90/Duplicated object"));
}

void RemoteObjectNameRegisterTest::
                        testUniqueNameWhenAmbiguousNameWithDuplicatedParent() {
    TargetApplication::self()->setTargetApplicationFilePath(mPath);
    TargetApplication::self()->start();

    QVERIFY(waitForTargetApplicationToStart(10000));

    RemoteObjectNameRegister remoteObjectNameRegister;

    QVERIFY(waitForNamesToBeRegistered(remoteObjectNameRegister, 10000));

    RemoteObject* mainWindow =
                TargetApplication::self()->remoteEditorSupport()->mainWindow();
    RemoteObject* remoteObject =
                        mainWindow->children()[8]->children()[1]->children()[0];

    QCOMPARE(remoteObjectNameRegister.uniqueName(remoteObject),
             QString("Duplicated grandparent/Another duplicated parent/"
                     "Duplicated object"));
}

void RemoteObjectNameRegisterTest::
                            testUniqueNameWhenAmbiguousNameWithEmptyParent() {
    TargetApplication::self()->setTargetApplicationFilePath(mPath);
    TargetApplication::self()->start();

    QVERIFY(waitForTargetApplicationToStart(10000));

    RemoteObjectNameRegister remoteObjectNameRegister;

    QVERIFY(waitForNamesToBeRegistered(remoteObjectNameRegister, 10000));

    RemoteObject* mainWindow =
                TargetApplication::self()->remoteEditorSupport()->mainWindow();
    RemoteObject* remoteObject =
                        mainWindow->children()[8]->children()[3]->children()[0];

    QCOMPARE(remoteObjectNameRegister.uniqueName(remoteObject),
             QString("Duplicated grandparent/Duplicated object"));
}

void RemoteObjectNameRegisterTest::testUniqueNameWhenEmptyName() {
    TargetApplication::self()->setTargetApplicationFilePath(mPath);
    TargetApplication::self()->start();

    QVERIFY(waitForTargetApplicationToStart(10000));

    RemoteObjectNameRegister remoteObjectNameRegister;

    QVERIFY(waitForNamesToBeRegistered(remoteObjectNameRegister, 10000));

    RemoteObject* mainWindow =
                TargetApplication::self()->remoteEditorSupport()->mainWindow();
    RemoteObject* remoteObject = mainWindow->children()[7]->children()[0];

    QCOMPARE(remoteObjectNameRegister.uniqueName(remoteObject), QString(""));
}

void RemoteObjectNameRegisterTest::testUniqueNameWhenNameNotUnique() {
    TargetApplication::self()->setTargetApplicationFilePath(mPath);
    TargetApplication::self()->start();

    QVERIFY(waitForTargetApplicationToStart(10000));

    RemoteObjectNameRegister remoteObjectNameRegister;

    QVERIFY(waitForNamesToBeRegistered(remoteObjectNameRegister, 10000));

    RemoteObject* mainWindow =
                TargetApplication::self()->remoteEditorSupport()->mainWindow();
    RemoteObject* remoteObject =
                        mainWindow->children()[8]->children()[0]->children()[1];

    QCOMPARE(remoteObjectNameRegister.uniqueName(remoteObject), QString(""));

    remoteObject = mainWindow->children()[8]->children()[2]->children()[0];

    QCOMPARE(remoteObjectNameRegister.uniqueName(remoteObject), QString(""));

    remoteObject = mainWindow->children()[8]->children()[3]->children()[1];

    QCOMPARE(remoteObjectNameRegister.uniqueName(remoteObject), QString(""));
}

void RemoteObjectNameRegisterTest::testBestNames() {
    TargetApplication::self()->setTargetApplicationFilePath(mPath);
    TargetApplication::self()->start();

    QVERIFY(waitForTargetApplicationToStart(10000));

    RemoteObjectNameRegister remoteObjectNameRegister;

    QVERIFY(waitForNamesToBeRegistered(remoteObjectNameRegister, 10000));

    QStringList bestNames =
                    remoteObjectNameRegister.bestNames("Duplicated object");

    QCOMPARE(bestNames.count(), 10);
    QVERIFY(bestNames.contains("The object name 50/Duplicated object"));
    QVERIFY(bestNames.contains("Duplicated grandparent/Duplicated parent/"
                                 "Duplicated object"));
    QVERIFY(bestNames.contains("The object name 7/Duplicated object"));
    QVERIFY(bestNames.contains("The object name 8/Duplicated object"));
    QVERIFY(bestNames.contains("The object name 90/Duplicated object"));
    QVERIFY(bestNames.contains("The object name 90/Duplicated object"));
    QVERIFY(bestNames.contains("Duplicated grandparent/Another duplicated "
                                 "parent/Duplicated object"));
    QVERIFY(bestNames.contains("Duplicated grandparent/Another duplicated "
                                 "parent/Duplicated object"));
    QVERIFY(bestNames.contains("Duplicated grandparent/Duplicated object"));
    QVERIFY(bestNames.contains("Duplicated grandparent/Duplicated object"));
}

void RemoteObjectNameRegisterTest::testBestNamesWithSingleRemoteObject() {
    TargetApplication::self()->setTargetApplicationFilePath(mPath);
    TargetApplication::self()->start();

    QVERIFY(waitForTargetApplicationToStart(10000));

    RemoteObjectNameRegister remoteObjectNameRegister;

    QVERIFY(waitForNamesToBeRegistered(remoteObjectNameRegister, 10000));

    QStringList bestNames =
                    remoteObjectNameRegister.bestNames("The object name 423");

    QCOMPARE(bestNames.count(), 1);
    QVERIFY(bestNames.contains("The object name 423"));
}

/////////////////////////////////// Helpers ////////////////////////////////////

bool waitFor(bool (*condition)(), int timeout) {
    QElapsedTimer timer;
    timer.start();
    do {
        QTest::qWait(100);
    } while (!condition() && timer.elapsed() < timeout);

    if (timer.elapsed() >= timeout) {
        return false;
    }

    return true;    
}

bool isTargetApplicationStarted() {
    return TargetApplication::self()->remoteEditorSupport();
}

bool isTargetApplicationStopped() {
    return !TargetApplication::self()->remoteEditorSupport();
}

bool RemoteObjectNameRegisterTest::waitForTargetApplicationToStart(
                                                            int timeout) const {
    return waitFor(&isTargetApplicationStarted, timeout);
}

bool RemoteObjectNameRegisterTest::waitForTargetApplicationToStop(
                                                            int timeout) const {
    return waitFor(&isTargetApplicationStopped, timeout);
}

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

class RegisterNotBeingUpdatedCondition {
public:
    const RemoteObjectNameRegister* mRemoteObjectNameRegister;

    bool condition() const {
        return !mRemoteObjectNameRegister->isBeingUpdated();
    }
};

bool RemoteObjectNameRegisterTest::waitForNamesToBeRegistered(
                    const RemoteObjectNameRegister& remoteObjectNameRegister,
                    int timeout) const {
    RegisterNotBeingUpdatedCondition helper;
    helper.mRemoteObjectNameRegister = &remoteObjectNameRegister;
    return waitFor(helper, &RegisterNotBeingUpdatedCondition::condition,
                   timeout);
}

void RemoteObjectNameRegisterTest::assertNames(const QStringList& names) const {
    QCOMPARE(names.count(), 102);
    QVERIFY(names.contains("The object name 42"));
    QVERIFY(names.contains("The object name 420"));
    QVERIFY(names.contains("The object name 421"));
    QVERIFY(names.contains("The object name 422"));
    QVERIFY(names.contains("The object name 423"));
    QVERIFY(names.contains("The object name 7"));
    QVERIFY(names.contains("The object name 62"));
    QVERIFY(names.contains("The object name 833"));
    QVERIFY(names.contains("Duplicated object"));
    QVERIFY(names.contains("Duplicated parent"));
    QVERIFY(names.contains("Duplicated grandparent"));
}

QTEST_MAIN(RemoteObjectNameRegisterTest)

#include "RemoteObjectNameRegisterTest.moc"
