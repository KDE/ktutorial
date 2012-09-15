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

#include <QDialog>

#include "WindowOnTopEnforcer.h"

namespace ktutorial {
namespace view {

class WindowOnTopEnforcerTest: public QObject {
Q_OBJECT

private slots:

    void testConstructor();

    void testBaseWindow();

    void testChildWindow();

    void testModalDialog();

    void testNestedModalDialog();

    void testSeveralModalDialogs();

    void testNestedModalDialogOnChildWindow();

    void testModalWidget();

    void testModalWidgetSiblingOfParent();

    void testShowModalWidgetTwice();

    void testHideNestedModalWidgetsInOrder();

    void testHideNestedModalWidgetsInReverseOrder();

    void testShowNestedModalWidgetsInOrder();

    void testShowNestedModalWidgetsInReverseOrder();

    void testShowNestedModalWidgetsInMixedOrder();

    void testSetBaseWindowAfterSeveralModalWidgetsWereShown();

private:

    void queueAssertParent(QWidget* widget, QWidget* parent, int timeToWait);
    void queueAssertIsVisibleWindow(QWidget* widget, int timeToWait);

    void assertWindow(QWidget* window, QWidget* expectedParent) const;

};

void WindowOnTopEnforcerTest::testConstructor() {
    QWidget widget;
    WindowOnTopEnforcer* enforcer = new WindowOnTopEnforcer(&widget);

    QCOMPARE(enforcer->parent(), &widget);
}

void WindowOnTopEnforcerTest::testBaseWindow() {
    QWidget* window = new QWidget();
    window->setWindowTitle("Window");
    window->show();

    QPointer<QWidget> windowToKeepOnTop = new QWidget(window);
    windowToKeepOnTop->setWindowTitle("Window to keep on top");
    windowToKeepOnTop->setWindowFlags(Qt::Window);
    windowToKeepOnTop->show();

    WindowOnTopEnforcer* enforcer = new WindowOnTopEnforcer(windowToKeepOnTop);
    enforcer->setBaseWindow(window);

    assertWindow(windowToKeepOnTop, window);

    window->hide();

    assertWindow(windowToKeepOnTop, window);

    window->show();

    assertWindow(windowToKeepOnTop, window);

    delete window;

    QVERIFY(!windowToKeepOnTop);
}

void WindowOnTopEnforcerTest::testChildWindow() {
    QWidget* window = new QWidget();
    window->setWindowTitle("Window");
    window->show();

    QPointer<QWidget> windowToKeepOnTop = new QWidget(window);
    windowToKeepOnTop->setWindowTitle("Window to keep on top");
    windowToKeepOnTop->setWindowFlags(Qt::Window);
    windowToKeepOnTop->show();

    WindowOnTopEnforcer* enforcer = new WindowOnTopEnforcer(windowToKeepOnTop);
    enforcer->setBaseWindow(window);

    QWidget* childWindow = new QWidget(window);
    childWindow->setWindowTitle("Child window");
    childWindow->setWindowFlags(Qt::Window);
    childWindow->show();

    assertWindow(windowToKeepOnTop, window);

    childWindow->hide();

    assertWindow(windowToKeepOnTop, window);

    childWindow->show();

    assertWindow(windowToKeepOnTop, window);

    delete childWindow;

    assertWindow(windowToKeepOnTop, window);

    delete window;

    QVERIFY(!windowToKeepOnTop);
}

void WindowOnTopEnforcerTest::testModalDialog() {
    QWidget* window = new QWidget();
    window->setWindowTitle("Window");
    window->show();

    QPointer<QWidget> windowToKeepOnTop = new QWidget(window);
    windowToKeepOnTop->setWindowTitle("Window to keep on top");
    windowToKeepOnTop->setWindowFlags(Qt::Window);
    windowToKeepOnTop->show();

    WindowOnTopEnforcer* enforcer = new WindowOnTopEnforcer(windowToKeepOnTop);
    enforcer->setBaseWindow(window);

    QDialog* modalDialog = new QDialog(window);
    modalDialog->setWindowTitle("Modal dialog");

    QTimer timerAccept;
    timerAccept.setSingleShot(true);
    timerAccept.setInterval(1000);
    connect(&timerAccept, SIGNAL(timeout()), modalDialog, SLOT(accept()));

    queueAssertParent(windowToKeepOnTop, modalDialog, 500);
    queueAssertIsVisibleWindow(windowToKeepOnTop, 500);

    timerAccept.start();
    modalDialog->exec();

    assertWindow(windowToKeepOnTop, window);

    delete window;

    QVERIFY(!windowToKeepOnTop);
}

void WindowOnTopEnforcerTest::testNestedModalDialog() {
    QWidget* window = new QWidget();
    window->setWindowTitle("Window");
    window->show();

    QPointer<QWidget> windowToKeepOnTop = new QWidget(window);
    windowToKeepOnTop->setWindowTitle("Window to keep on top");
    windowToKeepOnTop->setWindowFlags(Qt::Window);
    windowToKeepOnTop->show();

    WindowOnTopEnforcer* enforcer = new WindowOnTopEnforcer(windowToKeepOnTop);
    enforcer->setBaseWindow(window);

    QDialog* modalDialog = new QDialog(window);
    modalDialog->setWindowTitle("Modal dialog");
    QDialog* nestedModalDialog = new QDialog(modalDialog);
    nestedModalDialog->setWindowTitle("Nested modal dialog");

    QTimer timerAccept;
    timerAccept.setSingleShot(true);
    timerAccept.setInterval(2500);
    connect(&timerAccept, SIGNAL(timeout()), modalDialog, SLOT(accept()));

    QTimer timerExecNested;
    timerExecNested.setSingleShot(true);
    timerExecNested.setInterval(500);
    connect(&timerExecNested, SIGNAL(timeout()),
            nestedModalDialog, SLOT(exec()));

    QTimer timerAcceptNested;
    timerAcceptNested.setSingleShot(true);
    timerAcceptNested.setInterval(1500);
    connect(&timerAcceptNested, SIGNAL(timeout()),
            nestedModalDialog, SLOT(accept()));

    queueAssertParent(windowToKeepOnTop, nestedModalDialog, 1000);
    queueAssertIsVisibleWindow(windowToKeepOnTop, 1000);
    queueAssertParent(windowToKeepOnTop, modalDialog, 2000);
    queueAssertIsVisibleWindow(windowToKeepOnTop, 2000);

    timerAccept.start();
    timerExecNested.start();
    timerAcceptNested.start();
    modalDialog->exec();

    assertWindow(windowToKeepOnTop, window);

    delete window;

    QVERIFY(!windowToKeepOnTop);
}

void WindowOnTopEnforcerTest::testSeveralModalDialogs() {
    QWidget* window = new QWidget();
    window->setWindowTitle("Window");
    window->show();

    QPointer<QWidget> windowToKeepOnTop = new QWidget(window);
    windowToKeepOnTop->setWindowTitle("Window to keep on top");
    windowToKeepOnTop->setWindowFlags(Qt::Window);
    windowToKeepOnTop->show();

    WindowOnTopEnforcer* enforcer = new WindowOnTopEnforcer(windowToKeepOnTop);
    enforcer->setBaseWindow(window);

    QDialog* modalDialog = new QDialog(window);
    modalDialog->setWindowTitle("Modal dialog");
    QDialog* nestedModalDialog = new QDialog(modalDialog);
    nestedModalDialog->setWindowTitle("Nested modal dialog");
    QDialog* nestedModalDialog2 = new QDialog(modalDialog);
    nestedModalDialog2->setWindowTitle("Nested modal dialog 2");

    QTimer timerAccept;
    timerAccept.setSingleShot(true);
    timerAccept.setInterval(4500);
    connect(&timerAccept, SIGNAL(timeout()), modalDialog, SLOT(accept()));

    QTimer timerExecNested;
    timerExecNested.setSingleShot(true);
    timerExecNested.setInterval(500);
    connect(&timerExecNested, SIGNAL(timeout()),
            nestedModalDialog, SLOT(exec()));

    QTimer timerAcceptNested;
    timerAcceptNested.setSingleShot(true);
    timerAcceptNested.setInterval(1500);
    connect(&timerAcceptNested, SIGNAL(timeout()),
            nestedModalDialog, SLOT(accept()));

    QTimer timerExecNested2;
    timerExecNested2.setSingleShot(true);
    timerExecNested2.setInterval(2500);
    connect(&timerExecNested2, SIGNAL(timeout()),
            nestedModalDialog2, SLOT(exec()));

    QTimer timerAcceptNested2;
    timerAcceptNested2.setSingleShot(true);
    timerAcceptNested2.setInterval(3500);
    connect(&timerAcceptNested2, SIGNAL(timeout()),
            nestedModalDialog2, SLOT(accept()));

    queueAssertParent(windowToKeepOnTop, nestedModalDialog, 1000);
    queueAssertIsVisibleWindow(windowToKeepOnTop, 1000);
    queueAssertParent(windowToKeepOnTop, modalDialog, 2000);
    queueAssertIsVisibleWindow(windowToKeepOnTop, 2000);
    queueAssertParent(windowToKeepOnTop, nestedModalDialog2, 3000);
    queueAssertIsVisibleWindow(windowToKeepOnTop, 3000);
    queueAssertParent(windowToKeepOnTop, modalDialog, 4000);
    queueAssertIsVisibleWindow(windowToKeepOnTop, 4000);

    timerAccept.start();
    timerExecNested.start();
    timerAcceptNested.start();
    timerExecNested2.start();
    timerAcceptNested2.start();
    modalDialog->exec();

    assertWindow(windowToKeepOnTop, window);

    QDialog* modalDialog2 = new QDialog(window);
    modalDialog2->setWindowTitle("Modal dialog 2");
    QDialog* nestedModalDialog3 = new QDialog(modalDialog);
    nestedModalDialog3->setWindowTitle("Nested modal dialog 3");

    QTimer timerAccept2;
    timerAccept2.setSingleShot(true);
    timerAccept2.setInterval(2500);
    connect(&timerAccept2, SIGNAL(timeout()), modalDialog2, SLOT(accept()));

    QTimer timerExecNested3;
    timerExecNested3.setSingleShot(true);
    timerExecNested3.setInterval(500);
    connect(&timerExecNested3, SIGNAL(timeout()),
            nestedModalDialog3, SLOT(exec()));

    QTimer timerAcceptNested3;
    timerAcceptNested3.setSingleShot(true);
    timerAcceptNested3.setInterval(1500);
    connect(&timerAcceptNested3, SIGNAL(timeout()),
            nestedModalDialog3, SLOT(accept()));

    queueAssertParent(windowToKeepOnTop, nestedModalDialog3, 1000);
    queueAssertIsVisibleWindow(windowToKeepOnTop, 1000);
    queueAssertParent(windowToKeepOnTop, modalDialog2, 2000);
    queueAssertIsVisibleWindow(windowToKeepOnTop, 2000);

    timerAccept2.start();
    timerExecNested3.start();
    timerAcceptNested3.start();
    modalDialog2->exec();

    assertWindow(windowToKeepOnTop, window);

    delete window;

    QVERIFY(!windowToKeepOnTop);
}

void WindowOnTopEnforcerTest::testNestedModalDialogOnChildWindow() {
    QWidget* window = new QWidget();
    window->setWindowTitle("Window");
    window->show();

    QPointer<QWidget> windowToKeepOnTop = new QWidget(window);
    windowToKeepOnTop->setWindowTitle("Window to keep on top");
    windowToKeepOnTop->setWindowFlags(Qt::Window);
    windowToKeepOnTop->show();

    WindowOnTopEnforcer* enforcer = new WindowOnTopEnforcer(windowToKeepOnTop);
    enforcer->setBaseWindow(window);

    QWidget* childWindow = new QWidget(window);
    childWindow->setWindowTitle("Child window");
    childWindow->setWindowFlags(Qt::Window);
    childWindow->show();

    QDialog* nestedModalDialog = new QDialog(childWindow);
    nestedModalDialog->setWindowTitle("Nested modal dialog");

    QTimer timerAccept;
    timerAccept.setSingleShot(true);
    timerAccept.setInterval(1000);
    connect(&timerAccept, SIGNAL(timeout()), nestedModalDialog, SLOT(accept()));

    queueAssertParent(windowToKeepOnTop, nestedModalDialog, 500);
    queueAssertIsVisibleWindow(windowToKeepOnTop, 500);

    timerAccept.start();
    nestedModalDialog->exec();

    assertWindow(windowToKeepOnTop, window);

    delete window;

    QVERIFY(!windowToKeepOnTop);
}

void WindowOnTopEnforcerTest::testModalWidget() {
    QWidget* window = new QWidget();
    window->setWindowTitle("Window");
    window->show();

    QPointer<QWidget> windowToKeepOnTop = new QWidget(window);
    windowToKeepOnTop->setWindowTitle("Window to keep on top");
    windowToKeepOnTop->setWindowFlags(Qt::Window);
    windowToKeepOnTop->show();

    WindowOnTopEnforcer* enforcer = new WindowOnTopEnforcer(windowToKeepOnTop);
    enforcer->setBaseWindow(window);

    QWidget* modalWidget = new QWidget(window);
    modalWidget->setWindowTitle("Modal widget");
    modalWidget->setWindowFlags(Qt::Window);
    modalWidget->setWindowModality(Qt::ApplicationModal);
    modalWidget->show();

    assertWindow(windowToKeepOnTop, modalWidget);

    modalWidget->hide();

    assertWindow(windowToKeepOnTop, window);

    modalWidget->show();

    assertWindow(windowToKeepOnTop, modalWidget);

    delete modalWidget;

    assertWindow(windowToKeepOnTop, window);

    delete window;

    QVERIFY(!windowToKeepOnTop);
}

//This test may cause next test to fail (with a segmentation fault) when the
//modal widget in the next test is shown.
//The crash seems to be random, but has nothing to do (checked by isolating the
//code that crashes) with KTutorial classes.
//Under some unknown circumstances (I have yet to see the crash when GDB is
//recording the execution to analyze it properly :( ), "delete window" in
//testModalWidgetSiblingOfParent method makes qt_last_mouse_receiver (declared
//in qapplication_x11.cpp) to get the address of the window (or one of its child
//windows) when it is deleted.
//As the address is no longer valid, when that variable is used internally in
//the next modalWidget->show() ugly things happen.
//Hopefully I'll get enough information about this weird behavior to fill a bug.
void WindowOnTopEnforcerTest::testModalWidgetSiblingOfParent() {
    QWidget* window = new QWidget();
    window->setWindowTitle("Window");
    window->show();

    QPointer<QWidget> windowToKeepOnTop = new QWidget(window);
    windowToKeepOnTop->setWindowTitle("Window to keep on top");
    windowToKeepOnTop->setWindowFlags(Qt::Window);
    windowToKeepOnTop->show();

    WindowOnTopEnforcer* enforcer = new WindowOnTopEnforcer(windowToKeepOnTop);
    enforcer->setBaseWindow(window);

    QWidget* siblingModalWidget = new QWidget(window);
    siblingModalWidget->setWindowTitle("Sibling modal widget");
    siblingModalWidget->setWindowFlags(Qt::Window);
    siblingModalWidget->setWindowModality(Qt::ApplicationModal);
    siblingModalWidget->show();

    QWidget* modalWidget = new QWidget(window);
    modalWidget->setWindowTitle("Modal widget");
    modalWidget->setWindowFlags(Qt::Window);
    modalWidget->setWindowModality(Qt::ApplicationModal);
    modalWidget->show();

    assertWindow(windowToKeepOnTop, modalWidget);

    modalWidget->hide();

    assertWindow(windowToKeepOnTop, siblingModalWidget);

    modalWidget->show();

    assertWindow(windowToKeepOnTop, modalWidget);

    delete siblingModalWidget;

    assertWindow(windowToKeepOnTop, modalWidget);

    delete modalWidget;

    assertWindow(windowToKeepOnTop, window);

    delete window;

    QVERIFY(!windowToKeepOnTop);
}

//If a step contains a WaitForWindow and the step it changes to adds two
//WaitForWindows or more in its setup, the WindowOnTopEnforcer will get the Show
//event for the new window twice.
//This is a pretty strange and complex phenomenon that involves some Qt
//internals (http://bugreports.qt.nokia.com/browse/QTBUG-14651):
//Each object contains an internal list with its event filters. When a new
//filter is installed, it is prepended to the list. This list is traversed when
//the events for the object are being filtered; the first filter in the list is
//executed, then the second filter, then the third... and so on until there are
//no more filters in the list (or some filter stopped the event from being
//handled further).
//If a new filter is installed in an object while an event for that object is
//being filtered, the new filter will be prepended, the list will be modified,
//and the filter being executed will now take the next position in the list.
//When the next filter to be executed is fetch it will be the same filter that
//has just been executed.
//When the window that WaitForWindow is waiting for is shown, WaitForWindow will
//change to the next step in the tutorial. When this new step is set up, it adds
//two new WaitForWindows. The WaitForWindows will install a filter for each
//widget, which includes the window that has been shown. So, although it happens
//deep in the call stack, two event filters are installed while an event filter
//for that object is being executed.
//But, why has it to be two and not just one? And why does it affect
//WindowOnTopEnforcer if they are installed in a WaitForWindow?
//The reason is that the filter of the WaitForWindow that changes to the next
//step is the last filter executed, just after the filter of the
//WindowOnTopEnforcer. When the tutorial changes to the next step, it deletes
//the WaitForWindow of the deactivated step, thus removing its filter. Then it
//activates the next step, which causes the two WaitForWindow to be setup, and
//thus two filters are prepended to the filter list. So, as the filter after
//the WindowOnTopEnforcer filter is removed and two new ones are prepended, the
//WindowOnTopEnforcer filter ends one place after the one that was being
//executed. When the execution of that one ends, the end of the list was not
//reached yet, so the loop executes the next filter, and thus causes the
//WindowOnTopEnforcer filter to be executed again.
//And that's all. Strange? I know, I was the one who had to debug it :P
//This test is a synthesized version of all the things explained above.
void WindowOnTopEnforcerTest::testShowModalWidgetTwice() {
    QWidget* window = new QWidget();
    window->setWindowTitle("Window");
    window->show();

    QPointer<QWidget> windowToKeepOnTop = new QWidget(window);
    windowToKeepOnTop->setWindowTitle("Window to keep on top");
    windowToKeepOnTop->setWindowFlags(Qt::Window);
    windowToKeepOnTop->show();

    WindowOnTopEnforcer* enforcer = new WindowOnTopEnforcer(windowToKeepOnTop);
    enforcer->setBaseWindow(window);

    QWidget* modalWidget = new QWidget(window);
    modalWidget->setWindowTitle("Modal widget");
    modalWidget->setWindowFlags(Qt::Window);
    modalWidget->setWindowModality(Qt::ApplicationModal);
    modalWidget->show();

    QShowEvent showEvent;
    QCoreApplication::sendEvent(modalWidget, &showEvent);

    assertWindow(windowToKeepOnTop, modalWidget);

    modalWidget->hide();

    assertWindow(windowToKeepOnTop, window);

    delete window;

    QVERIFY(!windowToKeepOnTop);
}

void WindowOnTopEnforcerTest::testHideNestedModalWidgetsInOrder() {
    QWidget* window = new QWidget();
    window->setWindowTitle("Window");
    window->show();

    QPointer<QWidget> windowToKeepOnTop = new QWidget(window);
    windowToKeepOnTop->setWindowTitle("Window to keep on top");
    windowToKeepOnTop->setWindowFlags(Qt::Window);
    windowToKeepOnTop->show();

    WindowOnTopEnforcer* enforcer = new WindowOnTopEnforcer(windowToKeepOnTop);
    enforcer->setBaseWindow(window);

    QWidget* modalWidget = new QWidget(window);
    modalWidget->setWindowTitle("Modal widget");
    modalWidget->setWindowFlags(Qt::Window);
    modalWidget->setWindowModality(Qt::ApplicationModal);
    modalWidget->show();

    QWidget* nestedModalWidget = new QWidget(modalWidget);
    nestedModalWidget->setWindowTitle("Nested modal widget");
    nestedModalWidget->setWindowFlags(Qt::Window);
    nestedModalWidget->setWindowModality(Qt::ApplicationModal);
    nestedModalWidget->show();

    assertWindow(windowToKeepOnTop, nestedModalWidget);

    nestedModalWidget->hide();

    assertWindow(windowToKeepOnTop, modalWidget);

    modalWidget->hide();

    assertWindow(windowToKeepOnTop, window);

    window->hide();

    assertWindow(windowToKeepOnTop, window);

    delete window;

    QVERIFY(!windowToKeepOnTop);
}

void WindowOnTopEnforcerTest::testHideNestedModalWidgetsInReverseOrder() {
    QWidget* window = new QWidget();
    window->setWindowTitle("Window");
    window->show();

    QPointer<QWidget> windowToKeepOnTop = new QWidget(window);
    windowToKeepOnTop->setWindowTitle("Window to keep on top");
    windowToKeepOnTop->setWindowFlags(Qt::Window);
    windowToKeepOnTop->show();

    WindowOnTopEnforcer* enforcer = new WindowOnTopEnforcer(windowToKeepOnTop);
    enforcer->setBaseWindow(window);

    QWidget* modalWidget = new QWidget(window);
    modalWidget->setWindowTitle("Modal widget");
    modalWidget->setWindowFlags(Qt::Window);
    modalWidget->setWindowModality(Qt::ApplicationModal);
    modalWidget->show();

    QWidget* nestedModalWidget = new QWidget(modalWidget);
    nestedModalWidget->setWindowTitle("Nested modal widget");
    nestedModalWidget->setWindowFlags(Qt::Window);
    nestedModalWidget->setWindowModality(Qt::ApplicationModal);
    nestedModalWidget->show();

    assertWindow(windowToKeepOnTop, nestedModalWidget);

    window->hide();

    assertWindow(windowToKeepOnTop, nestedModalWidget);

    modalWidget->hide();

    assertWindow(windowToKeepOnTop, nestedModalWidget);

    nestedModalWidget->hide();

    assertWindow(windowToKeepOnTop, window);

    delete window;

    QVERIFY(!windowToKeepOnTop);
}

void WindowOnTopEnforcerTest::testShowNestedModalWidgetsInOrder() {
    QWidget* window = new QWidget();
    window->setWindowTitle("Window");
    window->show();

    QPointer<QWidget> windowToKeepOnTop = new QWidget(window);
    windowToKeepOnTop->setWindowTitle("Window to keep on top");
    windowToKeepOnTop->setWindowFlags(Qt::Window);
    windowToKeepOnTop->show();

    WindowOnTopEnforcer* enforcer = new WindowOnTopEnforcer(windowToKeepOnTop);
    enforcer->setBaseWindow(window);

    QWidget* modalWidget = new QWidget(window);
    modalWidget->setWindowTitle("Modal widget");
    modalWidget->setWindowFlags(Qt::Window);
    modalWidget->setWindowModality(Qt::ApplicationModal);
    modalWidget->show();

    QWidget* nestedModalWidget = new QWidget(modalWidget);
    nestedModalWidget->setWindowTitle("Nested modal widget");
    nestedModalWidget->setWindowFlags(Qt::Window);
    nestedModalWidget->setWindowModality(Qt::ApplicationModal);
    nestedModalWidget->show();

    assertWindow(windowToKeepOnTop, nestedModalWidget);

    nestedModalWidget->hide();
    modalWidget->hide();
    window->hide();

    window->show();

    assertWindow(windowToKeepOnTop, window);

    modalWidget->show();

    assertWindow(windowToKeepOnTop, modalWidget);

    nestedModalWidget->show();

    assertWindow(windowToKeepOnTop, nestedModalWidget);

    delete nestedModalWidget;

    assertWindow(windowToKeepOnTop, modalWidget);

    delete modalWidget;

    assertWindow(windowToKeepOnTop, window);

    delete window;

    QVERIFY(!windowToKeepOnTop);
}

void WindowOnTopEnforcerTest::testShowNestedModalWidgetsInReverseOrder() {
    QWidget* window = new QWidget();
    window->setWindowTitle("Window");
    window->show();

    QPointer<QWidget> windowToKeepOnTop = new QWidget(window);
    windowToKeepOnTop->setWindowTitle("Window to keep on top");
    windowToKeepOnTop->setWindowFlags(Qt::Window);
    windowToKeepOnTop->show();

    WindowOnTopEnforcer* enforcer = new WindowOnTopEnforcer(windowToKeepOnTop);
    enforcer->setBaseWindow(window);

    QWidget* modalWidget = new QWidget(window);
    modalWidget->setWindowTitle("Modal widget");
    modalWidget->setWindowFlags(Qt::Window);
    modalWidget->setWindowModality(Qt::ApplicationModal);
    modalWidget->show();

    QWidget* nestedModalWidget = new QWidget(modalWidget);
    nestedModalWidget->setWindowTitle("Nested modal widget");
    nestedModalWidget->setWindowFlags(Qt::Window);
    nestedModalWidget->setWindowModality(Qt::ApplicationModal);
    nestedModalWidget->show();

    assertWindow(windowToKeepOnTop, nestedModalWidget);

    window->hide();
    modalWidget->hide();
    nestedModalWidget->hide();

    nestedModalWidget->show();

    assertWindow(windowToKeepOnTop, nestedModalWidget);

    modalWidget->show();

    assertWindow(windowToKeepOnTop, nestedModalWidget);

    window->show();

    assertWindow(windowToKeepOnTop, nestedModalWidget);

    delete nestedModalWidget;

    assertWindow(windowToKeepOnTop, modalWidget);

    delete modalWidget;

    assertWindow(windowToKeepOnTop, window);

    delete window;

    QVERIFY(!windowToKeepOnTop);
}

void WindowOnTopEnforcerTest::testShowNestedModalWidgetsInMixedOrder() {
    QWidget* window = new QWidget();
    window->setWindowTitle("Window");
    window->show();

    QPointer<QWidget> windowToKeepOnTop = new QWidget(window);
    windowToKeepOnTop->setWindowTitle("Window to keep on top");
    windowToKeepOnTop->setWindowFlags(Qt::Window);
    windowToKeepOnTop->show();

    WindowOnTopEnforcer* enforcer = new WindowOnTopEnforcer(windowToKeepOnTop);
    enforcer->setBaseWindow(window);

    QWidget* modalWidget = new QWidget(window);
    modalWidget->setWindowTitle("Modal widget");
    modalWidget->setWindowFlags(Qt::Window);
    modalWidget->setWindowModality(Qt::ApplicationModal);
    modalWidget->show();

    QWidget* nestedModalWidget = new QWidget(modalWidget);
    nestedModalWidget->setWindowTitle("Nested modal widget");
    nestedModalWidget->setWindowFlags(Qt::Window);
    nestedModalWidget->setWindowModality(Qt::ApplicationModal);
    nestedModalWidget->show();

    QWidget* nestedNestedModalWidget = new QWidget(nestedModalWidget);
    nestedNestedModalWidget->setWindowTitle("Nested nested modal widget");
    nestedNestedModalWidget->setWindowFlags(Qt::Window);
    nestedNestedModalWidget->setWindowModality(Qt::ApplicationModal);
    nestedNestedModalWidget->show();

    QWidget* nestedNestedNestedModalWidget =
                                        new QWidget(nestedNestedModalWidget);
    nestedNestedNestedModalWidget->setWindowTitle("Nested nested nested modal widget");
    nestedNestedNestedModalWidget->setWindowFlags(Qt::Window);
    nestedNestedNestedModalWidget->setWindowModality(Qt::ApplicationModal);
    nestedNestedNestedModalWidget->show();

    assertWindow(windowToKeepOnTop, nestedNestedNestedModalWidget);

    window->hide();
    modalWidget->hide();
    nestedModalWidget->hide();
    nestedNestedModalWidget->hide();
    nestedNestedNestedModalWidget->hide();

    nestedNestedModalWidget->show();

    assertWindow(windowToKeepOnTop, nestedNestedModalWidget);

    modalWidget->show();

    assertWindow(windowToKeepOnTop, nestedNestedModalWidget);

    nestedNestedNestedModalWidget->show();

    assertWindow(windowToKeepOnTop, nestedNestedNestedModalWidget);

    window->show();

    assertWindow(windowToKeepOnTop, nestedNestedNestedModalWidget);

    nestedModalWidget->show();

    assertWindow(windowToKeepOnTop, nestedNestedNestedModalWidget);

    delete nestedNestedNestedModalWidget;

    assertWindow(windowToKeepOnTop, nestedNestedModalWidget);

    delete nestedNestedModalWidget;

    assertWindow(windowToKeepOnTop, nestedModalWidget);

    delete nestedModalWidget;

    assertWindow(windowToKeepOnTop, modalWidget);

    delete modalWidget;

    assertWindow(windowToKeepOnTop, window);

    delete window;

    QVERIFY(!windowToKeepOnTop);
}

//There is a limitation in the current behavior of WindowOnTopEnforcer which can
//show up as a bug when testing a tutorial from the editor. The limitation is
//that, to work properly, the base window must be set in the WindowOnTopEnforcer
//when there is only one window. If there are modal widgets or modal dialogs the
//reparenting does not work as it should, and the widget to keep on top of the
//others is only kept on top of the main window, but below the modal widgets or
//modal dialogs. This can happen, for example, if a tutorial is tested in the
//edtior when the target application is showing a modal dialog.
//The reason is that the reparenting system works using a stack that stores
//the order in which modal widgets or modal dialogs were opened, so it can
//reparent them back to the proper one when they are hidden. That stack is built
//once the base window was set by spying the show and hidden events of its child
//windows. However, I have not found any way to initialize it with the proper
//order of windows if they are already visible when the base window is set.
//The modal stack is stored by Qt in the variable QWidgetList* qt_modal_stack
//(declared in qapplication.cpp). Alas, this variable seems to be not exported,
//so it can not be used to initialize the WindowOnTopEnforcer stack.
//I have tried another approach spying WindowBlocked and WindowUnblocked events
//and not using a stack at all, but I was not able to come with anything that
//worked in all the cases.
//So... I just keep this test here as a reference of something that should be
//fixed, but that unfortunately I do not know how to do it :(
void WindowOnTopEnforcerTest::
                        testSetBaseWindowAfterSeveralModalWidgetsWereShown() {
    QSKIP("Pending bug: WindowOnTopEnforcer does not work properly if there \
are modal widgets already shown when the base window is set. However, I have \
not found a fix for it yet :(", SkipAll);

    QWidget* window = new QWidget();
    window->setWindowTitle("Window");
    window->show();

    QPointer<QWidget> windowToKeepOnTop = new QWidget(window);
    windowToKeepOnTop->setWindowTitle("Window to keep on top");
    windowToKeepOnTop->setWindowFlags(Qt::Dialog);
    windowToKeepOnTop->show();

    QWidget* siblingModalWidget = new QWidget(window);
    siblingModalWidget->setWindowTitle("Sibling modal widget");
    siblingModalWidget->setWindowFlags(Qt::Window);
    siblingModalWidget->setWindowModality(Qt::ApplicationModal);
    siblingModalWidget->show();

    QWidget* modalWidget = new QWidget(window);
    modalWidget->setWindowTitle("Modal widget");
    modalWidget->setWindowFlags(Qt::Window);
    modalWidget->setWindowModality(Qt::ApplicationModal);
    modalWidget->show();

    QWidget* nestedModalWidget = new QWidget(window);
    nestedModalWidget->setWindowTitle("Nested modal widget");
    nestedModalWidget->setWindowFlags(Qt::Window);
    nestedModalWidget->setWindowModality(Qt::ApplicationModal);
    nestedModalWidget->show();

    QWidget* nestedSiblingModalWidget = new QWidget(siblingModalWidget);
    nestedSiblingModalWidget->setWindowTitle("Nested sibling modal widget");
    nestedSiblingModalWidget->setWindowFlags(Qt::Window);
    nestedSiblingModalWidget->setWindowModality(Qt::ApplicationModal);
    nestedSiblingModalWidget->show();

    WindowOnTopEnforcer* enforcer = new WindowOnTopEnforcer(windowToKeepOnTop);
    enforcer->setBaseWindow(window);
    
    assertWindow(windowToKeepOnTop, nestedSiblingModalWidget);

    nestedSiblingModalWidget->hide();

    assertWindow(windowToKeepOnTop, nestedModalWidget);

    nestedSiblingModalWidget->show();

    assertWindow(windowToKeepOnTop, nestedSiblingModalWidget);

    delete nestedSiblingModalWidget;

    assertWindow(windowToKeepOnTop, nestedModalWidget);

    delete nestedModalWidget;

    assertWindow(windowToKeepOnTop, modalWidget);

    delete modalWidget;

    assertWindow(windowToKeepOnTop, siblingModalWidget);

    delete siblingModalWidget;

    assertWindow(windowToKeepOnTop, window);

    delete window;

    QVERIFY(!windowToKeepOnTop);
}

/////////////////////////////////// Helpers ////////////////////////////////////

//The dialogs are modal, so they won't return to the test code until they are
//closed. Thus, the asserts to be performed while the dialogs are being shown
//(like checking the parent of a widget) must be "queued".
class QueuedActionsHelper: public QObject {
Q_OBJECT
public:

