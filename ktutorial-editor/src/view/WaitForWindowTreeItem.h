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

#ifndef WAITFORWINDOWTREEITEM_H
#define WAITFORWINDOWTREEITEM_H

#include "WaitForTreeItem.h"

class WaitForWindow;

/**
 * A TreeItem that represents a WaitForWindow.
 * The tree representation of a WaitForWindow is a plain text:
 * When the window "object name" is shown
 *
 * If the window object name isn't set yet, a placeholder is put instead. The
 * object name placeholder is "(object name not set)" (without quotes, but with
 * parenthesis).
 *
 * Whenever the WaitForWindow data changes, the WaitForWindowTreeItem text is
 * updated as needed.
 */
class WaitForWindowTreeItem: public WaitForTreeItem {
Q_OBJECT
public:

    /**
     * Creates a new WaitForWindowTreeItem for the given WaitForWindow and with
     * the given parent.
     *
     * @param waitForWindow The WaitForWindow to represent.
     * @param parent The parent TreeItem.
     */
    explicit WaitForWindowTreeItem(WaitForWindow* waitForWindow,
                                   TreeItem* parent = 0);

    /**
     * Returns the description of the WaitForWindow.
     *
     * @return The text for this TreeItem.
     */
    virtual QString text() const;

private:

    /**
     * The window object name of the WaitForWindow.
     */
    QString mWindowObjectName;

private Q_SLOTS:

    /**
     * Updates this WaitForWindowTreeItem when the data of its WaitForWindow
     * changed.
     *
     * @param waitFor The WaitForWindow.
     */
    void update(WaitFor* waitFor);

};

#endif
