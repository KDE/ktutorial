/***************************************************************************
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

#include "WaitForPropertyWidget.h"

#include "ui_WaitForPropertyWidget.h"
#include "../data/WaitForProperty.h"

#ifdef QT_QTDBUS_FOUND
#include <KDebug>

#include "RemoteObjectNameWidget.h"
#include "../targetapplication/RemoteClass.h"
#include "../targetapplication/RemoteObject.h"
#endif

//public:

WaitForPropertyWidget::WaitForPropertyWidget(WaitForProperty* waitForProperty,
                                             QWidget* parent):
        EditionWidget(parent),
    mWaitForProperty(waitForProperty) {

    ui = new Ui::WaitForPropertyWidget();
#ifdef QT_QTDBUS_FOUND
    //Hack: RemoteObjectNameWidget is created before the other widgets to be
    //the first widget in the tab order. I feel dumb, but I tried using
    //setTabOrder and got nothing...
    mRemoteObjectNameWidget = new RemoteObjectNameWidget(this);
#endif
    ui->setupUi(this);

#ifdef QT_QTDBUS_FOUND
    //Replace ui->objectNameLineEdit with mRemoteObjectNameWidget
    ui->valueVerticalLayout->removeWidget(ui->objectNameLineEdit);
    delete ui->objectNameLineEdit;

    ui->valueVerticalLayout->insertWidget(0, mRemoteObjectNameWidget);
    ui->objectNameLabel->setBuddy(mRemoteObjectNameWidget);

    connect(mRemoteObjectNameWidget, SIGNAL(remoteObjectChosen(RemoteObject*)),
            this, SLOT(setChosenRemoteObject(RemoteObject*)));

    mRemoteObjectNameWidget->setName(waitForProperty->objectName());
#else
    ui->objectNameLineEdit->setText(waitForProperty->objectName());
#endif

    ui->propertyNameLineEdit->setText(waitForProperty->propertyName());

    ui->valueLineEdit->setText(waitForProperty->value());
}

WaitForPropertyWidget::~WaitForPropertyWidget() {
    delete ui;
}

void WaitForPropertyWidget::saveChanges() {
#ifdef QT_QTDBUS_FOUND
    QString objectName = mRemoteObjectNameWidget->name();
#else
    QString objectName = ui->objectNameLineEdit->text();
#endif
    if (mWaitForProperty->objectName() != objectName) {
        mWaitForProperty->setObjectName(objectName);
    }

    QString propertyName = ui->propertyNameLineEdit->text();
    if (mWaitForProperty->propertyName() != propertyName) {
        mWaitForProperty->setPropertyName(propertyName);
    }

    QString value = ui->valueLineEdit->text();
    if (mWaitForProperty->value() != value) {
        mWaitForProperty->setValue(value);
    }
}

//private:

#ifdef QT_QTDBUS_FOUND
void WaitForPropertyWidget::setPropertyCompletion(RemoteClass* remoteClass)
throw (DBusException) {
    KCompletion* completion = ui->propertyNameLineEdit->completionObject();
    completion->clear();
    completion->setOrder(KCompletion::Sorted);

    while (remoteClass) {
        foreach (const QString& property, remoteClass->propertyList()) {
            completion->addItem(property);
        }

        remoteClass = remoteClass->superClass();
    }
}
#endif

//private slots:

#ifdef QT_QTDBUS_FOUND
void WaitForPropertyWidget::setChosenRemoteObject(RemoteObject* remoteObject) {
    if (!remoteObject) {
        setPropertyCompletion(0);
        return;
    }

    try {
        setPropertyCompletion(remoteObject->remoteClass());
    } catch (DBusException e) {
        kWarning() << "The property completion could not be set, there was a"
                   << "problem getting the class of the remote object ("
                   << e.message() << ").";
    }
}
#endif
