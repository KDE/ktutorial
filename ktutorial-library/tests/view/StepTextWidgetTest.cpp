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
#include <qtest_kde.h>

#include <QAction>

#include <KXmlGuiWindow>

#include "StepTextWidget.h"
#define protected public
#define private public
#include "../KTutorial.h"
#undef private
#undef protected
#include "../extendedinformation/WidgetHighlighter.h"

using ktutorial::extendedinformation::WidgetHighlighter;

namespace ktutorial {
namespace view {

class StepTextWidgetTest: public QObject {
Q_OBJECT
private slots:

    void init();

    void testConstructor();

    void testSizeHintWithBiggerText();
    void testSizeHintWithSmallerText();

    void testHighlightWidgetClickingOnAnchor();
    void testStopHighlightingWidgetClickingOnAnchor();
    void testHighlightWidgetUsingContextMenu();
    void testStopHighlightingWidgetUsingContextMenu();
    void testCancelContextMenu();
    void testCancelContextMenuOnOtherLink();
    void testHighlightSeveralWidgets();
    void testHighlightUnknownWidget();

    void testStopHighlightingWidgetWhenFocused();

    void testSetTextWhenWidgetIsBeingHighlighted();
    void testDeleteWhenWidgetIsBeingHighlighted();
    void testDeleteWhenHighlightedWidgetWasDestroyed();

private:

    QPoint centerOfText(const StepTextWidget& widget, const QString& text);

    bool waitForHighlighterToStop(const QWidget* widget, int timeout) const;

    void showContextMenuAndSelectFirstOption(const StepTextWidget& widget,
                                             const QPoint& position);

