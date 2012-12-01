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

#ifndef REACTIONCOMMANDS_H
#define REACTIONCOMMANDS_H

#include "../data/Reaction.h"

class QString;
class QUndoCommand;

class WaitFor;

/**
 * Factory for reaction edition commands.
 * This class provides methods to create undoable commands to edit a reaction.
 * A different command is provided for each data modification method in the
 * Reaction class.
 *
 * Note that each method just creates and returns the command. It does not
 * execute it before returning.
 *
 * @see QUndoCommand
 */
class ReactionCommands {
public:

    /**
     * Creates a new ReactionCommands for the given reaction.
     *
     * @param reaction The reaction to create the commands for.
     */
    explicit ReactionCommands(Reaction* reaction);

    /**
     * Creates a new undoable command to set the trigger type.
     *
     * @param triggerType The trigger type of the reaction to set.
     * @param parent The parent QUndoCommand.
     * @return The new QUndoCommand.
     */
    QUndoCommand* setTriggerType(Reaction::TriggerType triggerType,
                                 QUndoCommand* parent = 0);

    /**
     * Creates a new undoable command to set the option name.
     *
     * @param optionName The option name of the reaction to set.
     * @param parent The parent QUndoCommand.
     * @return The new QUndoCommand.
     */
    QUndoCommand* setOptionName(const QString& optionName,
                                QUndoCommand* parent = 0);

    /**
     * Creates a new undoable command to set the condition to wait for.
     *
     * @param waitFor The condition to wait for of the reaction to set.
     * @param parent The parent QUndoCommand.
     * @return The new QUndoCommand.
     */
    QUndoCommand* setWaitFor(WaitFor* waitFor, QUndoCommand* parent = 0);

    /**
     * Creates a new undoable command to set the response type.
     *
     * @param responseType The response type of the reaction to set.
     * @param parent The parent QUndoCommand.
     * @return The new QUndoCommand.
     */
    QUndoCommand* setResponseType(Reaction::ResponseType responseType,
                                  QUndoCommand* parent = 0);

    /**
     * Creates a new undoable command to set the next step id.
     *
     * @param nextStepId The next step id of the reaction to set.
     * @param parent The parent QUndoCommand.
     * @return The new QUndoCommand.
     */
    QUndoCommand* setNextStepId(const QString& nextStepId,
                                QUndoCommand* parent = 0);

    /**
     * Creates a new undoable command to set the custom response code.
     *
     * @param customCode The custom response code of the reaction to set.
     * @param parent The parent QUndoCommand.
     * @return The new QUndoCommand.
     */
    QUndoCommand* setCustomCode(const QString& customCode,
                                QUndoCommand* parent = 0);

private:

    /**
     * The reaction to create the commands for.
     */
    Reaction* mReaction;

};

#endif
