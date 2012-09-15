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

#include "EditionDialog.h"

#include <KLocalizedString>
#include <KPushButton>

#include "EditionWidget.h"

//public:

EditionDialog::EditionDialog(EditionWidget* editionWidget, QWidget* parent):
        KDialog(parent),
    mEditionWidget(editionWidget) {

    editionWidget->setParent(this);

    setMainWidget(editionWidget);
    setWindowTitle(editionWidget->windowTitle());

    setModal(true);

    setButtons(KDialog::Ok | KDialog::Cancel);

    button(KDialog::Ok)->setObjectName("okButton");
    button(KDialog::Cancel)->setObjectName("cancelButton");
}

//protected slots:

void EditionDialog::slotButtonClicked(int button) {
    if (button == KDialog::Ok) {
        mEditionWidget->saveChanges();
    }

    KDialog::slotButtonClicked(button);
}
