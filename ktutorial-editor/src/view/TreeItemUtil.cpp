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

#include "TreeItemUtil.h"

#include "TextTreeItem.h"

//public:

void TreeItemUtil::addFlatItemIfNeeded(TreeItem* root, TextTreeItem*& item,
                                       int index) {
    if (item == 0) {
        item = new TextTreeItem(root);
        root->insertChild(item, index);
    }
}

void TreeItemUtil::removeFlatItemIfNeeded(TextTreeItem*& item) {
    if (item != 0) {
        item->parent()->removeChild(item);
        delete item;
        item = 0;
    }
}

void TreeItemUtil::addNestedItemIfNeeded(TreeItem* root, TextTreeItem*& item,
                                         int index, const QString& parentText) {
    if (item == 0) {
        TextTreeItem* parentItem = new TextTreeItem(root);
        parentItem->setText(parentText);

        item = new TextTreeItem(parentItem);
        parentItem->appendChild(item);

        root->insertChild(parentItem, index);
    }
}

void TreeItemUtil::removeNestedItemIfNeeded(TextTreeItem*& item) {
    if (item != 0) {
        item->parent()->parent()->removeChild(item->parent());
        delete item->parent();
        item = 0;
    }
}
