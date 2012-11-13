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

#include "WaitForSignalWidget.h"

#include "ui_WaitForSignalWidget.h"
#include "../data/WaitForSignal.h"

#ifdef QT_QTDBUS_FOUND
#include <KDebug>

#include "RemoteObjectNameWidget.h"
#include "../targetapplication/RemoteClass.h"
#include "../targetapplication/RemoteObject.h"
#endif

#include <QDebug>

//public:

WaitForSignalWidget::WaitForSignalWidget(WaitForSignal* waitForSignal,
                                         QWidget* parent):
        EditionWidget(parent),
    mWaitForSignal(waitForSignal) {

    ui = new Ui::WaitForSignalWidget();
#ifdef QT_QTDBUS_FOUND
    //Hack: RemoteObjectNameWidget is created before the other widgets to be
    //the first widget in the tab order. I feel dumb, but I tried using
    //setTabOrder and got nothing...
    mRemoteObjectNameWidget = new RemoteObjectNameWidget(this);
#endif
    ui->setupUi(this);

#ifdef QT_QTDBUS_FOUND
    //Replace ui->emitterNameLineEdit with mRemoteObjectNameWidget
    ui->valueVerticalLayout->removeWidget(ui->emitterNameLineEdit);
    delete ui->emitterNameLineEdit;

    ui->valueVerticalLayout->insertWidget(0, mRemoteObjectNameWidget);
    ui->emitterNameLabel->setBuddy(mRemoteObjectNameWidget);

    connect(mRemoteObjectNameWidget, SIGNAL(remoteObjectChosen(RemoteObject*)),
            this, SLOT(setChosenRemoteObject(RemoteObject*)));

    mRemoteObjectNameWidget->setName(waitForSignal->emitterName());
#else
    ui->emitterNameLineEdit->setText(waitForSignal->emitterName());
#endif

    ui->signalNameLineEdit->setText(waitForSignal->signalName());
}

WaitForSignalWidget::~WaitForSignalWidget() {
    delete ui;
}

void WaitForSignalWidget::saveChanges() {
#ifdef QT_QTDBUS_FOUND
    QString emitterName = mRemoteObjectNameWidget->name();
#else
    QString emitterName = ui->emitterNameLineEdit->text();
#endif
    if (mWaitForSignal->emitterName() != emitterName) {
        mWaitForSignal->setEmitterName(emitterName);
    }

    QString signalName = ui->signalNameLineEdit->text();
    if (mWaitForSignal->signalName() != signalName) {
        mWaitForSignal->setSignalName(signalName);
    }
}

//private:

#ifdef QT_QTDBUS_FOUND
void WaitForSignalWidget::setSignalCompletion(RemoteClass* remoteClass)
throw (DBusException) {
    KCompletion* completion = ui->signalNameLineEdit->completionObject();
    completion->clear();
    completion->setOrder(KCompletion::Sorted);

    while (remoteClass) {
        foreach (const QString& signal, remoteClass->signalList()) {
            completion->addItem(signal);
        }

        remoteClass = remoteClass->superClass();
    }
}
#endif

//private slots:

#ifdef QT_QTDBUS_FOUND
void WaitForSignalWidget::setChosenRemoteObject(RemoteObject* remoteObject) {
    if (!remoteObject) {
        setSignalCompletion(0);
        return;
    }

    try {
        setSignalCompletion(remoteObject->remoteClass());
    } catch (DBusException e) {
        kWarning() << "The signal completion could not be set, there was a"
                   << "problem getting the class of the remote object ("
                   << e.message() << ").";
    }
}
#endif
