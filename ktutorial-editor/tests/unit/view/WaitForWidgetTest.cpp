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

#include "WaitForWidget.h"

#include <QTimer>
#include <QTreeView>

#include <KComboBox>
#include <KLineEdit>
#include <KPushButton>

#include "EditionDialog.h"
#include "../data/WaitForComposed.h"
#include "../data/WaitForEvent.h"
#include "../data/WaitForNot.h"
#include "../data/WaitForProperty.h"
#include "../data/WaitForSignal.h"
#include "../data/WaitForStepActivation.h"
#include "../data/WaitForWindow.h"

class WaitForWidgetTest: public QObject {
Q_OBJECT
public slots:

    void addWaitForSignal() const;
    void cancelAddWaitForDialog() const;

    void setEventData() const;
    void setPropertyData() const;
    void setSignalData() const;
    void setWindowData() const;

private slots:

    void init();
    void cleanup();

    void testConstructor();
    void testConstructorFull();

    void testSelectWaitForAnd();
    void testSelectWaitForOr();
    void testSelectWaitForNot();
    void testSelectWaitForNotEmpty();
    void testSelectWaitForEvent();
    void testSelectWaitForProperty();
    void testSelectWaitForSignal();
    void testSelectWaitForStepActivation();
    void testSelectWaitForWindow();
    void testClearSelection();

    void testAddWaitForWhenEmpty();
    void testAddWaitForToWaitForComposed();
    void testAddWaitForToWaitForNot();
    void testAddWaitForCancellingDialog();

    void testEditWaitForEvent();
    void testEditWaitForProperty();
    void testEditWaitForSignal();
    void testEditWaitForWindow();

    void testRemoveWaitForFromRoot();
    void testRemoveWaitForFromWaitForComposed();
    void testRemoveWaitForFromWaitForNot();

private:

    WaitForComposed* mWaitFor;
    WaitForSignal* mWaitFor1;
    WaitForComposed* mWaitFor2;
    WaitForEvent* mWaitFor2_1;
    WaitForWindow* mWaitFor2_2;
    WaitForProperty* mWaitFor2_3;
    WaitForStepActivation* mWaitFor2_4;
    WaitForNot* mWaitFor3;
    WaitForSignal* mWaitFor3_1;
    WaitForNot* mWaitFor4;

    WaitForWidget* mWidget;

    QModelIndex getIndex(int row,
                         const QModelIndex& parent = QModelIndex()) const;

    void selectItem(const QModelIndex& index) const;

    KPushButton* button(const QString& name, WaitForWidget* widget = 0) const;

    void assertButtonEnabled(bool addButtonEnabled,
                             bool editButtonEnabled,
                             bool removeButtonEnabled,
                             WaitForWidget* widget = 0) const;

};

void WaitForWidgetTest::init() {
    mWaitFor = new WaitForComposed();
    mWaitFor->setCompositionType(WaitForComposed::And);

    mWaitFor1 = new WaitForSignal();
    mWaitFor1->setEmitterName("emitter1");
    mWaitFor1->setSignalName("signal1");
    mWaitFor->addWaitFor(mWaitFor1);

    mWaitFor2 = new WaitForComposed();
    mWaitFor2->setCompositionType(WaitForComposed::Or);
    mWaitFor->addWaitFor(mWaitFor2);

    mWaitFor2_1 = new WaitForEvent();
    mWaitFor2_1->setReceiverName("receiver");
    mWaitFor2_1->setEventName("event");
    mWaitFor2->addWaitFor(mWaitFor2_1);

    mWaitFor2_2 = new WaitForWindow();
    mWaitFor2_2->setWindowObjectName("windowObject");
    mWaitFor2->addWaitFor(mWaitFor2_2);

    mWaitFor2_3 = new WaitForProperty();
    mWaitFor2_3->setObjectName("object");
    mWaitFor2_3->setPropertyName("property");
    mWaitFor2_3->setValue("value");
    mWaitFor2->addWaitFor(mWaitFor2_3);

    mWaitFor2_4 = new WaitForStepActivation();
    mWaitFor2->addWaitFor(mWaitFor2_4);

    mWaitFor3 = new WaitForNot();
    mWaitFor->addWaitFor(mWaitFor3);

    mWaitFor3_1 = new WaitForSignal();
    mWaitFor3_1->setEmitterName("emitter2");
    mWaitFor3_1->setSignalName("signal2");
    mWaitFor3->setNegatedWaitFor(mWaitFor3_1);

    mWaitFor4 = new WaitForNot();
    mWaitFor->addWaitFor(mWaitFor4);

    mWidget = new WaitForWidget(mWaitFor);
}

