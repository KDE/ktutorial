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

#include "ReactionWidget.h"

#include <QRadioButton>
#include <QTimer>
#include <QTreeView>
#include <QUndoStack>

#include <KComboBox>
#include <KDialog>
#include <KLineEdit>
#include <KPushButton>
#include <KTextEdit>

#include "WaitForWidget.h"
#include "../data/Reaction.h"
#include "../data/Step.h"
#include "../data/Tutorial.h"
#include "../data/WaitForComposed.h"
#include "../data/WaitForSignal.h"

class ReactionWidgetTest: public QObject {
Q_OBJECT

private slots:

    void testConstructor();
    void testConstructorWithRichText();

    void testEnableStepIdCompletion();

    void testSelectTriggerTypeOption();
    void testSelectTriggerTypeCondition();

    void testSelectResponseTypeStep();
    void testSelectResponseTypeCustomCode();

    void testSaveChanges();
    void testSaveChangesAddRootWaitFor();
    void testSaveChangesRemoveRootWaitFor();
    void testSaveChangesNoOldWaitForAndNoNewWaitFor();
    void testSaveChangesEqualWaitFor();

private:

    QRadioButton* triggerOptionRadioButton(ReactionWidget* widget) const;
    KLineEdit* triggerOptionLineEdit(ReactionWidget* widget) const;
    QRadioButton* triggerConditionRadioButton(ReactionWidget* widget) const;
    WaitForWidget* triggerConditionWidget(ReactionWidget* widget) const;
    QRadioButton* responseStepRadioButton(ReactionWidget* widget) const;
    KLineEdit* responseStepLineEdit(ReactionWidget* widget) const;
    QRadioButton* responseCodeRadioButton(ReactionWidget* widget) const;
    KTextEdit* responseCodeTextEdit(ReactionWidget* widget) const;

    void addWaitForSignal(ReactionWidget* widget);
    void addWaitForSignalToRootWaitFor(ReactionWidget* widget);
    void removeRootWaitFor(ReactionWidget* widget);

};

void ReactionWidgetTest::testConstructor() {
    WaitFor* waitFor = new WaitForSignal();
    Reaction reaction;
    reaction.setTriggerType(Reaction::ConditionMet);
    reaction.setWaitFor(waitFor);
    reaction.setOptionName("The option name");
    reaction.setResponseType(Reaction::CustomCode);
    reaction.setCustomCode("The custom code");
    reaction.setNextStepId("The step id");

    QWidget parent;
    ReactionWidget* widget = new ReactionWidget(&reaction, &parent);

    QCOMPARE(widget->parentWidget(), &parent);
    QVERIFY(!triggerOptionRadioButton(widget)->isChecked());
    QVERIFY(!triggerOptionLineEdit(widget)->isEnabled());
    QCOMPARE(triggerOptionLineEdit(widget)->text(), QString("The option name"));
    QVERIFY(triggerConditionRadioButton(widget)->isChecked());
    QVERIFY(triggerConditionWidget(widget)->isEnabled());
    QVERIFY(triggerConditionWidget(widget)->waitFor() != waitFor);
    QVERIFY(*triggerConditionWidget(widget)->waitFor() == *waitFor);
    QVERIFY(!responseStepRadioButton(widget)->isChecked());
    QVERIFY(!responseStepLineEdit(widget)->isEnabled());
    QCOMPARE(responseStepLineEdit(widget)->text(), QString("The step id"));
    QVERIFY(responseCodeRadioButton(widget)->isChecked());
    QVERIFY(responseCodeTextEdit(widget)->isEnabled());
    QCOMPARE(responseCodeTextEdit(widget)->toPlainText(),
             QString("The custom code"));
}

