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

#ifndef WAITFORNOTTREEITEM_H
#define WAITFORNOTTREEITEM_H

#include "WaitForTreeItem.h"

class WaitForNot;

/**
 * A TreeItem that represents a WaitForNot.
 * The tree representation of a WaitForNot is:
 * The contained condition can't have been met
 *   -Representation of the negated WaitFor
 *
 * Whenever another negated WaitFor is set, the child item of the
 * WaitForNotTreeItem is updated as needed.
 */
class WaitForNotTreeItem: public WaitForTreeItem {
Q_OBJECT
public:

    /**
     * Creates a new WaitForNotTreeItem for the given WaitForNot and with the
     * given parent.
     *
     * @param waitForNot The WaitForNot to represent.
     * @param parent The parent TreeItem.
     */
    explicit WaitForNotTreeItem(WaitForNot* waitForNot, TreeItem* parent = 0);

    /**
     * Returns "The contained condition can't have been met".
     *
     * @return The text for this TreeItem.
     */
    virtual QString text() const;

private:

    /**
     * The representation of the negated WaitFor.
     */
    WaitForTreeItem* mNegatedWaitForItem;

private Q_SLOTS:

    /**
     * Updates the child WaitForTreeItem when the negated WaitFor changes in the
     * given WaitFor.
     *
     * @param waitFor The WaitForNot.
     */
    void update(WaitFor* waitFor);

};

#endif