void WaitForWidgetTest::cleanup() {
    delete mWidget;
    delete mWaitFor;
}

void WaitForWidgetTest::testConstructor() {
    QWidget parent;
    WaitForWidget* widget = new WaitForWidget(0, &parent);

    QCOMPARE(widget->parentWidget(), &parent);
    QCOMPARE(widget->waitFor(), (WaitFor*)0);
    assertButtonEnabled(true, false, false, widget);
}

void WaitForWidgetTest::testConstructorFull() {
    QWidget parent;
    WaitForWidget* widget = new WaitForWidget(mWaitFor, &parent);

    QCOMPARE(widget->parentWidget(), &parent);
    QCOMPARE(widget->waitFor(), (WaitFor*)mWaitFor);
    assertButtonEnabled(false, false, false, widget);
}

void WaitForWidgetTest::testSelectWaitForAnd() {
    selectItem(getIndex(0));

    assertButtonEnabled(true, false, true);
}

void WaitForWidgetTest::testSelectWaitForOr() {
    selectItem(getIndex(1, getIndex(0)));

    assertButtonEnabled(true, false, true);
}

void WaitForWidgetTest::testSelectWaitForNot() {
    selectItem(getIndex(2, getIndex(0)));

    assertButtonEnabled(false, false, true);
}

void WaitForWidgetTest::testSelectWaitForNotEmpty() {
    selectItem(getIndex(3, getIndex(0)));

    assertButtonEnabled(true, false, true);
}

void WaitForWidgetTest::testSelectWaitForEvent() {
    selectItem(getIndex(0, getIndex(1, getIndex(0))));

    assertButtonEnabled(false, true, true);
}

void WaitForWidgetTest::testSelectWaitForProperty() {
    selectItem(getIndex(2, getIndex(1, getIndex(0))));

    assertButtonEnabled(false, true, true);
}

void WaitForWidgetTest::testSelectWaitForSignal() {
    selectItem(getIndex(0, getIndex(0)));

    assertButtonEnabled(false, true, true);
}

void WaitForWidgetTest::testSelectWaitForStepActivation() {
    selectItem(getIndex(3, getIndex(1, getIndex(0))));

    assertButtonEnabled(false, false, true);
}

void WaitForWidgetTest::testSelectWaitForWindow() {
    selectItem(getIndex(1, getIndex(1, getIndex(0))));

    assertButtonEnabled(false, true, true);
}

void WaitForWidgetTest::testClearSelection() {
    selectItem(getIndex(0));
    selectItem(QModelIndex());

    assertButtonEnabled(false, false, false);
}

void WaitForWidgetTest::testAddWaitForWhenEmpty() {
    delete mWidget;
    mWidget = new WaitForWidget(0);

    //The dialog is modal, so it won't return to the test code until it is
    //closed. Thus, the commands to execute on the dialog must be "queued",
    //as calling addWaitForSignal after the button click won't work.
    QTimer::singleShot(500, this, SLOT(addWaitForSignal()));

    button("addButton")->click();

    QVERIFY(mWidget->waitFor());
    QVERIFY(qobject_cast<WaitForSignal*>(mWidget->waitFor()));
    QVERIFY(getIndex(0).isValid());
    //No item is selected after adding the WaitFor
    assertButtonEnabled(false, false, false);
    delete mWidget->waitFor();
}

