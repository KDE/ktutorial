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

#ifndef TREEITEMUTIL_H
#define TREEITEMUTIL_H

#include "TreeItem.h"

class TextTreeItem;

/**
 * Utility class for TreeItems.
 * It provides some static methods to modify the structure of TreeItems, adding
 * or removing children.
 */
class TreeItemUtil {
public:

    /**
     * If there is no item, inserts a new TextTreeItem at the given index in the
     * root item.
     * The new TextTreeItem is stored in the given item.
     *
     * @param root The root item to add the item to.
     * @param item The item to check and the variable to store the new item in.
     * @param index The index in the root item to add the item.
     */
    static void addFlatItemIfNeeded(TreeItem* root, TextTreeItem*& item,
                                    int index);

    /**
     * If there is an item, it is removed from its parent item.
     * The given item is set to null after deleting it.
     *
     * @param item The item to remove, delete and clean.
     */
    static void removeFlatItemIfNeeded(TextTreeItem*& item);

    /**
     * If there is no item, inserts a new nested TextTreeItem at the given index
     * in the root item.
     * A TextTreeItem is inserted with the given parentText, and a new
     * TextTreeItem child is appended to that parent.
     * The child is stored in the given item.
     *
     * @param root The root item to add the parent item to.
     * @param item The item to check and the place to store the new item.
     * @param index The index in the root item to add the parent item.
     * @param parentText The text to set in the added parent item.
     */
    static void addNestedItemIfNeeded(TreeItem* root, TextTreeItem*& item,
                                      int index, const QString& parentText);

    /**
     * If there is an item, its parent is removed from its parent item.
     * The given item is set to null after its parent is deleted.
     *
     * @param item The item to clean after removing and deleting its parent.
     */
    static void removeNestedItemIfNeeded(TextTreeItem*& item);

};

#endif
