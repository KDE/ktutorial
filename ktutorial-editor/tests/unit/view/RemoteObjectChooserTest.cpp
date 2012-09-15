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
#include <QtTest/QTestKeyClicksEvent>

#define protected public
#define private public
#include "RemoteObjectChooser.h"
#undef private
#undef protected

#include <QCheckBox>
#include <QPushButton>
#include <QTreeView>

#include <KDialog>
#include <KProcess>

#define protected public
#define private public
#include "../targetapplication/TargetApplication.h"
#undef private
#undef protected

#include "TargetApplicationView.h"
#include "../targetapplication/RemoteEditorSupport.h"
#include "../targetapplication/RemoteObject.h"

class RemoteObjectChooserTest: public QObject {
Q_OBJECT

private slots:

    void init();
    void cleanup();

    void testConstructor();

    void testTargetApplicationNotStarted();
    void testTargetApplicationAlreadyStarted();
    void testTargetApplicationWithInvalidPath();
    void testTargetApplicationNotUsingKTutorial();

    void testTargetApplicationStopped();
    void testTargetApplicationStoppedBeforeClosingInformationMessageBox();

    void close();
    //Closing with ALT+F4 can't be tested, as it depends on the window manager
    //rather than the widget

    void testShowOnlyNamedObjects();
    void testShowOnlyWidgets();
    void testShowOnlyNamedWidgets();

    void testSelectRemoteObject();

    void testOkButton();
    void testCancelButton();

private:

    QString mPath;

    void closeInformationMessageBox(int timeToWait);
    void closeInformationMessageBoxOnceTargetApplicationEnds(int timeToWait);
    void closeSorryMessageBox(QWidget* widget, int timeToWait);

    void killTargetApplicationOnceMessageBoxIsShown(int timeToWait);

    bool waitForSorryMessageBoxToBeClosed(QWidget* widget, int timeout);

    bool waitForTargetApplicationToStart(int timeout) const;
    bool waitForTargetApplicationToStop(int timeout) const;

    QCheckBox* showOnlyNamedObjectsCheckBox(RemoteObjectChooser* widget) const;
    QCheckBox* showOnlyWidgetsCheckBox(RemoteObjectChooser* widget) const;

    QTreeView* remoteObjectsTreeView(RemoteObjectChooser* widget) const;

    QPushButton* okButton(RemoteObjectChooser* widget) const;
    QPushButton* cancelButton(RemoteObjectChooser* widget) const;

};

void RemoteObjectChooserTest::init() {
    mPath = QApplication::applicationDirPath() +
                                "/../targetapplication/TargetApplicationStub";

    //Avoid signals from previous tests to be delivered to the next ones
    //setting a new TargetApplication
    delete TargetApplication::sSelf;
    TargetApplication::sSelf = new TargetApplication();
}

void RemoteObjectChooserTest::cleanup() {
    delete TargetApplication::sSelf;
    TargetApplication::sSelf = 0;
}

void RemoteObjectChooserTest::testConstructor() {
    TargetApplication::self()->setTargetApplicationFilePath(mPath);

    QWidget parent(0, Qt::Window);
    RemoteObjectChooser* chooser = new RemoteObjectChooser(&parent);

    QCOMPARE(chooser->parent(), &parent);
    QVERIFY(chooser->windowFlags() & Qt::Dialog);
    QVERIFY(!chooser->mCurrentRemoteObject);
    QVERIFY(remoteObjectsTreeView(chooser));
    QVERIFY(okButton(chooser));
    QVERIFY(!okButton(chooser)->isEnabled());
    QVERIFY(cancelButton(chooser));
    QVERIFY(cancelButton(chooser)->isEnabled());
}

void RemoteObjectChooserTest::testTargetApplicationNotStarted() {
    TargetApplication::self()->setTargetApplicationFilePath(mPath);

    QWidget window(0, Qt::Window);
    window.show();
    QWidget* dialog = new QWidget(&window, Qt::Dialog);
    dialog->show();

    //Queue closing the information message box
    closeInformationMessageBox(10000);
    RemoteObjectChooser* chooser = new RemoteObjectChooser(dialog);
    chooser->show();

    QVERIFY(waitForTargetApplicationToStart(10000));

    QVERIFY(chooser->isVisible());
    QVERIFY(!window.isVisible());
    QVERIFY(!dialog->isVisible());
    QVERIFY(remoteObjectsTreeView(chooser)->model());
    QVERIFY(remoteObjectsTreeView(chooser)->model()->hasIndex(0, 0));
    QVERIFY(!okButton(chooser)->isEnabled());
    QVERIFY(cancelButton(chooser)->isEnabled());
}

