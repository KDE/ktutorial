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

#include "TutorialInformationWidget.h"

#include "ui_TutorialInformationWidget.h"
#include "../commands/TutorialCommands.h"
#include "../data/Tutorial.h"

//public:

TutorialInformationWidget::TutorialInformationWidget(Tutorial* tutorial,
                                                     QWidget* parent):
        CommandWidget(parent),
    mTutorial(tutorial) {
    ui = new Ui::TutorialInformationWidget();
    ui->setupUi(this);

    ui->nameLineEdit->setText(tutorial->name());
    ui->descriptionTextEdit->setPlainText(tutorial->description());
}

TutorialInformationWidget::~TutorialInformationWidget() {
    delete ui;
}

//protected:

QList<QUndoCommand*> TutorialInformationWidget::createSaveCommands(
                                                        QUndoCommand* parent) {
    QList<QUndoCommand*> commands;
    TutorialCommands tutorialCommands(mTutorial);

    QString name = ui->nameLineEdit->text();
    if (mTutorial->name() != name) {
        commands.append(tutorialCommands.setName(name, parent));
    }

    QString description = ui->descriptionTextEdit->toPlainText();
    if (mTutorial->description() != description) {
        commands.append(tutorialCommands.setDescription(description, parent));
    }

    return commands;
}
