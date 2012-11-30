/***************************************************************************
 *   Copyright (C) 2009 by Daniel Calviño Sánchez <danxuliu@gmail.com>     *
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
#include <QWhatsThis>
#include <qtest_kde.h>

#define protected public
#define private public
#include "StepWidget.h"
#undef private
#undef protected

#include "ui_StepWidget.h"
#include "StepTextWidget.h"
#include "../Option.h"
#include "../Step.h"

namespace ktutorial {
namespace view {

class StepWidgetTest: public QObject {
Q_OBJECT

public slots:

    void dummySlot() {
        mDummySlotCallCount++;
    }

    void anotherDummySlot() {
        mAnotherDummySlotCallCount++;
    }

private slots:

    void init() {
        mDummySlotCallCount = 0;
        mAnotherDummySlotCallCount = 0;
    }

    void testConstructor();

    void testSetStep();
    void testSetStepSeveralSteps();
    void testSetStepHidden();

    void selectOption();

    void moveDragging();
    void moveDraggingAfterRelease();

    void enterWhatIsThisMode();

    void close();
    void closeUsingEscKey();
    //Closing with ALT+F4 can't be tested, as it depends on the window manager 
    //rather than the widget

    void moveAfterShowingModalDialogAndThenClose();

private:

    int mDummySlotCallCount;
    int mAnotherDummySlotCallCount;

    StepTextWidget* textWidget(StepWidget* stepWidget);
    KPushButton* closeButton(StepWidget* stepWidget);

    bool waitForWidgetToMove(const QWidget* widget,
                             const QPoint& previousPosition, int timeout) const;

    void queueAssertWidgetDragged(StepWidget* stepWidget, int timeToWait);

};

void StepWidgetTest::testConstructor() {
    StepWidget stepWidget("Test tutorial");

    QCOMPARE(stepWidget.windowTitle(), 
             i18n("Tutorial: %1", QString("Test tutorial")));
    QCOMPARE(stepWidget.windowFlags(), Qt::Dialog | Qt::FramelessWindowHint);
    QVERIFY(closeButton(&stepWidget));
    QVERIFY(textWidget(&stepWidget));
    QCOMPARE(textWidget(&stepWidget)->toPlainText(), QString(""));
    QVERIFY(stepWidget.mOptionsLayout);
    QCOMPARE(stepWidget.mOptionsLayout->count(), 0);
}

void StepWidgetTest::testSetStep() {
    StepWidget stepWidget("Test tutorial");
    stepWidget.show();

    Step step("step1");
    step.setText("First step");
    step.addOption(new Option("Step 1 - Option 1"), this, SLOT(dummySlot()));

    stepWidget.setStep(&step);

    QCOMPARE(textWidget(&stepWidget)->toPlainText(), QString("First step"));
    QCOMPARE(stepWidget.mOptionsLayout->count(), 1);
    KPushButton* button1 = qobject_cast<KPushButton*>(
                                stepWidget.mOptionsLayout->itemAt(0)->widget());
    QVERIFY(button1);
    QCOMPARE(button1->text(), QString("Step 1 - Option 1"));
    QVERIFY(stepWidget.isVisible());
}

void StepWidgetTest::testSetStepSeveralSteps() {
    StepWidget stepWidget("Test tutorial");
    stepWidget.show();

    Step step1("step1");
    step1.setText("First step");
    step1.addOption(new Option("Step 1 - Option 1"), this, SLOT(dummySlot()));

    stepWidget.setStep(&step1);

    QCOMPARE(textWidget(&stepWidget)->toPlainText(), QString("First step"));
    QCOMPARE(stepWidget.mOptionsLayout->count(), 1);
    KPushButton* button = qobject_cast<KPushButton*>(
                                stepWidget.mOptionsLayout->itemAt(0)->widget());
    QVERIFY(button);
    QCOMPARE(button->text(), QString("Step 1 - Option 1"));
    QVERIFY(stepWidget.isVisible());

    Step step2("step2");
    step2.setText("Second step");
    step2.addOption(new Option("Step 2 - Option 1"), this, SLOT(dummySlot()));
    step2.addOption(new Option("Step 2 - Option 2"), this, SLOT(dummySlot()));

    stepWidget.setStep(&step2);

    QCOMPARE(textWidget(&stepWidget)->toPlainText(), QString("Second step"));
    QCOMPARE(stepWidget.mOptionsLayout->count(), 2);
    button = qobject_cast<KPushButton*>(
                                stepWidget.mOptionsLayout->itemAt(0)->widget());
    QVERIFY(button);
    QCOMPARE(button->text(), QString("Step 2 - Option 1"));
    button = qobject_cast<KPushButton*>(
                                stepWidget.mOptionsLayout->itemAt(1)->widget());
    QVERIFY(button);
    QCOMPARE(button->text(), QString("Step 2 - Option 2"));
    QVERIFY(stepWidget.isVisible());

    Step step3("step3");
    step3.setText("Third step");

    stepWidget.setStep(&step3);

    QCOMPARE(textWidget(&stepWidget)->toPlainText(), QString("Third step"));
    QCOMPARE(stepWidget.mOptionsLayout->count(), 0);
    QVERIFY(stepWidget.isVisible());
}

void StepWidgetTest::testSetStepHidden() {
    StepWidget stepWidget("Test tutorial");

    Step step("step1");
    step.setText("First step");
    step.addOption(new Option("Step 1 - Option 1"), this, SLOT(dummySlot()));

    stepWidget.setStep(&step);

    QCOMPARE(textWidget(&stepWidget)->toPlainText(), QString("First step"));
    QCOMPARE(stepWidget.mOptionsLayout->count(), 1);
    KPushButton* button1 = qobject_cast<KPushButton*>(
                                stepWidget.mOptionsLayout->itemAt(0)->widget());
    QVERIFY(button1);
    QCOMPARE(button1->text(), QString("Step 1 - Option 1"));
    QVERIFY(stepWidget.isVisible());
}

void StepWidgetTest::selectOption() {
    StepWidget stepWidget("Test tutorial");
    stepWidget.show();

    Step step("step");
    step.setText("A step with two options");
    step.addOption(new Option("Option 1"), this, SLOT(dummySlot()));
    step.addOption(new Option("Option 2"), this, SLOT(anotherDummySlot()));
    step.setActive(true);

    stepWidget.setStep(&step);

    KPushButton* button1 = qobject_cast<KPushButton*>(
                                stepWidget.mOptionsLayout->itemAt(0)->widget());
    KPushButton* button2 = qobject_cast<KPushButton*>(
                                stepWidget.mOptionsLayout->itemAt(1)->widget());

    QTest::mouseClick(button1, Qt::LeftButton);

    QCOMPARE(mDummySlotCallCount, 1);
    QCOMPARE(mAnotherDummySlotCallCount, 0);

    QTest::mouseClick(button2, Qt::LeftButton);

    QCOMPARE(mDummySlotCallCount, 1);
    QCOMPARE(mAnotherDummySlotCallCount, 1);
}

void StepWidgetTest::moveDragging() {
    StepWidget stepWidget("Test tutorial");
    stepWidget.show();

    QTest::qWaitForWindowShown(&stepWidget);

    QPoint previousPosition = stepWidget.pos();

    QPoint widgetCenter(stepWidget.size().width()/2, 
                        stepWidget.size().height()/2);

    QTest::mouseMove(&stepWidget);

    //Use setMouseTracking and QCursor::setPos as a workaround. 
    //QTest::mouseMove, due to unknown reasons for me, doesn't make StepWidget 
    //to receive QMouseEvent for move. Neither QCursor::setPos does if tracking 
    //isn't enabled
    stepWidget.setMouseTracking(true);
    QTest::mousePress(&stepWidget, Qt::LeftButton,
                      Qt::NoModifier, QPoint());
    QCursor::setPos(previousPosition + widgetCenter + QPoint(42, 23));

    QVERIFY(waitForWidgetToMove(&stepWidget, previousPosition, 10000));

    QTest::mouseRelease(&stepWidget, Qt::LeftButton,
                      Qt::NoModifier, QPoint(), 500);
    stepWidget.setMouseTracking(false);

    QVERIFY(previousPosition.x() + 41 <= stepWidget.pos().x() &&
            previousPosition.x() + 43 >= stepWidget.pos().x());
    QVERIFY(previousPosition.y() + 22 <= stepWidget.pos().y() &&
            previousPosition.y() + 24 >= stepWidget.pos().y());
}

void StepWidgetTest::moveDraggingAfterRelease() {
    StepWidget stepWidget("Test tutorial");
    stepWidget.show();

    QTest::qWaitForWindowShown(&stepWidget);

    QPoint previousPosition = stepWidget.pos();

    QPoint widgetCenter(stepWidget.size().width()/2,
                        stepWidget.size().height()/2);

    QTest::mouseMove(&stepWidget);

    //Use setMouseTracking and QCursor::setPos as a workaround.
    //QTest::mouseMove, due to unknown reasons for me, doesn't make StepWidget
    //to receive QMouseEvent for move. Neither QCursor::setPos does if tracking
    //isn't enabled
    stepWidget.setMouseTracking(true);
    QTest::mousePress(&stepWidget, Qt::LeftButton,
                      Qt::NoModifier, QPoint());
    QCursor::setPos(previousPosition + widgetCenter + QPoint(42, 23));

    QVERIFY(waitForWidgetToMove(&stepWidget, previousPosition, 10000));

    QTest::mouseRelease(&stepWidget, Qt::LeftButton,
                      Qt::NoModifier, QPoint(), 500);
    QCursor::setPos(previousPosition + widgetCenter + QPoint(42, 23) +  
                    QPoint(16, 15));
    //Give the cursor time to move
    QTest::qWait(500);
    stepWidget.setMouseTracking(false);

    QVERIFY(previousPosition.x() + 41 <= stepWidget.pos().x() &&
            previousPosition.x() + 43 >= stepWidget.pos().x());
    QVERIFY(previousPosition.y() + 22 <= stepWidget.pos().y() &&
            previousPosition.y() + 24 >= stepWidget.pos().y());
}

void StepWidgetTest::enterWhatIsThisMode() {
    StepWidget stepWidget("Test tutorial");
    stepWidget.show();

    QTest::qWaitForWindowShown(&stepWidget);

    QTest::keyClick(&stepWidget, Qt::Key_F1, Qt::ShiftModifier);

    QVERIFY(QWhatsThis::inWhatsThisMode());
    QWhatsThis::leaveWhatsThisMode();
}

void StepWidgetTest::close() {
    StepWidget* stepWidget = new StepWidget("Test tutorial");
    stepWidget->show();

    QTest::qWaitForWindowShown(stepWidget);

    QSignalSpy destroyedSpy(stepWidget, SIGNAL(destroyed(QObject*)));

    QTest::mouseClick(closeButton(stepWidget), Qt::LeftButton,
                      Qt::NoModifier, QPoint());

    //Process deleteLater()
    QCoreApplication::sendPostedEvents(stepWidget, QEvent::DeferredDelete);

    QCOMPARE(destroyedSpy.count(), 1);
}

void StepWidgetTest::closeUsingEscKey() {
    StepWidget* stepWidget = new StepWidget("Test tutorial");
    stepWidget->show();

    QTest::qWaitForWindowShown(stepWidget);

    QSignalSpy destroyedSpy(stepWidget, SIGNAL(destroyed(QObject*)));

    QTest::keyClick(stepWidget, Qt::Key_Escape);

    //Give Close button time to complete the press animation
    QTest::qWait(500);

    QCOMPARE(destroyedSpy.count(), 1);
}

void StepWidgetTest::moveAfterShowingModalDialogAndThenClose() {
    QWidget window;
    window.show();

    StepWidget* stepWidget = new StepWidget("Test tutorial");
    stepWidget->setMainApplicationWindow(&window);
    stepWidget->show();

    QDialog* modalDialog = new QDialog(&window); //krazy:exclude=qclasses

    QTimer timerAccept;
    timerAccept.setSingleShot(true);
    timerAccept.setInterval(2000);
    connect(&timerAccept, SIGNAL(timeout()), modalDialog, SLOT(accept()));

    queueAssertWidgetDragged(stepWidget, 500);

    timerAccept.start();
    modalDialog->exec(); //krazy:exclude=crashy

    QSignalSpy destroyedSpy(stepWidget, SIGNAL(destroyed(QObject*)));

    QTest::mouseClick(closeButton(stepWidget), Qt::LeftButton,
                      Qt::NoModifier, QPoint(), 500);

    //Process deleteLater()
    QCoreApplication::sendPostedEvents(stepWidget, QEvent::DeferredDelete);

    QCOMPARE(destroyedSpy.count(), 1);
}

/////////////////////////////////// Helpers ////////////////////////////////////

StepTextWidget* StepWidgetTest::textWidget(StepWidget* stepWidget) {
    return stepWidget->ui->textWidget;
}

KPushButton* StepWidgetTest::closeButton(StepWidget* stepWidget) {
    return stepWidget->ui->closeButton;
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

class WidgetMovedCondition {
public:
    const QWidget* mWidget;
    QPoint mPreviousPosition;

    bool condition() const {
        return mPreviousPosition.x() + 41 <= mWidget->pos().x() &&
               mPreviousPosition.x() + 43 >= mWidget->pos().x() &&
               mPreviousPosition.y() + 22 <= mWidget->pos().y() &&
               mPreviousPosition.y() + 24 >= mWidget->pos().y();
    }
};

bool StepWidgetTest::waitForWidgetToMove(const QWidget* widget,
                                         const QPoint& previousPosition,
                                         int timeout) const {
    WidgetMovedCondition helper;
    helper.mWidget = widget;
    helper.mPreviousPosition = previousPosition;
    return waitFor(helper, &WidgetMovedCondition::condition, timeout);
}

//Modal dialogs don't return to the test code until they are closed. Thus, the
//actions or asserts to be performed while a modal dialog is being shown (like
//checking the position of the widget) must be "queued".
class QueuedActionsHelper: public QObject {
Q_OBJECT
public:

    QueuedActionsHelper(QObject* parent = 0): QObject(parent) {
    }

    void setStepWidget(StepWidget* stepWidget) {
        mStepWidget = stepWidget;
    }

public slots:

    void assertWidgetDragged() {
        QVERIFY(mStepWidget->isVisible());

        QPoint previousPosition = mStepWidget->pos();

        QPoint widgetCenter(mStepWidget->size().width()/2,
                            mStepWidget->size().height()/2);

        QTest::mouseMove(mStepWidget);

        //Use setMouseTracking and QCursor::setPos as a workaround.
        //QTest::mouseMove, due to unknown reasons for me, doesn't make
        //StepWidget to receive QMouseEvent for move. Neither QCursor::setPos
        //does if tracking isn't enabled
        mStepWidget->setMouseTracking(true);
        QTest::mousePress(mStepWidget, Qt::LeftButton,
                          Qt::NoModifier, QPoint());
        QCursor::setPos(previousPosition + widgetCenter + QPoint(42, 23));

        //waitForWidgetToMove is a method of StepWidgetTest, so it can not be
        //called here
        WidgetMovedCondition helper;
        helper.mWidget = mStepWidget;
        helper.mPreviousPosition = previousPosition;
        QVERIFY(waitFor(helper, &WidgetMovedCondition::condition, 10000));

        QTest::mouseRelease(mStepWidget, Qt::LeftButton,
                            Qt::NoModifier, QPoint(), 500);
        mStepWidget->setMouseTracking(false);
    }

private:

    StepWidget* mStepWidget;

};

void StepWidgetTest::queueAssertWidgetDragged(StepWidget* stepWidget,
                                              int timeToWait) {
    QueuedActionsHelper* helper = new QueuedActionsHelper(this);
    helper->setStepWidget(stepWidget);
    QTimer::singleShot(timeToWait, helper, SLOT(assertWidgetDragged()));
}

}
}

QTEST_KDEMAIN(ktutorial::view::StepWidgetTest, GUI)

#include "StepWidgetTest.moc"