void ReactionWidgetTest::testConstructorWithRichText() {
    WaitFor* waitFor = new WaitForSignal();
    Reaction reaction;
    reaction.setTriggerType(Reaction::ConditionMet);
    reaction.setWaitFor(waitFor);
    reaction.setOptionName("The option name");
    reaction.setResponseType(Reaction::CustomCode);
    reaction.setCustomCode("<p>The custom code</p>");
    reaction.setNextStepId("The step id");

    QWidget parent;
    ReactionWidget* widget = new ReactionWidget(&reaction, &parent);

    QCOMPARE(widget->parentWidget(), &parent);
    QVERIFY(!triggerOptionRadioButton(widget)->isChecked());
    QVERIFY(!triggerOptionLineEdit(widget)->isEnabled());
    QCOMPARE(triggerOptionLineEdit(widget)->text(), QString("The option name"));
    QVERIFY(triggerConditionRadioButton(widget)->isChecked());
    QVERIFY(triggerConditionWidget(widget)->isEnabled());
    QVERIFY(triggerConditionWidget(widget)->waitFor() != waitFor);
    QVERIFY(*triggerConditionWidget(widget)->waitFor() == *waitFor);
    QVERIFY(!responseStepRadioButton(widget)->isChecked());
    QVERIFY(!responseStepLineEdit(widget)->isEnabled());
    QCOMPARE(responseStepLineEdit(widget)->text(), QString("The step id"));
    QVERIFY(responseCodeRadioButton(widget)->isChecked());
    QVERIFY(responseCodeTextEdit(widget)->isEnabled());
    QCOMPARE(responseCodeTextEdit(widget)->toPlainText(),
             QString("<p>The custom code</p>"));
}

void ReactionWidgetTest::testEnableStepIdCompletion() {
    Reaction* reaction = new Reaction();

    ReactionWidget widget(reaction);

    Tutorial tutorial;

    Step* emptyIdStep = new Step();
    emptyIdStep->setId("");
    tutorial.addStep(emptyIdStep);

    Step* firstStep = new Step();
    firstStep->setId("First step");
    tutorial.addStep(firstStep);

    Step* secondStep = new Step();
    secondStep->setId("Second step");
    tutorial.addStep(secondStep);

    Step* thirdStep = new Step();
    thirdStep->setId("Third step");
    tutorial.addStep(thirdStep);

    widget.enableStepIdCompletion(&tutorial, firstStep);

    QStringList completionItems =
                    responseStepLineEdit(&widget)->completionObject()->items();
    QCOMPARE(completionItems.count(), 2);
    QVERIFY(completionItems.contains("Second step"));
    QVERIFY(completionItems.contains("Third step"));

    secondStep->addReaction(reaction);

    widget.enableStepIdCompletion(&tutorial);

    completionItems =
                    responseStepLineEdit(&widget)->completionObject()->items();
    QCOMPARE(completionItems.count(), 2);
    QVERIFY(completionItems.contains("First step"));
    QVERIFY(completionItems.contains("Third step"));
}

void ReactionWidgetTest::testSelectTriggerTypeOption() {
    Reaction reaction;
    reaction.setTriggerType(Reaction::ConditionMet);

    ReactionWidget widget(&reaction);

    triggerOptionRadioButton(&widget)->setChecked(true);

    QVERIFY(!triggerConditionRadioButton(&widget)->isChecked());
    QVERIFY(!triggerConditionWidget(&widget)->isEnabled());
    QVERIFY(triggerOptionLineEdit(&widget)->isEnabled());
}

void ReactionWidgetTest::testSelectTriggerTypeCondition() {
    Reaction reaction;
    reaction.setTriggerType(Reaction::OptionSelected);

    ReactionWidget widget(&reaction);

    triggerConditionRadioButton(&widget)->setChecked(true);

    QVERIFY(!triggerOptionRadioButton(&widget)->isChecked());
    QVERIFY(!triggerOptionLineEdit(&widget)->isEnabled());
    QVERIFY(triggerConditionWidget(&widget)->isEnabled());
}

