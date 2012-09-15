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

#include "WaitForPropertyTreeItem.h"

#include <KLocalizedString>

#include "../data/WaitForProperty.h"

//public:

WaitForPropertyTreeItem::WaitForPropertyTreeItem(
                                            WaitForProperty* waitForProperty,
                                            TreeItem* parent):
        WaitForTreeItem(waitForProperty, parent) {
    mObjectName = waitForProperty->objectName();
    mPropertyName = waitForProperty->propertyName();
    mValue = waitForProperty->value();

    connect(waitForProperty, SIGNAL(dataChanged(WaitFor*)),
            this, SLOT(update(WaitFor*)));
}

QString WaitForPropertyTreeItem::text() const {
    QString objectName;
    if (mObjectName.isEmpty()) {
        objectName = i18nc("@item", "(object name not set)");
    } else {
        objectName = "\"" + mObjectName + "\"";
    }

    QString propertyName;
    if (mPropertyName.isEmpty()) {
        propertyName = i18nc("@item", "(property not set)");
    } else {
        propertyName = "\"" + mPropertyName + "\"";
    }

    QString value;
    if (mValue.isEmpty()) {
        value = i18nc("@item", "(value not set)");
    } else {
        value = mValue;
    }

    return i18nc("@item", "When the property %1 in the object %2 has the value "
                          "%3", propertyName, objectName, value);
}

//private:

void WaitForPropertyTreeItem::update(WaitFor* waitFor) {
    WaitForProperty* waitForProperty = static_cast<WaitForProperty*>(waitFor);
    mObjectName = waitForProperty->objectName();
    mPropertyName = waitForProperty->propertyName();
    mValue = waitForProperty->value();

    emit dataChanged(this);
}
