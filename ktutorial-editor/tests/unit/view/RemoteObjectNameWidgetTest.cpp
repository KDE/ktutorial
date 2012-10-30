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
#include <QtTest/QTestKeyClicksEvent>

#define protected public
#define private public
#include "RemoteObjectNameWidget.h"
#undef private
#undef protected

#include <QLabel>
#include <QSignalSpy>
#include <QtDBus/QtDBus>

#include <KCompletionBox>
#include <KDialog>
#include <KLineEdit>
#include <KProcess>

#include "RemoteObjectNameRegister.h"
#include "../targetapplication/RemoteClassStubs.h"
#include "../targetapplication/RemoteEditorSupport.h"
#include "../targetapplication/RemoteObject.h"
#define protected public
#define private public
#include "../targetapplication/TargetApplication.h"
#undef private
#undef protected

class RemoteObjectNameWidgetTest: public QObject {
Q_OBJECT

private slots:

    void initTestCase();
    void init();
    void cleanup();

    void testConstructor();

    void testSetName();
    void testSetNameWithPath();
    void testSetNameWithUnknownRemoteObjectName();
    void testSetNameDuringNameRegisterUpdate();

    void testSetChosenRemoteObject();
    void testSetChosenRemoteObjectWithUniqueComplexName();
    void testSetChosenRemoteObjectWithNameNotUnique();
    void testSetChosenRemoteObjectDuringNameRegisterUpdate();
    void testSetChosenRemoteObjectDuringNameRegisterUpdateKillingApplication();

    void testNameCompletion();
    void testDuplicatedNameCompletion();
    void testNameCompletionDuringNameRegisterUpdate();
    void testNameSubcompletionDuringNameRegisterUpdate();

    void testTargetApplicationStartedAfterWidget();
    void testTargetApplicationStopped();

    void testAddingOrRemovingRemoteObjects();

    void testSetFocus();

    void testWidgetAsBuddyOfLabel();

private:

    QString mPath;

    int mRemoteObjectStarType;

    StubEditorSupport* mEditorSupport;
    StubObjectRegister* mObjectRegister;

    KLineEdit* objectNameLineEdit(RemoteObjectNameWidget* widget) const;

    bool waitForTargetApplicationToStart(int timeout) const;
    bool waitForTargetApplicationToStop(int timeout) const;
    
    bool waitForNamesToBeRegistered(const RemoteObjectNameWidget* widget,
                                    int timeout) const;

    void closeSorryMessageBox(QWidget* widget, int timeToWait);

    void assertRemoteObjectSignal(const QSignalSpy& spy, int index,
                                  const RemoteObject* remoteObject) const;

    void assertCompletionItems(const QStringList& items) const;

};

void RemoteObjectNameWidgetTest::initTestCase() {
    //RemoteObject* must be registered in order to be used with QSignalSpy
    mRemoteObjectStarType = qRegisterMetaType<RemoteObject*>("RemoteObject*");
}

void RemoteObjectNameWidgetTest::init() {
    mPath = QApplication::applicationDirPath() +
                                "/../targetapplication/TargetApplicationStub";

    //Avoid signals from previous tests to be delivered to the next ones
    //setting a new TargetApplication
    delete TargetApplication::sSelf;
    TargetApplication::sSelf = new TargetApplication();
}

void RemoteObjectNameWidgetTest::cleanup() {
    delete TargetApplication::sSelf;
    TargetApplication::sSelf = 0;
}

void RemoteObjectNameWidgetTest::testConstructor() {
    TargetApplication::self()->setTargetApplicationFilePath(mPath);
    TargetApplication::self()->start();

    QVERIFY(waitForTargetApplicationToStart(10000));

    QWidget parent;
    RemoteObjectNameWidget* widget = new RemoteObjectNameWidget(&parent);

    QCOMPARE(objectNameLineEdit(widget)->cursor().shape(), Qt::BusyCursor);

    QVERIFY(waitForNamesToBeRegistered(widget, 10000));

    QCOMPARE(objectNameLineEdit(widget)->cursor().shape(), Qt::IBeamCursor);

    QCOMPARE(widget->parentWidget(), &parent);
    QCOMPARE(widget->name(), QString(""));
    QStringList items = objectNameLineEdit(widget)->completionObject()->items();
    assertCompletionItems(items);
}

