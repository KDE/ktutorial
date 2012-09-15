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

#include "ReactionCommands.h"

#include <QUndoCommand>

#include <KLocalizedString>

#include "../data/Reaction.h"
#include "../data/WaitFor.h"

class SetReactionTriggerType: public QUndoCommand {
public:

    Reaction* mReaction;
    Reaction::TriggerType mOldTriggerType;
    Reaction::TriggerType mNewTriggerType;

    SetReactionTriggerType(QUndoCommand* parent = 0): QUndoCommand(parent) {
        setText(i18nc("@action", "Set reaction trigger type"));
    }

    virtual void redo() {
        mOldTriggerType = mReaction->triggerType();
        mReaction->setTriggerType(mNewTriggerType);
    }

    virtual void undo() {
        mReaction->setTriggerType(mOldTriggerType);
    }

};

class SetReactionOptionName: public QUndoCommand {
public:

    Reaction* mReaction;
    QString mOldOptionName;
    QString mNewOptionName;

    SetReactionOptionName(QUndoCommand* parent = 0): QUndoCommand(parent) {
        setText(i18nc("@action", "Set reaction option name"));
    }

    virtual void redo() {
        mOldOptionName = mReaction->optionName();
        mReaction->setOptionName(mNewOptionName);
    }

    virtual void undo() {
        mReaction->setOptionName(mOldOptionName);
    }

};

class SetReactionWaitFor: public QUndoCommand {
public:

    Reaction* mReaction;
    WaitFor* mOldWaitFor;
    WaitFor* mNewWaitFor;
    bool mRedoDone;

    SetReactionWaitFor(QUndoCommand* parent = 0): QUndoCommand(parent),
        mRedoDone(false) {
        setText(i18nc("@action", "Set reaction condition to wait for"));
    }

    virtual ~SetReactionWaitFor() {
        if (mRedoDone) {
            delete mOldWaitFor;
        } else {
            delete mNewWaitFor;
        }
    }

    virtual void redo() {
        mRedoDone = true;
        mOldWaitFor = mReaction->waitFor();
        mReaction->setWaitFor(mNewWaitFor);
    }

    virtual void undo() {
        mRedoDone = false;
        mReaction->setWaitFor(mOldWaitFor);
    }

};

class SetReactionResponseType: public QUndoCommand {
public:

    Reaction* mReaction;
    Reaction::ResponseType mOldResponseType;
    Reaction::ResponseType mNewResponseType;

    SetReactionResponseType(QUndoCommand* parent = 0): QUndoCommand(parent) {
        setText(i18nc("@action", "Set reaction response type"));
    }

    virtual void redo() {
        mOldResponseType = mReaction->responseType();
        mReaction->setResponseType(mNewResponseType);
    }

    virtual void undo() {
        mReaction->setResponseType(mOldResponseType);
    }

};

class SetReactionNextStepId: public QUndoCommand {
public:

    Reaction* mReaction;
    QString mOldNextStepId;
    QString mNewNextStepId;

    SetReactionNextStepId(QUndoCommand* parent = 0): QUndoCommand(parent) {
        setText(i18nc("@action", "Set reaction next step id"));
    }

    virtual void redo() {
        mOldNextStepId = mReaction->nextStepId();
        mReaction->setNextStepId(mNewNextStepId);
    }

    virtual void undo() {
        mReaction->setNextStepId(mOldNextStepId);
    }

};

class SetReactionCustomCode: public QUndoCommand {
public:

    Reaction* mReaction;
    QString mOldCustomCode;
    QString mNewCustomCode;

    SetReactionCustomCode(QUndoCommand* parent = 0): QUndoCommand(parent) {
        setText(i18nc("@action", "Set reaction custom code"));
    }

    virtual void redo() {
        mOldCustomCode = mReaction->customCode();
        mReaction->setCustomCode(mNewCustomCode);
    }

    virtual void undo() {
        mReaction->setCustomCode(mOldCustomCode);
    }

};

//public:

ReactionCommands::ReactionCommands(Reaction* reaction):
    mReaction(reaction) {
}

QUndoCommand* ReactionCommands::setTriggerType(
                                            Reaction::TriggerType triggerType,
                                            QUndoCommand* parent) {
    SetReactionTriggerType* command = new SetReactionTriggerType(parent);
    command->mReaction = mReaction;
    command->mNewTriggerType = triggerType;
    return command;
}

QUndoCommand* ReactionCommands::setOptionName(const QString& optionName,
                                              QUndoCommand* parent) {
    SetReactionOptionName* command = new SetReactionOptionName(parent);
    command->mReaction = mReaction;
    command->mNewOptionName = optionName;
    return command;
}

QUndoCommand* ReactionCommands::setWaitFor(WaitFor* waitFor,
                                           QUndoCommand* parent) {
    SetReactionWaitFor* command = new SetReactionWaitFor(parent);
    command->mReaction = mReaction;
    command->mNewWaitFor = waitFor;
    return command;
}

QUndoCommand* ReactionCommands::setResponseType(
                                            Reaction::ResponseType responseType,
                                            QUndoCommand* parent) {
    SetReactionResponseType* command = new SetReactionResponseType(parent);
    command->mReaction = mReaction;
    command->mNewResponseType = responseType;
    return command;
}

QUndoCommand* ReactionCommands::setNextStepId(const QString& nextStepId,
                                              QUndoCommand* parent) {
    SetReactionNextStepId* command = new SetReactionNextStepId(parent);
    command->mReaction = mReaction;
    command->mNewNextStepId = nextStepId;
    return command;
}

QUndoCommand* ReactionCommands::setCustomCode(const QString& customCode,
                                              QUndoCommand* parent) {
    SetReactionCustomCode* command = new SetReactionCustomCode(parent);
    command->mReaction = mReaction;
    command->mNewCustomCode = customCode;
    return command;
}
