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

#include "StepDataWidget.h"

#include <KLineEdit>
#include <KTextEdit>

#include "../data/Reaction.h"
#include "../data/Step.h"
#include "../data/Tutorial.h"

class StepDataWidgetTest: public QObject {
Q_OBJECT

private slots:

    void testConstructor();
    void testConstructorWithRichText();

    void testEnableStepIdCompletion();

    void testSaveChanges();

private:

    KLineEdit* idLineEdit(StepDataWidget* widget) const;
    KTextEdit* textTextEdit(StepDataWidget* widget) const;

};

void StepDataWidgetTest::testConstructor() {
    Step step;
    step.setId("The id");
    step.setText("The text");

    QWidget parent;
    StepDataWidget* widget = new StepDataWidget(&step, &parent);

    QCOMPARE(widget->parentWidget(), &parent);
    QCOMPARE(idLineEdit(widget)->text(), QString("The id"));
    QCOMPARE(textTextEdit(widget)->toPlainText(), QString("The text"));
}

void StepDataWidgetTest::testConstructorWithRichText() {
    Step step;
    step.setId("The id");
    step.setText("<p>The text</p>");

    QWidget parent;
    StepDataWidget* widget = new StepDataWidget(&step, &parent);

    QCOMPARE(widget->parentWidget(), &parent);
    QCOMPARE(idLineEdit(widget)->text(), QString("The id"));
    QCOMPARE(textTextEdit(widget)->toPlainText(), QString("<p>The text</p>"));
}

void StepDataWidgetTest::testEnableStepIdCompletion() {
    Step* step = new Step;

    StepDataWidget widget(step);

    Tutorial tutorial;
    tutorial.addStep(step);

    Reaction* stepFirstReaction = new Reaction();
    stepFirstReaction->setNextStepId("Unused id in the step of the widget");
    step->addReaction(stepFirstReaction);

    Step* emptyStep = new Step();
    emptyStep->setId("");
    tutorial.addStep(emptyStep);

    Step* firstStep = new Step();
    firstStep->setId("First step");
    tutorial.addStep(firstStep);

    Reaction* firstStepFirstReaction = new Reaction();
    firstStepFirstReaction->setNextStepId("Second step");
    firstStep->addReaction(firstStepFirstReaction);

    Reaction* firstStepSecondReaction = new Reaction();
    firstStepSecondReaction->setNextStepId("Unused id");
    firstStep->addReaction(firstStepSecondReaction);

    Reaction* firstStepThirdReaction = new Reaction();
    firstStepThirdReaction->setNextStepId("Duplicated unused id");
    firstStep->addReaction(firstStepThirdReaction);

    Reaction* firstStepEmptyReaction = new Reaction();
    firstStep->addReaction(firstStepEmptyReaction);

    Step* secondStep = new Step();
    secondStep->setId("Second step");
    tutorial.addStep(secondStep);

    Reaction* secondStepFirstReaction = new Reaction();
    secondStepFirstReaction->setNextStepId("Duplicated unused id");
    secondStep->addReaction(secondStepFirstReaction);

    Reaction* secondStepSecondReaction = new Reaction();
    secondStepSecondReaction->setNextStepId("Another unused id");
    secondStep->addReaction(secondStepSecondReaction);

    widget.enableStepIdCompletion(&tutorial);

    QStringList completionItems =
                            idLineEdit(&widget)->completionObject()->items();
    QCOMPARE(completionItems.count(), 3);
    QVERIFY(completionItems.contains("Unused id"));
    QVERIFY(completionItems.contains("Duplicated unused id"));
    QVERIFY(completionItems.contains("Another unused id"));
}

void StepDataWidgetTest::testSaveChanges() {
    Step step;
    step.setId("The id");
    step.setText("The text");

    StepDataWidget widget(&step);
    idLineEdit(&widget)->setText("The new id");
    textTextEdit(&widget)->setText("The new text");

    widget.saveChanges();

    QCOMPARE(step.id(), QString("The new id"));
    QCOMPARE(step.text(), QString("The new text"));
}

/////////////////////////////////// Helpers ////////////////////////////////////

KLineEdit* StepDataWidgetTest::idLineEdit(StepDataWidget* widget) const {
    return widget->findChild<KLineEdit*>("idLineEdit");
}

KTextEdit* StepDataWidgetTest::textTextEdit(StepDataWidget* widget) const {
    return widget->findChild<KTextEdit*>("textTextEdit");
}

QTEST_MAIN(StepDataWidgetTest)

#include "StepDataWidgetTest.moc"