void RemoteObjectNameWidgetTest::testSetName() {
    TargetApplication::self()->setTargetApplicationFilePath(mPath);
    TargetApplication::self()->start();

    QVERIFY(waitForTargetApplicationToStart(10000));

    RemoteObjectNameWidget widget;
    QSignalSpy remoteObjectChosenSpy(&widget,
                                     SIGNAL(remoteObjectChosen(RemoteObject*)));

    QVERIFY(waitForNamesToBeRegistered(&widget, 10000));

    widget.setName("The object name 423");

    RemoteObject* mainWindow =
                TargetApplication::self()->remoteEditorSupport()->mainWindow();

    QCOMPARE(widget.name(), QString("The object name 423"));
    QCOMPARE(remoteObjectChosenSpy.count(), 1);
    assertRemoteObjectSignal(remoteObjectChosenSpy, 0,
                             mainWindow->children()[3]);
}

void RemoteObjectNameWidgetTest::testSetNameWithPath() {
    TargetApplication::self()->setTargetApplicationFilePath(mPath);
    TargetApplication::self()->start();

    QVERIFY(waitForTargetApplicationToStart(10000));

    RemoteObjectNameWidget widget;
    QSignalSpy remoteObjectChosenSpy(&widget,
                                     SIGNAL(remoteObjectChosen(RemoteObject*)));

    QVERIFY(waitForNamesToBeRegistered(&widget, 10000));

    widget.setName("The object name 7/Duplicated object");

    RemoteObject* mainWindow =
                TargetApplication::self()->remoteEditorSupport()->mainWindow();

    QCOMPARE(widget.name(), QString("The object name 7/Duplicated object"));
    QCOMPARE(remoteObjectChosenSpy.count(), 1);
    assertRemoteObjectSignal(remoteObjectChosenSpy, 0,
                    mainWindow->children()[6]->children()[0]->children()[0]);
}

void RemoteObjectNameWidgetTest::testSetNameWithUnknownRemoteObjectName() {
    TargetApplication::self()->setTargetApplicationFilePath(mPath);
    TargetApplication::self()->start();

    QVERIFY(waitForTargetApplicationToStart(10000));

    RemoteObjectNameWidget widget;
    QSignalSpy remoteObjectChosenSpy(&widget,
                                     SIGNAL(remoteObjectChosen(RemoteObject*)));

    QVERIFY(waitForNamesToBeRegistered(&widget, 10000));

    widget.setName("The object name 108");

    QCOMPARE(widget.name(), QString("The object name 108"));
    QCOMPARE(remoteObjectChosenSpy.count(), 1);
    assertRemoteObjectSignal(remoteObjectChosenSpy, 0, 0);
}

void RemoteObjectNameWidgetTest::testSetNameDuringNameRegisterUpdate() {
    TargetApplication::self()->setTargetApplicationFilePath(mPath);
    TargetApplication::self()->start();

    QVERIFY(waitForTargetApplicationToStart(10000));

    RemoteObjectNameWidget widget;
    QSignalSpy remoteObjectChosenSpy(&widget,
                                     SIGNAL(remoteObjectChosen(RemoteObject*)));

    widget.setName("The object name 423");

    //Ensure that the register is still being updated
    QVERIFY(widget.mRemoteObjectNameRegister->isBeingUpdated());

    //Check that the name is set, but the signal was not emitted yet
    QCOMPARE(widget.name(), QString("The object name 423"));
    QCOMPARE(remoteObjectChosenSpy.count(), 0);

    QVERIFY(waitForNamesToBeRegistered(&widget, 10000));

    RemoteObject* mainWindow =
                TargetApplication::self()->remoteEditorSupport()->mainWindow();

    QCOMPARE(widget.name(), QString("The object name 423"));
    QCOMPARE(remoteObjectChosenSpy.count(), 1);
    assertRemoteObjectSignal(remoteObjectChosenSpy, 0,
                             mainWindow->children()[3]);
}