void RemoteObjectChooserTest::testTargetApplicationAlreadyStarted() {
    TargetApplication::self()->setTargetApplicationFilePath(mPath);
    TargetApplication::self()->start();

    QVERIFY(waitForTargetApplicationToStart(10000));

    QWidget window(0, Qt::Window);
    window.show();
    QWidget* dialog = new QWidget(&window, Qt::Dialog);
    dialog->show();

    //Queue closing the information message box
    closeInformationMessageBox(10000);
    RemoteObjectChooser* chooser = new RemoteObjectChooser(dialog);
    chooser->show();

    QVERIFY(chooser->isVisible());
    QVERIFY(!window.isVisible());
    QVERIFY(!dialog->isVisible());
    QVERIFY(remoteObjectsTreeView(chooser)->model());
    QVERIFY(remoteObjectsTreeView(chooser)->model()->hasIndex(0, 0));
    QVERIFY(!okButton(chooser)->isEnabled());
    QVERIFY(cancelButton(chooser)->isEnabled());
}

void RemoteObjectChooserTest::testTargetApplicationWithInvalidPath() {
    TargetApplication::self()->setTargetApplicationFilePath(mPath + "Invalid");

    QWidget window(0, Qt::Window);
    window.show();
    QWidget* dialog = new QWidget(&window, Qt::Dialog);
    dialog->show();

    QPointer<RemoteObjectChooser> chooser = new RemoteObjectChooser(dialog);
    chooser->show();

    //The sorry message box will be shown and then closed once the target
    //application fails to start
    QVERIFY(waitForSorryMessageBoxToBeClosed(chooser, 10000));

    //Process deleteLater()
    QCoreApplication::sendPostedEvents(chooser, QEvent::DeferredDelete);

    QVERIFY(!chooser);
    QVERIFY(window.isVisible());
    QVERIFY(dialog->isVisible());
}

void RemoteObjectChooserTest::testTargetApplicationNotUsingKTutorial() {
    QString path = QApplication::applicationDirPath() +
                                "/../targetapplication/DummyApplication";
    TargetApplication::self()->setTargetApplicationFilePath(path);

    QWidget window(0, Qt::Window);
    window.show();
    QWidget* dialog = new QWidget(&window, Qt::Dialog);
    dialog->show();

    QPointer<RemoteObjectChooser> chooser = new RemoteObjectChooser(dialog);
    chooser->show();

    //The sorry message box will be shown and then closed once the target
    //application fails to start
    QVERIFY(waitForSorryMessageBoxToBeClosed(chooser, 10000));

    //Process deleteLater()
    QCoreApplication::sendPostedEvents(chooser, QEvent::DeferredDelete);

    QVERIFY(!chooser);
    QVERIFY(window.isVisible());
    QVERIFY(dialog->isVisible());
}

void RemoteObjectChooserTest::testTargetApplicationStopped() {
    TargetApplication::self()->setTargetApplicationFilePath(mPath);

    QWidget window(0, Qt::Window);
    window.show();
    QWidget* dialog = new QWidget(&window, Qt::Dialog);
    dialog->show();

    //Queue closing the information message box
    closeInformationMessageBox(10000);
    QPointer<RemoteObjectChooser> chooser = new RemoteObjectChooser(dialog);
    chooser->show();

    QVERIFY(waitForTargetApplicationToStart(10000));

    //Queue closing the sorry message box
    closeSorryMessageBox(chooser, 10000);

    TargetApplication::self()->mProcess->kill();

    QVERIFY(waitForTargetApplicationToStop(10000));

    //Process deleteLater()
    QCoreApplication::sendPostedEvents(chooser, QEvent::DeferredDelete);

    QVERIFY(!chooser);
    QVERIFY(window.isVisible());
    QVERIFY(dialog->isVisible());
}

