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

#include "StepCustomCodeWidget.h"

#include <KLineEdit>
#include <KTextEdit>

#include "../data/Step.h"

class StepCustomCodeWidgetTest: public QObject {
Q_OBJECT

private slots:

    void testConstructorSetup();
    void testConstructorSetupWithRichText();
    void testConstructorTearDown();
    void testConstructorTearDownWithRichText();

    void testSaveChangesSetup();
    void testSaveChangesTearDown();

private:

    KTextEdit* customCodeTextEdit(StepCustomCodeWidget* widget) const;

};

void StepCustomCodeWidgetTest::testConstructorSetup() {
    Step step;
    step.setCustomSetupCode("The setup code");

    QWidget parent;
    StepCustomCodeWidget* widget =
                        new StepCustomCodeWidget(&step,
                                                 StepCustomCodeWidget::Setup,
                                                 &parent);

    QCOMPARE(widget->parentWidget(), &parent);
    QCOMPARE(customCodeTextEdit(widget)->toPlainText(),
             QString("The setup code"));
}

void StepCustomCodeWidgetTest::testConstructorSetupWithRichText() {
    Step step;
    step.setCustomSetupCode("<p>The setup code</p>");

    QWidget parent;
    StepCustomCodeWidget* widget =
                        new StepCustomCodeWidget(&step,
                                                 StepCustomCodeWidget::Setup,
                                                 &parent);

    QCOMPARE(widget->parentWidget(), &parent);
    QCOMPARE(customCodeTextEdit(widget)->toPlainText(),
             QString("<p>The setup code</p>"));
}

void StepCustomCodeWidgetTest::testConstructorTearDown() {
    Step step;
    step.setCustomTearDownCode("The tear down code");

    QWidget parent;
    StepCustomCodeWidget* widget =
                        new StepCustomCodeWidget(&step,
                                                 StepCustomCodeWidget::TearDown,
                                                 &parent);

    QCOMPARE(widget->parentWidget(), &parent);
    QCOMPARE(customCodeTextEdit(widget)->toPlainText(),
             QString("The tear down code"));
}

void StepCustomCodeWidgetTest::testConstructorTearDownWithRichText() {
    Step step;
    step.setCustomTearDownCode("<p>The tear down code</p>");

    QWidget parent;
    StepCustomCodeWidget* widget =
                        new StepCustomCodeWidget(&step,
                                                 StepCustomCodeWidget::TearDown,
                                                 &parent);

    QCOMPARE(widget->parentWidget(), &parent);
    QCOMPARE(customCodeTextEdit(widget)->toPlainText(),
             QString("<p>The tear down code</p>"));
}

void StepCustomCodeWidgetTest::testSaveChangesSetup() {
    Step step;
    step.setCustomSetupCode("The setup code");

    StepCustomCodeWidget widget(&step, StepCustomCodeWidget::Setup);
    customCodeTextEdit(&widget)->setText("The new setup code");

    widget.saveChanges();

    QCOMPARE(step.customSetupCode(), QString("The new setup code"));
}

void StepCustomCodeWidgetTest::testSaveChangesTearDown() {
    Step step;
    step.setCustomTearDownCode("The tear down code");

    StepCustomCodeWidget widget(&step, StepCustomCodeWidget::TearDown);
    customCodeTextEdit(&widget)->setText("The new tear down code");

    widget.saveChanges();

    QCOMPARE(step.customTearDownCode(), QString("The new tear down code"));
}

/////////////////////////////////// Helpers ////////////////////////////////////

KTextEdit* StepCustomCodeWidgetTest::customCodeTextEdit(
                                        StepCustomCodeWidget* widget) const {
    return widget->findChild<KTextEdit*>("customCodeTextEdit");
}

QTEST_MAIN(StepCustomCodeWidgetTest)

#include "StepCustomCodeWidgetTest.moc"
