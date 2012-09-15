/***************************************************************************
 *   Copyright (C) 2010 by Daniel Calviño Sánchez <danxuliu@gmail.com>     *
 *   Copyright (C) 2011 by Daniel Calviño Sánchez <danxuliu@gmail.com>     *
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

#include "EditActions.h"

#include <KAction>
#include <KActionCollection>
#include <KLocalizedString>
#include <KUndoStack>

#include "KTutorialEditor.h"
#include "commands/StepCommands.h"
#include "commands/TutorialCommands.h"
#include "data/Reaction.h"
#include "data/Step.h"
#include "view/DialogRunner.h"
#include "view/EditionDialog.h"
#include "view/LicenseWidget.h"
#include "view/ReactionWidget.h"
#include "view/StepCustomCodeWidget.h"
#include "view/StepDataWidget.h"
#include "view/TutorialCustomCodeWidget.h"
#include "view/TutorialInformationWidget.h"

//public:

EditActions::EditActions(KTutorialEditor* tutorialEditor):
        QObject(tutorialEditor),
    mTutorialEditor(tutorialEditor),
    mCurrentStep(0),
    mCurrentReaction(0) {

    mUndoStack = new KUndoStack(this);
    connect(mUndoStack, SIGNAL(cleanChanged(bool)),
            this, SIGNAL(cleanChanged(bool)));

    setupActions();
}

void EditActions::clearCommands() {
    mUndoStack->clear();
}

void EditActions::setClean() {
    mUndoStack->setClean();
}

//public slots:

void EditActions::selectStep(Step* step) {
    mCurrentStep = step;

    KActionCollection* actionCollection = mTutorialEditor->actionCollection();
    if (mCurrentStep) {
        actionCollection->action("setStepData")->setEnabled(true);
        actionCollection->action("setStepSetup")->setEnabled(true);
        actionCollection->action("setStepTearDown")->setEnabled(true);
        actionCollection->action("removeStep")->setEnabled(true);
        actionCollection->action("addReaction")->setEnabled(true);
    } else {
        actionCollection->action("setStepData")->setEnabled(false);
        actionCollection->action("setStepSetup")->setEnabled(false);
        actionCollection->action("setStepTearDown")->setEnabled(false);
        actionCollection->action("removeStep")->setEnabled(false);
        actionCollection->action("addReaction")->setEnabled(false);
    }
}

void EditActions::selectReaction(Reaction* reaction) {
    mCurrentReaction = reaction;

    KActionCollection* actionCollection = mTutorialEditor->actionCollection();
    if (mCurrentReaction) {
        actionCollection->action("setReactionData")->setEnabled(true);
        actionCollection->action("removeReaction")->setEnabled(true);
    } else {
        actionCollection->action("setReactionData")->setEnabled(false);
        actionCollection->action("removeReaction")->setEnabled(false);
    }
}

//private:

void EditActions::setupActions() {
    KActionCollection* actionCollection = mTutorialEditor->actionCollection();

    mUndoStack->createUndoAction(actionCollection);

    mUndoStack->createRedoAction(actionCollection);

    KAction* action = new KAction(this);
    action->setText(i18nc("@action", "Set information..."));
    action->setStatusTip(i18nc("@info:status", "Set the name and description "
"of the tutorial."));
    action->setIcon(KIcon("documentinfo"));
    actionCollection->addAction("setTutorialInformation", action);
    connect(action, SIGNAL(triggered(bool)),
            this, SLOT(setTutorialInformation()));

    action = new KAction(this);
    action->setText(i18nc("@action", "Set license..."));
    action->setStatusTip(i18nc("@info:status", "Set the license text of the "
"tutorial."));
    action->setIcon(KIcon("document-edit"));
    actionCollection->addAction("setTutorialLicense", action);
    connect(action, SIGNAL(triggered(bool)), this, SLOT(setTutorialLicense()));

    action = new KAction(this);
    action->setText(i18nc("@action", "Set setup code..."));
    action->setStatusTip(i18nc("@info:status", "Set the custom code to be "
"executed when the tutorial starts."));
    action->setIcon(KIcon("code-function"));
    actionCollection->addAction("setTutorialSetup", action);
    connect(action, SIGNAL(triggered(bool)), this, SLOT(setTutorialSetup()));

    action = new KAction(this);
    action->setText(i18nc("@action", "Set tear down code..."));
    action->setStatusTip(i18nc("@info:status", "Set the custom code to be "
"executed when the tutorial finishes."));
    action->setIcon(KIcon("code-function"));
    actionCollection->addAction("setTutorialTearDown", action);
    connect(action, SIGNAL(triggered(bool)), this, SLOT(setTutorialTearDown()));

    action = new KAction(this);
    action->setText(i18nc("@action", "Add step..."));
    action->setStatusTip(i18nc("@info:status", "Add a new step to the "
"tutorial."));
    action->setIcon(KIcon("list-add"));
    actionCollection->addAction("addStep", action);
    connect(action, SIGNAL(triggered(bool)), this, SLOT(addStep()));

    action = new KAction(this);
    action->setText(i18nc("@action", "Set step data..."));
    action->setStatusTip(i18nc("@info:status", "Set the name and text of the "
"currently selected step."));
    action->setIcon(KIcon("document-edit"));
    action->setEnabled(false);
    actionCollection->addAction("setStepData", action);
    connect(action, SIGNAL(triggered(bool)), this, SLOT(setStepData()));

    action = new KAction(this);
    action->setText(i18nc("@action", "Set setup code..."));
    action->setStatusTip(i18nc("@info:status", "Set the custom code to be "
"executed when the tutorial passes to the currently selected step."));
    action->setIcon(KIcon("code-function"));
    action->setEnabled(false);
    actionCollection->addAction("setStepSetup", action);
    connect(action, SIGNAL(triggered(bool)), this, SLOT(setStepSetup()));

    action = new KAction(this);
    action->setText(i18nc("@action", "Set tear down code..."));
    action->setStatusTip(i18nc("@info:status", "Set the custom code to be "
"executed when the tutorial changes from the currently selected step to "
"another step."));
    action->setIcon(KIcon("code-function"));
    action->setEnabled(false);
    actionCollection->addAction("setStepTearDown", action);
    connect(action, SIGNAL(triggered(bool)), this, SLOT(setStepTearDown()));

    action = new KAction(this);
    action->setText(i18nc("@action", "Remove step"));
    action->setStatusTip(i18nc("@info:status", "Removes the currently selected "
"step from the tutorial."));
    action->setIcon(KIcon("list-remove"));
    action->setEnabled(false);
    actionCollection->addAction("removeStep", action);
    connect(action, SIGNAL(triggered(bool)), this, SLOT(removeStep()));

    action = new KAction(this);
    action->setText(i18nc("@action", "Add reaction..."));
    action->setStatusTip(i18nc("@info:status", "Add a new reaction to the "
"selected step."));
    action->setIcon(KIcon("list-add"));
    action->setEnabled(false);
    actionCollection->addAction("addReaction", action);
    connect(action, SIGNAL(triggered(bool)), this, SLOT(addReaction()));

    action = new KAction(this);
    action->setText(i18nc("@action", "Set reaction data..."));
    action->setStatusTip(i18nc("@info:status", "Set the trigger and the "
"response of the currently selected reaction."));
    action->setIcon(KIcon("document-edit"));
    action->setEnabled(false);
    actionCollection->addAction("setReactionData", action);
    connect(action, SIGNAL(triggered(bool)), this, SLOT(setReactionData()));

    action = new KAction(this);
    action->setText(i18nc("@action", "Remove reaction"));
    action->setStatusTip(i18nc("@info:status", "Removes the currently selected "
"reaction from its step."));
    action->setIcon(KIcon("list-remove"));
    action->setEnabled(false);
    actionCollection->addAction("removeReaction", action);
    connect(action, SIGNAL(triggered(bool)), this, SLOT(removeReaction()));
}

int EditActions::showEditionDialog(CommandWidget* commandWidget) {
    commandWidget->setUndoStack(mUndoStack);

    QString dialogName = commandWidget->objectName();
    dialogName.replace("Widget", "Dialog");

    EditionDialog* dialog = new EditionDialog(commandWidget, mTutorialEditor);
    dialog->setObjectName(dialogName);
    return DialogRunner(dialog).exec();
}

//private slots:

void EditActions::setTutorialInformation() {
    showEditionDialog(new TutorialInformationWidget(
                                                mTutorialEditor->tutorial()));
}

void EditActions::setTutorialLicense() {
    showEditionDialog(new LicenseWidget(mTutorialEditor->tutorial()));
}

void EditActions::setTutorialSetup() {
    showEditionDialog(new TutorialCustomCodeWidget(mTutorialEditor->tutorial(),
                                        TutorialCustomCodeWidget::Setup));
}

void EditActions::setTutorialTearDown() {
    showEditionDialog(new TutorialCustomCodeWidget(mTutorialEditor->tutorial(),
                                        TutorialCustomCodeWidget::TearDown));
}

void EditActions::addStep() {
    Step* step = new Step();

    QUndoCommand* parentCommand = new QUndoCommand();
    parentCommand->setText(i18nc("@action", "Add step"));
    TutorialCommands(mTutorialEditor->tutorial()).addStep(step, parentCommand);

    StepDataWidget* widget = new StepDataWidget(step);
    widget->enableStepIdCompletion(mTutorialEditor->tutorial());
    widget->setParentUndoCommand(parentCommand);
    if (showEditionDialog(widget) == QDialog::Rejected) {
        delete parentCommand;
    }
}

void EditActions::setStepData() {
    Q_ASSERT(mCurrentStep);

    StepDataWidget* widget = new StepDataWidget(mCurrentStep);
    widget->enableStepIdCompletion(mTutorialEditor->tutorial());
    showEditionDialog(widget);
}

void EditActions::setStepSetup() {
    showEditionDialog(new StepCustomCodeWidget(mCurrentStep,
                                               StepCustomCodeWidget::Setup));
}

void EditActions::setStepTearDown() {
    showEditionDialog(new StepCustomCodeWidget(mCurrentStep,
                                               StepCustomCodeWidget::TearDown));
}

void EditActions::removeStep() {
    Q_ASSERT(mCurrentStep);

    TutorialCommands tutorialCommands(mTutorialEditor->tutorial());
    mUndoStack->push(tutorialCommands.removeStep(mCurrentStep));
}

void EditActions::addReaction() {
    Q_ASSERT(mCurrentStep);

    Reaction* reaction = new Reaction();

    QUndoCommand* parentCommand = new QUndoCommand();
    parentCommand->setText(i18nc("@action", "Add reaction"));
    StepCommands(mCurrentStep).addReaction(reaction, parentCommand);

    ReactionWidget* widget = new ReactionWidget(reaction);
    widget->enableStepIdCompletion(mTutorialEditor->tutorial(), mCurrentStep);
    widget->setParentUndoCommand(parentCommand);
    if (showEditionDialog(widget) == QDialog::Rejected) {
        delete parentCommand;
    }
}

void EditActions::setReactionData() {
    Q_ASSERT(mCurrentReaction);

    ReactionWidget* widget = new ReactionWidget(mCurrentReaction);
    widget->enableStepIdCompletion(mTutorialEditor->tutorial());
    showEditionDialog(widget);
}

void EditActions::removeReaction() {
    Q_ASSERT(mCurrentStep);

    mUndoStack->push(StepCommands(mCurrentStep).removeReaction(
                                                            mCurrentReaction));
}
