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

#include "CommandWidget.h"

#include <QUndoStack>

//public:

CommandWidget::CommandWidget(QWidget* parent): EditionWidget(parent),
    mParentUndoCommand(0),
    mUndoStack(0) {
}

void CommandWidget::saveChanges() {
    QUndoCommand* parent = mParentUndoCommand;
    if (!parent) {
        parent = new QUndoCommand();
    }

    QList<QUndoCommand*> commands = createSaveCommands(parent);

    if (!mParentUndoCommand && commands.count() > 1) {
        parent->setText(windowTitle());
    } else if (!mParentUndoCommand && commands.count() == 1) {
        parent->setText(parent->child(0)->text());
    }

    if (mUndoStack && (commands.count() > 0 || mParentUndoCommand)){
        mUndoStack->push(parent);
        return;
    }

    if (mUndoStack) {
        delete parent;
        return;
    }

    if (mParentUndoCommand) {
        return;
    }

    parent->redo();
    delete parent;
}

void CommandWidget::setParentUndoCommand(QUndoCommand* parentUndoCommand) {
    mParentUndoCommand = parentUndoCommand;
}

void CommandWidget::setUndoStack(QUndoStack* undoStack) {
    mUndoStack = undoStack;
}
