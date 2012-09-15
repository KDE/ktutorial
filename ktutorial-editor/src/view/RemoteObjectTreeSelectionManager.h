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

#ifndef REMOTEOBJECTTREESELECTIONMANAGER_H
#define REMOTEOBJECTTREESELECTIONMANAGER_H

#include <QItemSelectionModel>
#include <QObject>

class RemoteObject;
class TreeItem;

/**
 * Watches the QItemSelectionModel of a TreeModel (or a proxy to a TreeModel)
 * composed by RemoteObjectTreeItems for changes in the selection.
 * When an item is selected in the TreeModel (or its proxy),
 * remoteObjectSelected(RemoteObject*) signal is emitted with the RemoteObject
 * represented by the selected item.
 *
 * Only single item selections are supported.
 */
class RemoteObjectTreeSelectionManager: public QObject {
Q_OBJECT
public:

    /**
     * Creates a new RemoteObjectTreeSelectionManager that watchs the given
     * selection model.
     *
     * @param itemSelectionModel The selection model to watch for changes in the
     *                           selection.
     * @param parent The parent object.
     */
    explicit RemoteObjectTreeSelectionManager(
                QItemSelectionModel* itemSelectionModel, QObject* parent = 0);

Q_SIGNALS:

    /**
     * Emitted when a RemoteObject is selected.
     * If the RemoteObject is deselected and the new selected item isn't a
     * RemoteObject, the signal is emitted with a null pointer.
     *
     * @param remoteObject The selected RemoteObject, or null if it was
     *        deselected.
     */
    void remoteObjectSelected(RemoteObject* remoteObject);

private:

    /**
     * Emits remoteObjectSelected(RemoteObject*) signal based on the selected
     * and deselected items.
     *
     * @param selected The selected item, if any.
     * @param deselected The deselected item, if any.
     */
    void updateRemoteObjectSelection(TreeItem* selected, TreeItem* deselected);

    /**
     * Returns the RemoteObject represented by the given item.
     * If the item doesn't represent a RemoteObject or there is no item, a null
     * pointer is returned.
     *
     * @param item The item to get its represented RemoteObject.
     * @return The RemoteObject.
     */
    RemoteObject* getRemoteObjectForTreeItem(TreeItem* item);

    /**
     * Ensures that the index comes from a tree model and not from a proxy
     * model.
     *
     * @param index The index to "clean".
     * @return The index mapped to the given one in the source model, or the
     *         given index if it already comes from a source model.
     */
    QModelIndex getTreeModelIndex(const QModelIndex& index) const;

private Q_SLOTS:

    /**
     * Handles a change in the selection in the watched selection model.
     * Signals are emitted as needed based on the selected and deselected items.
     *
     * @param selected The item selection of selected items.
     * @param selected The item selection of deselected items.
     */
    void handleSelectionChanged(const QItemSelection& selected,
                                const QItemSelection& deselected);

};

#endif