void RemoteObjectNameWidgetTest::testSetChosenRemoteObject() {
    TargetApplication::self()->setTargetApplicationFilePath(mPath);
    TargetApplication::self()->start();

    QVERIFY(waitForTargetApplicationToStart(10000));

    RemoteObjectNameWidget widget;
    QSignalSpy remoteObjectChosenSpy(&widget,
                                     SIGNAL(remoteObjectChosen(RemoteObject*)));

    QVERIFY(waitForNamesToBeRegistered(&widget, 10000));

    RemoteObject* mainWindow =
                TargetApplication::self()->remoteEditorSupport()->mainWindow();
    RemoteObject* remoteObject =
                        mainWindow->children()[4]->children()[0]->children()[1];

    widget.setChosenRemoteObject(remoteObject);

    QCOMPARE(widget.name(), QString("The object name 501"));
    QCOMPARE(remoteObjectChosenSpy.count(), 1);
    assertRemoteObjectSignal(remoteObjectChosenSpy, 0, remoteObject);
}

void RemoteObjectNameWidgetTest::
                            testSetChosenRemoteObjectWithUniqueComplexName() {
    TargetApplication::self()->setTargetApplicationFilePath(mPath);
    TargetApplication::self()->start();

    QVERIFY(waitForTargetApplicationToStart(10000));

    RemoteObjectNameWidget widget;
    QSignalSpy remoteObjectChosenSpy(&widget,
                                     SIGNAL(remoteObjectChosen(RemoteObject*)));

    QVERIFY(waitForNamesToBeRegistered(&widget, 10000));

    RemoteObject* mainWindow =
                TargetApplication::self()->remoteEditorSupport()->mainWindow();

    RemoteObject* remoteObject =
                        mainWindow->children()[4]->children()[0]->children()[0];
    widget.setChosenRemoteObject(remoteObject);

    QCOMPARE(widget.name(), QString("The object name 50/Duplicated object"));
    QCOMPARE(remoteObjectChosenSpy.count(), 1);
    assertRemoteObjectSignal(remoteObjectChosenSpy, 0, remoteObject);

    remoteObject = mainWindow->children()[5]->children()[0]->children()[0];
    widget.setChosenRemoteObject(remoteObject);

    QCOMPARE(widget.name(), QString("Duplicated grandparent/Duplicated parent/"
                                    "Duplicated object"));
    QCOMPARE(remoteObjectChosenSpy.count(), 2);
    assertRemoteObjectSignal(remoteObjectChosenSpy, 1, remoteObject);

    remoteObject = mainWindow->children()[6]->children()[0]->children()[0];
    widget.setChosenRemoteObject(remoteObject);

    QCOMPARE(widget.name(), QString("The object name 7/Duplicated object"));
    QCOMPARE(remoteObjectChosenSpy.count(), 3);
    assertRemoteObjectSignal(remoteObjectChosenSpy, 2, remoteObject);
}

void RemoteObjectNameWidgetTest::testSetChosenRemoteObjectWithNameNotUnique() {
    TargetApplication::self()->setTargetApplicationFilePath(mPath);
    TargetApplication::self()->start();

    QVERIFY(waitForTargetApplicationToStart(10000));

    RemoteObjectNameWidget widget;

    QVERIFY(waitForNamesToBeRegistered(&widget, 10000));

    widget.setName("An object name");

    QSignalSpy remoteObjectChosenSpy(&widget,
                                     SIGNAL(remoteObjectChosen(RemoteObject*)));

    RemoteObject* mainWindow =
                TargetApplication::self()->remoteEditorSupport()->mainWindow();
    RemoteObject* remoteObject =
                        mainWindow->children()[8]->children()[0]->children()[1];

    //The widget must be shown for a proper parenting of the sorry message box
    widget.show();

    //Queue closing the sorry message box
    closeSorryMessageBox(&widget, 10000);

    widget.setChosenRemoteObject(remoteObject);

    QCOMPARE(widget.name(), QString("An object name"));
    QCOMPARE(remoteObjectChosenSpy.count(), 0);
}

