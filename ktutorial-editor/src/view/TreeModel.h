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

#ifndef TREEMODEL_H
#define TREEMODEL_H

#include <QAbstractItemModel>

class TreeItem;

/**
 * A read only model to represent nested data in a single column tree.
 * The model gets its data from a tree structure made of TreeItems. The root
 * item provides the data for the header and its children (and the children of
 * its children, and the children of the children of...) the data for the
 * contents.
 *
 * TreeItem objects are mapped one to one to items in the model. Each TreeItem
 * (but the root item) has its own index in the model. Top level items (direct
 * children of root item) have no parent indexes. For child items, their parent
 * index is the index of its parent item. The row count for any index is the
 * number of children of its TreeItem. There is only one column in any index (as
 * it is a single column tree).
 *
 * Each TreeItem provides the data to be shown by its associated model item.
 * Display role shows the text of the TreeItem.
 *
 * Changes in the tree layout or data provided by the TreeItems are notified to
 * the views, so they can be updated as needed.
 *
 * @see TreeItem
 */
class TreeModel: public QAbstractItemModel {
Q_OBJECT
public:

    /**
     * Creates a new TreeModel for the given root TreeItem.
     *
     * @param rootItem The root of the tree.
     * @param parent The parent object.
     */
    explicit TreeModel(TreeItem* rootItem, QObject* parent = 0);

    /**
     * Destroys this TreeModel and its tree of items.
     */
    virtual ~TreeModel();

    /**
     * Returns the data for the given index and role.
     * Display role with valid index returns the text of the TreeItem referred
     * to by the index. An invalid variant is returned otherwise.
     *
     * @param index The index.
     * @param role The role.
     * @return The data for the given index and role, or an invalid variant if
     *         there is no data.
     */
    virtual QVariant data(const QModelIndex& index,
                          int role = Qt::DisplayRole) const;

    /**
     * Returns the flags for the given index.
     * If the index is valid, the flags enable the item and allow it to be
     * selected.
     *
     * @param index The index.
     * @return The flags for the given index.
     */
    virtual Qt::ItemFlags flags(const QModelIndex& index) const;

    /**
     * Returns the data for the given role and section in the header with the
     * specified orientation.
     * Display role in section 0 and horizontal orientation returns the text of
     * the root item. An invalid variant is returned otherwise.
     *
     * @param section The section of the header.
     * @param orientation The orientation of the header.
     * @param role The role of the header.
     * @return The data for the header.
     */
    virtual QVariant headerData(int section, Qt::Orientation orientation,
                                int role = Qt::DisplayRole) const;

    /**
     * Returns the index of the item in the model specified by the given row,
     * column and parent index.
     * If the row or column is out of bounds for the given parent, an invalid
     * index is returned.
     *
     * @param row The row of the index.
     * @param column The column of the index.
     * @param parent The parent of the index.
     * @return The index for the given row, column and parent.
     */
    virtual QModelIndex index(int row, int column,
                              const QModelIndex& parent = QModelIndex()) const;

    /**
     * Returns the index for the parent of the model item with the given index.
     * If the index has no parent (it is invalid, or a top level index), an
     * invalid index is returned.
     *
     * @param index The index to get its parent.
     * @return The parent index of the given index.
     */
    virtual QModelIndex parent(const QModelIndex& index) const;

    /**
     * Returns the number of rows under the given parent.
     * It is the number of children of the TreeItem referred to by the index. If
     * the index is invalid, it is the number of top level items.
     *
     * @param parent The parent index.
     * @return The number of child rows.
     */
    virtual int rowCount(const QModelIndex& parent = QModelIndex()) const;

    /**
     * Returns the number of columns for the children of the given parent.
     * As this model is a single column model, it always returns 1.
     *
     * @param parent The parent index.
     * @return The number of columns for the children.
     */
    virtual int columnCount(const QModelIndex& parent = QModelIndex()) const;

private:

    /**
     * The root item.
     */
    TreeItem* mRootItem;

    /**
     * Starts watching changes in the given item and its children.
     * When something change in a registered item (the data is modified, a child
     * is added or a child is removed), the views are notified so they can be
     * updated as needed.
     *
     * @param item The item to register.
     */
    void registerTreeItemForUpdates(TreeItem* item);

    /**
     * Stops watching changes in the given item and its children.
     *
     * @param item The item to deregister.
     */
    void deregisterTreeItemFromUpdates(TreeItem* item);

    /**
     * Returns the index for the parent of the given item.
     *
     * @param item The item to get the index of its parent.
     * @return The parent index.
     */
    QModelIndex getParentIndex(TreeItem* item);

private Q_SLOTS:

    /**
     * Notifies the views that the given item is going to be added.
     *
     * @param item The item that is going to be added.
     * @param treeItemIndex The index in the parent item where the item is going
     *        to be added.
     */
    void treeItemAboutToBeInserted(TreeItem* item, int treeItemIndex);

    /**
     * Notifies the views that the given item has been added,
     *
     * @param item The item added.
     */
    void treeItemInserted(TreeItem* item);

    /**
     * Notifies the views that the given item is going to be removed.
     *
     * @param item The item that is going to be removed.
     */
    void treeItemAboutToBeRemoved(TreeItem* item);

    /**
     * Notifies the views that the given item has been removed.
     *
     * @param item The item removed.
     */
    void treeItemRemoved(TreeItem* item);

    /**
     * Notifies the views about a change in the data of the given item.
     *
     * @param item The item that changed.
     */
    void treeItemDataChanged(TreeItem* item);

};

#endif