void RemoteObjectChooserTest::
testTargetApplicationStoppedBeforeClosingInformationMessageBox() {
    TargetApplication::self()->setTargetApplicationFilePath(mPath);

    QWidget window(0, Qt::Window);
    window.show();
    QWidget* dialog = new QWidget(&window, Qt::Dialog);
    dialog->show();

    //Queue killing the target application once the message box is shown
    killTargetApplicationOnceMessageBoxIsShown(10000);

    //Queue closing the information message box once the target application was
    //killed
    closeInformationMessageBoxOnceTargetApplicationEnds(10000);

    QPointer<RemoteObjectChooser> chooser = new RemoteObjectChooser(dialog);
    chooser->show();

    //The sorry message box will be shown and then closed once the information
    //message box is closed
    QVERIFY(waitForSorryMessageBoxToBeClosed(chooser, 10000));

    //Process deleteLater()
    QCoreApplication::sendPostedEvents(chooser, QEvent::DeferredDelete);

    QVERIFY(!chooser);
    QVERIFY(window.isVisible());
    QVERIFY(dialog->isVisible());
}

void RemoteObjectChooserTest::close() {
    TargetApplication::self()->setTargetApplicationFilePath(mPath);

    QWidget window(0, Qt::Window);
    window.show();
    QWidget* dialog = new QWidget(&window, Qt::Dialog);
    dialog->show();

    //Queue closing the information message box
    closeInformationMessageBox(10000);
    QPointer<RemoteObjectChooser> chooser = new RemoteObjectChooser(dialog);
    chooser->show();

    QVERIFY(waitForTargetApplicationToStart(10000));

    chooser->close();

    //Process deleteLater()
    QCoreApplication::sendPostedEvents(chooser, QEvent::DeferredDelete);

    QVERIFY(!chooser);
    QVERIFY(window.isVisible());
    QVERIFY(dialog->isVisible());
}

void RemoteObjectChooserTest::testShowOnlyNamedObjects() {
    TargetApplication::self()->setTargetApplicationFilePath(mPath);

    QWidget window(0, Qt::Window);
    //Queue closing the information message box
    closeInformationMessageBox(10000);
    RemoteObjectChooser* chooser = new RemoteObjectChooser(&window);
    chooser->show();

    QVERIFY(waitForTargetApplicationToStart(10000));

    QVERIFY(remoteObjectsTreeView(chooser)->model());

    QModelIndex index = remoteObjectsTreeView(chooser)->model()->index(7, 0);
    QCOMPARE(remoteObjectsTreeView(chooser)->model()->rowCount(index), 4);

    showOnlyNamedObjectsCheckBox(chooser)->click();

    QCOMPARE(remoteObjectsTreeView(chooser)->model()->rowCount(index), 3);

    showOnlyNamedObjectsCheckBox(chooser)->click();

    QCOMPARE(remoteObjectsTreeView(chooser)->model()->rowCount(index), 4);
}

void RemoteObjectChooserTest::testShowOnlyWidgets() {
    TargetApplication::self()->setTargetApplicationFilePath(mPath);

    QWidget window(0, Qt::Window);
    //Queue closing the information message box
    closeInformationMessageBox(10000);
    RemoteObjectChooser* chooser = new RemoteObjectChooser(&window);
    chooser->show();

    QVERIFY(waitForTargetApplicationToStart(10000));

    QVERIFY(remoteObjectsTreeView(chooser)->model());

    QModelIndex index = remoteObjectsTreeView(chooser)->model()->index(7, 0);
    QCOMPARE(remoteObjectsTreeView(chooser)->model()->rowCount(index), 4);

    showOnlyWidgetsCheckBox(chooser)->click();

    //The index changes because the other base items were hidden
    index = remoteObjectsTreeView(chooser)->model()->index(0, 0);
    QCOMPARE(remoteObjectsTreeView(chooser)->model()->rowCount(index), 3);

    showOnlyWidgetsCheckBox(chooser)->click();

    index = remoteObjectsTreeView(chooser)->model()->index(7, 0);
    QCOMPARE(remoteObjectsTreeView(chooser)->model()->rowCount(index), 4);
}

