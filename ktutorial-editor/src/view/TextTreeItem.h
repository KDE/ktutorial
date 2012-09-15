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

#ifndef TEXTTREEITEM_H
#define TEXTTREEITEM_H

#include "TreeItem.h"

/**
 * A simple TreeItem that represents a plain string.
 */
class TextTreeItem: public TreeItem {
public:

    /**
     * Creates a new TextTreeItem with the given parent.
     *
     * @param parent The parent TreeItem.
     */
    explicit TextTreeItem(TreeItem* parent = 0);

    /**
     * The text to be shown for this node of the tree,
     *
     * @return The text for this TreeItem.
     */
    virtual QString text() const;

    /**
     * Sets the text of this TextTreeItem.
     *
     * @param text The text to set.
     */
    void setText(const QString& text);

private:

    /**
     * The text to show.
     */
    QString mText;

};

#endif
