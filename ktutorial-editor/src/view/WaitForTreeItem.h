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

#ifndef WAITFORTREEITEM_H
#define WAITFORTREEITEM_H

#include "TreeItem.h"

class WaitFor;

/**
 * Abstract base class for TreeItems that represent WaitFors.
 * It provides a static method, treeItemForWaitFor(WaitFor*), to create a new
 * WaitForTreeItem subclass object suitable for the given WaitFor.
 *
 * Subclasses must provide the full representation of its WaitFor, implementing
 * text() method and adding the necessary child tree items.
 */
class WaitForTreeItem: public TreeItem {
Q_OBJECT
public:

    /**
     * Returns a new TreeItem that represents the given WaitFor.
     *
     * @param waitFor The WaitFor to create its representation.
     * @param parent The parent TreeItem.
     * @return The new WaitForTreeItem to represent the given WaitFor.
     */
    static WaitForTreeItem* treeItemForWaitFor(WaitFor* waitFor,
                                               TreeItem* parent);

    /**
     * Creates a new WaitForTreeItem for the given WaitFor and with the given
     * parent.
     *
     * @param waitFor The WaitFor to represent.
     * @param parent The parent TreeItem.
     */
    explicit WaitForTreeItem(WaitFor* waitFor, TreeItem* parent = 0);

    /**
     * Returns the WaitFor.
     *
     * @return The WaitFor.
     */
    WaitFor* waitFor() const;

private:

    /**
     * The WaitFor.
     */
    WaitFor* mWaitFor;

};

#endif
