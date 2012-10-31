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

#include "WaitForNotTreeItem.h"

#include <KLocalizedString>

#include "../data/WaitForNot.h"

//public:

WaitForNotTreeItem::WaitForNotTreeItem(WaitForNot* waitForNot,
                                       TreeItem* parent):
        WaitForTreeItem(waitForNot, parent),
    mNegatedWaitForItem(0) {

    update(waitForNot);

    connect(waitForNot, SIGNAL(dataChanged(WaitFor*)),
            this, SLOT(update(WaitFor*)));
}

QString WaitForNotTreeItem::text() const {
    return i18nc("@item", "The contained condition can not have been met");
}

//private slots:

void WaitForNotTreeItem::update(WaitFor* waitFor) {
    if (mNegatedWaitForItem) {
        removeChild(mNegatedWaitForItem);
        delete mNegatedWaitForItem;
        mNegatedWaitForItem = 0;
    }

    WaitForNot* waitForNot = static_cast<WaitForNot*>(waitFor);
    WaitFor* negatedWaitFor = waitForNot->negatedWaitFor();
    if (negatedWaitFor) {
        mNegatedWaitForItem = WaitForTreeItem::treeItemForWaitFor(
                                                        negatedWaitFor, this);

        appendChild(mNegatedWaitForItem);
    }
}
