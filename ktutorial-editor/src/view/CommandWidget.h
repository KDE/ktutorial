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

#ifndef COMMANDWIDGET_H
#define COMMANDWIDGET_H

#include "EditionWidget.h"

class QUndoCommand;
class QUndoStack;

/**
 * Base abstract class for command widgets.
 * Command widgets are edition widgets that use undoable commands to save the
 * data, for example, to set the name of a tutorial.
 *
 * To be done or undone by the user, the command must be added to a stack of
 * commands. The stack that the commands will be added to can be set using
 * setUndoStack(QUndoStack*) method. When the stack is set, the commands that
 * are executed to save the data are added to the stack, which effectively
 * causes them to be executed.
 *
 * However, the CommandWidget does not add the commands directly to the stack.
 * Each command can have a parent command. When this happens, the child commands
 * are done when their parent is done, and are undone when their parent is
 * undone. It makes easier to compose several small commands to be executed as
 * one bigger command.
 *
 * All the commands created in the widget have the same parent command, which is
 * the one added directly to the stack. If no parent command was set, a new one
 * is created automatically. If it has several children, its name will be the
 * window title of the widget. If it has only one child, its name will be the
 * name of its child.
 *
 * Note that if there is a stack, but no parent command and no child commands
 * (for example, because there were no changes to save), the automatically
 * created parent command is not added to the stack. However, if a parent
 * command was explicitly set it is added to the stack even if no children were
 * added to it.
 *
 * If no stack is set but there is a parent command, the commands will be added
 * to the parent without executing them.
 *
 * If no stack and no parent are set, the commands will be executed when the
 * changes are saved, but they will be deleted afterwards.
 *
 * Subclasses must implement createSaveCommands(QUndoCommand*) method. It has
 * to return the list of the commands to execute to save the changes.
 *
 * @see QUndoCommand
 */
class CommandWidget: public EditionWidget {
Q_OBJECT
public:

    /**
     * Creates a new CommandWidget with the given parent widget.
     *
     * @param parent The parent widget.
     */
    CommandWidget(QWidget* parent = 0);

    /**
     * Saves the changes to the object being edited.
     */
    virtual void saveChanges();

    /**
     * Sets the parent command.
     *
     * @param parentUndoCommand The parent command.
     */
    void setParentUndoCommand(QUndoCommand* parentUndoCommand);

    /**
     * Sets the undo stack to add the commands to.
     *
     * @param undoStack The undo stack.
     */
    void setUndoStack(QUndoStack* undoStack);

protected:

    /**
     * Creates the commands needed to save the changes.
     * Subclasses must implement this method.
     *
     * @param parent The parent of all the created commands.
     * @return A list with the commands.
     */
    virtual QList<QUndoCommand*> createSaveCommands(QUndoCommand* parent) = 0;

private:

    /**
     * The explicitly set parent command.
     */
    QUndoCommand* mParentUndoCommand;

    /**
     * The undo stack to add the commands to.
     */
    QUndoStack* mUndoStack;

};

#endif
