/***************************************************************************
 *   Copyright (C) 2008 by Daniel Calviño Sánchez <danxuliu@gmail.com>     *
 *   Copyright (C) 2009 by Daniel Calviño Sánchez <danxuliu@gmail.com>     *
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

#include "TutorialManagerDialog.h"
#include "TutorialListModel.h"
#include "ui_TutorialManagerDialog.h"
#include "../TutorialInformation.h"
#include "../TutorialManager.h"

namespace ktutorial {
namespace view {

//public:

TutorialManagerDialog::TutorialManagerDialog(TutorialManager* tutorialManager,
                                             QWidget* parent):
        KDialog(parent),
    mTutorialManager(tutorialManager),
    mCurrentTutorialInformation(0) {

    QWidget *widget = new QWidget(this);
    ui = new Ui::TutorialManagerDialog();
    ui->setupUi(widget);

    setMainWidget(widget);

    setCaption(i18nc("@title:window", "Tutorial manager"));
    setButtons(KDialog::User1 | KDialog::Close);

    ui->tutorialsList->setModel(new TutorialListModel(tutorialManager, this));

    setButtonIcon(User1, KIcon("dialog-ok"));
    setButtonText(User1, i18nc("@action:button Used to start a tutorial", "Start"));
    setButtonToolTip(User1, i18nc("@info:tooltip", "Start the selected tutorial"));
    setButtonWhatsThis(User1, i18nc("@info:whatsthis", "Start the selected tutorial"));
    setDefaultButton(User1);
    enableButton(User1, false);

    connect(ui->tutorialsList->selectionModel(),
            SIGNAL(selectionChanged(QItemSelection,QItemSelection)),
            this, SLOT(select(QItemSelection)));
    connect(this, SIGNAL(user1Clicked()), this, SLOT(start()));

    connect(mTutorialManager, SIGNAL(finished()), this, SLOT(finish()));
}

TutorialManagerDialog::~TutorialManagerDialog() {
    delete ui;
}

//public slots:

void TutorialManagerDialog::finish() {
    show();
}

//private:

void TutorialManagerDialog::select(const TutorialInformation* tutorialInformation) {
    if (tutorialInformation == 0) {
        return;
    }

    enableButton(User1, true);

    ui->descriptionLabel->setText(tutorialInformation->description());

    mCurrentTutorialInformation = tutorialInformation;
}

//private slots:

void TutorialManagerDialog::select(const QItemSelection& selected) {
    const TutorialListModel* model = qobject_cast<const TutorialListModel*>(selected.indexes()[0].model());

    select(model->getTutorialInformationForIndex(selected.indexes()[0]));
}

void TutorialManagerDialog::start() {
    hide();

    mTutorialManager->start(mCurrentTutorialInformation->id());
}

}
}
