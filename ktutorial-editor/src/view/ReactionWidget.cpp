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

#include "ReactionWidget.h"

#include "ui_ReactionWidget.h"
#include "WaitForWidget.h"
#include "../commands/ReactionCommands.h"
#include "../data/Reaction.h"
#include "../data/Step.h"
#include "../data/Tutorial.h"
#include "../data/WaitFor.h"

//public:

ReactionWidget::ReactionWidget(Reaction* reaction, QWidget* parent):
        CommandWidget(parent),
    mReaction(reaction),
    mWaitForClone(0) {

    ui = new Ui::ReactionWidget();
    ui->setupUi(this);

    if (reaction->waitFor()) {
        mWaitForClone = reaction->waitFor()->clone();
    }

    mWaitForWidget = new WaitForWidget(mWaitForClone, this);
    mWaitForWidget->setObjectName("triggerConditionWaitForWidget");
    ui->triggerConditionVerticalLayout->addWidget(mWaitForWidget);

    connect(ui->triggerConditionRadioButton, SIGNAL(toggled(bool)),
            this, SLOT(selectTriggerCondition(bool)));
    connect(ui->triggerOptionRadioButton, SIGNAL(toggled(bool)),
            this, SLOT(selectTriggerOption(bool)));
    connect(ui->responseCodeRadioButton, SIGNAL(toggled(bool)),
            this, SLOT(selectResponseCode(bool)));
    connect(ui->responseStepRadioButton, SIGNAL(toggled(bool)),
            this, SLOT(selectResponseStep(bool)));

    mCurrentTriggerType = reaction->triggerType();
    if (reaction->triggerType() == Reaction::ConditionMet) {
        ui->triggerConditionRadioButton->setChecked(true);
    } else {
        ui->triggerOptionRadioButton->setChecked(true);
    }

    mCurrentResponseType = reaction->responseType();
    if (reaction->responseType() == Reaction::CustomCode) {
        ui->responseCodeRadioButton->setChecked(true);
    } else {
        ui->responseStepRadioButton->setChecked(true);
    }

    ui->triggerOptionLineEdit->setText(reaction->optionName());
    ui->responseCodeTextEdit->setPlainText(reaction->customCode());
    ui->responseStepLineEdit->setText(reaction->nextStepId());
}

ReactionWidget::~ReactionWidget() {
    delete mWaitForClone;
    delete ui;
}

void ReactionWidget::enableStepIdCompletion(const Tutorial* tutorial,
                                            const Step* ownStep /*= 0*/) {
    QStringList stepIds;

    foreach (Step* step, tutorial->steps()) {
        if (step != ownStep && !step->reactions().contains(mReaction) &&
            !step->id().isEmpty()) {
            stepIds.append(step->id());
        }
    }

    ui->responseStepLineEdit->completionObject()->setItems(stepIds);
}

//protected:

QList<QUndoCommand*> ReactionWidget::createSaveCommands(QUndoCommand* parent) {
    QList<QUndoCommand*> commands;
    ReactionCommands reactionCommands(mReaction);

    QString optionName = ui->triggerOptionLineEdit->text();
    if (mReaction->optionName() != optionName) {
        commands.append(reactionCommands.setOptionName(optionName, parent));
    }

    if ((!mReaction->waitFor() && mWaitForWidget->waitFor()) ||
        (mReaction->waitFor() && !mWaitForWidget->waitFor()) ||
        (mReaction->waitFor() && mWaitForWidget->waitFor() &&
             *mReaction->waitFor() != *mWaitForWidget->waitFor())) {
        commands.append(reactionCommands.setWaitFor(mWaitForWidget->waitFor(),
                                                    parent));

        //Don't delete the cloned WaitFor when this ReactionWidget is deleted if
        //it was set to the reaction
        if (mWaitForClone == mWaitForWidget->waitFor()) {
            mWaitForClone = 0;
        }
    }

    QString customCode = ui->responseCodeTextEdit->toPlainText();
    if (mReaction->customCode() != customCode) {
        commands.append(reactionCommands.setCustomCode(customCode, parent));
    }

    QString nextStepId = ui->responseStepLineEdit->text();
    if (mReaction->nextStepId() != nextStepId) {
        commands.append(reactionCommands.setNextStepId(nextStepId, parent));
    }

    if (mReaction->triggerType() != mCurrentTriggerType) {
        commands.append(reactionCommands.setTriggerType(mCurrentTriggerType,
                                                        parent));
    }

    if (mReaction->responseType() != mCurrentResponseType) {
        commands.append(reactionCommands.setResponseType(mCurrentResponseType,
                                                         parent));
    }

    return commands;
}

//private slots:

void ReactionWidget::selectTriggerCondition(bool checked) {
    if (!checked) {
        return;
    }

    ui->triggerOptionLineEdit->setEnabled(false);
    mWaitForWidget->setEnabled(true);

    mCurrentTriggerType = Reaction::ConditionMet;
}

void ReactionWidget::selectTriggerOption(bool checked) {
    if (!checked) {
        return;
    }

    mWaitForWidget->setEnabled(false);
    ui->triggerOptionLineEdit->setEnabled(true);

    mCurrentTriggerType = Reaction::OptionSelected;
}

void ReactionWidget::selectResponseStep(bool checked) {
    if (!checked) {
        return;
    }

    ui->responseCodeTextEdit->setEnabled(false);
    ui->responseStepLineEdit->setEnabled(true);

    mCurrentResponseType = Reaction::NextStep;
}

void ReactionWidget::selectResponseCode(bool checked) {
    if (!checked) {
        return;
    }

    ui->responseStepLineEdit->setEnabled(false);
    ui->responseCodeTextEdit->setEnabled(true);

    mCurrentResponseType = Reaction::CustomCode;
}
