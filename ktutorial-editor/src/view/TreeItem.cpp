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

#include "TreeItem.h"

TreeItem::TreeItem(TreeItem* parent):
    mParent(parent) {
}

TreeItem::~TreeItem() {
    qDeleteAll(mChildren);
}

TreeItem* TreeItem::child(int index) {
    return mChildren.value(index);
}

int TreeItem::childCount() const {
    return mChildren.count();
}

void TreeItem::appendChild(TreeItem* child) {
    Q_ASSERT(!mChildren.contains(child));
    Q_ASSERT(child->parent() == this);

    emit childAboutToBeInserted(child, mChildren.count());

    mChildren.append(child);

    emit childInserted(child);
}

void TreeItem::insertChild(TreeItem* child, int index) {
    Q_ASSERT(!mChildren.contains(child));
    Q_ASSERT(child->parent() == this);

    emit childAboutToBeInserted(child, index);

    mChildren.insert(index, child);

    emit childInserted(child);
}

void TreeItem::removeChild(TreeItem* child) {
    Q_ASSERT(mChildren.contains(child));
    Q_ASSERT(child->parent() == this);

    emit childAboutToBeRemoved(child);

    mChildren.removeAt(mChildren.indexOf(child));

    emit childRemoved(child);
}

TreeItem* TreeItem::parent() {
    return mParent;
}

int TreeItem::childIndex() const {
    for (int i = 0; i < mParent->childCount(); ++i) {
        if (mParent->child(i) == this) {
            return i;
        }
    }

    return -1;
}