void RemoteObjectNameWidgetTest::
                        testSetChosenRemoteObjectDuringNameRegisterUpdate() {
    TargetApplication::self()->setTargetApplicationFilePath(mPath);
    TargetApplication::self()->start();

    QVERIFY(waitForTargetApplicationToStart(10000));

    RemoteObjectNameWidget widget;
    QSignalSpy remoteObjectChosenSpy(&widget,
                                     SIGNAL(remoteObjectChosen(RemoteObject*)));

    RemoteObject* mainWindow =
                TargetApplication::self()->remoteEditorSupport()->mainWindow();
    RemoteObject* remoteObject =
                        mainWindow->children()[4]->children()[0]->children()[1];

    widget.setChosenRemoteObject(remoteObject);

    //Ensure that the register is still being updated
    QVERIFY(widget.mRemoteObjectNameRegister->isBeingUpdated());

    //Check that the name is not set and the signal was not emitted yet
    QCOMPARE(widget.name(), QString(""));
    QCOMPARE(remoteObjectChosenSpy.count(), 0);

    QVERIFY(waitForNamesToBeRegistered(&widget, 10000));

    QCOMPARE(widget.name(), QString("The object name 501"));
    QCOMPARE(remoteObjectChosenSpy.count(), 1);
    assertRemoteObjectSignal(remoteObjectChosenSpy, 0, remoteObject);
}

void RemoteObjectNameWidgetTest::
        testSetChosenRemoteObjectDuringNameRegisterUpdateKillingApplication() {
    TargetApplication::self()->setTargetApplicationFilePath(mPath);
    TargetApplication::self()->start();

    QVERIFY(waitForTargetApplicationToStart(10000));

    RemoteObjectNameWidget widget;
    widget.setName("An object name");

    QSignalSpy remoteObjectChosenSpy(&widget,
                                     SIGNAL(remoteObjectChosen(RemoteObject*)));

    RemoteObject* mainWindow =
                TargetApplication::self()->remoteEditorSupport()->mainWindow();
    RemoteObject* remoteObject =
                        mainWindow->children()[4]->children()[0]->children()[1];

    widget.setChosenRemoteObject(remoteObject);

    //Ensure that the register is still being updated
    QVERIFY(widget.mRemoteObjectNameRegister->isBeingUpdated());

    TargetApplication::self()->mProcess->kill();

    QVERIFY(waitForTargetApplicationToStop(10000));

    //Ensure that the register is still being updated
    QVERIFY(widget.mRemoteObjectNameRegister->isBeingUpdated());

    //Check that the name is not set and the signal was not emitted yet
    QCOMPARE(widget.name(), QString("An object name"));
    QCOMPARE(remoteObjectChosenSpy.count(), 0);

    //If the pending RemoteObject is not stored using a guarded pointer, getting
    //its unique name after the target application was killed would lead to a
    //crash
    QVERIFY(waitForNamesToBeRegistered(&widget, 10000));

    //If there is no RemoteObject to be set, nothing is done. However, the
    //remoteObjectChosen signal was queued to be emitted when the name was first
    //set (as the names were already being uptated), although it should not be
    //emitted again when setting the null RemoteObject.
    QCOMPARE(widget.name(), QString("An object name"));
    QCOMPARE(remoteObjectChosenSpy.count(), 1);
}

