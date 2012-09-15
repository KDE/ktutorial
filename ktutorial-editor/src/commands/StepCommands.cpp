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

#include "StepCommands.h"

#include <QUndoCommand>

#include <KLocalizedString>

#include "../data/Reaction.h"
#include "../data/Step.h"

class SetStepId: public QUndoCommand {
public:

    Step* mStep;
    QString mOldId;
    QString mNewId;

    SetStepId(QUndoCommand* parent = 0): QUndoCommand(parent) {
        setText(i18nc("@action", "Set step id"));
    }

    virtual void redo() {
        mOldId = mStep->id();
        mStep->setId(mNewId);
    }

    virtual void undo() {
        mStep->setId(mOldId);
    }

};

class SetStepText: public QUndoCommand {
public:

    Step* mStep;
    QString mOldText;
    QString mNewText;

    SetStepText(QUndoCommand* parent = 0): QUndoCommand(parent) {
        setText(i18nc("@action", "Set step text"));
    }

    virtual void redo() {
        mOldText = mStep->text();
        mStep->setText(mNewText);
    }

    virtual void undo() {
        mStep->setText(mOldText);
    }

};

class SetStepCustomSetupCode: public QUndoCommand {
public:

    Step* mStep;
    QString mOldCustomSetupCode;
    QString mNewCustomSetupCode;

    SetStepCustomSetupCode(QUndoCommand* parent = 0): QUndoCommand(parent) {
        setText(i18nc("@action", "Set step setup code"));
    }

    virtual void redo() {
        mOldCustomSetupCode = mStep->customSetupCode();
        mStep->setCustomSetupCode(mNewCustomSetupCode);
    }

    virtual void undo() {
        mStep->setCustomSetupCode(mOldCustomSetupCode);
    }

};

class SetStepCustomTearDownCode: public QUndoCommand {
public:

    Step* mStep;
    QString mOldCustomTearDownCode;
    QString mNewCustomTearDownCode;

    SetStepCustomTearDownCode(QUndoCommand* parent = 0): QUndoCommand(parent) {
        setText(i18nc("@action", "Set step tear down code"));
    }

    virtual void redo() {
        mOldCustomTearDownCode = mStep->customTearDownCode();
        mStep->setCustomTearDownCode(mNewCustomTearDownCode);
    }

    virtual void undo() {
        mStep->setCustomTearDownCode(mOldCustomTearDownCode);
    }

};

class BaseReactionCommand: public QUndoCommand {
public:

    Step* mStep;
    Reaction* mReaction;
    bool mDeleteReactionInDestructor;

    BaseReactionCommand(QUndoCommand* parent = 0): QUndoCommand(parent),
        mStep(0),
        mReaction(0),
        mDeleteReactionInDestructor(false) {
    }

    virtual ~BaseReactionCommand() {
        if (mDeleteReactionInDestructor) {
            delete mReaction;
        }
    }

};

class AddReaction: public BaseReactionCommand {
public:

    AddReaction(QUndoCommand* parent = 0): BaseReactionCommand(parent) {
        setText(i18nc("@action", "Add reaction"));
        mDeleteReactionInDestructor = true;
    }

    virtual void undo() {
        mDeleteReactionInDestructor = true;
        mStep->removeReaction(mReaction);
    }

};

class AddReactionAtEnd: public AddReaction {
public:

    AddReactionAtEnd(QUndoCommand* parent = 0): AddReaction(parent) {
    }

    virtual void redo() {
        mDeleteReactionInDestructor = false;
        mStep->addReaction(mReaction);
    }

};

class AddReactionAtIndex: public AddReaction {
public:

    int mIndex;

    AddReactionAtIndex(QUndoCommand* parent = 0): AddReaction(parent) {
    }

    virtual void redo() {
        mDeleteReactionInDestructor = false;
        mStep->addReaction(mReaction, mIndex);
    }

};

class RemoveReaction: public BaseReactionCommand {
public:

    int mIndex;

    RemoveReaction(QUndoCommand* parent = 0): BaseReactionCommand(parent) {
        setText(i18nc("@action", "Remove reaction"));
        mDeleteReactionInDestructor = false;
    }

    virtual void redo() {
        mDeleteReactionInDestructor = true;
        mIndex = mStep->reactions().indexOf(mReaction);
        mStep->removeReaction(mReaction);
    }

    virtual void undo() {
        mDeleteReactionInDestructor = false;
        mStep->addReaction(mReaction, mIndex);
    }

};

//public:

StepCommands::StepCommands(Step* step):
    mStep(step) {
}

QUndoCommand* StepCommands::setId(const QString& id, QUndoCommand* parent) {
    SetStepId* command = new SetStepId(parent);
    command->mStep = mStep;
    command->mNewId = id;
    return command;
}

QUndoCommand* StepCommands::setText(const QString& text, QUndoCommand* parent) {
    SetStepText* command = new SetStepText(parent);
    command->mStep = mStep;
    command->mNewText = text;
    return command;
}

QUndoCommand* StepCommands::setCustomSetupCode(const QString& code,
                                               QUndoCommand* parent) {
    SetStepCustomSetupCode* command = new SetStepCustomSetupCode(parent);
    command->mStep = mStep;
    command->mNewCustomSetupCode = code;
    return command;
}

QUndoCommand* StepCommands::setCustomTearDownCode(const QString& code,
                                                  QUndoCommand* parent) {
    SetStepCustomTearDownCode* command = new SetStepCustomTearDownCode(parent);
    command->mStep = mStep;
    command->mNewCustomTearDownCode = code;
    return command;
}

QUndoCommand* StepCommands::addReaction(Reaction* reaction,
                                        QUndoCommand* parent) {
    AddReactionAtEnd* command = new AddReactionAtEnd(parent);
    command->mStep = mStep;
    command->mReaction = reaction;
    return command;
}

QUndoCommand* StepCommands::addReaction(Reaction* reaction, int index,
                                        QUndoCommand* parent) {
    AddReactionAtIndex* command = new AddReactionAtIndex(parent);
    command->mStep = mStep;
    command->mReaction = reaction;
    command->mIndex = index;
    return command;
}

QUndoCommand* StepCommands::removeReaction(Reaction* reaction,
                                           QUndoCommand* parent) {
    RemoveReaction* command = new RemoveReaction(parent);
    command->mStep = mStep;
    command->mReaction = reaction;
    return command;
}