void RemoteObjectChooserTest::testShowOnlyNamedWidgets() {
    TargetApplication::self()->setTargetApplicationFilePath(mPath);

    QWidget window(0, Qt::Window);
    //Queue closing the information message box
    closeInformationMessageBox(10000);
    RemoteObjectChooser* chooser = new RemoteObjectChooser(&window);
    chooser->show();

    QVERIFY(waitForTargetApplicationToStart(10000));

    QVERIFY(remoteObjectsTreeView(chooser)->model());

    QModelIndex index = remoteObjectsTreeView(chooser)->model()->index(7, 0);
    QCOMPARE(remoteObjectsTreeView(chooser)->model()->rowCount(index), 4);

    showOnlyWidgetsCheckBox(chooser)->click();

    //The index changes because the other base items were hidden
    index = remoteObjectsTreeView(chooser)->model()->index(0, 0);
    QCOMPARE(remoteObjectsTreeView(chooser)->model()->rowCount(index), 3);

    showOnlyNamedObjectsCheckBox(chooser)->click();

    QCOMPARE(remoteObjectsTreeView(chooser)->model()->rowCount(index), 2);

    showOnlyWidgetsCheckBox(chooser)->click();

    index = remoteObjectsTreeView(chooser)->model()->index(7, 0);
    QCOMPARE(remoteObjectsTreeView(chooser)->model()->rowCount(index), 3);

    showOnlyNamedObjectsCheckBox(chooser)->click();

    QCOMPARE(remoteObjectsTreeView(chooser)->model()->rowCount(index), 4);
}

void RemoteObjectChooserTest::testSelectRemoteObject() {
    TargetApplication::self()->setTargetApplicationFilePath(mPath);

    QWidget window(0, Qt::Window);
    //Queue closing the information message box
    closeInformationMessageBox(10000);
    RemoteObjectChooser* chooser = new RemoteObjectChooser(&window);
    chooser->show();

    QVERIFY(waitForTargetApplicationToStart(10000));

    QVERIFY(remoteObjectsTreeView(chooser)->model());
    QModelIndex index = remoteObjectsTreeView(chooser)->model()->index(1, 0);
    remoteObjectsTreeView(chooser)->selectionModel()->
                            select(index, QItemSelectionModel::SelectCurrent);

    QVERIFY(TargetApplication::self()->remoteEditorSupport());
    RemoteObject* mainWindow =
                TargetApplication::self()->remoteEditorSupport()->mainWindow();
    QCOMPARE(chooser->mCurrentRemoteObject, mainWindow->children()[1]);
    QVERIFY(okButton(chooser)->isEnabled());
    QVERIFY(cancelButton(chooser)->isEnabled());

    index = remoteObjectsTreeView(chooser)->model()->index(0, 0);
    remoteObjectsTreeView(chooser)->selectionModel()->
                            select(index, QItemSelectionModel::SelectCurrent);

    QCOMPARE(chooser->mCurrentRemoteObject, mainWindow->children()[0]);
    QVERIFY(okButton(chooser)->isEnabled());
    QVERIFY(cancelButton(chooser)->isEnabled());

    index = remoteObjectsTreeView(chooser)->model()->index(23, 42);
    remoteObjectsTreeView(chooser)->selectionModel()->
                            select(index, QItemSelectionModel::SelectCurrent);

    QCOMPARE(chooser->mCurrentRemoteObject, (RemoteObject*)0);
    QVERIFY(!okButton(chooser)->isEnabled());
    QVERIFY(cancelButton(chooser)->isEnabled());
}

//RemoteObject* must be declared as a metatype to be used in qvariant_cast
Q_DECLARE_METATYPE(RemoteObject*);

