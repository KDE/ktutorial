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

#ifndef WAITFORPROPERTYTREEITEM_H
#define WAITFORPROPERTYTREEITEM_H

#include "WaitForTreeItem.h"

class WaitForProperty;

/**
 * A TreeItem that represents a WaitForProperty.
 * The tree representation of a WaitForProperty is a plain text:
 * When the property "property name" in the object "object name" has the value
 * value
 *
 * If the property, the object name or the value aren't set yet, a placeholder
 * is put instead. Property placeholder is "(property not set)", the object name
 * placeholder is "(object name not set)" , and the value placeholder is "(value
 * not set)" (without quotes, but with parenthesis).
 *
 * Whenever the WaitForProperty data changes, the WaitForPropertyTreeItem text
 * is updated as needed.
 */
class WaitForPropertyTreeItem: public WaitForTreeItem {
Q_OBJECT
public:

    /**
     * Creates a new WaitForPropertyTreeItem for the given WaitForProperty and
     * with the given parent.
     *
     * @param waitForProperty The WaitForProperty to represent.
     * @param parent The parent TreeItem.
     */
    explicit WaitForPropertyTreeItem(WaitForProperty* waitForProperty,
                                     TreeItem* parent = 0);

    /**
     * Returns the description of the WaitForProperty.
     *
     * @return The text for this TreeItem.
     */
    virtual QString text() const;

private:

    /**
     * The object name of the WaitForProperty.
     */
    QString mObjectName;

    /**
     * The property name of the WaitForProperty.
     */
    QString mPropertyName;

    /**
     * The value of the WaitForProperty.
     */
    QString mValue;

private Q_SLOTS:

    /**
     * Updates this WaitForPropertyTreeItem when the data of its WaitForProperty
     * changed.
     *
     * @param waitFor The WaitForProperty.
     */
    void update(WaitFor* waitFor);

};

#endif