void ReactionWidgetTest::testSelectResponseTypeStep() {
    Reaction reaction;
    reaction.setResponseType(Reaction::CustomCode);

    ReactionWidget widget(&reaction);

    responseStepRadioButton(&widget)->setChecked(true);

    QVERIFY(!responseCodeRadioButton(&widget)->isChecked());
    QVERIFY(!responseCodeTextEdit(&widget)->isEnabled());
    QVERIFY(responseStepLineEdit(&widget)->isEnabled());
}

void ReactionWidgetTest::testSelectResponseTypeCustomCode() {
    Reaction reaction;
    reaction.setResponseType(Reaction::NextStep);

    ReactionWidget widget(&reaction);

    responseCodeRadioButton(&widget)->setChecked(true);

    QVERIFY(!responseStepRadioButton(&widget)->isChecked());
    QVERIFY(!responseStepLineEdit(&widget)->isEnabled());
    QVERIFY(responseCodeTextEdit(&widget)->isEnabled());
}

void ReactionWidgetTest::testSaveChanges() {
    WaitFor* oldWaitFor = new WaitForComposed();
    Reaction reaction;
    reaction.setTriggerType(Reaction::ConditionMet);
    reaction.setResponseType(Reaction::CustomCode);
    reaction.setWaitFor(oldWaitFor);

    ReactionWidget widget(&reaction);

    addWaitForSignalToRootWaitFor(&widget);
    triggerOptionRadioButton(&widget)->setChecked(true);
    triggerOptionLineEdit(&widget)->setText("The option name");

    responseCodeTextEdit(&widget)->setText("The custom code");
    responseStepRadioButton(&widget)->setChecked(true);
    responseStepLineEdit(&widget)->setText("The step id");

    widget.saveChanges();

    QCOMPARE(reaction.triggerType(), Reaction::OptionSelected);
    QCOMPARE(reaction.optionName(), QString("The option name"));
    QVERIFY(reaction.waitFor() != oldWaitFor);
    WaitForComposed* waitFor =
                            qobject_cast<WaitForComposed*>(reaction.waitFor());
    QVERIFY(waitFor);
    QCOMPARE(waitFor->waitFors().count(), 1);
    QVERIFY(qobject_cast<WaitForSignal*>(waitFor->waitFors()[0]));
    QCOMPARE(reaction.responseType(), Reaction::NextStep);
    QCOMPARE(reaction.nextStepId(), QString("The step id"));
    QCOMPARE(reaction.customCode(), QString("The custom code"));
}

void ReactionWidgetTest::testSaveChangesAddRootWaitFor() {
    Reaction reaction;
    reaction.setTriggerType(Reaction::ConditionMet);

    ReactionWidget widget(&reaction);

    addWaitForSignal(&widget);
    widget.saveChanges();

    QVERIFY(reaction.waitFor() != 0);
    QVERIFY(qobject_cast<WaitForSignal*>(reaction.waitFor()));
}

void ReactionWidgetTest::testSaveChangesRemoveRootWaitFor() {
    Reaction reaction;
    reaction.setTriggerType(Reaction::ConditionMet);
    reaction.setWaitFor(new WaitForSignal());

    ReactionWidget widget(&reaction);

    removeRootWaitFor(&widget);
    widget.saveChanges();

    QCOMPARE(reaction.waitFor(), (WaitFor*)0);
}

void ReactionWidgetTest::testSaveChangesNoOldWaitForAndNoNewWaitFor() {
    Reaction reaction;

    ReactionWidget widget(&reaction);

    QUndoStack undoStack;
    widget.setUndoStack(&undoStack);

    widget.saveChanges();

    QCOMPARE(undoStack.count(), 0);
    QCOMPARE(reaction.waitFor(), (WaitFor*)0);
}

void ReactionWidgetTest::testSaveChangesEqualWaitFor() {
    Reaction reaction;
    WaitForSignal* waitFor = new WaitForSignal();
    reaction.setWaitFor(waitFor);

    ReactionWidget widget(&reaction);

    QUndoStack undoStack;
    widget.setUndoStack(&undoStack);

    addWaitForSignal(&widget);
    widget.saveChanges();

    QCOMPARE(undoStack.count(), 0);
    QCOMPARE(reaction.waitFor(), waitFor);
}