void RemoteObjectChooserTest::testOkButton() {
    TargetApplication::self()->setTargetApplicationFilePath(mPath);

    QWidget window(0, Qt::Window);
    window.show();
    QWidget* dialog = new QWidget(&window, Qt::Dialog);
    dialog->show();

    //Queue closing the information message box
    closeInformationMessageBox(10000);
    QPointer<RemoteObjectChooser> chooser = new RemoteObjectChooser(dialog);
    chooser->show();

    QVERIFY(waitForTargetApplicationToStart(10000));

    QVERIFY(remoteObjectsTreeView(chooser)->model());
    QModelIndex index = remoteObjectsTreeView(chooser)->model()->index(1, 0);
    remoteObjectsTreeView(chooser)->selectionModel()->
                            select(index, QItemSelectionModel::SelectCurrent);

    //RemoteObject* must be registered in order to be used with QSignalSpy
    int remoteObjectStarType =
                            qRegisterMetaType<RemoteObject*>("RemoteObject*");
    QSignalSpy remoteObjectChosenSpy(chooser,
                                     SIGNAL(remoteObjectChosen(RemoteObject*)));

    okButton(chooser)->click();

    //Process deleteLater()
    QCoreApplication::sendPostedEvents(chooser, QEvent::DeferredDelete);

    QVERIFY(!chooser);
    QVERIFY(window.isVisible());
    QVERIFY(dialog->isVisible());

    QVERIFY(TargetApplication::self()->remoteEditorSupport());
    RemoteObject* mainWindow =
                TargetApplication::self()->remoteEditorSupport()->mainWindow();
    QCOMPARE(remoteObjectChosenSpy.count(), 1);
    QVariant argument = remoteObjectChosenSpy.at(0).at(0);
    QCOMPARE(argument.userType(), remoteObjectStarType);
    QCOMPARE(qvariant_cast<RemoteObject*>(argument), mainWindow->children()[1]);
}

void RemoteObjectChooserTest::testCancelButton() {
    TargetApplication::self()->setTargetApplicationFilePath(mPath);

    QWidget window(0, Qt::Window);
    window.show();
    QWidget* dialog = new QWidget(&window, Qt::Dialog);
    dialog->show();

    //Queue closing the information message box
    closeInformationMessageBox(10000);
    QPointer<RemoteObjectChooser> chooser = new RemoteObjectChooser(dialog);
    chooser->show();

    QVERIFY(waitForTargetApplicationToStart(10000));

    QVERIFY(remoteObjectsTreeView(chooser)->model());
    QModelIndex index = remoteObjectsTreeView(chooser)->model()->index(1, 0);
    remoteObjectsTreeView(chooser)->selectionModel()->
                            select(index, QItemSelectionModel::SelectCurrent);

    //RemoteObject* must be registered in order to be used with QSignalSpy
    qRegisterMetaType<RemoteObject*>("RemoteObject*");
    QSignalSpy remoteObjectChosenSpy(chooser,
                                     SIGNAL(remoteObjectChosen(RemoteObject*)));

    cancelButton(chooser)->click();

    //Process deleteLater()
    QCoreApplication::sendPostedEvents(chooser, QEvent::DeferredDelete);

    QVERIFY(!chooser);
    QVERIFY(window.isVisible());
    QVERIFY(dialog->isVisible());
    QCOMPARE(remoteObjectChosenSpy.count(), 0);
}

/////////////////////////////////// Helpers ////////////////////////////////////

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

QWidget* findRemoteObjectChooser() {
    foreach (QWidget* widget, QApplication::topLevelWidgets()) {
        if (qobject_cast<RemoteObjectChooser*>(widget)) {
            return widget;
        }
    }

    return 0;
}

KDialog* findInformationMessageBox() {
    QWidget* widget = findRemoteObjectChooser();
    if (!widget) {
        return 0;
    }

    return widget->findChild<KDialog*>("information");
}

class CloseInformationMessageBoxHelper: public QueuedAction {
public:

    CloseInformationMessageBoxHelper(int timeout, QObject* parent):
        QueuedAction(timeout, parent) {
    }

protected:

    virtual bool action() {
        //The information message box is created in the constructor itself of
        //RemoteObjectChooser, so the information message box can not be passed
        //as an argument when this helper is created. It must be got when it is
        //going to be destroyed.
        KDialog* messageBox = findInformationMessageBox();
        if (!messageBox) {
            return false;
        }

        delete messageBox;

        return true;
    }
};