void RemoteObjectNameWidgetTest::testNameCompletion() {
    TargetApplication::self()->setTargetApplicationFilePath(mPath);
    TargetApplication::self()->start();

    QVERIFY(waitForTargetApplicationToStart(10000));

    RemoteObjectNameWidget widget;

    QVERIFY(waitForNamesToBeRegistered(&widget, 10000));

    KCompletion* completion = objectNameLineEdit(&widget)->completionObject();
    QCOMPARE(completion->order(), KCompletion::Sorted);

    QStringList completionItems = completion->allMatches("The object name 42");
    QCOMPARE(completionItems.count(), 5);
    QCOMPARE(completionItems[0], QString("The object name 42"));
    QCOMPARE(completionItems[1], QString("The object name 420"));
    QCOMPARE(completionItems[2], QString("The object name 421"));
    QCOMPARE(completionItems[3], QString("The object name 422"));
    QCOMPARE(completionItems[4], QString("The object name 423"));

    completionItems = completion->allMatches("the Object Name 42");
    QCOMPARE(completionItems.count(), 5);
    QCOMPARE(completionItems[0], QString("The object name 42"));
    QCOMPARE(completionItems[1], QString("The object name 420"));
    QCOMPARE(completionItems[2], QString("The object name 421"));
    QCOMPARE(completionItems[3], QString("The object name 422"));
    QCOMPARE(completionItems[4], QString("The object name 423"));
}

void RemoteObjectNameWidgetTest::testDuplicatedNameCompletion() {
    TargetApplication::self()->setTargetApplicationFilePath(mPath);
    TargetApplication::self()->start();

    QVERIFY(waitForTargetApplicationToStart(10000));

    RemoteObjectNameWidget widget;

    QVERIFY(waitForNamesToBeRegistered(&widget, 10000));

    KLineEdit* lineEdit = objectNameLineEdit(&widget);
    lineEdit->setText("Duplicated ");

    QTest::keyClick(lineEdit, Qt::Key_O, Qt::NoModifier, 500);

    KCompletionBox* completionBox = lineEdit->completionBox();
    QStringList completionItems = completionBox->items();

    QVERIFY(completionBox->isVisible());

    QCOMPARE(completionItems.count(), 10);
    QCOMPARE(completionItems[0],
             QString("Duplicated grandparent/Another duplicated parent/"
                     "Duplicated object"));
    QCOMPARE(completionItems[1],
             QString("Duplicated grandparent/Another duplicated parent/"
                     "Duplicated object"));
    QCOMPARE(completionItems[2],
             QString("Duplicated grandparent/Duplicated object"));
    QCOMPARE(completionItems[3],
             QString("Duplicated grandparent/Duplicated object"));
    QCOMPARE(completionItems[4],
             QString("Duplicated grandparent/Duplicated parent/"
                     "Duplicated object"));
    QCOMPARE(completionItems[5],
             QString("The object name 50/Duplicated object"));
    QCOMPARE(completionItems[6],
             QString("The object name 7/Duplicated object"));
    QCOMPARE(completionItems[7],
             QString("The object name 8/Duplicated object"));
    QCOMPARE(completionItems[8],
             QString("The object name 90/Duplicated object"));
    QCOMPARE(completionItems[9],
             QString("The object name 90/Duplicated object"));
}

void RemoteObjectNameWidgetTest::testNameCompletionDuringNameRegisterUpdate() {
    TargetApplication::self()->setTargetApplicationFilePath(mPath);
    TargetApplication::self()->start();

    QVERIFY(waitForTargetApplicationToStart(10000));

    RemoteObjectNameWidget widget;

    KLineEdit* lineEdit = objectNameLineEdit(&widget);
    lineEdit->setText("Duplicated ");

    QTest::keyClick(lineEdit, Qt::Key_O, Qt::NoModifier);

    KCompletionBox* completionBox = lineEdit->completionBox();
    QStringList completionItems = completionBox->items();

    //Ensure that the register is still being updated
    QVERIFY(widget.mRemoteObjectNameRegister->isBeingUpdated());

    QVERIFY(!completionBox->isVisible());
    QCOMPARE(completionItems.count(), 0);

    QVERIFY(waitForNamesToBeRegistered(&widget, 10000));

    completionBox = lineEdit->completionBox();
    completionItems = completionBox->items();

    QVERIFY(completionBox->isVisible());

    QCOMPARE(completionItems.count(), 10);
    QCOMPARE(completionItems[0],
             QString("Duplicated grandparent/Another duplicated parent/"
                     "Duplicated object"));
    QCOMPARE(completionItems[1],
             QString("Duplicated grandparent/Another duplicated parent/"
                     "Duplicated object"));
    QCOMPARE(completionItems[2],
             QString("Duplicated grandparent/Duplicated object"));
    QCOMPARE(completionItems[3],
             QString("Duplicated grandparent/Duplicated object"));
    QCOMPARE(completionItems[4],
             QString("Duplicated grandparent/Duplicated parent/"
                     "Duplicated object"));
    QCOMPARE(completionItems[5],
             QString("The object name 50/Duplicated object"));
    QCOMPARE(completionItems[6],
             QString("The object name 7/Duplicated object"));
    QCOMPARE(completionItems[7],
             QString("The object name 8/Duplicated object"));
    QCOMPARE(completionItems[8],
             QString("The object name 90/Duplicated object"));
    QCOMPARE(completionItems[9],
             QString("The object name 90/Duplicated object"));
}

