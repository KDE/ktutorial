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

#ifndef WAITFORCOMPOSEDTREEITEM_H
#define WAITFORCOMPOSEDTREEITEM_H

#include "WaitForTreeItem.h"
#include "../data/WaitForComposed.h"

/**
 * A TreeItem that represents a WaitForComposed.
 * The tree representation of a WaitForComposed is:
 * TextThatDependsOnTheTypeOfWaitForComposed
 *   |-Representation of first WaitFor child
 *   |-Representation of second WaitFor child
 *   ...
 *
 * The text that depends on the type of the WaitForComposed is
 * -"When all the contained conditions match" for And type
 * -"When any of the contained conditions match" for Or type
 *
 * Whenever the WaitForComposed data changes, a child is added or a child is
 * removed, the WaitForComposedTreeItem and its child items are updated as
 * needed.
 */
class WaitForComposedTreeItem: public WaitForTreeItem {
Q_OBJECT
public:

    /**
     * Creates a new WaitForComposedTreeItem for the given WaitForComposed and
     * with the given parent.
     *
     * @param waitForComposed The WaitForComposed to represent.
     * @param parent The parent TreeItem.
     */
    explicit WaitForComposedTreeItem(WaitForComposed* waitForComposed,
                                     TreeItem* parent = 0);

    /**
     * Returns the appropriate text for the type of the WaitForComposed.
     *
     * @return The text for this TreeItem.
     */
    virtual QString text() const;

private:

    /**
     * The composition type of the WaitForComposed.
     */
    WaitForComposed::CompositionType mCompositionType;

    /**
     * The WaitForTreeItem for each child of the WaitForComposed.
     */
    QList<WaitForTreeItem*> mWaitForTreeItems;

    /**
     * Returns the WaitForTreeItem for the given WaitFor.
     *
     * @param waitFor The WaitFor to get its WaitForTreeItem.
     * @return The WaitForTreeItem.
     */
    WaitForTreeItem* treeItemForWaitFor(WaitFor* waitFor);

private Q_SLOTS:

    /**
     * Updates this WaitForComposedTreeItem when the data of its WaitForComposed
     * changed.
     *
     * @param waitFor The WaitForComposed.
     */
    void update(WaitFor* waitFor);

    /**
     * Adds a new WaitForTreeItem when a WaitFor is added to the
     * WaitForComposed.
     *
     * @param waitFor The WaitFor added in the WaitForComposed.
     */
    void addWaitFor(WaitFor* waitFor);

    /**
     * Removes the WaitForTreeItem for the WaitFor removed from the
     * WaitForComposed.
     *
     * @param waitFor The WaitFor removed from the WaitForComposed.
     */
    void removeWaitFor(WaitFor* waitFor);

};

#endif
