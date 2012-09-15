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

#ifndef WAITFOREVENTTREEITEM_H
#define WAITFOREVENTTREEITEM_H

#include "WaitForTreeItem.h"

class WaitForEvent;

/**
 * A TreeItem that represents a WaitForEvent.
 * The tree representation of a WaitForEvent is a plain text:
 * When the event "event name" is received by the object "object name"
 *
 * If the event or the receiver name aren't set yet, a placeholder is put
 * instead. Event placeholder is "(event not set)", and the receiver name
 * placeholder is "(object name not set)" (without quotes, but with
 * parenthesis).
 *
 * Whenever the WaitForEvent data changes, the WaitForEventTreeItem text is
 * updated as needed.
 */
class WaitForEventTreeItem: public WaitForTreeItem {
Q_OBJECT
public:

    /**
     * Creates a new WaitForEventTreeItem for the given WaitForEvent and with
     * the given parent.
     *
     * @param waitForEvent The WaitForEvent to represent.
     * @param parent The parent TreeItem.
     */
    explicit WaitForEventTreeItem(WaitForEvent* waitForEvent,
                                  TreeItem* parent = 0);

    /**
     * Returns the description of the WaitForEvent.
     *
     * @return The text for this TreeItem.
     */
    virtual QString text() const;

private:

    /**
     * The receiver name of the WaitForEvent.
     */
    QString mReceiverName;

    /**
     * The event name of the WaitForEvent.
     */
    QString mEventName;

private Q_SLOTS:

    /**
     * Updates this WaitForEventTreeItem when the data of its WaitForEvent
     * changed.
     *
     * @param waitFor The WaitForEvent.
     */
    void update(WaitFor* waitFor);

};

#endif