void RemoteObjectNameWidgetTest::
                            testNameSubcompletionDuringNameRegisterUpdate() {
    TargetApplication::self()->setTargetApplicationFilePath(mPath);
    TargetApplication::self()->start();

    QVERIFY(waitForTargetApplicationToStart(10000));

    RemoteObjectNameWidget widget;

    KLineEdit* lineEdit = objectNameLineEdit(&widget);

    //Using the configured KShortcut with QTest::keyClick is too cumbersome,
    //so the binding is overridden for this test
    lineEdit->setKeyBinding(KCompletionBase::SubstringCompletion,
                            KShortcut("Ctrl+T"));
    QTest::keyClick(lineEdit, Qt::Key_T, Qt::ControlModifier);

    KCompletionBox* completionBox = lineEdit->completionBox();
    QStringList completionItems = completionBox->items();

    //Ensure that the register is still being updated
    QVERIFY(widget.mRemoteObjectNameRegister->isBeingUpdated());

    QVERIFY(!completionBox->isVisible());
    QCOMPARE(completionItems.count(), 0);

    QVERIFY(waitForNamesToBeRegistered(&widget, 10000));

    completionBox = lineEdit->completionBox();
    completionItems = completionBox->items();

    QVERIFY(completionBox->isVisible());

    QCOMPARE(completionItems.count(), 102);
    QVERIFY(completionItems.contains("The object name 42"));
    QVERIFY(completionItems.contains("The object name 420"));
    QVERIFY(completionItems.contains("The object name 421"));
    QVERIFY(completionItems.contains("The object name 422"));
    QVERIFY(completionItems.contains("The object name 423"));
    QVERIFY(completionItems.contains("Duplicated grandparent/Duplicated parent/"
                                     "Duplicated object"));
    QVERIFY(completionItems.contains("The object name 50/Duplicated object"));
    QVERIFY(completionItems.contains("The object name 7/Duplicated object"));
    QVERIFY(completionItems.contains("The object name 8/Duplicated object"));
}

void RemoteObjectNameWidgetTest::testTargetApplicationStartedAfterWidget() {
    RemoteObjectNameWidget widget;
    QSignalSpy remoteObjectChosenSpy(&widget,
                                     SIGNAL(remoteObjectChosen(RemoteObject*)));

    QCOMPARE(objectNameLineEdit(&widget)->cursor().shape(), Qt::IBeamCursor);

    QStringList items = objectNameLineEdit(&widget)->completionObject()->
                                                                        items();
    QCOMPARE(items.count(), 0);

    widget.setName("The object name 42");

    QCOMPARE(widget.name(), QString("The object name 42"));
    QCOMPARE(remoteObjectChosenSpy.count(), 1);
    assertRemoteObjectSignal(remoteObjectChosenSpy, 0, 0);

    TargetApplication::self()->setTargetApplicationFilePath(mPath);
    TargetApplication::self()->start();

    QVERIFY(waitForTargetApplicationToStart(10000));

    QCOMPARE(objectNameLineEdit(&widget)->cursor().shape(), Qt::BusyCursor);

    QVERIFY(waitForNamesToBeRegistered(&widget, 10000));

    QCOMPARE(objectNameLineEdit(&widget)->cursor().shape(), Qt::IBeamCursor);

    items = objectNameLineEdit(&widget)->completionObject()->items();
    assertCompletionItems(items);

    widget.setName("The object name 423");

    RemoteObject* mainWindow =
                TargetApplication::self()->remoteEditorSupport()->mainWindow();

    QCOMPARE(widget.name(), QString("The object name 423"));
    QCOMPARE(remoteObjectChosenSpy.count(), 2);
    assertRemoteObjectSignal(remoteObjectChosenSpy, 1,
                             mainWindow->children()[3]);
}

