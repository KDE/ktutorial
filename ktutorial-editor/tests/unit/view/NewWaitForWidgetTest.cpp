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

#include "NewWaitForWidget.h"

#include <KComboBox>

#include "../data/WaitForComposed.h"
#include "../data/WaitForEvent.h"
#include "../data/WaitForNot.h"
#include "../data/WaitForProperty.h"
#include "../data/WaitForSignal.h"
#include "../data/WaitForStepActivation.h"
#include "../data/WaitForWindow.h"

class NewWaitForWidgetTest: public QObject {
Q_OBJECT

private slots:

    void testConstructor();

    void testWaitForWhenAndConditionIsSelected();
    void testWaitForWhenOrConditionIsSelected();
    void testWaitForWhenNotConditionIsSelected();
    void testWaitForWhenSignalConditionIsSelected();
    void testWaitForWhenEventConditionIsSelected();
    void testWaitForWhenWindowConditionIsSelected();
    void testWaitForWhenPropertyConditionIsSelected();
    void testWaitForWhenStepActivationConditionIsSelected();

private:

    void selectOption(NewWaitForWidget* widget, int index) const;

};

void NewWaitForWidgetTest::testConstructor() {
    QWidget parent;
    NewWaitForWidget* widget = new NewWaitForWidget(&parent);

    QCOMPARE(widget->parentWidget(), &parent);
}

void NewWaitForWidgetTest::testWaitForWhenAndConditionIsSelected() {
    NewWaitForWidget widget;

    selectOption(&widget, 0);

    WaitForComposed* waitFor = qobject_cast<WaitForComposed*>(widget.waitFor());
    QVERIFY(waitFor);
    QCOMPARE(waitFor->compositionType(), WaitForComposed::And);
    delete waitFor;
}

void NewWaitForWidgetTest::testWaitForWhenOrConditionIsSelected() {
    NewWaitForWidget widget;

    selectOption(&widget, 1);

    WaitForComposed* waitFor = qobject_cast<WaitForComposed*>(widget.waitFor());
    QVERIFY(waitFor);
    QCOMPARE(waitFor->compositionType(), WaitForComposed::Or);
    delete waitFor;
}

void NewWaitForWidgetTest::testWaitForWhenNotConditionIsSelected() {
    NewWaitForWidget widget;

    selectOption(&widget, 2);

    WaitForNot* waitFor = qobject_cast<WaitForNot*>(widget.waitFor());
    QVERIFY(waitFor);
    delete waitFor;
}

void NewWaitForWidgetTest::testWaitForWhenSignalConditionIsSelected() {
    NewWaitForWidget widget;

    selectOption(&widget, 3);

    WaitForSignal* waitFor = qobject_cast<WaitForSignal*>(widget.waitFor());
    QVERIFY(waitFor);
    delete waitFor;
}

void NewWaitForWidgetTest::testWaitForWhenEventConditionIsSelected() {
    NewWaitForWidget widget;

    selectOption(&widget, 4);

    WaitForEvent* waitFor = qobject_cast<WaitForEvent*>(widget.waitFor());
    QVERIFY(waitFor);
    delete waitFor;
}

void NewWaitForWidgetTest::testWaitForWhenWindowConditionIsSelected() {
    NewWaitForWidget widget;

    selectOption(&widget, 5);

    WaitForWindow* waitFor = qobject_cast<WaitForWindow*>(widget.waitFor());
    QVERIFY(waitFor);
    delete waitFor;
}

void NewWaitForWidgetTest::testWaitForWhenPropertyConditionIsSelected() {
    NewWaitForWidget widget;

    selectOption(&widget, 6);

    WaitForProperty* waitFor = qobject_cast<WaitForProperty*>(widget.waitFor());
    QVERIFY(waitFor);
    delete waitFor;
}

void NewWaitForWidgetTest::testWaitForWhenStepActivationConditionIsSelected() {
    NewWaitForWidget widget;

    selectOption(&widget, 7);

    WaitForStepActivation* waitFor =
                        qobject_cast<WaitForStepActivation*>(widget.waitFor());
    QVERIFY(waitFor);
    delete waitFor;
}

/////////////////////////////////// Helpers ////////////////////////////////////

void NewWaitForWidgetTest::selectOption(NewWaitForWidget* widget,
                                        int index) const {
    KComboBox* comboBox = widget->findChild<KComboBox*>("waitForTypeComboBox");

    QVERIFY(comboBox);
    comboBox->setCurrentIndex(index);
}

QTEST_MAIN(NewWaitForWidgetTest)

#include "NewWaitForWidgetTest.moc"
