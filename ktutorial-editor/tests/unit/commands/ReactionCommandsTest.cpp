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

#include "ReactionCommands.h"

#include <QUndoCommand>

#include <KLocalizedString>

#include "../data/Step.h"
#include "../data/WaitForSignal.h"

class ReactionCommandsTest: public QObject {
Q_OBJECT

private slots:

    void testSetTriggerTypeRedo();
    void testSetTriggerTypeUndo();

    void testSetOptionNameRedo();
    void testSetOptionNameUndo();

    void testSetWaitForRedo();
    void testSetWaitForUndo();

    void testSetResponseTypeRedo();
    void testSetResponseTypeUndo();

    void testSetNextStepIdRedo();
    void testSetNextStepIdUndo();

    void testSetCustomCodeRedo();
    void testSetCustomCodeUndo();

};

void ReactionCommandsTest::testSetTriggerTypeRedo() {
    Reaction reaction;
    ReactionCommands commands(&reaction);
    QUndoCommand parent;

    reaction.setTriggerType(Reaction::OptionSelected);

    QUndoCommand* command = commands.setTriggerType(Reaction::ConditionMet,
                                                    &parent);
    command->redo();

    QCOMPARE(command->text(), i18nc("@action", "Set reaction trigger type"));
    QCOMPARE(parent.child(0), command);
    QCOMPARE(reaction.triggerType(), Reaction::ConditionMet);
}

void ReactionCommandsTest::testSetTriggerTypeUndo() {
    Reaction reaction;
    ReactionCommands commands(&reaction);
    QUndoCommand parent;

    reaction.setTriggerType(Reaction::OptionSelected);

    QUndoCommand* command = commands.setTriggerType(Reaction::ConditionMet,
                                                    &parent);
    command->redo();
    command->undo();

    QCOMPARE(command->text(), i18nc("@action", "Set reaction trigger type"));
    QCOMPARE(parent.child(0), command);
    QCOMPARE(reaction.triggerType(), Reaction::OptionSelected);
}

void ReactionCommandsTest::testSetOptionNameRedo() {
    Reaction reaction;
    ReactionCommands commands(&reaction);
    QUndoCommand parent;

    QUndoCommand* command = commands.setOptionName("The option name", &parent);
    command->redo();

    QCOMPARE(command->text(), i18nc("@action", "Set reaction option name"));
    QCOMPARE(parent.child(0), command);
    QCOMPARE(reaction.optionName(), QString("The option name"));
}

void ReactionCommandsTest::testSetOptionNameUndo() {
    Reaction reaction;
    ReactionCommands commands(&reaction);
    QUndoCommand parent;

    reaction.setOptionName("The old option name");

    QUndoCommand* command = commands.setOptionName("The new option name",
                                                   &parent);
    command->redo();
    command->undo();

    QCOMPARE(command->text(), i18nc("@action", "Set reaction option name"));
    QCOMPARE(parent.child(0), command);
    QCOMPARE(reaction.optionName(), QString("The old option name"));
}

void ReactionCommandsTest::testSetWaitForRedo() {
    Reaction reaction;
    ReactionCommands commands(&reaction);
    QUndoCommand parent;
    WaitForSignal* waitFor = new WaitForSignal();

    QUndoCommand* command = commands.setWaitFor(waitFor, &parent);
    command->redo();

    QCOMPARE(command->text(),
             i18nc("@action", "Set reaction condition to wait for"));
    QCOMPARE(parent.child(0), command);
    QCOMPARE(reaction.waitFor(), waitFor);
}

void ReactionCommandsTest::testSetWaitForUndo() {
    Reaction reaction;
    ReactionCommands commands(&reaction);
    QUndoCommand parent;
    WaitForSignal* oldWaitFor = new WaitForSignal();
    WaitForSignal* newWaitFor = new WaitForSignal();

    reaction.setWaitFor(oldWaitFor);

    QUndoCommand* command = commands.setWaitFor(newWaitFor, &parent);
    command->redo();
    command->undo();

    QCOMPARE(command->text(),
             i18nc("@action", "Set reaction condition to wait for"));
    QCOMPARE(parent.child(0), command);
    QCOMPARE(reaction.waitFor(), oldWaitFor);
}

void ReactionCommandsTest::testSetResponseTypeRedo() {
    Reaction reaction;
    ReactionCommands commands(&reaction);
    QUndoCommand parent;

    reaction.setResponseType(Reaction::CustomCode);

    QUndoCommand* command = commands.setResponseType(Reaction::NextStep,
                                                     &parent);
    command->redo();

    QCOMPARE(command->text(), i18nc("@action", "Set reaction response type"));
    QCOMPARE(parent.child(0), command);
    QCOMPARE(reaction.responseType(), Reaction::NextStep);
}

void ReactionCommandsTest::testSetResponseTypeUndo() {
    Reaction reaction;
    ReactionCommands commands(&reaction);
    QUndoCommand parent;

    reaction.setResponseType(Reaction::CustomCode);

    QUndoCommand* command = commands.setResponseType(Reaction::NextStep,
                                                     &parent);
    command->redo();
    command->undo();

    QCOMPARE(command->text(), i18nc("@action", "Set reaction response type"));
    QCOMPARE(parent.child(0), command);
    QCOMPARE(reaction.responseType(), Reaction::CustomCode);
}

void ReactionCommandsTest::testSetNextStepIdRedo() {
    Reaction reaction;
    ReactionCommands commands(&reaction);
    QUndoCommand parent;

    QUndoCommand* command = commands.setNextStepId("The next step id", &parent);
    command->redo();

    QCOMPARE(command->text(), i18nc("@action", "Set reaction next step id"));
    QCOMPARE(parent.child(0), command);
    QCOMPARE(reaction.nextStepId(), QString("The next step id"));
}

void ReactionCommandsTest::testSetNextStepIdUndo() {
    Reaction reaction;
    ReactionCommands commands(&reaction);
    QUndoCommand parent;

    reaction.setNextStepId("The old next step id");

    QUndoCommand* command = commands.setNextStepId("The new next step id",
                                                   &parent);
    command->redo();
    command->undo();

    QCOMPARE(command->text(), i18nc("@action", "Set reaction next step id"));
    QCOMPARE(parent.child(0), command);
    QCOMPARE(reaction.nextStepId(), QString("The old next step id"));
}

void ReactionCommandsTest::testSetCustomCodeRedo() {
    Reaction reaction;
    ReactionCommands commands(&reaction);
    QUndoCommand parent;

    QUndoCommand* command = commands.setCustomCode("The custom code", &parent);
    command->redo();

    QCOMPARE(command->text(), i18nc("@action", "Set reaction custom code"));
    QCOMPARE(parent.child(0), command);
    QCOMPARE(reaction.customCode(), QString("The custom code"));
}

void ReactionCommandsTest::testSetCustomCodeUndo() {
    Reaction reaction;
    ReactionCommands commands(&reaction);
    QUndoCommand parent;

    reaction.setCustomCode("The old custom code");

    QUndoCommand* command = commands.setCustomCode("The new custom code",
                                                   &parent);
    command->redo();
    command->undo();

    QCOMPARE(command->text(), i18nc("@action", "Set reaction custom code"));
    QCOMPARE(parent.child(0), command);
    QCOMPARE(reaction.customCode(), QString("The old custom code"));
}

QTEST_MAIN(ReactionCommandsTest)

#include "ReactionCommandsTest.moc"