void WaitForWidgetTest::testAddWaitForToWaitForComposed() {
    selectItem(getIndex(1, getIndex(0)));

    //The dialog is modal, so it won't return to the test code until it is
    //closed. Thus, the commands to execute on the dialog must be "queued",
    //as calling addWaitForSignal after the button click won't work.
    QTimer::singleShot(500, this, SLOT(addWaitForSignal()));

    button("addButton")->click();

    selectItem(getIndex(0));

    QTimer::singleShot(500, this, SLOT(addWaitForSignal()));

    button("addButton")->click();

    QCOMPARE(mWaitFor->waitFors().count(), 5);
    QCOMPARE(mWaitFor->waitFors()[0], mWaitFor1);
    QCOMPARE(mWaitFor->waitFors()[1], mWaitFor2);
    QCOMPARE(mWaitFor->waitFors()[2], mWaitFor3);
    QCOMPARE(mWaitFor->waitFors()[3], mWaitFor4);
    QVERIFY(qobject_cast<WaitForSignal*>(mWaitFor->waitFors()[4]));
    QCOMPARE(mWaitFor2->waitFors().count(), 5);
    QCOMPARE(mWaitFor2->waitFors()[0], mWaitFor2_1);
    QCOMPARE(mWaitFor2->waitFors()[1], mWaitFor2_2);
    QCOMPARE(mWaitFor2->waitFors()[2], mWaitFor2_3);
    QCOMPARE(mWaitFor2->waitFors()[3], mWaitFor2_4);
    QVERIFY(qobject_cast<WaitForSignal*>(mWaitFor2->waitFors()[4]));
    QVERIFY(mWaitFor->waitFors()[4] != mWaitFor2->waitFors()[4]);
}

void WaitForWidgetTest::testAddWaitForToWaitForNot() {
    selectItem(getIndex(3, getIndex(0)));

    //The dialog is modal, so it won't return to the test code until it is
    //closed. Thus, the commands to execute on the dialog must be "queued",
    //as calling addWaitForSignal after the button click won't work.
    QTimer::singleShot(500, this, SLOT(addWaitForSignal()));

    button("addButton")->click();

    QCOMPARE(mWaitFor->waitFors().count(), 4);
    QCOMPARE(mWaitFor->waitFors()[0], mWaitFor1);
    QCOMPARE(mWaitFor->waitFors()[1], mWaitFor2);
    QCOMPARE(mWaitFor->waitFors()[2], mWaitFor3);
    QCOMPARE(mWaitFor->waitFors()[3], mWaitFor4);
    QVERIFY(qobject_cast<WaitForSignal*>(mWaitFor4->negatedWaitFor()));
    //The WaitForNot item is selected after adding the negated WaitFor, so "Add"
    //button must be disabled
    assertButtonEnabled(false, false, true);
}

void WaitForWidgetTest::testAddWaitForCancellingDialog() {
    delete mWidget;
    mWidget = new WaitForWidget(0);

    //The dialog is modal, so it won't return to the test code until it is
    //closed. Thus, the commands to execute on the dialog must be "queued",
    //as calling cancelAddWaitForDialog after the button click won't work.
    QTimer::singleShot(500, this, SLOT(cancelAddWaitForDialog()));

    button("addButton")->click();

    QVERIFY(!mWidget->waitFor());
    QVERIFY(!getIndex(0).isValid());
    //Buttons shouldn't change after cancelling the dialog
    assertButtonEnabled(true, false, false);
}

void WaitForWidgetTest::testEditWaitForEvent() {
    selectItem(getIndex(0, getIndex(1, getIndex(0))));

    //The dialog is modal, so it won't return to the test code until it is
    //closed. Thus, the commands to execute on the dialog must be "queued",
    //as calling setEventData after the button click won't work.
    QTimer::singleShot(500, this, SLOT(setEventData()));

    button("editButton")->click();

    QCOMPARE(mWaitFor2_1->receiverName(), QString("The new receiver name"));
    QCOMPARE(mWaitFor2_1->eventName(), QString("The new event name"));
}

void WaitForWidgetTest::testEditWaitForProperty() {
    selectItem(getIndex(2, getIndex(1, getIndex(0))));

    //The dialog is modal, so it won't return to the test code until it is
    //closed. Thus, the commands to execute on the dialog must be "queued",
    //as calling setPropertyData after the button click won't work.
    QTimer::singleShot(500, this, SLOT(setPropertyData()));

    button("editButton")->click();

    QCOMPARE(mWaitFor2_3->objectName(), QString("The new object name"));
    QCOMPARE(mWaitFor2_3->propertyName(), QString("The new property name"));
    QCOMPARE(mWaitFor2_3->value(), QString("The new value"));
}

