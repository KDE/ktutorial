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

#include "StepCustomCodeWidget.h"

#include <KLocalizedString>

#include "ui_CustomCodeWidget.h"
#include "../commands/StepCommands.h"
#include "../data/Step.h"

//public:

StepCustomCodeWidget::StepCustomCodeWidget(Step* step, Type type,
                                           QWidget* parent):
        CommandWidget(parent),
    mStep(step),
    mType(type) {

    ui = new Ui::CustomCodeWidget();
    ui->setupUi(this);

    if (type == Setup) {
        ui->customCodeTextEdit->setPlainText(step->customSetupCode());
        ui->customCodeGroupBox->setTitle(i18nc("@title", "Step setup code"));
        setWindowTitle(i18nc("@title", "Set the step setup code"));
        setWhatsThis(i18nc("@info:whatsthis", "<para>Set the code to be "
"executed when the tutorials passes to the step.</para>"
"<para>The code will be written as is to the setup of the step, after the "
"setup code generated automatically by the editor. This means that the code "
"must be written in the same programming language the tutorial will be "
"exported to.</para>"));
    } else {
        ui->customCodeTextEdit->setPlainText(step->customTearDownCode());
        ui->customCodeGroupBox->setTitle(i18nc("@title",
                                               "Step tear down code"));
        setWindowTitle(i18nc("@title", "Set the step tear down code"));
        setWhatsThis(i18nc("@info:whatsthis", "<para>Set the code to be "
"executed when the tutorial changes from this step to another.</para>"
"<para>The code will be written as is to the tear down of the step, before the "
"tear down code generated automatically by the editor. This means that the "
"code must be written in the same programming language the tutorial will be "
"exported to.</para>"));
    }
}

StepCustomCodeWidget::~StepCustomCodeWidget() {
    delete ui;
}

//protected:

QList<QUndoCommand*> StepCustomCodeWidget::createSaveCommands(
                                                        QUndoCommand* parent) {
    QList<QUndoCommand*> commands;
    StepCommands stepCommands(mStep);

    QString customCode = ui->customCodeTextEdit->toPlainText();
    if (mType == Setup && mStep->customSetupCode() != customCode) {
        commands.append(stepCommands.setCustomSetupCode(customCode, parent));
    }

    if (mType == TearDown && mStep->customTearDownCode() != customCode) {
        commands.append(stepCommands.setCustomTearDownCode(customCode, parent));
    }

    return commands;
}
