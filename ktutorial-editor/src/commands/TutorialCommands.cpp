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

#include "TutorialCommands.h"

#include <QUndoCommand>

#include <KLocalizedString>

#include "../data/Step.h"
#include "../data/Tutorial.h"

class SetTutorialName: public QUndoCommand {
public:

    Tutorial* mTutorial;
    QString mOldName;
    QString mNewName;

    SetTutorialName(QUndoCommand* parent = 0): QUndoCommand(parent) {
        setText(i18nc("@action", "Set tutorial name"));
    }

    virtual void redo() {
        mOldName = mTutorial->name();
        mTutorial->setName(mNewName);
    }

    virtual void undo() {
        mTutorial->setName(mOldName);
    }

};

class SetTutorialDescription: public QUndoCommand {
public:

    Tutorial* mTutorial;
    QString mOldDescription;
    QString mNewDescription;

    SetTutorialDescription(QUndoCommand* parent = 0): QUndoCommand(parent) {
        setText(i18nc("@action", "Set tutorial description"));
    }

    virtual void redo() {
        mOldDescription = mTutorial->description();
        mTutorial->setDescription(mNewDescription);
    }

    virtual void undo() {
        mTutorial->setDescription(mOldDescription);
    }

};

class SetTutorialLicenseText: public QUndoCommand {
public:

    Tutorial* mTutorial;
    QString mOldLicenseText;
    QString mNewLicenseText;

    SetTutorialLicenseText(QUndoCommand* parent = 0): QUndoCommand(parent) {
        setText(i18nc("@action", "Set tutorial license"));
    }

    virtual void redo() {
        mOldLicenseText = mTutorial->licenseText();
        mTutorial->setLicenseText(mNewLicenseText);
    }

    virtual void undo() {
        mTutorial->setLicenseText(mOldLicenseText);
    }

};

class SetTutorialCustomSetupCode: public QUndoCommand {
public:

    Tutorial* mTutorial;
    QString mOldCustomSetupCode;
    QString mNewCustomSetupCode;

    SetTutorialCustomSetupCode(QUndoCommand* parent = 0): QUndoCommand(parent) {
        setText(i18nc("@action", "Set tutorial setup code"));
    }

    virtual void redo() {
        mOldCustomSetupCode = mTutorial->customSetupCode();
        mTutorial->setCustomSetupCode(mNewCustomSetupCode);
    }

    virtual void undo() {
        mTutorial->setCustomSetupCode(mOldCustomSetupCode);
    }

};

class SetTutorialCustomTearDownCode: public QUndoCommand {
public:

    Tutorial* mTutorial;
    QString mOldCustomTearDownCode;
    QString mNewCustomTearDownCode;

    SetTutorialCustomTearDownCode(QUndoCommand* parent = 0):
            QUndoCommand(parent) {
        setText(i18nc("@action", "Set tutorial tear down code"));
    }

    virtual void redo() {
        mOldCustomTearDownCode = mTutorial->customTearDownCode();
        mTutorial->setCustomTearDownCode(mNewCustomTearDownCode);
    }

    virtual void undo() {
        mTutorial->setCustomTearDownCode(mOldCustomTearDownCode);
    }

};

class BaseStepCommand: public QUndoCommand {
public:

    Tutorial* mTutorial;
    Step* mStep;
    bool mDeleteStepInDestructor;

    BaseStepCommand(QUndoCommand* parent = 0): QUndoCommand(parent),
        mTutorial(0),
        mStep(0),
        mDeleteStepInDestructor(false) {
    }

    virtual ~BaseStepCommand() {
        if (mDeleteStepInDestructor) {
            delete mStep;
        }
    }

};

class AddStep: public BaseStepCommand {
public:

    AddStep(QUndoCommand* parent = 0): BaseStepCommand(parent) {
        setText(i18nc("@action", "Add step"));
        mDeleteStepInDestructor = true;
    }

    virtual void undo() {
        mDeleteStepInDestructor = true;
        mTutorial->removeStep(mStep);
    }

};

class AddStepAtEnd: public AddStep {
public:

    AddStepAtEnd(QUndoCommand* parent = 0): AddStep(parent) {
    }

    virtual void redo() {
        mDeleteStepInDestructor = false;
        mTutorial->addStep(mStep);
    }

};

class AddStepAtIndex: public AddStep {
public:

    int mIndex;

    AddStepAtIndex(QUndoCommand* parent = 0): AddStep(parent) {
    }

    virtual void redo() {
        mDeleteStepInDestructor = false;
        mTutorial->addStep(mStep, mIndex);
    }

};

class RemoveStep: public BaseStepCommand {
public:

    int mIndex;

    RemoveStep(QUndoCommand* parent = 0): BaseStepCommand(parent) {
        setText(i18nc("@action", "Remove step"));
        mDeleteStepInDestructor = false;
    }

    virtual void redo() {
        mDeleteStepInDestructor = true;
        mIndex = mTutorial->steps().indexOf(mStep);
        mTutorial->removeStep(mStep);
    }

    virtual void undo() {
        mDeleteStepInDestructor = false;
        mTutorial->addStep(mStep, mIndex);
    }

};

//public:

TutorialCommands::TutorialCommands(Tutorial* tutorial):
    mTutorial(tutorial) {
}

QUndoCommand* TutorialCommands::setName(const QString& name,
                                        QUndoCommand* parent) {
    SetTutorialName* command = new SetTutorialName(parent);
    command->mTutorial = mTutorial;
    command->mNewName = name;
    return command;
}

QUndoCommand* TutorialCommands::setDescription(const QString& description,
                                               QUndoCommand* parent) {
    SetTutorialDescription* command = new SetTutorialDescription(parent);
    command->mTutorial = mTutorial;
    command->mNewDescription = description;
    return command;
}

QUndoCommand* TutorialCommands::setLicenseText(const QString& licenseText,
                                               QUndoCommand* parent) {
    SetTutorialLicenseText* command = new SetTutorialLicenseText(parent);
    command->mTutorial = mTutorial;
    command->mNewLicenseText = licenseText;
    return command;
}

QUndoCommand* TutorialCommands::setCustomSetupCode(const QString& code,
                                                   QUndoCommand* parent) {
    SetTutorialCustomSetupCode* command =
                                        new SetTutorialCustomSetupCode(parent);
    command->mTutorial = mTutorial;
    command->mNewCustomSetupCode = code;
    return command;
}

QUndoCommand* TutorialCommands::setCustomTearDownCode(const QString& code,
                                                      QUndoCommand* parent) {
    SetTutorialCustomTearDownCode* command =
                                    new SetTutorialCustomTearDownCode(parent);
    command->mTutorial = mTutorial;
    command->mNewCustomTearDownCode = code;
    return command;
}

QUndoCommand* TutorialCommands::addStep(Step* step, QUndoCommand* parent) {
    AddStepAtEnd* command = new AddStepAtEnd(parent);
    command->mTutorial = mTutorial;
    command->mStep = step;
    return command;
}

QUndoCommand* TutorialCommands::addStep(Step* step, int index,
                                        QUndoCommand* parent) {
    AddStepAtIndex* command = new AddStepAtIndex(parent);
    command->mTutorial = mTutorial;
    command->mStep = step;
    command->mIndex = index;
    return command;
}

QUndoCommand* TutorialCommands::removeStep(Step* step, QUndoCommand* parent) {
    RemoveStep* command = new RemoveStep(parent);
    command->mTutorial = mTutorial;
    command->mStep = step;
    return command;
}
