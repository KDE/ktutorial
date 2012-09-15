/***************************************************************************
 *   Copyright (C) 2010 by Daniel Calviño Sánchez <danxuliu@gmail.com>     *
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

#include "RemoteObjectTreeSelectionManager.h"

#include <QAbstractProxyModel>

#include "RemoteObjectTreeItem.h"

//public:

RemoteObjectTreeSelectionManager::RemoteObjectTreeSelectionManager(
                                    QItemSelectionModel* itemSelectionModel,
                                    QObject* parent): QObject(parent) {
    connect(itemSelectionModel,
            SIGNAL(selectionChanged(QItemSelection,QItemSelection)),
            this,
            SLOT(handleSelectionChanged(QItemSelection,QItemSelection)));
}

//private:

void RemoteObjectTreeSelectionManager::updateRemoteObjectSelection(
                                                       TreeItem* selected,
                                                       TreeItem* deselected) {
    RemoteObject* selectedRemoteObject = getRemoteObjectForTreeItem(selected);
    RemoteObject* deselectedRemoteObject = getRemoteObjectForTreeItem(
                                                                    deselected);

    if (selectedRemoteObject &&
            selectedRemoteObject != deselectedRemoteObject) {
        emit remoteObjectSelected(selectedRemoteObject);
        return;
    }

    if (!selectedRemoteObject && deselectedRemoteObject) {
        emit remoteObjectSelected(0);
        return;
    }
}

RemoteObject* RemoteObjectTreeSelectionManager::getRemoteObjectForTreeItem(
                                                            TreeItem* item) {
    if (qobject_cast<RemoteObjectTreeItem*>(item)) {
        return static_cast<RemoteObjectTreeItem*>(item)->remoteObject();
    }

    if (item == 0 || item->parent() == 0) {
        return 0;
    }

    return getRemoteObjectForTreeItem(item->parent());
}

QModelIndex RemoteObjectTreeSelectionManager::getTreeModelIndex(
                                            const QModelIndex& index) const {
    const QAbstractProxyModel* proxyModel =
                        qobject_cast<const QAbstractProxyModel*>(index.model());
    if (proxyModel) {
        return proxyModel->mapToSource(index);
    }

    return index;
}

//private slots:

void RemoteObjectTreeSelectionManager::handleSelectionChanged(
                                            const QItemSelection& selected,
                                            const QItemSelection& deselected) {
    //Only single selections are supported
    Q_ASSERT(selected.count() <= 1);
    Q_ASSERT(deselected.count() <= 1);

    TreeItem* selectedItem = 0;
    TreeItem* deselectedItem = 0;

    if (selected.count() == 1) {
        Q_ASSERT(selected.at(0).indexes().count() == 1);

        QModelIndex index = selected.at(0).indexes().at(0);
        index = getTreeModelIndex(index);
        selectedItem = static_cast<TreeItem*>(index.internalPointer());
    }

    if (deselected.count() == 1) {
        Q_ASSERT(deselected.at(0).indexes().count() == 1);

        QModelIndex index = deselected.at(0).indexes().at(0);
        index = getTreeModelIndex(index);
        deselectedItem = static_cast<TreeItem*>(index.internalPointer());
    }

    updateRemoteObjectSelection(selectedItem, deselectedItem);
}
