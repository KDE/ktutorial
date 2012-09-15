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

#include "WaitForWindowWidget.h"

#include "ui_WaitForWindowWidget.h"
#include "../data/WaitForWindow.h"

#ifdef QT_QTDBUS_FOUND
#include "RemoteObjectNameWidget.h"
#endif

//public:

WaitForWindowWidget::WaitForWindowWidget(WaitForWindow* waitForWindow,
                                         QWidget* parent):
        EditionWidget(parent),
    mWaitForWindow(waitForWindow) {

    ui = new Ui::WaitForWindowWidget();
    ui->setupUi(this);

#ifdef QT_QTDBUS_FOUND
    //Replace ui->windowObjectNameLineEdit with mRemoteObjectNameWidget
    ui->valueVerticalLayout->removeWidget(ui->windowObjectNameLineEdit);
    delete ui->windowObjectNameLineEdit;

    mRemoteObjectNameWidget = new RemoteObjectNameWidget(this);
    ui->valueVerticalLayout->insertWidget(0, mRemoteObjectNameWidget);
    ui->windowObjectNameLabel->setBuddy(mRemoteObjectNameWidget);

    mRemoteObjectNameWidget->setName(waitForWindow->windowObjectName());
#else
    ui->windowObjectNameLineEdit->setText(waitForWindow->windowObjectName());
#endif
}

WaitForWindowWidget::~WaitForWindowWidget() {
    delete ui;
}

void WaitForWindowWidget::saveChanges() {
#ifdef QT_QTDBUS_FOUND
    QString windowObjectName = mRemoteObjectNameWidget->name();
#else
    QString windowObjectName = ui->windowObjectNameLineEdit->text();
#endif
    if (mWaitForWindow->windowObjectName() != windowObjectName) {
        mWaitForWindow->setWindowObjectName(windowObjectName);
    }
}