void WaitForWidgetTest::testEditWaitForSignal() {
    selectItem(getIndex(0, getIndex(0)));

    //The dialog is modal, so it won't return to the test code until it is
    //closed. Thus, the commands to execute on the dialog must be "queued",
    //as calling setSignalData after the button click won't work.
    QTimer::singleShot(500, this, SLOT(setSignalData()));

    button("editButton")->click();

    QCOMPARE(mWaitFor1->emitterName(), QString("The new emitter name"));
    QCOMPARE(mWaitFor1->signalName(), QString("The new signal name"));
}

void WaitForWidgetTest::testEditWaitForWindow() {
    selectItem(getIndex(1, getIndex(1, getIndex(0))));

    //The dialog is modal, so it won't return to the test code until it is
    //closed. Thus, the commands to execute on the dialog must be "queued",
    //as calling setWindowData after the button click won't work.
    QTimer::singleShot(500, this, SLOT(setWindowData()));

    button("editButton")->click();

    QCOMPARE(mWaitFor2_2->windowObjectName(),
             QString("The new window object name"));
}

void WaitForWidgetTest::testRemoveWaitForFromRoot() {
    delete mWidget;
    WaitForSignal* waitForSignal = new WaitForSignal();
    mWidget = new WaitForWidget(waitForSignal);

    selectItem(getIndex(0));
    button("removeButton")->click();

    QVERIFY(!mWidget->waitFor());
    QVERIFY(!getIndex(0).isValid());
    assertButtonEnabled(true, false, false);
}

void WaitForWidgetTest::testRemoveWaitForFromWaitForComposed() {
    selectItem(getIndex(2, getIndex(0)));
    button("removeButton")->click();

    QCOMPARE(mWaitFor->waitFors().count(), 3);
    QCOMPARE(mWaitFor->waitFors()[0], mWaitFor1);
    QCOMPARE(mWaitFor->waitFors()[1], mWaitFor2);
    QCOMPARE(mWaitFor->waitFors()[2], mWaitFor4);
}

void WaitForWidgetTest::testRemoveWaitForFromWaitForNot() {
    selectItem(getIndex(0, getIndex(2, getIndex(0))));
    button("removeButton")->click();

    QCOMPARE(mWaitFor->waitFors().count(), 4);
    QCOMPARE(mWaitFor->waitFors()[0], mWaitFor1);
    QCOMPARE(mWaitFor->waitFors()[1], mWaitFor2);
    QCOMPARE(mWaitFor->waitFors()[2], mWaitFor3);
    QCOMPARE(mWaitFor->waitFors()[3], mWaitFor4);
    QVERIFY(!mWaitFor3->negatedWaitFor());
}

/////////////////////////////////// Helpers ////////////////////////////////////

void WaitForWidgetTest::addWaitForSignal() const {
    KComboBox* comboBox = mWidget->findChild<KComboBox*>("waitForTypeComboBox");
    QVERIFY(comboBox);
    comboBox->setCurrentIndex(3);

    KDialog* dialog = mWidget->findChild<KDialog*>("addWaitForDialog");
    QVERIFY(dialog);
    dialog->button(KDialog::Ok)->click();
}

void WaitForWidgetTest::cancelAddWaitForDialog() const {
    KDialog* dialog = mWidget->findChild<KDialog*>("addWaitForDialog");
    QVERIFY(dialog);
    dialog->button(KDialog::Cancel)->click();
}

void WaitForWidgetTest::setEventData() const {
    KLineEdit* receiverNameLineEdit =
#ifdef QT_QTDBUS_FOUND
                        mWidget->findChild<KLineEdit*>("objectNameLineEdit");
#else
                        mWidget->findChild<KLineEdit*>("receiverNameLineEdit");
#endif
    QVERIFY(receiverNameLineEdit);
    receiverNameLineEdit->setText("The new receiver name");

    KLineEdit* eventNameLineEdit =
                        mWidget->findChild<KLineEdit*>("eventNameLineEdit");
    QVERIFY(eventNameLineEdit);
    eventNameLineEdit->setText("The new event name");

    EditionDialog* dialog =
                mWidget->findChild<EditionDialog*>("editWaitForEventDialog");
    QVERIFY(dialog);
    dialog->button(KDialog::Ok)->click();
}

