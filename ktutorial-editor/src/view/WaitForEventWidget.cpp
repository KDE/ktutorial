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

#include "WaitForEventWidget.h"

#include <QMetaEnum>

#include "ui_WaitForEventWidget.h"
#include "../data/WaitForEvent.h"

#ifdef QT_QTDBUS_FOUND
#include "RemoteObjectNameWidget.h"
#include "../targetapplication/RemoteObject.h"
#endif

//public:

WaitForEventWidget::WaitForEventWidget(WaitForEvent* waitForEvent,
                                       QWidget* parent):
        EditionWidget(parent),
    mWaitForEvent(waitForEvent) {

    ui = new Ui::WaitForEventWidget();
#ifdef QT_QTDBUS_FOUND
    //Hack: RemoteObjectNameWidget is created before the other widgets to be
    //the first widget in the tab order. I feel dumb, but I tried using
    //setTabOrder and got nothing...
    mRemoteObjectNameWidget = new RemoteObjectNameWidget(this);
#endif
    ui->setupUi(this);

#ifdef QT_QTDBUS_FOUND
    //Replace ui->receiverNameLineEdit with mRemoteObjectNameWidget
    ui->valueVerticalLayout->removeWidget(ui->receiverNameLineEdit);
    delete ui->receiverNameLineEdit;

    ui->valueVerticalLayout->insertWidget(0, mRemoteObjectNameWidget);
    ui->receiverNameLabel->setBuddy(mRemoteObjectNameWidget);

    mRemoteObjectNameWidget->setName(waitForEvent->receiverName());
#else
    ui->receiverNameLineEdit->setText(waitForEvent->receiverName());
#endif

    ui->eventNameLineEdit->setText(waitForEvent->eventName());

    ui->eventNameLineEdit->completionObject()->setOrder(KCompletion::Sorted);
    ui->eventNameLineEdit->completionObject()->setIgnoreCase(true);
    addItemsToEventNameCompletion();
}

WaitForEventWidget::~WaitForEventWidget() {
    delete ui;
}

void WaitForEventWidget::saveChanges() {
#ifdef QT_QTDBUS_FOUND
    QString receiverName = mRemoteObjectNameWidget->name();
#else
    QString receiverName = ui->receiverNameLineEdit->text();
#endif
    if (mWaitForEvent->receiverName() != receiverName) {
        mWaitForEvent->setReceiverName(receiverName);
    }

    QString eventName = ui->eventNameLineEdit->text();
    if (mWaitForEvent->eventName() != eventName) {
        mWaitForEvent->setEventName(eventName);
    }
}

//private:

void WaitForEventWidget::addItemsToEventNameCompletion() {
    int index = QEvent::staticMetaObject.indexOfEnumerator("Type");
    QMetaEnum eventTypeEnumerator = QEvent::staticMetaObject.enumerator(index);

    KCompletion* completion = ui->eventNameLineEdit->completionObject();
    for (int i=0; i<eventTypeEnumerator.keyCount(); ++i) {
        completion->addItem(eventTypeEnumerator.key(i));
    }
}
