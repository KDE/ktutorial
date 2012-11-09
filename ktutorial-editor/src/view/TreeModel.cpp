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

#include "TreeModel.h"

#include "TreeItem.h"

//public:

TreeModel::TreeModel(TreeItem* rootItem, QObject* parent):
        QAbstractItemModel(parent),
    mRootItem(rootItem) {
    Q_ASSERT(rootItem);

    registerTreeItemForUpdates(rootItem);
}

TreeModel::~TreeModel() {
    delete mRootItem;
}

QVariant TreeModel::data(const QModelIndex& index, int role) const {
    if (!index.isValid()) {
        return QVariant();
    }

    if (role != Qt::DisplayRole) {
        return QVariant();
    }

    TreeItem* item = static_cast<TreeItem*>(index.internalPointer());

    return item->text();
}

Qt::ItemFlags TreeModel::flags(const QModelIndex& index) const {
    if (!index.isValid()) {
        return Qt::NoItemFlags;
    }

    return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}

QVariant TreeModel::headerData(int section, Qt::Orientation orientation,
                               int role) const {
    if (section == 0 && orientation == Qt::Horizontal &&
            role == Qt::DisplayRole) {
        return mRootItem->text();
    }

    return QVariant();
}

QModelIndex TreeModel::index(int row, int column,
                             const QModelIndex& parent) const {
    if (!hasIndex(row, column, parent)) {
        return QModelIndex();
    }

    TreeItem* parentItem;

    if (!parent.isValid()) {
        parentItem = mRootItem;
    } else {
        parentItem = static_cast<TreeItem*>(parent.internalPointer());
    }

    TreeItem* childItem = parentItem->child(row);

    return createIndex(row, column, childItem);
}

QModelIndex TreeModel::parent(const QModelIndex& index) const {
    if (!index.isValid()) {
        return QModelIndex();
    }

    TreeItem* childItem = static_cast<TreeItem*>(index.internalPointer());
    TreeItem* parentItem = childItem->parent();

    if (parentItem == mRootItem) {
        return QModelIndex();
    }

    return createIndex(parentItem->childIndex(), 0, parentItem);
}

int TreeModel::rowCount(const QModelIndex& parent) const {
    TreeItem* parentItem;
    if (!parent.isValid()) {
         parentItem = mRootItem;
    } else {
         parentItem = static_cast<TreeItem*>(parent.internalPointer());
    }

    return parentItem->childCount();
}

int TreeModel::columnCount(const QModelIndex& parent) const {
    Q_UNUSED(parent);

    return 1;
}

//private:

void TreeModel::registerTreeItemForUpdates(TreeItem* item) {
    connect(item, SIGNAL(childAboutToBeInserted(TreeItem*,int)),
            this, SLOT(treeItemAboutToBeInserted(TreeItem*,int)));
    connect(item, SIGNAL(childInserted(TreeItem*)),
            this, SLOT(treeItemInserted(TreeItem*)));
    connect(item, SIGNAL(childAboutToBeRemoved(TreeItem*)),
            this, SLOT(treeItemAboutToBeRemoved(TreeItem*)));
    connect(item, SIGNAL(childRemoved(TreeItem*)),
            this, SLOT(treeItemRemoved(TreeItem*)));
    connect(item, SIGNAL(dataChanged(TreeItem*)),
            this, SLOT(treeItemDataChanged(TreeItem*)));

    for (int i=0; i<item->childCount(); ++i) {
        registerTreeItemForUpdates(item->child(i));
    }
}

void TreeModel::deregisterTreeItemFromUpdates(TreeItem* item) {
    disconnect(item, SIGNAL(childAboutToBeInserted(TreeItem*,int)),
            this, SLOT(treeItemAboutToBeInserted(TreeItem*,int)));
    disconnect(item, SIGNAL(childInserted(TreeItem*)),
            this, SLOT(treeItemInserted(TreeItem*)));
    disconnect(item, SIGNAL(childAboutToBeRemoved(TreeItem*)),
            this, SLOT(treeItemAboutToBeRemoved(TreeItem*)));
    disconnect(item, SIGNAL(childRemoved(TreeItem*)),
            this, SLOT(treeItemRemoved(TreeItem*)));
    disconnect(item, SIGNAL(dataChanged(TreeItem*)),
            this, SLOT(treeItemDataChanged(TreeItem*)));

    for (int i=0; i<item->childCount(); ++i) {
        deregisterTreeItemFromUpdates(item->child(i));
    }
}

QModelIndex TreeModel::getParentIndex(TreeItem* item) {
    QModelIndex parent;

    if (item->parent() != mRootItem) {
        parent = createIndex(item->parent()->childIndex(), 0, item->parent());
    }

    return parent;
}

//private slots:

void TreeModel::treeItemAboutToBeInserted(TreeItem* item, int treeItemIndex) {
    beginInsertRows(getParentIndex(item), treeItemIndex, treeItemIndex);
}

void TreeModel::treeItemInserted(TreeItem* item) {
    endInsertRows();

    registerTreeItemForUpdates(item);
}

void TreeModel::treeItemAboutToBeRemoved(TreeItem* item) {
    int treeItemIndex = item->childIndex();

    beginRemoveRows(getParentIndex(item), treeItemIndex, treeItemIndex);
}

void TreeModel::treeItemRemoved(TreeItem* item) {
    deregisterTreeItemFromUpdates(item);

    endRemoveRows();
}

void TreeModel::treeItemDataChanged(TreeItem* item) {
    if (item == mRootItem) {
        emit headerDataChanged(Qt::Horizontal, 0, 0);
        return;
    }

    QModelIndex index = createIndex(item->childIndex(), 0, item);

    emit dataChanged(index, index);
}