void RemoteObjectNameWidgetTest::testTargetApplicationStopped() {
    TargetApplication::self()->setTargetApplicationFilePath(mPath);
    TargetApplication::self()->start();

    QVERIFY(waitForTargetApplicationToStart(10000));

    RemoteObjectNameWidget widget;

    QVERIFY(waitForNamesToBeRegistered(&widget, 10000));

    QSignalSpy remoteObjectChosenSpy(&widget,
                                     SIGNAL(remoteObjectChosen(RemoteObject*)));

    TargetApplication::self()->mProcess->kill();

    QVERIFY(waitForTargetApplicationToStop(10000));

    QStringList items = objectNameLineEdit(&widget)->completionObject()->
                                                                        items();
    QCOMPARE(items.count(), 0);

    widget.setName("The object name 42");

    QCOMPARE(widget.name(), QString("The object name 42"));
    QCOMPARE(remoteObjectChosenSpy.count(), 1);
    assertRemoteObjectSignal(remoteObjectChosenSpy, 0, 0);
}

void RemoteObjectNameWidgetTest::testAddingOrRemovingRemoteObjects() {
    QSKIP("Unfortunately, testing if the completion list is updated when an "
          "object is added or removed in the target application is too "
          "burdensome so the test must be done manually", SkipAll);
}

void RemoteObjectNameWidgetTest::testSetFocus() {
    QWidget parent;
    QLineEdit* otherLineEdit = new QLineEdit(&parent); //krazy:exclude=qclasses

    RemoteObjectNameWidget* widget = new RemoteObjectNameWidget(&parent);

    //Focus is not set in hidden widgets until they are shown
    parent.show();

    QTest::qWaitForWindowShown(&parent);

    otherLineEdit->setFocus();

    QVERIFY(!widget->hasFocus());
    QVERIFY(!objectNameLineEdit(widget)->hasFocus());

    widget->setFocus();

    QVERIFY(widget->hasFocus());
    QVERIFY(objectNameLineEdit(widget)->hasFocus());
}

void RemoteObjectNameWidgetTest::testWidgetAsBuddyOfLabel() {
    QWidget parent;
    QLineEdit* otherLineEdit = new QLineEdit(&parent); //krazy:exclude=qclasses

    RemoteObjectNameWidget* widget = new RemoteObjectNameWidget(&parent);

    QLabel* widgetLabel = new QLabel(&parent);
    widgetLabel->setText("&Widget label:");
    widgetLabel->setBuddy(widget);

    //Focus is not set in hidden widgets until they are shown
    parent.show();

    QTest::qWaitForWindowShown(&parent);

    otherLineEdit->setFocus();

    QVERIFY(!widget->hasFocus());
    QVERIFY(!objectNameLineEdit(widget)->hasFocus());

    QTest::keyClick(&parent, Qt::Key_W, Qt::AltModifier);

    QVERIFY(widget->hasFocus());
    QVERIFY(objectNameLineEdit(widget)->hasFocus());
}

/////////////////////////////////// Helpers ////////////////////////////////////

KLineEdit* RemoteObjectNameWidgetTest::objectNameLineEdit(
                                        RemoteObjectNameWidget* widget) const {
    return widget->findChild<KLineEdit*>("objectNameLineEdit");
}

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

bool RemoteObjectNameWidgetTest::waitForTargetApplicationToStart(
                                                            int timeout) const {
    return waitFor(&isTargetApplicationStarted, timeout);
}

