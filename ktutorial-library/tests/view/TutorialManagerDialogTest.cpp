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

#include <QtTest>
#include <QtTest/QTestKeyClicksEvent>
#include <qtest_kde.h>
#include <kpushbutton.h>

#define protected public
#define private public
#include "TutorialManagerDialog.h"
#undef private
#undef protected

#include "ui_TutorialManagerDialog.h"
#include "../Step.h"
#include "../Tutorial.h"
#include "../TutorialInformation.h"
#include "../TutorialManager.h"

namespace ktutorial {
namespace view {

class MockTutorial;

class TutorialManagerDialogTest: public QObject {
Q_OBJECT

private slots:

    void init();
    void cleanup();

    void testConstructor();

    void show();

    void selectTutorial();
    void selectSeveralTutorials();

    void startTutorial();
    void startTutorialUsingEnterKey();

    void close();
    void closeUsingEscKey();

private:

    TutorialManagerDialog* mTutorialManagerDialog;
    TutorialManager* mTutorialManager;
    TutorialInformation* mTutorialInformation1;
    Step* mStartStep1;
    MockTutorial* mTutorial1;
    TutorialInformation* mTutorialInformation2;
    Step* mStartStep2;
    MockTutorial* mTutorial2;
    TutorialInformation* mTutorialInformation3;
    Step* mStartStep3;
    MockTutorial* mTutorial3;

    QAbstractItemView* tutorialList(
                            TutorialManagerDialog* tutorialManagerDialog);

    QLabel* descriptionLabel(TutorialManagerDialog* tutorialManagerDialog);

    KPushButton* startButton(TutorialManagerDialog* tutorialManagerDialog);
    KPushButton* closeButton(TutorialManagerDialog* tutorialManagerDialog);

};

class MockTutorial: public Tutorial {
public:
    MockTutorial(TutorialInformation* tutorialInformation):
        Tutorial(tutorialInformation) {
    }

