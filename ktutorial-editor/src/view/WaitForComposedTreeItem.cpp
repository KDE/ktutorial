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

#include "WaitForComposedTreeItem.h"

#include <KLocalizedString>

#include "../data/WaitForComposed.h"

//public:

WaitForComposedTreeItem::WaitForComposedTreeItem(
                        WaitForComposed* waitForComposed, TreeItem* parent):
        WaitForTreeItem(waitForComposed, parent) {
    mCompositionType = waitForComposed->compositionType();

    foreach (WaitFor* waitFor, waitForComposed->waitFors()) {
        addWaitFor(waitFor);
    }

    connect(waitForComposed, SIGNAL(dataChanged(WaitFor*)),
            this, SLOT(update(WaitFor*)));
    connect(waitForComposed, SIGNAL(waitForAdded(WaitFor*)),
            this, SLOT(addWaitFor(WaitFor*)));
    connect(waitForComposed, SIGNAL(waitForRemoved(WaitFor*)),
            this, SLOT(removeWaitFor(WaitFor*)));
}

QString WaitForComposedTreeItem::text() const {
    if (mCompositionType == WaitForComposed::And) {
        return i18nc("@item", "When all the contained conditions match");
    }

    return i18nc("@item", "When any of the contained conditions match");
}

//private:

WaitForTreeItem* WaitForComposedTreeItem::treeItemForWaitFor(WaitFor* waitFor) {
    foreach (WaitForTreeItem* waitForTreeItem, mWaitForTreeItems) {
        if (waitForTreeItem->waitFor() == waitFor) {
            return waitForTreeItem;
        }
    }

    return 0;
}

//private slots:

void WaitForComposedTreeItem::update(WaitFor* waitFor) {
    mCompositionType =
                    static_cast<WaitForComposed*>(waitFor)->compositionType();

    emit dataChanged(this);
}

void WaitForComposedTreeItem::addWaitFor(WaitFor* waitFor) {
    WaitForTreeItem* item = WaitForTreeItem::treeItemForWaitFor(waitFor, this);
    appendChild(item);
    mWaitForTreeItems.append(item);
}

void WaitForComposedTreeItem::removeWaitFor(WaitFor* waitFor) {
    WaitForTreeItem* item = treeItemForWaitFor(waitFor);

    removeChild(item);
    mWaitForTreeItems.removeOne(item);
    delete item;
}