bool RemoteObjectNameWidgetTest::waitForTargetApplicationToStop(
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

bool RemoteObjectNameWidgetTest::waitForNamesToBeRegistered(
                    const RemoteObjectNameWidget* widget, int timeout) const {    
    RegisterNotBeingUpdatedCondition helper;
    helper.mRemoteObjectNameRegister = widget->mRemoteObjectNameRegister;
    return waitFor(helper, &RegisterNotBeingUpdatedCondition::condition,
                   timeout);
}

//The message boxes are modal, so they won't return to the test code until they
//are closed. Thus, the actions to be performed while the message boxes are
//being shown (like closing the message boxes themselves) must be "queued".
//Instead of queueing them to be performed after some fixed amount of time,
//it is checked if the action can be performed and, if not, the action is
//retried again after a little amount of time. The retries continue until the
//timeout expires.
class QueuedAction: public QObject {
Q_OBJECT
public:

    QueuedAction(int timeout, QObject* parent): QObject(parent),
        mTimeout(timeout),
        mFinished(false),
        mActionDone(false) {
    }

    void run() {
        mElapsedTime.start();
        mFinished = false;

        checkAction();
    }

    bool isFinished() const {
        return mFinished;
    }

    bool isActionDone() const {
        return mActionDone;
    }
    
public slots:

    void checkAction() {
        mActionDone = action();
        if (!mActionDone && mElapsedTime.elapsed() < mTimeout) {
            QTimer::singleShot(100, this, SLOT(checkAction()));
        } else {
            mFinished = true;
        }
    }

protected:

    virtual bool action() = 0;

private:

    int mTimeout;
    QTime mElapsedTime;
    bool mFinished;
    bool mActionDone;

};

class CloseSorryMessageBoxHelper: public QueuedAction {
public:

    CloseSorryMessageBoxHelper(int timeout, QObject* parent):
        QueuedAction(timeout, parent),
        mWidget(0) {
    }

    void setWidget(QWidget* widget) {
        mWidget = widget;
    }

protected:

    virtual bool action() {
        KDialog* messageBox = mWidget->findChild<KDialog*>("sorry");
        if (!messageBox) {
            return false;
        }

        delete messageBox;

        return true;
    }

private:

    QWidget* mWidget;

};

void RemoteObjectNameWidgetTest::closeSorryMessageBox(QWidget* widget,
                                                      int timeToWait) {
    CloseSorryMessageBoxHelper* helper = new CloseSorryMessageBoxHelper(
                                                            timeToWait, this);
    helper->setWidget(widget);
    helper->run();    
}

//RemoteObject* must be declared as a metatype to be used in qvariant_cast
Q_DECLARE_METATYPE(RemoteObject*);

void RemoteObjectNameWidgetTest::assertRemoteObjectSignal(const QSignalSpy& spy,
                            int index, const RemoteObject* remoteObject) const {
    QVariant argument = spy.at(index).at(0);
    QCOMPARE(argument.userType(), mRemoteObjectStarType);
    QCOMPARE(qvariant_cast<RemoteObject*>(argument), remoteObject);
}

void RemoteObjectNameWidgetTest::assertCompletionItems(
                                            const QStringList& items) const {
    QCOMPARE(items.count(), 89);
    QVERIFY(items.contains("The object name 42"));
    QVERIFY(items.contains("The object name 420"));
    QVERIFY(items.contains("The object name 421"));
    QVERIFY(items.contains("The object name 422"));
    QVERIFY(items.contains("The object name 423"));
    QVERIFY(items.contains("The object name 7"));
    QVERIFY(items.contains("The object name 62"));
    QVERIFY(items.contains("The object name 833"));
    QVERIFY(items.contains("Duplicated object"));
    QVERIFY(items.contains("Duplicated parent"));
    QVERIFY(items.contains("Duplicated grandparent"));
}

QTEST_MAIN(RemoteObjectNameWidgetTest)

#include "RemoteObjectNameWidgetTest.moc"