    void emitFinished() {
        emit finished(this);
    }
};

void TutorialManagerDialogTest::init() {
    mTutorialManager = new TutorialManager();

    mTutorialInformation1 = new TutorialInformation("tutorial1");
    mTutorialInformation1->setName("First tutorial");
    mTutorialInformation1->setDescription("This is the first tutorial");
    mTutorial1 = new MockTutorial(mTutorialInformation1);
    mStartStep1 = new Step("start");
    mTutorial1->addStep(mStartStep1);
    
    mTutorialInformation2 = new TutorialInformation("tutorial2");
    mTutorialInformation2->setName("Second tutorial");
    mTutorialInformation2->setDescription("This is the second tutorial");
    mTutorial2 = new MockTutorial(mTutorialInformation2);
    mStartStep2 = new Step("start");
    mTutorial2->addStep(mStartStep2);

    mTutorialInformation3 = new TutorialInformation("tutorial3");
    mTutorialInformation3->setName("Third tutorial");
    mTutorialInformation3->setDescription("This is the third tutorial");
    mTutorial3 = new MockTutorial(mTutorialInformation3);
    mStartStep3 = new Step("start");
    mTutorial3->addStep(mStartStep3);

    mTutorialManagerDialog = new TutorialManagerDialog(mTutorialManager);
}

void TutorialManagerDialogTest::cleanup() {
    delete mTutorialManagerDialog;

    if (!mTutorialManager->tutorialInformations().contains(
                                                    mTutorialInformation1)) {
        delete mTutorial1;
    }

    if (!mTutorialManager->tutorialInformations().contains(
                                                    mTutorialInformation2)) {
        delete mTutorial2;
    }

    if (!mTutorialManager->tutorialInformations().contains(
                                                    mTutorialInformation3)) {
        delete mTutorial3;
    }

    delete mTutorialManager;
}

void TutorialManagerDialogTest::testConstructor() {
    QCOMPARE(mTutorialManagerDialog->mTutorialManager, mTutorialManager);
    QCOMPARE(mTutorialManagerDialog->mCurrentTutorialInformation,
             (const TutorialInformation*)0);
    QVERIFY(!startButton(mTutorialManagerDialog)->isEnabled());
    QVERIFY(closeButton(mTutorialManagerDialog)->isEnabled());
}

void TutorialManagerDialogTest::show() {
    mTutorialManager->registerTutorial(mTutorial1);
    mTutorialManager->registerTutorial(mTutorial2);
    mTutorialManager->registerTutorial(mTutorial3);

    //Ensure that no wrong signals set the current tutorial when showing the 
    //dialog (like connecting currentChanged in tutorialList made)
    mTutorialManagerDialog->show();
    QTest::qWait(500);

    QCOMPARE(mTutorialManagerDialog->mTutorialManager, mTutorialManager);
    QCOMPARE(mTutorialManagerDialog->mCurrentTutorialInformation,
             (const TutorialInformation*)0);
    QVERIFY(!startButton(mTutorialManagerDialog)->isEnabled());
    QVERIFY(closeButton(mTutorialManagerDialog)->isEnabled());
}

void TutorialManagerDialogTest::selectTutorial() {
    mTutorialManager->registerTutorial(mTutorial1);
    mTutorialManager->registerTutorial(mTutorial2);
    mTutorialManager->registerTutorial(mTutorial3);

    mTutorialManagerDialog->show();

    QTest::qWaitForWindowShown(mTutorialManagerDialog);

    tutorialList(mTutorialManagerDialog)->setFocus();

    QTest::keyClick(tutorialList(mTutorialManagerDialog), Qt::Key_Space,
                    Qt::NoModifier);

    QItemSelectionModel* selectionModel = 
                        tutorialList(mTutorialManagerDialog)->selectionModel();
    QCOMPARE(selectionModel->selectedRows().count(), 1);
    QCOMPARE(selectionModel->selectedRows()[0].row(), 0);
    QCOMPARE(descriptionLabel(mTutorialManagerDialog)->text(),
             QString("This is the first tutorial"));
    QCOMPARE(mTutorialManagerDialog->mCurrentTutorialInformation,
             mTutorialInformation1);

    QTest::keyClick(tutorialList(mTutorialManagerDialog), Qt::Key_Down);

    QCOMPARE(selectionModel->selectedRows().count(), 1);
    QCOMPARE(selectionModel->selectedRows()[0].row(), 1);
    QCOMPARE(descriptionLabel(mTutorialManagerDialog)->text(),
             QString("This is the second tutorial"));
    QCOMPARE(mTutorialManagerDialog->mCurrentTutorialInformation,
             mTutorialInformation2);

    QTest::keyClick(tutorialList(mTutorialManagerDialog), Qt::Key_Down);

    QCOMPARE(selectionModel->selectedRows().count(), 1);
    QCOMPARE(selectionModel->selectedRows()[0].row(), 2);
    QCOMPARE(descriptionLabel(mTutorialManagerDialog)->text(),
             QString("This is the third tutorial"));
    QCOMPARE(mTutorialManagerDialog->mCurrentTutorialInformation,
             mTutorialInformation3);
}

void TutorialManagerDialogTest::selectSeveralTutorials() {
    mTutorialManager->registerTutorial(mTutorial1);
    mTutorialManager->registerTutorial(mTutorial2);
    mTutorialManager->registerTutorial(mTutorial3);

    mTutorialManagerDialog->show();

    QTest::qWaitForWindowShown(mTutorialManagerDialog);

    tutorialList(mTutorialManagerDialog)->setFocus();

    QTest::keyClick(tutorialList(mTutorialManagerDialog), Qt::Key_Space,
                    Qt::NoModifier);

    QItemSelectionModel* selectionModel =
                        tutorialList(mTutorialManagerDialog)->selectionModel();
    QCOMPARE(selectionModel->selectedRows().count(), 1);
    QCOMPARE(selectionModel->selectedRows()[0].row(), 0);
    QCOMPARE(descriptionLabel(mTutorialManagerDialog)->text(),
             QString("This is the first tutorial"));
    QCOMPARE(mTutorialManagerDialog->mCurrentTutorialInformation,
             mTutorialInformation1);

    QTest::keyClick(tutorialList(mTutorialManagerDialog), Qt::Key_Down, 
                    Qt::ShiftModifier);

    QCOMPARE(selectionModel->selectedRows().count(), 1);
    QCOMPARE(selectionModel->selectedRows()[0].row(), 1);
    QCOMPARE(descriptionLabel(mTutorialManagerDialog)->text(),
             QString("This is the second tutorial"));
    QCOMPARE(mTutorialManagerDialog->mCurrentTutorialInformation,
             mTutorialInformation2);

    QTest::keyClick(tutorialList(mTutorialManagerDialog), Qt::Key_Down,
                    Qt::ShiftModifier);

    QCOMPARE(selectionModel->selectedRows().count(), 1);
    QCOMPARE(selectionModel->selectedRows()[0].row(), 2);
    QCOMPARE(descriptionLabel(mTutorialManagerDialog)->text(),
             QString("This is the third tutorial"));
    QCOMPARE(mTutorialManagerDialog->mCurrentTutorialInformation,
             mTutorialInformation3);
}

void TutorialManagerDialogTest::startTutorial() {
    mTutorialManager->registerTutorial(mTutorial1);
    mTutorialManager->registerTutorial(mTutorial2);
    mTutorialManager->registerTutorial(mTutorial3);

    mTutorialManagerDialog->show();

    QTest::qWaitForWindowShown(mTutorialManagerDialog);

    QItemSelectionModel* selectionModel =
                        tutorialList(mTutorialManagerDialog)->selectionModel();
    selectionModel->select(selectionModel->model()->index(1, 0),
                           QItemSelectionModel::Select);

    QTest::mouseClick(startButton(mTutorialManagerDialog), Qt::LeftButton,
                      Qt::NoModifier, QPoint());

    QVERIFY(mTutorialManagerDialog->isHidden());
    QVERIFY(mStartStep2->isActive());

    mTutorial2->emitFinished();

    QVERIFY(mTutorialManagerDialog->isVisible());
}

void TutorialManagerDialogTest::startTutorialUsingEnterKey() {
    mTutorialManager->registerTutorial(mTutorial1);
    mTutorialManager->registerTutorial(mTutorial2);
    mTutorialManager->registerTutorial(mTutorial3);

    mTutorialManagerDialog->show();

    //When the dialog is shown, the focus is given to the Close button. The
    //Start button (User1) is the default one, but as it is disabled, the Close
    //button becomes the default dialog button. The Start button will become the
    //default button again when the Close button receives a FocusOut event.
    //However, the dialog must be the active window for the Close button to
    //receive the FocusOut event. If the focus is set to the tutorial list but
    //the dialog is not the active window no FocusOut event will be sent (as
    //QWidget::setFocus calls QApplicationPrivate::setFocusWidget only when the
    //widget is in the active window). Even if the dialog became the active
    //window later once the focus was set to the tutorial list, no FocusOut
    //event would be sent to the Close button.
    //Thus, the dialog must have been truly shown before setting the focus to
    //the tutorial list. Note that if the dialog is shown but it is not the
    //active window (which can happen, for example, when debugging step by step)
    //the FocusOut event will not be sent either, but in normal test running
    //showing the dialog will make it the active window.
    QTest::qWaitForWindowShown(mTutorialManagerDialog);

    tutorialList(mTutorialManagerDialog)->setFocus();

    QItemSelectionModel* selectionModel =
                        tutorialList(mTutorialManagerDialog)->selectionModel();
    selectionModel->select(selectionModel->model()->index(1, 0),
                           QItemSelectionModel::Select);

    QTest::keyClick(tutorialList(mTutorialManagerDialog), Qt::Key_Enter,
                    Qt::NoModifier);

    QVERIFY(mTutorialManagerDialog->isHidden());
    QVERIFY(mStartStep2->isActive());

    mTutorial2->emitFinished();

    QVERIFY(mTutorialManagerDialog->isVisible());
}

void TutorialManagerDialogTest::close() {
    TutorialManagerDialog tutorialManagerDialog(mTutorialManager);
    mTutorialManagerDialog->show();

    QTest::mouseClick(closeButton(mTutorialManagerDialog), Qt::LeftButton);

    QVERIFY(mTutorialManagerDialog->isHidden());
}

void TutorialManagerDialogTest::closeUsingEscKey() {
    TutorialManagerDialog tutorialManagerDialog(mTutorialManager);
    mTutorialManagerDialog->show();

    QTest::qWaitForWindowShown(mTutorialManagerDialog);

    QTest::keyClick(tutorialList(mTutorialManagerDialog), Qt::Key_Escape,
                    Qt::NoModifier);

    //Give Close button time to complete the press animation
    QTest::qWait(500);

    QVERIFY(mTutorialManagerDialog->isHidden());
}

////////////////////////////////// Helpers /////////////////////////////////////

QAbstractItemView* TutorialManagerDialogTest::tutorialList(
                                TutorialManagerDialog* tutorialManagerDialog) {
    return tutorialManagerDialog->ui->tutorialsList;
}

QLabel* TutorialManagerDialogTest::descriptionLabel(
                                TutorialManagerDialog* tutorialManagerDialog) {
    return tutorialManagerDialog->ui->descriptionLabel;
}

KPushButton* TutorialManagerDialogTest::startButton(
                                TutorialManagerDialog* tutorialManagerDialog) {
    return tutorialManagerDialog->button(KDialog::User1);
}

KPushButton* TutorialManagerDialogTest::closeButton(
                                TutorialManagerDialog* tutorialManagerDialog) {
    return tutorialManagerDialog->button(KDialog::Close);
}

}
}

QTEST_KDEMAIN(ktutorial::view::TutorialManagerDialogTest, GUI)

#include "TutorialManagerDialogTest.moc"
