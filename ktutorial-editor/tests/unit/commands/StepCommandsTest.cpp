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

#include "StepCommands.h"

#include <QUndoCommand>

#include <KLocalizedString>

#include "../data/Reaction.h"
#include "../data/Step.h"

class StepCommandsTest: public QObject {
Q_OBJECT

private slots:

    void testSetIdRedo();
    void testSetIdUndo();

    void testSetTextRedo();
    void testSetTextUndo();

    void testSetCustomSetupCodeRedo();
    void testSetCustomSetupCodeUndo();

    void testSetCustomTearDownCodeRedo();
    void testSetCustomTearDownCodeUndo();

    void testAddReactionRedo();
    void testAddReactionUndo();

    void testAddReactionAtIndexRedo();
    void testAddReactionAtIndexUndo();

    void testRemoveReactionRedo();
    void testRemoveReactionUndo();
    void testRemoveReactionAtMiddleUndo();

};

void StepCommandsTest::testSetIdRedo() {
    Step step;
    StepCommands commands(&step);
    QUndoCommand parent;

    QUndoCommand* command = commands.setId("The id", &parent);
    command->redo();

    QCOMPARE(command->text(), i18nc("@action", "Set step id"));
    QCOMPARE(parent.child(0), command);
    QCOMPARE(step.id(), QString("The id"));
}

void StepCommandsTest::testSetIdUndo() {
    Step step;
    StepCommands commands(&step);
    QUndoCommand parent;

    step.setId("The old id");

    QUndoCommand* command = commands.setId("The new id", &parent);
    command->redo();
    command->undo();

    QCOMPARE(command->text(), i18nc("@action", "Set step id"));
    QCOMPARE(parent.child(0), command);
    QCOMPARE(step.id(), QString("The old id"));
}

void StepCommandsTest::testSetTextRedo() {
    Step step;
    StepCommands commands(&step);
    QUndoCommand parent;

    QUndoCommand* command = commands.setText("The text", &parent);
    command->redo();

    QCOMPARE(command->text(), i18nc("@action", "Set step text"));
    QCOMPARE(parent.child(0), command);
    QCOMPARE(step.text(), QString("The text"));
}

void StepCommandsTest::testSetTextUndo() {
    Step step;
    StepCommands commands(&step);
    QUndoCommand parent;

    step.setText("The old text");

    QUndoCommand* command = commands.setText("The new text", &parent);
    command->redo();
    command->undo();

    QCOMPARE(command->text(), i18nc("@action", "Set step text"));
    QCOMPARE(parent.child(0), command);
    QCOMPARE(step.text(), QString("The old text"));
}

void StepCommandsTest::testSetCustomSetupCodeRedo() {
    Step step;
    StepCommands commands(&step);
    QUndoCommand parent;

    QUndoCommand* command = commands.setCustomSetupCode("The custom setup code",
                                                        &parent);
    command->redo();

    QCOMPARE(command->text(), i18nc("@action", "Set step setup code"));
    QCOMPARE(parent.child(0), command);
    QCOMPARE(step.customSetupCode(), QString("The custom setup code"));
}

void StepCommandsTest::testSetCustomSetupCodeUndo() {
    Step step;
    StepCommands commands(&step);
    QUndoCommand parent;

    step.setCustomSetupCode("The old custom setup code");

    QUndoCommand* command = commands.setCustomSetupCode(
                                        "The new custom setup code", &parent);
    command->redo();
    command->undo();

    QCOMPARE(command->text(), i18nc("@action", "Set step setup code"));
    QCOMPARE(parent.child(0), command);
    QCOMPARE(step.customSetupCode(), QString("The old custom setup code"));
}

void StepCommandsTest::testSetCustomTearDownCodeRedo() {
    Step step;
    StepCommands commands(&step);
    QUndoCommand parent;

    QUndoCommand* command = commands.setCustomTearDownCode(
                                        "The custom tear down code", &parent);
    command->redo();

    QCOMPARE(command->text(), i18nc("@action", "Set step tear down code"));
    QCOMPARE(parent.child(0), command);
    QCOMPARE(step.customTearDownCode(),
             QString("The custom tear down code"));
}

void StepCommandsTest::testSetCustomTearDownCodeUndo() {
    Step step;
    StepCommands commands(&step);
    QUndoCommand parent;

    step.setCustomTearDownCode("The old custom tear down code");

    QUndoCommand* command = commands.setCustomTearDownCode(
                                    "The new custom tear down code", &parent);
    command->redo();
    command->undo();

    QCOMPARE(command->text(), i18nc("@action", "Set step tear down code"));
    QCOMPARE(parent.child(0), command);
    QCOMPARE(step.customTearDownCode(),
             QString("The old custom tear down code"));
}

