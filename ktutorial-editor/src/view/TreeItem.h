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

#ifndef TREEITEM_H
#define TREEITEM_H

#include <QString>
#include <QVariant>

/**
 * An item in a TreeModel.
 * TreeItems form tree structures that provide the data to TreeModels. The
 * TreeItems act as an adapter between the model and the real data: through
 * TreeItem custom subclasses a source of data can be represented as several
 * nested TreeItem objects that expose the data in some specific structure.
 *
 * For example, an object of a Person class with name, age and profession
 * attributes can be represented as a tree with a parent TreeItem that contains
 * the name of the person and two child TreeItems, one that contains the age and
 * one that contains the profession.
 *
 * The structure of a tree can be modified after being added to a TreeModel.
 * Changes in the layout (adding or removing child items) or the data are
 * notified to the TreeModel, which notifies the views to be updated as needed.
 *
 * TreeItem is an abstract class. Subclasses must implement its text() method,
 * that provides the data for that item to the TreeModel when DisplayRole is
 * used. They also have to emit dataChanged(TreeItem*) signal when the data to
 * be shown is modified.
 *
 * @see TreeModel
 */
class TreeItem: public QObject {
Q_OBJECT
public:

    /**
     * Creates a new TreeItem with the given parent.
     *
     * @param parent The parent TreeItem.
     */
    explicit TreeItem(TreeItem* parent = 0);

    /**
     * Destroys this TreeItem and all its children.
     */
    virtual ~TreeItem();

    /**
     * The text to be shown for this node of the tree,
     * This method must be implemented by subclasses.
     *
     * @return The text for this TreeItem.
     */
    virtual QString text() const = 0;

    /**
     * Returns the child tree item with the given index.
     *
     * @param index The index of the child to get.
     * @return The child with the given index.
     */
    TreeItem* child(int index);

    /**
     * Returns the number of child tree items.
     *
     * @return The number of children.
     */
    int childCount() const;

    /**
     * Adds a child at the end of the child list.
     *
     * @param child The child to add.
     */
    void appendChild(TreeItem* child);

    /**
     * Inserts a child at the given position of the child list.
     *
     * @param child The child to insert.
     * @param index The position to insert the child into.
     */
    void insertChild(TreeItem* child, int index);

    /**
     * Removes the given child from the child list.
     *
     * @param child The child to remove.
     */
    void removeChild(TreeItem* child);

    /**
     * The parent of this TreeItem.
     *
     * @return The parent of this TreeItem.
     */
    TreeItem* parent();

    /**
     * The index of this TreeItem in the child list of its parent.
     * If this TreeItem was not added or was removed from its parent, -1 is
     * returned.
     *
     * @return The index of this TreeItem.
     */
    int childIndex() const;

Q_SIGNALS:

    /**
     * Emitted just before a child is inserted or appended.
     * It is used to notify the TreeModel in order to update the views.
     *
     * @param child The child to be added.
     * @param index The position to insert it (if the item is appended, the
     *              index is equal to the child count).
     */
    void childAboutToBeInserted(TreeItem* child, int index);

    /**
     * Emitted after a child is inserted or appended.
     * It is used to notify the TreeModel in order to update the views.
     *
     * @param child The child added.
     */
    void childInserted(TreeItem* child);

    /**
     * Emitted just before a child is removed.
     * It is used to notify the TreeModel in order to update the views.
     *
     * @param child The child to be removed.
     */
    void childAboutToBeRemoved(TreeItem* child);

    /**
     * Emitted after a child is removed.
     * It is used to notify the TreeModel in order to update the views.
     *
     * @param child The child removed.
     */
    void childRemoved(TreeItem* child);

    /**
     * Emitted when the data to be used by the TreeModel changes.
     * It is used to notify the TreeModel in order to update the views.
     *
     * It must be emitted by subclasses as needed.
     *
     * @param item This TreeItem.
     */
    void dataChanged(TreeItem* item);

private:

    /**
     * The parent of this TreeItem.
     */
    TreeItem* mParent;

    /**
     * The children of this TreeItem.
     */
    QList<TreeItem*> mChildren;

};

#endif
