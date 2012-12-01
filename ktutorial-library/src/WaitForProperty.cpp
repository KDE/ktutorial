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

#include "WaitForProperty.h"
#include "WaitForProperty_p.h"

#include <QMetaProperty>

#include <KDebug>

namespace ktutorial {
extern int debugArea();
}

namespace ktutorial {

//public:

WaitForProperty::WaitForProperty(): WaitFor(),
    d(new WaitForPropertyPrivate()) {
}

WaitForProperty::WaitForProperty(QObject* object, const QString& propertyName,
                                 const QVariant& value): WaitFor(),
    d(new WaitForPropertyPrivate()) {
    setProperty(object, propertyName, value);
}

WaitForProperty::~WaitForProperty() {
    delete d;
}

void WaitForProperty::setProperty(QObject* object, const QString& propertyName,
                                  const QVariant& value) {
    d->mObject = object;
    d->mPropertyName = propertyName;
    d->mValue = value;

    if (!d->mObject) {
        kWarning(debugArea()) << "The object that contains the property"
                              << d->mPropertyName << "to wait for is null!";
        return;
    }

    const QMetaObject* metaObject = d->mObject->metaObject();
    int propertyIndex = metaObject->indexOfProperty(d->mPropertyName.toUtf8());

    if (propertyIndex == -1) {
        kWarning(debugArea()) << "The class" << metaObject->className()
                              << "does not contain a property named"
                              << d->mPropertyName << "!";
        return;
    }

    QMetaProperty metaProperty = metaObject->property(propertyIndex);

    if (!metaProperty.hasNotifySignal()) {
        kWarning(debugArea()) << "The property" << d->mPropertyName << "in the"
                              << "class" << metaObject->className()
                              << "does not have a notification signal!";
        return;
    }

    const char* notifySignalSignature = metaProperty.notifySignal().signature();
    QString notifySignal = QString("2%1").arg(notifySignalSignature);

    connect(object, notifySignal.toUtf8(),
            this, SLOT(checkPropertyValueToEndTheWait()));
}

void WaitForProperty::setPropertyToWaitFor(QObject* object,
                                           const QString& propertyName,
                                           const QVariant& value) {
    setProperty(object, propertyName, value);
}

bool WaitForProperty::conditionMet() const {
    if (!d->mObject) {
        return false;
    }

    if (d->mObject->property(d->mPropertyName.toUtf8()) != d->mValue) {
        return false;
    }

    return true;
}

//private slots:

void WaitForProperty::checkPropertyValueToEndTheWait() {
    if (!isActive()) {
        return;
    }

    if (conditionMet()) {
        emit waitEnded(this);
    }
}

}
