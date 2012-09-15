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

//public:

WaitForProperty::WaitForProperty(QObject* parent): WaitFor(parent) {
}

WaitFor* WaitForProperty::clone() const {
    WaitForProperty* cloned = new WaitForProperty();
    cloned->setObjectName(mObjectName);
    cloned->setPropertyName(mPropertyName);
    cloned->setValue(mValue);

    return cloned;
}

bool WaitForProperty::equals(const WaitFor& waitFor) const {
    if (!qobject_cast<const WaitForProperty*>(&waitFor)) {
        return false;
    }

    const WaitForProperty* waitForProperty =
                                static_cast<const WaitForProperty*>(&waitFor);
    if (waitForProperty->objectName() != mObjectName) {
        return false;
    }

    if (waitForProperty->propertyName() != mPropertyName) {
        return false;
    }

    if (waitForProperty->value() != mValue) {
        return false;
    }

    return true;
}

QString WaitForProperty::objectName() const {
    return mObjectName;
}

void WaitForProperty::setObjectName(const QString& objectName) {
    mObjectName = objectName;

    emit dataChanged(this);
}

QString WaitForProperty::propertyName() const {
    return mPropertyName;
}

void WaitForProperty::setPropertyName(const QString& propertyName) {
    mPropertyName = propertyName;

    emit dataChanged(this);
}

QString WaitForProperty::value() const {
    return mValue;
}

void WaitForProperty::setValue(const QString& value) {
    mValue = value;

    emit dataChanged(this);
}