    void showContextMenuCheckFirstOptionAndCancel(const StepTextWidget& widget,
                                                  const QPoint& position,
                                                  const QString& text);

};

void StepTextWidgetTest::init() {
    delete KTutorial::sSelf;
    KTutorial::sSelf = new KTutorial();
}

void StepTextWidgetTest::testConstructor() {
    QWidget parent;
    StepTextWidget* widget = new StepTextWidget(&parent);

    QCOMPARE(widget->parentWidget(), &parent);
    QVERIFY(widget->isReadOnly());
    QCOMPARE(widget->frameShape(), QFrame::NoFrame);
    QCOMPARE(widget->frameShadow(), QFrame::Plain);
    QCOMPARE(widget->horizontalScrollBarPolicy(), Qt::ScrollBarAlwaysOff);
    QCOMPARE(widget->verticalScrollBarPolicy(), Qt::ScrollBarAlwaysOff);
    QCOMPARE(widget->palette().color(QPalette::Base), QColor(Qt::transparent));
}

void StepTextWidgetTest::testSizeHintWithBiggerText() {
    StepTextWidget widget;
    widget.setText("Some short text");

    QSize oldSizeHint = widget.sizeHint();

    widget.setText("Some bigger text to be shown hopefully in several lines");

    QSize newSizeHint = widget.sizeHint();
    QCOMPARE(widget.heightForWidth(newSizeHint.width()), newSizeHint.height());
    QVERIFY(newSizeHint.width() > newSizeHint.height());
    QVERIFY(newSizeHint.height() > widget.minimumSizeHint().height());
    QVERIFY(newSizeHint.width() > widget.minimumSizeHint().width());
    QVERIFY(newSizeHint.height() > oldSizeHint.height());
    QVERIFY(newSizeHint.width() > oldSizeHint.width());
}

void StepTextWidgetTest::testSizeHintWithSmallerText() {
    StepTextWidget widget;
    widget.setText("Some bigger text to be shown hopefully in several lines");

    QSize oldSizeHint = widget.sizeHint();

    widget.setText("Some short text");

    QSize newSizeHint = widget.sizeHint();
    QCOMPARE(widget.heightForWidth(newSizeHint.width()), newSizeHint.height());
    QVERIFY(newSizeHint.width() > newSizeHint.height());
    QVERIFY(newSizeHint.height() < oldSizeHint.height());
    QVERIFY(newSizeHint.width() < oldSizeHint.width());
}

void StepTextWidgetTest::testHighlightWidgetClickingOnAnchor() {
    KXmlGuiWindow mainWindow;
    KTutorial::self()->setup(&mainWindow);
    QWidget* widgetToHighlight = new QWidget(&mainWindow);
    widgetToHighlight->setObjectName("widgetName");

    StepTextWidget widget;
    widget.setText("The <a href=\"widget:widgetName\">widget to highlight</a>");
    widget.show();

    QTest::qWaitForWindowShown(&widget);

    QPoint position = centerOfText(widget, "widget to highlight");
    QTest::mouseClick(widget.viewport(), Qt::LeftButton, Qt::NoModifier,
                      position);

    QVERIFY(widgetToHighlight->findChild<WidgetHighlighter*>(""));
}

void StepTextWidgetTest::testStopHighlightingWidgetClickingOnAnchor() {
    KXmlGuiWindow mainWindow;
    KTutorial::self()->setup(&mainWindow);
    QWidget* widgetToHighlight = new QWidget(&mainWindow);
    widgetToHighlight->setObjectName("widgetName");

    StepTextWidget widget;
    widget.setText("The <a href=\"widget:widgetName\">widget to highlight</a>");
    widget.show();

    QTest::qWaitForWindowShown(&widget);

    QPoint position = centerOfText(widget, "widget to highlight");
    QTest::mouseClick(widget.viewport(), Qt::LeftButton, Qt::NoModifier,
                      position);
    QTest::mouseClick(widget.viewport(), Qt::LeftButton, Qt::NoModifier,
                      position, 500);

    QVERIFY(waitForHighlighterToStop(widgetToHighlight, 10000));
}

void StepTextWidgetTest::testHighlightWidgetUsingContextMenu() {
    KXmlGuiWindow mainWindow;
    KTutorial::self()->setup(&mainWindow);
    QWidget* widgetToHighlight = new QWidget(&mainWindow);
    widgetToHighlight->setObjectName("widgetName");

    StepTextWidget widget;
    widget.setText("The <a href=\"widget:widgetName\">widget to highlight</a>");
    widget.show();

    QTest::qWaitForWindowShown(&widget);

    QPoint position = centerOfText(widget, "widget to highlight");
    showContextMenuAndSelectFirstOption(widget, position);

    QVERIFY(widgetToHighlight->findChild<WidgetHighlighter*>(""));
}

void StepTextWidgetTest::testStopHighlightingWidgetUsingContextMenu() {
    KXmlGuiWindow mainWindow;
    KTutorial::self()->setup(&mainWindow);
    QWidget* widgetToHighlight = new QWidget(&mainWindow);
    widgetToHighlight->setObjectName("widgetName");

    StepTextWidget widget;
    widget.setText("The <a href=\"widget:widgetName\">widget to highlight</a>");
    widget.show();

    QTest::qWaitForWindowShown(&widget);

    QPoint position = centerOfText(widget, "widget to highlight");
    showContextMenuAndSelectFirstOption(widget, position);
    //Give the highlighter time to start
    QTest::qWait(500);
    showContextMenuAndSelectFirstOption(widget, position);

    QVERIFY(waitForHighlighterToStop(widgetToHighlight, 10000));
}

void StepTextWidgetTest::testCancelContextMenu() {
    KXmlGuiWindow mainWindow;
    KTutorial::self()->setup(&mainWindow);
    QWidget* widgetToHighlight = new QWidget(&mainWindow);
    widgetToHighlight->setObjectName("widgetName");

    StepTextWidget widget;
    widget.setText("The <a href=\"widget:widgetName\">widget to highlight</a>");
    widget.show();

    QTest::qWaitForWindowShown(&widget);

    QPoint position = centerOfText(widget, "widget to highlight");
    showContextMenuCheckFirstOptionAndCancel(widget, position,
                                             i18n("Highlight"));

    //Ensure that the option text is the right one after cancelling the menu
    showContextMenuCheckFirstOptionAndCancel(widget, position,
                                             i18n("Highlight"));

    //Check again when the widget is highlighted
    showContextMenuAndSelectFirstOption(widget, position);

    QVERIFY(widgetToHighlight->findChild<WidgetHighlighter*>(""));

    showContextMenuCheckFirstOptionAndCancel(widget, position,
                                             i18n("Stop highlighting"));

    //Ensure that just showing the context menu does not stop the highlighting
    QVERIFY(!waitForHighlighterToStop(widgetToHighlight, 1000));

    //Ensure that the option text is the right one after cancelling the menu
    showContextMenuCheckFirstOptionAndCancel(widget, position,
                                             i18n("Stop highlighting"));
}

void StepTextWidgetTest::testCancelContextMenuOnOtherLink() {
    KXmlGuiWindow mainWindow;
    KTutorial::self()->setup(&mainWindow);
    QWidget* widgetToHighlight1 = new QWidget(&mainWindow);
    widgetToHighlight1->setObjectName("widget1");
    QWidget* widgetToHighlight2 = new QWidget(&mainWindow);
    widgetToHighlight2->setObjectName("widget2");

    StepTextWidget widget;
    widget.setText("The <a href=\"widget:widget1\">first widget</a> and the "
"<a href=\"widget:widget2\">second widget</a>");
    widget.show();

    QTest::qWaitForWindowShown(&widget);

    QPoint position1 = centerOfText(widget, "first widget");
    QPoint position2 = centerOfText(widget, "second widget");
    
    showContextMenuAndSelectFirstOption(widget, position1);

    showContextMenuCheckFirstOptionAndCancel(widget, position2,
                                             i18n("Highlight"));

    //Ensure that just showing the context menu on the second link does not stop
    //the highlighting of the first widget
    QVERIFY(!waitForHighlighterToStop(widgetToHighlight1, 1000));

    //Ensure that the option text is the right one after cancelling the menu
    showContextMenuCheckFirstOptionAndCancel(widget, position2,
                                             i18n("Highlight"));
}

void StepTextWidgetTest::testHighlightSeveralWidgets() {
    KXmlGuiWindow mainWindow;
    KTutorial::self()->setup(&mainWindow);
    QWidget* widgetToHighlight1 = new QWidget(&mainWindow);
    widgetToHighlight1->setObjectName("widget1");
    QWidget* widgetToHighlight2 = new QWidget(&mainWindow);
    widgetToHighlight2->setObjectName("widget2");
    QWidget* widgetToHighlight3 = new QWidget(&mainWindow);
    widgetToHighlight3->setObjectName("widget3");

    StepTextWidget widget;
    widget.setText("The <a href=\"widget:widget1\">first widget</a>, "
"<a href=\"widget:widget2\">second widget</a> and "
"<a href=\"widget:widget3\">third widget</a>");
    widget.show();

    QTest::qWaitForWindowShown(&widget);

    QPoint position1 = centerOfText(widget, "first widget");
    QPoint position2 = centerOfText(widget, "second widget");
    QPoint position3 = centerOfText(widget, "third widget");

    QTest::mouseClick(widget.viewport(), Qt::LeftButton, Qt::NoModifier,
                      position1);
    QTest::mouseClick(widget.viewport(), Qt::LeftButton, Qt::NoModifier,
                      position2, 500);

    QVERIFY(waitForHighlighterToStop(widgetToHighlight1, 10000));

    QVERIFY(!widgetToHighlight1->findChild<WidgetHighlighter*>(""));
    QVERIFY(widgetToHighlight2->findChild<WidgetHighlighter*>(""));
    QVERIFY(!widgetToHighlight3->findChild<WidgetHighlighter*>(""));

    showContextMenuAndSelectFirstOption(widget, position3);

    QVERIFY(waitForHighlighterToStop(widgetToHighlight2, 10000));

    QVERIFY(!widgetToHighlight1->findChild<WidgetHighlighter*>(""));
    QVERIFY(!widgetToHighlight2->findChild<WidgetHighlighter*>(""));
    QVERIFY(widgetToHighlight3->findChild<WidgetHighlighter*>(""));

    showContextMenuAndSelectFirstOption(widget, position1);

    QVERIFY(waitForHighlighterToStop(widgetToHighlight3, 10000));

    QVERIFY(widgetToHighlight1->findChild<WidgetHighlighter*>(""));
    QVERIFY(!widgetToHighlight2->findChild<WidgetHighlighter*>(""));
    QVERIFY(!widgetToHighlight3->findChild<WidgetHighlighter*>(""));

    QTest::mouseClick(widget.viewport(), Qt::LeftButton, Qt::NoModifier,
                      position1, 500);

    QVERIFY(waitForHighlighterToStop(widgetToHighlight1, 10000));

    QVERIFY(!widgetToHighlight1->findChild<WidgetHighlighter*>(""));
    QVERIFY(!widgetToHighlight2->findChild<WidgetHighlighter*>(""));
    QVERIFY(!widgetToHighlight3->findChild<WidgetHighlighter*>(""));

    QTest::mouseClick(widget.viewport(), Qt::LeftButton, Qt::NoModifier,
                      position2, 500);
    //Give the highlighter time to start
    QTest::qWait(500);
    showContextMenuAndSelectFirstOption(widget, position2);

    QVERIFY(waitForHighlighterToStop(widgetToHighlight2, 10000));

    QVERIFY(!widgetToHighlight1->findChild<WidgetHighlighter*>(""));
    QVERIFY(!widgetToHighlight2->findChild<WidgetHighlighter*>(""));
    QVERIFY(!widgetToHighlight3->findChild<WidgetHighlighter*>(""));
}

void StepTextWidgetTest::testHighlightUnknownWidget() {
    KXmlGuiWindow mainWindow;
    KTutorial::self()->setup(&mainWindow);

    StepTextWidget widget;
    widget.setText("The <a href=\"widget:widgetName\">widget to highlight</a>");
    widget.show();

    QTest::qWaitForWindowShown(&widget);

    QPoint position = centerOfText(widget, "widget to highlight");
    QTest::mouseClick(widget.viewport(), Qt::LeftButton, Qt::NoModifier,
                      position);
    QTest::mouseClick(widget.viewport(), Qt::LeftButton, Qt::NoModifier,
                      position, 500);

    //No explicit check is made, if it does not crash everything is fine ;)
}

void StepTextWidgetTest::testStopHighlightingWidgetWhenFocused() {
    KXmlGuiWindow mainWindow;
    KTutorial::self()->setup(&mainWindow);
    QWidget* widgetToHighlight = new QWidget(&mainWindow);
    widgetToHighlight->setObjectName("widgetName");
    mainWindow.setCentralWidget(widgetToHighlight);
    mainWindow.show();

    StepTextWidget widget;
    widget.setText("The <a href=\"widget:widgetName\">widget to highlight</a>");
    widget.show();

    QTest::qWaitForWindowShown(&widget);

    QPoint position = centerOfText(widget, "widget to highlight");
    QTest::mouseClick(widget.viewport(), Qt::LeftButton, Qt::NoModifier,
                      position);

    //To get the focus, the parent window of the widget must be active
    mainWindow.activateWindow();
    widgetToHighlight->setFocus();

    QVERIFY(waitForHighlighterToStop(widgetToHighlight, 10000));
}

void StepTextWidgetTest::testSetTextWhenWidgetIsBeingHighlighted() {
    KXmlGuiWindow mainWindow;
    KTutorial::self()->setup(&mainWindow);
    QWidget* widgetToHighlight = new QWidget(&mainWindow);
    widgetToHighlight->setObjectName("widgetName");

    StepTextWidget widget;
    widget.setText("The <a href=\"widget:widgetName\">widget to highlight</a>");
    widget.show();

    QTest::qWaitForWindowShown(&widget);

    QPoint position = centerOfText(widget, "widget to highlight");
    QTest::mouseClick(widget.viewport(), Qt::LeftButton, Qt::NoModifier,
                      position);

    widget.setText("Another text");

    QVERIFY(!widgetToHighlight->findChild<WidgetHighlighter*>(""));
}

void StepTextWidgetTest::testDeleteWhenWidgetIsBeingHighlighted() {
    KXmlGuiWindow mainWindow;
    KTutorial::self()->setup(&mainWindow);
    QWidget* widgetToHighlight = new QWidget(&mainWindow);
    widgetToHighlight->setObjectName("widgetName");

    StepTextWidget* widget = new StepTextWidget();
    widget->setText("The <a href=\"widget:widgetName\">widget to highlight</a>");
    widget->show();

    QTest::qWaitForWindowShown(widget);

    QPoint position = centerOfText(*widget, "widget to highlight");
    QTest::mouseClick(widget->viewport(), Qt::LeftButton, Qt::NoModifier,
                      position);

    delete widget;

    QVERIFY(!widgetToHighlight->findChild<WidgetHighlighter*>(""));
}

void StepTextWidgetTest::testDeleteWhenHighlightedWidgetWasDestroyed() {
    KXmlGuiWindow mainWindow;
    KTutorial::self()->setup(&mainWindow);
    QWidget* widgetToHighlight = new QWidget(&mainWindow);
    widgetToHighlight->setObjectName("widgetName");

    StepTextWidget* widget = new StepTextWidget();
    widget->setText("The <a href=\"widget:widgetName\">widget to highlight</a>");
    widget->show();

    QTest::qWaitForWindowShown(widget);

    QPoint position = centerOfText(*widget, "widget to highlight");
    QTest::mouseClick(widget->viewport(), Qt::LeftButton, Qt::NoModifier,
                      position);

    delete widgetToHighlight;

    delete widget;

    //No explicit check is made, if it does not crash everything is fine ;)
}

/////////////////////////////////// Helpers ////////////////////////////////////

QPoint StepTextWidgetTest::centerOfText(const StepTextWidget& widget,
                                        const QString& text) {
    QTextCursor cursor = widget.document()->find(text);

    //The cursor rect doesn't include the selection, just a tiny rectangle for
    //the cursor position. To ensure that the menu is shown on the anchor, set
    //the cursor in the middle of the selection
    cursor.setPosition((cursor.selectionStart() + cursor.selectionEnd()) / 2);
    return widget.cursorRect(cursor).center();
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

class HighlighterStoppedCondition {
public:
    const QWidget* mWidget;

    bool condition() const {
        return !mWidget->findChild<WidgetHighlighter*>("");
    }
};

bool StepTextWidgetTest::waitForHighlighterToStop(const QWidget* widget,
                                                  int timeout) const {
    HighlighterStoppedCondition helper;
    helper.mWidget = widget;
    return waitFor(helper, &HighlighterStoppedCondition::condition, timeout);
}

//The context menu contains its own event loop, so it won't return to the test
//code until it is closed. Thus, the commands to execute on the menu must be
//"queued", as calling QTest::keyClick after showing the menu won't work.
class QueuedActionsHelper: public QObject {
Q_OBJECT
public:

    QueuedActionsHelper(QObject* parent = 0): QObject(parent) {
        setObjectName("helper");
    }

    void setExpectedMenuItemName(const QString& menuItemName) {
        mMenuItemName = menuItemName;
    }

public slots:

    void selectFirstContextMenuOption() {
        QVERIFY(QApplication::activePopupWidget());
        QTest::keyClick(QApplication::activePopupWidget(), Qt::Key_Down);
        QTest::keyClick(QApplication::activePopupWidget(), Qt::Key_Enter);
    }

    void assertFirstContextMenuOptionName() {
        QVERIFY(QApplication::activePopupWidget());
        QWidget* menu = QApplication::activePopupWidget();
        QCOMPARE(menu->actions()[0]->text(), mMenuItemName);
    }

    void cancelContextMenu() {
        QVERIFY(QApplication::activePopupWidget());
        QTest::keyClick(QApplication::activePopupWidget(), Qt::Key_Escape);
    }

private:

    QString mMenuItemName;

};

void StepTextWidgetTest::showContextMenuAndSelectFirstOption(
                        const StepTextWidget& widget, const QPoint& position) {
    //The context menu can't be triggered sending a right mouse button press
    //event, as that is platform dependent (that event is not handled by
    //QTextEdit or its parents, but by the QApplication for the platform that
    //creates a context menu event when needed). A explicit QContextMenuEvent
    //must be sent for it to work.
    QContextMenuEvent event(QContextMenuEvent::Mouse, position,
                            widget.mapToGlobal(position));

    QueuedActionsHelper* helper = new QueuedActionsHelper(this);
    QTimer::singleShot(500, helper, SLOT(selectFirstContextMenuOption()));

    QApplication::sendEvent(widget.viewport(), &event);
}

void StepTextWidgetTest::showContextMenuCheckFirstOptionAndCancel(
                                                const StepTextWidget& widget,
                                                const QPoint& position,
                                                const QString& text) {
    //The context menu can't be triggered sending a right mouse button press
    //event, as that is platform dependent (that event is not handled by
    //QTextEdit or its parents, but by the QApplication for the platform that
    //creates a context menu event when needed). An explicit QContextMenuEvent
    //must be sent for it to work.
    QContextMenuEvent event(QContextMenuEvent::Mouse, position,
                            widget.mapToGlobal(position));

    QueuedActionsHelper* helper = new QueuedActionsHelper(this);
    helper->setExpectedMenuItemName(text);
    QTimer::singleShot(500, helper, SLOT(assertFirstContextMenuOptionName()));
    QTimer::singleShot(600, helper, SLOT(cancelContextMenu()));

    QApplication::sendEvent(widget.viewport(), &event);
}

}
}

QTEST_KDEMAIN(ktutorial::view::StepTextWidgetTest, GUI)

#include "StepTextWidgetTest.moc"