void WaitForWidgetTest::setPropertyData() const {
    KLineEdit* objectNameLineEdit =
                        mWidget->findChild<KLineEdit*>("objectNameLineEdit");
    QVERIFY(objectNameLineEdit);
    objectNameLineEdit->setText("The new object name");

    KLineEdit* propertyNameLineEdit =
                        mWidget->findChild<KLineEdit*>("propertyNameLineEdit");
    QVERIFY(propertyNameLineEdit);
    propertyNameLineEdit->setText("The new property name");

    KLineEdit* valueLineEdit = mWidget->findChild<KLineEdit*>("valueLineEdit");
    QVERIFY(valueLineEdit);
    valueLineEdit->setText("The new value");

    EditionDialog* dialog =
                mWidget->findChild<EditionDialog*>("editWaitForPropertyDialog");
    QVERIFY(dialog);
    dialog->button(KDialog::Ok)->click();
}

void WaitForWidgetTest::setSignalData() const {
    KLineEdit* emitterNameLineEdit =
#ifdef QT_QTDBUS_FOUND
                        mWidget->findChild<KLineEdit*>("objectNameLineEdit");
#else
                        mWidget->findChild<KLineEdit*>("emitterNameLineEdit");
#endif
    QVERIFY(emitterNameLineEdit);
    emitterNameLineEdit->setText("The new emitter name");

    KLineEdit* signalNameLineEdit =
                        mWidget->findChild<KLineEdit*>("signalNameLineEdit");
    QVERIFY(signalNameLineEdit);
    signalNameLineEdit->setText("The new signal name");

    EditionDialog* dialog =
                mWidget->findChild<EditionDialog*>("editWaitForSignalDialog");
    QVERIFY(dialog);
    dialog->button(KDialog::Ok)->click();
}

void WaitForWidgetTest::setWindowData() const {
    KLineEdit* windowObjectNameLineEdit =
#ifdef QT_QTDBUS_FOUND
                    mWidget->findChild<KLineEdit*>("objectNameLineEdit");
#else
                    mWidget->findChild<KLineEdit*>("windowObjectNameLineEdit");
#endif
    QVERIFY(windowObjectNameLineEdit);
    windowObjectNameLineEdit->setText("The new window object name");

    EditionDialog* dialog =
                mWidget->findChild<EditionDialog*>("editWaitForWindowDialog");
    QVERIFY(dialog);
    dialog->button(KDialog::Ok)->click();
}

QModelIndex WaitForWidgetTest::getIndex(int row,
                                        const QModelIndex& parent) const {
    QTreeView* tree = mWidget->findChild<QTreeView*>("waitForTreeView");
    return tree->model()->index(row, 0, parent);
}

void WaitForWidgetTest::selectItem(const QModelIndex& index) const {
    QTreeView* tree = mWidget->findChild<QTreeView*>("waitForTreeView");
    QItemSelectionModel* model = tree->selectionModel();
    model->select(index, QItemSelectionModel::SelectCurrent);
}

KPushButton* WaitForWidgetTest::button(const QString& name,
                                       WaitForWidget* widget) const {
    if (!widget) {
        widget = mWidget;
    }

    return widget->findChild<KPushButton*>(name);
}

void WaitForWidgetTest::assertButtonEnabled(bool addButtonEnabled,
                                            bool editButtonEnabled,
                                            bool removeButtonEnabled,
                                            WaitForWidget* widget) const {
    if (!widget) {
        widget = mWidget;
    }

    KPushButton* addButton = button("addButton", widget);
    QVERIFY(addButton);
    QCOMPARE(addButton->isEnabled(), addButtonEnabled);

    KPushButton* editButton = button("editButton", widget);
    QVERIFY(editButton);
    QCOMPARE(editButton->isEnabled(), editButtonEnabled);

    KPushButton* removeButton = button("removeButton", widget);
    QVERIFY(removeButton);
    QCOMPARE(removeButton->isEnabled(), removeButtonEnabled);
}

QTEST_MAIN(WaitForWidgetTest)

#include "WaitForWidgetTest.moc"
