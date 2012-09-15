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

#include "TutorialCommands.h"

#include <QUndoCommand>

#include <KLocalizedString>

#include "../data/Step.h"
#include "../data/Tutorial.h"

class TutorialCommandsTest: public QObject {
Q_OBJECT

private slots:

    void testSetNameRedo();
    void testSetNameUndo();

    void testSetDescriptionRedo();
    void testSetDescriptionUndo();

    void testSetLicenseTextRedo();
    void testSetLicenseTextUndo();

    void testSetCustomSetupCodeRedo();
    void testSetCustomSetupCodeUndo();

    void testSetCustomTearDownCodeRedo();
    void testSetCustomTearDownCodeUndo();

    void testAddStepRedo();
    void testAddStepUndo();

    void testAddStepAtIndexRedo();
    void testAddStepAtIndexUndo();

    void testRemoveStepRedo();
    void testRemoveStepUndo();
    void testRemoveStepAtMiddleUndo();

};

void TutorialCommandsTest::testSetNameRedo() {
    Tutorial tutorial;
    TutorialCommands commands(&tutorial);
    QUndoCommand parent;

    QUndoCommand* command = commands.setName("The name", &parent);
    command->redo();

    QCOMPARE(command->text(), i18nc("@action", "Set tutorial name"));
    QCOMPARE(parent.child(0), command);
    QCOMPARE(tutorial.name(), QString("The name"));
}

void TutorialCommandsTest::testSetNameUndo() {
    Tutorial tutorial;
    TutorialCommands commands(&tutorial);
    QUndoCommand parent;

    tutorial.setName("The old name");

    QUndoCommand* command = commands.setName("The new name", &parent);
    command->redo();
    command->undo();

    QCOMPARE(command->text(), i18nc("@action", "Set tutorial name"));
    QCOMPARE(parent.child(0), command);
    QCOMPARE(tutorial.name(), QString("The old name"));
}

void TutorialCommandsTest::testSetDescriptionRedo() {
    Tutorial tutorial;
    TutorialCommands commands(&tutorial);
    QUndoCommand parent;

    QUndoCommand* command = commands.setDescription("The description", &parent);
    command->redo();

    QCOMPARE(command->text(), i18nc("@action", "Set tutorial description"));
    QCOMPARE(parent.child(0), command);
    QCOMPARE(tutorial.description(), QString("The description"));
}

void TutorialCommandsTest::testSetDescriptionUndo() {
    Tutorial tutorial;
    TutorialCommands commands(&tutorial);
    QUndoCommand parent;

    tutorial.setDescription("The old description");

    QUndoCommand* command = commands.setDescription("The new description",
                                                    &parent);
    command->redo();
    command->undo();

    QCOMPARE(command->text(), i18nc("@action", "Set tutorial description"));
    QCOMPARE(parent.child(0), command);
    QCOMPARE(tutorial.description(), QString("The old description"));
}

void TutorialCommandsTest::testSetLicenseTextRedo() {
    Tutorial tutorial;
    TutorialCommands commands(&tutorial);
    QUndoCommand parent;

    QUndoCommand* command = commands.setLicenseText("The license text",
                                                    &parent);
    command->redo();

    QCOMPARE(command->text(), i18nc("@action", "Set tutorial license"));
    QCOMPARE(parent.child(0), command);
    QCOMPARE(tutorial.licenseText(), QString("The license text"));
}

void TutorialCommandsTest::testSetLicenseTextUndo() {
    Tutorial tutorial;
    TutorialCommands commands(&tutorial);
    QUndoCommand parent;

    tutorial.setLicenseText("The old license text");

    QUndoCommand* command = commands.setLicenseText("The new license text",
                                                    &parent);
    command->redo();
    command->undo();

    QCOMPARE(command->text(), i18nc("@action", "Set tutorial license"));
    QCOMPARE(parent.child(0), command);
    QCOMPARE(tutorial.licenseText(), QString("The old license text"));
}

void TutorialCommandsTest::testSetCustomSetupCodeRedo() {
    Tutorial tutorial;
    TutorialCommands commands(&tutorial);
    QUndoCommand parent;

    QUndoCommand* command = commands.setCustomSetupCode("The custom setup code",
                                                        &parent);
    command->redo();

    QCOMPARE(command->text(), i18nc("@action", "Set tutorial setup code"));
    QCOMPARE(parent.child(0), command);
    QCOMPARE(tutorial.customSetupCode(), QString("The custom setup code"));
}

void TutorialCommandsTest::testSetCustomSetupCodeUndo() {
    Tutorial tutorial;
    TutorialCommands commands(&tutorial);
    QUndoCommand parent;

    tutorial.setCustomSetupCode("The old custom setup code");

    QUndoCommand* command = commands.setCustomSetupCode(
                                        "The new custom setup code", &parent);
    command->redo();
    command->undo();

    QCOMPARE(command->text(), i18nc("@action", "Set tutorial setup code"));
    QCOMPARE(parent.child(0), command);
    QCOMPARE(tutorial.customSetupCode(), QString("The old custom setup code"));
}

void TutorialCommandsTest::testSetCustomTearDownCodeRedo() {
    Tutorial tutorial;
    TutorialCommands commands(&tutorial);
    QUndoCommand parent;

    QUndoCommand* command = commands.setCustomTearDownCode(
                                        "The custom tear down code", &parent);
    command->redo();

    QCOMPARE(command->text(), i18nc("@action", "Set tutorial tear down code"));
    QCOMPARE(parent.child(0), command);
    QCOMPARE(tutorial.customTearDownCode(),
             QString("The custom tear down code"));
}