    QueuedActionsHelper(QObject* parent = 0): QObject(parent) {
    }

    void setAssertWidget(QWidget* widget) {
        mAssertWidget = widget;
    }

    void setAssertParent(QWidget* parent) {
        mAssertParent = parent;
    }

public slots:

    void assertParent() {
        QString message = "\nActual: " + 
                            mAssertWidget->parentWidget()->windowTitle() +
                            "\nExpected: " + mAssertParent->windowTitle() +
                            "\n"; 
        QVERIFY2(mAssertWidget->parentWidget() == mAssertParent,
                 message.toStdString().c_str());
    }

    void assertIsVisibleWindow() {
        QVERIFY(mAssertWidget->isVisible());
        QVERIFY(mAssertWidget->windowFlags() & Qt::Window);
    }

private:

    QWidget* mAssertWidget;
    QWidget* mAssertParent;

};

void WindowOnTopEnforcerTest::queueAssertParent(QWidget* widget,
                                                QWidget* parent,
                                                int timeToWait) {
    QueuedActionsHelper* helper = new QueuedActionsHelper(this);
    helper->setAssertWidget(widget);
    helper->setAssertParent(parent);
    QTimer::singleShot(timeToWait, helper, SLOT(assertParent()));
}

void WindowOnTopEnforcerTest::queueAssertIsVisibleWindow(QWidget* widget,
                                                         int timeToWait) {
    QueuedActionsHelper* helper = new QueuedActionsHelper(this);
    helper->setAssertWidget(widget);
    QTimer::singleShot(timeToWait, helper, SLOT(assertIsVisibleWindow()));
}

void WindowOnTopEnforcerTest::assertWindow(QWidget* window,
                                           QWidget* expectedParent) const {
    QVERIFY(window);
    QString message = "\nActual: " + window->parentWidget()->windowTitle() +
                      "\nExpected: " + expectedParent->windowTitle() + '\n';
    QVERIFY2(window->parentWidget() == expectedParent,
             message.toStdString().c_str());
    QVERIFY(window->isVisible());
    QVERIFY(window->windowFlags() & Qt::Window);
}

}
}

QTEST_MAIN(ktutorial::view::WindowOnTopEnforcerTest)

#include "WidgetOnTopEnforcerTest.moc"
