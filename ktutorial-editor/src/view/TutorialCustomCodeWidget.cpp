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

#include "TutorialCustomCodeWidget.h"

#include <KLocalizedString>

#include "ui_CustomCodeWidget.h"
#include "../commands/TutorialCommands.h"
#include "../data/Tutorial.h"

//public:

TutorialCustomCodeWidget::TutorialCustomCodeWidget(Tutorial* tutorial,
                                                   Type type, QWidget* parent):
        CommandWidget(parent),
    mTutorial(tutorial),
    mType(type) {

    ui = new Ui::CustomCodeWidget();
    ui->setupUi(this);

    if (type == Setup) {
        ui->customCodeTextEdit->setPlainText(tutorial->customSetupCode());
        ui->customCodeGroupBox->setTitle(i18nc("@title",
                                               "Tutorial setup code"));
        setWindowTitle(i18nc("@title", "Set the tutorial setup code"));
        setWhatsThis(i18nc("@info:whatsthis", "<para>Set the code to be "
"executed when the tutorial starts.</para>"
"<para>The code will be written as is to the setup of the tutorial, after the "
"setup code generated automatically by the editor. This means that the code "
"must be written in the same programming language the tutorial will be "
"exported to.</para>"));
    } else {
        ui->customCodeTextEdit->setPlainText(tutorial->customTearDownCode());
        ui->customCodeGroupBox->setTitle(i18nc("@title",
                                               "Tutorial tear down code"));
        setWindowTitle(i18nc("@title", "Set the tutorial tear down code"));
        setWhatsThis(i18nc("@info:whatsthis", "<para>Set the code to be "
"executed when the tutorial finishes.</para>"
"<para>The code will be written as is to the tear down of the tutorial, before "
"the tear down code generated automatically by the editor. This means that the "
"code must be written in the same programming language the tutorial will be "
"exported to.</para>"));
    }
}

TutorialCustomCodeWidget::~TutorialCustomCodeWidget() {
    delete ui;
}

//protected:

QList<QUndoCommand*> TutorialCustomCodeWidget::createSaveCommands(
                                                        QUndoCommand* parent) {
    QList<QUndoCommand*> commands;
    TutorialCommands tutorialCommands(mTutorial);

    QString customCode = ui->customCodeTextEdit->toPlainText();
    if (mType == Setup && mTutorial->customSetupCode() != customCode) {
        commands.append(tutorialCommands.setCustomSetupCode(customCode,
                                                            parent));
    }

    if (mType == TearDown && mTutorial->customTearDownCode() != customCode) {
        commands.append(tutorialCommands.setCustomTearDownCode(customCode,
                                                               parent));
    }

    return commands;
}
