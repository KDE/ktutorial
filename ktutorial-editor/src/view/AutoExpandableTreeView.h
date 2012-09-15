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

#ifndef AUTOEXPANDABLETREEVIEW_H
#define AUTOEXPANDABLETREEVIEW_H

#include <QTreeView>

/**
 * Special QTreeView that expands its item when are inserted or changed.
 * When an item is inserted, that item, all its children and all its parents are
 * expanded.
 * When an item is changed, the parents to that item are expanded. Note that
 * neither the item nor its children are expanded.
 *
 * Siblings of parent items are not expanded in any case. Only the parent
 * leading to the item to be expanded are expanded.
 */
class AutoExpandableTreeView: public QTreeView {
Q_OBJECT
public:

    /**
     * Creates a new AutoExpandableTreeView with the given parent.
     *
     * @param parent The parent widget.
     */
    AutoExpandableTreeView(QWidget* parent = 0);

protected:

    /**
     * This slot is called when items are changed in the model.
     * After executing QTreeView default behavior, all the parents of the
     * changed items are expanded. All the items should belong to the same
     * parent.
     *
     * Reimplemented from QTreeView::dataChanged(QModelIndex, QModelIndex).
     *
     * @param topLeft The top left item changed, inclusive.
     * @param bottomRight The bottom right item changed, inclusiev.
     */
    virtual void dataChanged(const QModelIndex& topLeft,
                             const QModelIndex& bottomRight);

    /**
     * This slot is called when rows are inserted.
     * After executing QTreeView default behavior, all the items inserted, all
     * their children and their parent are expanded.
     *
     * Reimplemented from QTreeView::rowsInserted(QModelIndex, int, int).
     *
     * @param parent The parent index where the children were added.
     * @param start The first row inserted, inclusive.
     * @param end The last row inserted, inclusive.
     */
    virtual void rowsInserted(const QModelIndex& parent, int start, int end);

private:

    /**
     * Expands all the parents of the given index.
     *
     * @param index The index to expand its parents.
     */
    void expandParents(const QModelIndex& index);

    /**
     * Expands an index and all its children.
     *
     * @param index The index to expand.
     */
    void expandRecursive(const QModelIndex& index);

};

#endif
