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

#ifndef STEPCOMMANDS_H
#define STEPCOMMANDS_H

class QString;
class QUndoCommand;

class Reaction;
class Step;

/**
 * Factory for step edition commands.
 * This class provides methods to create undoable commands to edit a step.
 * A different command is provided for each data modification method in the Step
 * class.
 *
 * Note that each method just creates and returns the command. It does not
 * execute it before returning.
 *
 * @see QUndoCommand
 */
class StepCommands {
public:

    /**
     * Creates a new StepCommands for the given step.
     *
     * @param step The step to create the commands for.
     */
    explicit StepCommands(Step* step);

    /**
     * Creates a new undoable command to set the step id.
     *
     * @param id The id of the step to set.
     * @param parent The parent QUndoCommand.
     * @return The new QUndoCommand.
     */
    QUndoCommand* setId(const QString& id, QUndoCommand* parent = 0);

    /**
     * Creates a new undoable command to set the step text.
     *
     * @param text The text of the step to set.
     * @param parent The parent QUndoCommand.
     * @return The new QUndoCommand.
     */
    QUndoCommand* setText(const QString& text, QUndoCommand* parent = 0);

    /**
     * Creates a new undoable command to set the step custom setup code.
     *
     * @param code The custom setup code of the step to set.
     * @param parent The parent QUndoCommand.
     * @return The new QUndoCommand.
     */
    QUndoCommand* setCustomSetupCode(const QString& code,
                                     QUndoCommand* parent = 0);

    /**
     * Creates a new undoable command to set the step custom tear down code.
     *
     * @param code The custom tear down code of the step to set.
     * @param parent The parent QUndoCommand.
     * @return The new QUndoCommand.
     */
    QUndoCommand* setCustomTearDownCode(const QString& code,
                                        QUndoCommand* parent = 0);

    /**
     * Creates a new undoable command to add the given reaction to the step.
     * If the command was not executed yet, or it was undone, the reaction is
     * deleted when the command is deleted.
     *
     * @param reaction The reaction to add to the step.
     * @param parent The parent QUndoCommand.
     * @return The new QUndoCommand.
     */
    QUndoCommand* addReaction(Reaction* reaction, QUndoCommand* parent = 0);

    /**
     * Creates a new undoable command to add the given reaction to the step at
     * the given index.
     * If the command was not executed yet, or it was undone, the reaction is
     * deleted when the command is deleted.
     *
     * @param reaction The reaction to add to the step.
     * @param index The index to add the reaction at.
     * @param parent The parent QUndoCommand.
     * @return The new QUndoCommand.
     */
    QUndoCommand* addReaction(Reaction* reaction, int index,
                              QUndoCommand* parent = 0);

    /**
     * Creates a new undoable command to remove the given reaction from the
     * step.
     * If the command was executed and not undone, the reaction is deleted when
     * the command is deleted.
     *
     * @param reaction The reaction to remove from the step.
     * @param parent The parent QUndoCommand.
     * @return The new QUndoCommand.
     */
    QUndoCommand* removeReaction(Reaction* reaction, QUndoCommand* parent = 0);

private:

    /**
     * The step to create the commands for.
     */
    Step* mStep;

};

#endif
