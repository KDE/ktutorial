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

#include "AutoExpandableTreeView.h"

//public:

AutoExpandableTreeView::AutoExpandableTreeView(QWidget* parent):
    QTreeView(parent) {
}

//protected:

void AutoExpandableTreeView::dataChanged(const QModelIndex& topLeft,
                                         const QModelIndex& bottomRight) {
    QTreeView::dataChanged(topLeft, bottomRight);

    expandParents(topLeft);
}

void AutoExpandableTreeView::rowsInserted(const QModelIndex& parent, int start,
                                          int end) {
    QTreeView::rowsInserted(parent, start, end);

    expand(parent);
    expandParents(parent);

    for (int i=start; i<=end; ++i) {
        expandRecursive(model()->index(i, 0, parent));
    }
}

//private:

void AutoExpandableTreeView::expandParents(const QModelIndex& index) {
    QModelIndex parent = index.parent();
    while (parent.isValid()) {
        expand(parent);
        parent = parent.parent();
    }
}

void AutoExpandableTreeView::expandRecursive(const QModelIndex& index) {
    expand(index);

    for (int i=0; i<model()->rowCount(index); ++i) {
        expandRecursive(model()->index(i, 0, index));
    }
}
