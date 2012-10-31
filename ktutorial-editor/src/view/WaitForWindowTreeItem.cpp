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

#include "WaitForWindowTreeItem.h"

#include <KLocalizedString>

#include "../data/WaitForWindow.h"

//public:

WaitForWindowTreeItem::WaitForWindowTreeItem(WaitForWindow* waitForWindow,
                                             TreeItem* parent):
        WaitForTreeItem(waitForWindow, parent) {
    mWindowObjectName = waitForWindow->windowObjectName();

    connect(waitForWindow, SIGNAL(dataChanged(WaitFor*)),
            this, SLOT(update(WaitFor*)));
}

QString WaitForWindowTreeItem::text() const {
    QString windowObjectName;
    if (mWindowObjectName.isEmpty()) {
        windowObjectName = i18nc("@item:intext", "(object name not set)");
    } else {
        windowObjectName = "\"" + mWindowObjectName + "\"";
    }

    return i18nc("@item", "When the window %1 is shown", windowObjectName);
}

//private:

void WaitForWindowTreeItem::update(WaitFor* waitFor) {
    WaitForWindow* waitForWindow = static_cast<WaitForWindow*>(waitFor);
    mWindowObjectName = waitForWindow->windowObjectName();

    emit dataChanged(this);
}
