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

#include "NewWaitForWidget.h"

#include "ui_NewWaitForWidget.h"
#include "../data/WaitForComposed.h"
#include "../data/WaitForEvent.h"
#include "../data/WaitForNot.h"
#include "../data/WaitForProperty.h"
#include "../data/WaitForSignal.h"
#include "../data/WaitForStepActivation.h"
#include "../data/WaitForWindow.h"

//public:

NewWaitForWidget::NewWaitForWidget(QWidget* parent): QWidget(parent) {
    ui = new Ui::NewWaitForWidget();
    ui->setupUi(this);
}

NewWaitForWidget::~NewWaitForWidget() {
    delete ui;
}

WaitFor* NewWaitForWidget::waitFor() const {
    int index = ui->waitForTypeComboBox->currentIndex();

    if (index == 0) {
        WaitForComposed* waitFor = new WaitForComposed();
        waitFor->setCompositionType(WaitForComposed::And);
        return waitFor;
    } else if (index == 1) {
        WaitForComposed* waitFor = new WaitForComposed();
        waitFor->setCompositionType(WaitForComposed::Or);
        return waitFor;
    } else if (index == 2) {
        return new WaitForNot();
    } else if (index == 3) {
        return new WaitForSignal();
    } else if (index == 4) {
        return new WaitForEvent();
    } else if (index == 5) {
        return new WaitForWindow();
    } else if (index == 6) {
        return new WaitForProperty();
    } else if (index == 7) {
        return new WaitForStepActivation();
    }

    return 0;
}