/////////////////////////////////// Helpers ////////////////////////////////////

QRadioButton* ReactionWidgetTest::triggerOptionRadioButton(
                                                ReactionWidget* widget) const {
    return widget->findChild<QRadioButton*>("triggerOptionRadioButton");
}

KLineEdit* ReactionWidgetTest::triggerOptionLineEdit(
                                                ReactionWidget* widget) const {
    return widget->findChild<KLineEdit*>("triggerOptionLineEdit");
}

QRadioButton* ReactionWidgetTest::triggerConditionRadioButton(
                                                ReactionWidget* widget) const {
    return widget->findChild<QRadioButton*>("triggerConditionRadioButton");
}

WaitForWidget* ReactionWidgetTest::triggerConditionWidget(
                                                ReactionWidget* widget) const {
    return widget->findChild<WaitForWidget*>("triggerConditionWaitForWidget");
}

QRadioButton* ReactionWidgetTest::responseStepRadioButton(
                                                ReactionWidget* widget) const {
    return widget->findChild<QRadioButton*>("responseStepRadioButton");
}

KLineEdit* ReactionWidgetTest::responseStepLineEdit(
                                                ReactionWidget* widget) const {
    return widget->findChild<KLineEdit*>("responseStepLineEdit");
}

QRadioButton* ReactionWidgetTest::responseCodeRadioButton(
                                                ReactionWidget* widget) const {
    return widget->findChild<QRadioButton*>("responseCodeRadioButton");
}

KTextEdit* ReactionWidgetTest::responseCodeTextEdit(
                                                ReactionWidget* widget) const {
    return widget->findChild<KTextEdit*>("responseCodeTextEdit");
}

class AddWaitForSignalHelper: public QObject {
Q_OBJECT
public:

    AddWaitForSignalHelper(QWidget* widget, QObject* parent):
            QObject(parent),
        mWidget(widget) {
    }

public slots:

    void addWaitForSignal() const {
        KComboBox* comboBox =
                        mWidget->findChild<KComboBox*>("waitForTypeComboBox");
        QVERIFY(comboBox);
        comboBox->setCurrentIndex(3);

        KDialog* dialog = mWidget->findChild<KDialog*>("addWaitForDialog");
        QVERIFY(dialog);
        dialog->button(KDialog::Ok)->click();
    }

private:

    QWidget* mWidget;

};

void ReactionWidgetTest::addWaitForSignal(ReactionWidget* widget) {
    AddWaitForSignalHelper* helper = new AddWaitForSignalHelper(widget, this);

    //The dialog is modal, so it won't return to the test code until it is
    //closed. Thus, the commands to execute on the dialog must be "queued",
    //as calling addWaitForSignal after the button click won't work.
    QTimer::singleShot(500, helper, SLOT(addWaitForSignal()));

    widget->findChild<KPushButton*>("addButton")->click();
}

void ReactionWidgetTest::addWaitForSignalToRootWaitFor(ReactionWidget* widget) {
    QTreeView* tree = widget->findChild<QTreeView*>("waitForTreeView");
    QModelIndex index = tree->model()->index(0, 0);
    tree->selectionModel()->select(index, QItemSelectionModel::SelectCurrent);

    addWaitForSignal(widget);
}

void ReactionWidgetTest::removeRootWaitFor(ReactionWidget* widget) {
    QTreeView* tree = widget->findChild<QTreeView*>("waitForTreeView");
    QModelIndex index = tree->model()->index(0, 0);
    tree->selectionModel()->select(index, QItemSelectionModel::SelectCurrent);

    widget->findChild<KPushButton*>("removeButton")->click();
}

QTEST_MAIN(ReactionWidgetTest)

#include "ReactionWidgetTest.moc"
