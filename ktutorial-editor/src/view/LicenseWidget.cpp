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

#include "LicenseWidget.h"

#include "ui_LicenseWidget.h"
#include "../commands/TutorialCommands.h"
#include "../data/Tutorial.h"

//public:

LicenseWidget::LicenseWidget(Tutorial* tutorial, QWidget* parent):
        CommandWidget(parent),
    mTutorial(tutorial) {
    ui = new Ui::LicenseWidget();
    ui->setupUi(this);

    ui->licenseTextEdit->setPlainText(tutorial->licenseText());
}

LicenseWidget::~LicenseWidget() {
    delete ui;
}

//protected:

QList<QUndoCommand*> LicenseWidget::createSaveCommands(QUndoCommand* parent) {
    QList<QUndoCommand*> commands;
    TutorialCommands tutorialCommands(mTutorial);

    QString licenseText = ui->licenseTextEdit->toPlainText();
    if (mTutorial->licenseText() != licenseText) {
        commands.append(tutorialCommands.setLicenseText(licenseText, parent));
    }

    return commands;
}