void TutorialCommandsTest::testSetCustomTearDownCodeUndo() {
    Tutorial tutorial;
    TutorialCommands commands(&tutorial);
    QUndoCommand parent;

    tutorial.setCustomTearDownCode("The old custom tear down code");

    QUndoCommand* command = commands.setCustomTearDownCode(
                                    "The new custom tear down code", &parent);
    command->redo();
    command->undo();

    QCOMPARE(command->text(), i18nc("@action", "Set tutorial tear down code"));
    QCOMPARE(parent.child(0), command);
    QCOMPARE(tutorial.customTearDownCode(),
             QString("The old custom tear down code"));
}

void TutorialCommandsTest::testAddStepRedo() {
    Tutorial tutorial;
    TutorialCommands commands(&tutorial);
    QUndoCommand parent;
    Step* step = new Step();

    QUndoCommand* command = commands.addStep(step, &parent);
    command->redo();

    QCOMPARE(command->text(), i18nc("@action", "Add step"));
    QCOMPARE(parent.child(0), command);
    QCOMPARE(tutorial.steps().count(), 1);
    QCOMPARE(tutorial.steps()[0], step);
}

void TutorialCommandsTest::testAddStepUndo() {
    Tutorial tutorial;
    TutorialCommands commands(&tutorial);
    QUndoCommand parent;
    Step* step = new Step();

    QUndoCommand* command = commands.addStep(step, &parent);
    command->redo();
    command->undo();

    QCOMPARE(command->text(), i18nc("@action", "Add step"));
    QCOMPARE(parent.child(0), command);
    QCOMPARE(tutorial.steps().count(), 0);
}

void TutorialCommandsTest::testAddStepAtIndexRedo() {
    Tutorial tutorial;
    TutorialCommands commands(&tutorial);
    QUndoCommand parent;
    Step* step1 = new Step();
    Step* step2 = new Step();
    Step* step3 = new Step();

    tutorial.addStep(step1);
    tutorial.addStep(step3);

    QUndoCommand* command = commands.addStep(step2, 1, &parent);
    command->redo();

    QCOMPARE(command->text(), i18nc("@action", "Add step"));
    QCOMPARE(parent.child(0), command);
    QCOMPARE(tutorial.steps().count(), 3);
    QCOMPARE(tutorial.steps()[0], step1);
    QCOMPARE(tutorial.steps()[1], step2);
    QCOMPARE(tutorial.steps()[2], step3);
}

void TutorialCommandsTest::testAddStepAtIndexUndo() {
    Tutorial tutorial;
    TutorialCommands commands(&tutorial);
    QUndoCommand parent;
    Step* step1 = new Step();
    Step* step2 = new Step();
    Step* step3 = new Step();

    tutorial.addStep(step1);
    tutorial.addStep(step3);

    QUndoCommand* command = commands.addStep(step2, 1, &parent);
    command->redo();
    command->undo();

    QCOMPARE(command->text(), i18nc("@action", "Add step"));
    QCOMPARE(parent.child(0), command);
    QCOMPARE(tutorial.steps().count(), 2);
    QCOMPARE(tutorial.steps()[0], step1);
    QCOMPARE(tutorial.steps()[1], step3);
}

void TutorialCommandsTest::testRemoveStepRedo() {
    Tutorial tutorial;
    TutorialCommands commands(&tutorial);
    QUndoCommand parent;
    Step* step = new Step();

    tutorial.addStep(step);

    QUndoCommand* command = commands.removeStep(step, &parent);
    command->redo();

    QCOMPARE(command->text(), i18nc("@action", "Remove step"));
    QCOMPARE(parent.child(0), command);
    QCOMPARE(tutorial.steps().count(), 0);
}

void TutorialCommandsTest::testRemoveStepUndo() {
    Tutorial tutorial;
    TutorialCommands commands(&tutorial);
    QUndoCommand parent;
    Step* step = new Step();

    tutorial.addStep(step);

    QUndoCommand* command = commands.removeStep(step, &parent);
    command->redo();
    command->undo();

    QCOMPARE(command->text(), i18nc("@action", "Remove step"));
    QCOMPARE(parent.child(0), command);
    QCOMPARE(tutorial.steps().count(), 1);
    QCOMPARE(tutorial.steps()[0], step);
}

void TutorialCommandsTest::testRemoveStepAtMiddleUndo() {
    Tutorial tutorial;
    TutorialCommands commands(&tutorial);
    QUndoCommand parent;
    Step* step1 = new Step();
    Step* step2 = new Step();
    Step* step3 = new Step();

    tutorial.addStep(step1);
    tutorial.addStep(step2);
    tutorial.addStep(step3);

    QUndoCommand* command = commands.removeStep(step2, &parent);
    command->redo();
    command->undo();

    QCOMPARE(command->text(), i18nc("@action", "Remove step"));
    QCOMPARE(parent.child(0), command);
    QCOMPARE(tutorial.steps().count(), 3);
    QCOMPARE(tutorial.steps()[0], step1);
    QCOMPARE(tutorial.steps()[1], step2);
    QCOMPARE(tutorial.steps()[2], step3);
}

QTEST_MAIN(TutorialCommandsTest)

#include "TutorialCommandsTest.moc"