class CloseInformationMessageBoxOnceTargetApplicationEndsHelper:
                                    public CloseInformationMessageBoxHelper {
public:

    CloseInformationMessageBoxOnceTargetApplicationEndsHelper(int timeout,
                                                              QObject* parent):
        CloseInformationMessageBoxHelper(timeout, parent) {
    }

protected:

    virtual bool action() {
        if (TargetApplication::self()->remoteEditorSupport()) {
            return false;
        }

        return CloseInformationMessageBoxHelper::action();
    }
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

class KillTargetApplicationOnceMessageBoxIsShownHelper: public QueuedAction {
public:

    KillTargetApplicationOnceMessageBoxIsShownHelper(int timeout,
                                                     QObject* parent):
        QueuedAction(timeout, parent) {
    }

protected:

    virtual bool action() {
        //The information message box is created in the constructor itself of
        //RemoteObjectChooser, so the information message box can not be passed
        //as an argument when this helper is created.
        KDialog* messageBox = findInformationMessageBox();
        if (!messageBox) {
            return false;
        }

        TargetApplication::self()->mProcess->kill();

        return true;
    }
};

void RemoteObjectChooserTest::closeInformationMessageBox(int timeToWait) {
    CloseInformationMessageBoxHelper* helper =
                        new CloseInformationMessageBoxHelper(timeToWait, this);
    helper->run();
}

void RemoteObjectChooserTest::
        closeInformationMessageBoxOnceTargetApplicationEnds(int timeToWait) {
    CloseInformationMessageBoxOnceTargetApplicationEndsHelper* helper =
                new CloseInformationMessageBoxOnceTargetApplicationEndsHelper(
                                                            timeToWait, this);
    helper->run();
}

void RemoteObjectChooserTest::closeSorryMessageBox(QWidget* widget,
                                                   int timeToWait) {
    CloseSorryMessageBoxHelper* helper = new CloseSorryMessageBoxHelper(
                                                            timeToWait, this);
    helper->setWidget(widget);
    helper->run();    
}

void RemoteObjectChooserTest::killTargetApplicationOnceMessageBoxIsShown(
                                                            int timeToWait) {
    KillTargetApplicationOnceMessageBoxIsShownHelper* helper =
        new KillTargetApplicationOnceMessageBoxIsShownHelper(timeToWait, this);
    helper->run();    
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

bool RemoteObjectChooserTest::waitForSorryMessageBoxToBeClosed(QWidget* widget,
                                                               int timeout) {
    CloseSorryMessageBoxHelper* helper = new CloseSorryMessageBoxHelper(timeout,
                                                                        this);
    helper->setWidget(widget);
    helper->run();

    waitFor<QueuedAction>(*helper, &QueuedAction::isFinished, timeout);
    
    return helper->isActionDone();
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

bool RemoteObjectChooserTest::waitForTargetApplicationToStart(
                                                            int timeout) const {
    return waitFor(&isTargetApplicationStarted, timeout);
}

bool RemoteObjectChooserTest::waitForTargetApplicationToStop(
                                                            int timeout) const {
    return waitFor(&isTargetApplicationStopped, timeout);
}

QCheckBox* RemoteObjectChooserTest::showOnlyNamedObjectsCheckBox(
                                            RemoteObjectChooser* widget) const {
    return widget->findChild<QCheckBox*>("showOnlyNamedObjectsCheckBox");
}

QCheckBox* RemoteObjectChooserTest::showOnlyWidgetsCheckBox(
                                            RemoteObjectChooser* widget) const {
    return widget->findChild<QCheckBox*>("showOnlyWidgetsCheckBox");
}

QTreeView* RemoteObjectChooserTest::remoteObjectsTreeView(
                                            RemoteObjectChooser* widget) const {
    return widget->findChild<QTreeView*>("remoteObjectsTreeView");
}

QPushButton* RemoteObjectChooserTest::okButton(
                                            RemoteObjectChooser* widget) const {
    return widget->findChild<QPushButton*>("okButton");
}

QPushButton* RemoteObjectChooserTest::cancelButton(
                                            RemoteObjectChooser* widget) const {
    return widget->findChild<QPushButton*>("cancelButton");
}

QTEST_MAIN(RemoteObjectChooserTest)

#include "RemoteObjectChooserTest.moc"