void StepCommandsTest::testAddReactionRedo() {
    Step step;
    StepCommands commands(&step);
    QUndoCommand parent;
    Reaction* reaction = new Reaction();

    QUndoCommand* command = commands.addReaction(reaction, &parent);
    command->redo();

    QCOMPARE(command->text(), i18nc("@action", "Add reaction"));
    QCOMPARE(parent.child(0), command);
    QCOMPARE(step.reactions().count(), 1);
    QCOMPARE(step.reactions()[0], reaction);
}

void StepCommandsTest::testAddReactionUndo() {
    Step step;
    StepCommands commands(&step);
    QUndoCommand parent;
    Reaction* reaction = new Reaction();

    QUndoCommand* command = commands.addReaction(reaction, &parent);
    command->redo();
    command->undo();

    QCOMPARE(command->text(), i18nc("@action", "Add reaction"));
    QCOMPARE(parent.child(0), command);
    QCOMPARE(step.reactions().count(), 0);
}

void StepCommandsTest::testAddReactionAtIndexRedo() {
    Step step;
    StepCommands commands(&step);
    QUndoCommand parent;
    Reaction* reaction1 = new Reaction();
    Reaction* reaction2 = new Reaction();
    Reaction* reaction3 = new Reaction();

    step.addReaction(reaction1);
    step.addReaction(reaction3);

    QUndoCommand* command = commands.addReaction(reaction2, 1, &parent);
    command->redo();

    QCOMPARE(command->text(), i18nc("@action", "Add reaction"));
    QCOMPARE(parent.child(0), command);
    QCOMPARE(step.reactions().count(), 3);
    QCOMPARE(step.reactions()[0], reaction1);
    QCOMPARE(step.reactions()[1], reaction2);
    QCOMPARE(step.reactions()[2], reaction3);
}

void StepCommandsTest::testAddReactionAtIndexUndo() {
    Step step;
    StepCommands commands(&step);
    QUndoCommand parent;
    Reaction* reaction1 = new Reaction();
    Reaction* reaction2 = new Reaction();
    Reaction* reaction3 = new Reaction();

    step.addReaction(reaction1);
    step.addReaction(reaction3);

    QUndoCommand* command = commands.addReaction(reaction2, 1, &parent);
    command->redo();
    command->undo();

    QCOMPARE(command->text(), i18nc("@action", "Add reaction"));
    QCOMPARE(parent.child(0), command);
    QCOMPARE(step.reactions().count(), 2);
    QCOMPARE(step.reactions()[0], reaction1);
    QCOMPARE(step.reactions()[1], reaction3);
}

void StepCommandsTest::testRemoveReactionRedo() {
    Step step;
    StepCommands commands(&step);
    QUndoCommand parent;
    Reaction* reaction = new Reaction();

    step.addReaction(reaction);

    QUndoCommand* command = commands.removeReaction(reaction, &parent);
    command->redo();

    QCOMPARE(command->text(), i18nc("@action", "Remove reaction"));
    QCOMPARE(parent.child(0), command);
    QCOMPARE(step.reactions().count(), 0);
}

void StepCommandsTest::testRemoveReactionUndo() {
    Step step;
    StepCommands commands(&step);
    QUndoCommand parent;
    Reaction* reaction = new Reaction();

    step.addReaction(reaction);

    QUndoCommand* command = commands.removeReaction(reaction, &parent);
    command->redo();
    command->undo();

    QCOMPARE(command->text(), i18nc("@action", "Remove reaction"));
    QCOMPARE(parent.child(0), command);
    QCOMPARE(step.reactions().count(), 1);
    QCOMPARE(step.reactions()[0], reaction);
}

void StepCommandsTest::testRemoveReactionAtMiddleUndo() {
    Step step;
    StepCommands commands(&step);
    QUndoCommand parent;
    Reaction* reaction1 = new Reaction();
    Reaction* reaction2 = new Reaction();
    Reaction* reaction3 = new Reaction();

    step.addReaction(reaction1);
    step.addReaction(reaction2);
    step.addReaction(reaction3);

    QUndoCommand* command = commands.removeReaction(reaction2, &parent);
    command->redo();
    command->undo();

    QCOMPARE(command->text(), i18nc("@action", "Remove reaction"));
    QCOMPARE(parent.child(0), command);
    QCOMPARE(step.reactions().count(), 3);
    QCOMPARE(step.reactions()[0], reaction1);
    QCOMPARE(step.reactions()[1], reaction2);
    QCOMPARE(step.reactions()[2], reaction3);
}

QTEST_MAIN(StepCommandsTest)

#include "StepCommandsTest.moc"
