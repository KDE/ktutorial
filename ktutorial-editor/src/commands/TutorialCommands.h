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

#ifndef TUTORIALCOMMANDS_H
#define TUTORIALCOMMANDS_H

#include <QString>

class QUndoCommand;
class Step;
class Tutorial;

/**
 * Factory for tutorial edition commands.
 * This class provides methods to create undoable commands to edit a tutorial.
 * A different command is provided for each data modification method in the
 * Tutorial class.
 *
 * Note that each method just creates and returns the command. It does not
 * execute it before returning.
 *
 * @see QUndoCommand
 */
class TutorialCommands {
public:

    /**
     * Creates a new TutorialCommands for the given tutorial.
     *
     * @param tutorial The tutorial to create the commands for.
     */
    explicit TutorialCommands(Tutorial* tutorial);

    /**
     * Creates a new undoable command to set the tutorial name.
     *
     * @param name The name of the tutorial to set.
     * @param parent The parent QUndoCommand.
     * @return The new QUndoCommand.
     */
    QUndoCommand* setName(const QString& name, QUndoCommand* parent = 0);

    /**
     * Creates a new undoable command to set the tutorial description.
     *
     * @param description The description of the tutorial to set.
     * @param parent The parent QUndoCommand.
     * @return The new QUndoCommand.
     */
    QUndoCommand* setDescription(const QString& description,
                                 QUndoCommand* parent = 0);

    /**
     * Creates a new undoable command to set the tutorial license text.
     *
     * @param licenseText The license text of the tutorial to set.
     * @param parent The parent QUndoCommand.
     * @return The new QUndoCommand.
     */
    QUndoCommand* setLicenseText(const QString& licenseText,
                                 QUndoCommand* parent = 0);

    /**
     * Creates a new undoable command to set the tutorial custom setup code.
     *
     * @param code The custom setup code of the tutorial to set.
     * @param parent The parent QUndoCommand.
     * @return The new QUndoCommand.
     */
    QUndoCommand* setCustomSetupCode(const QString& code,
                                     QUndoCommand* parent = 0);

    /**
     * Creates a new undoable command to set the tutorial custom tear down code.
     *
     * @param code The custom tear down code of the tutorial to set.
     * @param parent The parent QUndoCommand.
     * @return The new QUndoCommand.
     */
    QUndoCommand* setCustomTearDownCode(const QString& code,
                                        QUndoCommand* parent = 0);

    /**
     * Creates a new undoable command to add the given step to the tutorial.
     * If the command was not executed yet, or it was undone, the step is
     * deleted when the command is deleted.
     *
     * @param step The step to add to the tutorial.
     * @param parent The parent QUndoCommand.
     * @return The new QUndoCommand.
     */
    QUndoCommand* addStep(Step* step, QUndoCommand* parent = 0);

    /**
     * Creates a new undoable command to add the given step to the tutorial at
     * the given index.
     * If the command was not executed yet, or it was undone, the step is
     * deleted when the command is deleted.
     *
     * @param step The step to add to the tutorial.
     * @param index The index to add the step at.
     * @param parent The parent QUndoCommand.
     * @return The new QUndoCommand.
     */
    QUndoCommand* addStep(Step* step, int index, QUndoCommand* parent = 0);

    /**
     * Creates a new undoable command to remove the given step from the
     * tutorial.
     * If the command was executed and not undone, the step is deleted when the
     * command is deleted.
     *
     * @param step The step to remove from the tutorial.
     * @param parent The parent QUndoCommand.
     * @return The new QUndoCommand.
     */
    QUndoCommand* removeStep(Step* step, QUndoCommand* parent = 0);

private:

    /**
     * The tutorial to create the commands for.
     */
    Tutorial* mTutorial;

};

#endif
