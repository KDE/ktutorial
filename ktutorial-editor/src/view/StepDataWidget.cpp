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

#include "StepDataWidget.h"

#include <KActionCollection>
#include <KToolBar>

#include "ui_StepDataWidget.h"
#include "SemanticMarkupEdition.h"
#include "../commands/StepCommands.h"
#include "../data/Reaction.h"
#include "../data/Step.h"
#include "../data/Tutorial.h"

//public:

StepDataWidget::StepDataWidget(Step* step, QWidget* parent):
        CommandWidget(parent),
    mStep(step) {
    ui = new Ui::StepDataWidget();
    ui->setupUi(this);

    KToolBar* toolBar = new KToolBar(this);
    int textEditIndex = ui->textLayout->indexOf(ui->textTextEdit);
    ui->textLayout->insertWidget(textEditIndex, toolBar);

    KActionCollection* editionActions = new KActionCollection(this);
    editionActions->addAssociatedWidget(toolBar);

    SemanticMarkupEdition* semanticMarkupEdition =
                                    new SemanticMarkupEdition(ui->textTextEdit);
    semanticMarkupEdition->createActions(editionActions);

    ui->idLineEdit->setText(step->id());
    ui->textTextEdit->setPlainText(step->text());
}

StepDataWidget::~StepDataWidget() {
    delete ui;
}

void StepDataWidget::enableStepIdCompletion(const Tutorial* tutorial) {
    QStringList usedStepIds;
    QStringList nextStepIds;

    foreach (Step* step, tutorial->steps()) {
        if (!step->id().isEmpty()) {
            usedStepIds.append(step->id());
        }

        if (step == mStep) {
            continue;
        }

        foreach (Reaction* reaction, step->reactions()) {
            if (!reaction->nextStepId().isEmpty()) {
                nextStepIds.append(reaction->nextStepId());
            }
        }
    }

    QStringList unusedStepIds;

    foreach (const QString& nextStepId, nextStepIds) {
        if (!usedStepIds.contains(nextStepId)) {
            unusedStepIds.append(nextStepId);
        }
    }

    ui->idLineEdit->completionObject()->setItems(unusedStepIds);
}

//protected:

QList<QUndoCommand*> StepDataWidget::createSaveCommands(QUndoCommand* parent) {
    QList<QUndoCommand*> commands;
    StepCommands stepCommands(mStep);

    QString id = ui->idLineEdit->text();
    if (mStep->id() != id) {
        commands.append(stepCommands.setId(id, parent));
    }

    QString text = ui->textTextEdit->toPlainText();
    if (mStep->text() != text) {
        commands.append(stepCommands.setText(text, parent));
    }

    return commands;
}
