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

#include "WaitForEventTreeItem.h"

#include <KLocalizedString>

#include "../data/WaitForEvent.h"

//public:

WaitForEventTreeItem::WaitForEventTreeItem(WaitForEvent* waitForEvent,
                                           TreeItem* parent):
        WaitForTreeItem(waitForEvent, parent) {
    mReceiverName = waitForEvent->receiverName();
    mEventName = waitForEvent->eventName();

    connect(waitForEvent, SIGNAL(dataChanged(WaitFor*)),
            this, SLOT(update(WaitFor*)));
}

QString WaitForEventTreeItem::text() const {
    QString receiverName;
    if (mReceiverName.isEmpty()) {
        receiverName = i18nc("@item:intext", "(object not set)");
    } else {
        receiverName = "\"" + mReceiverName + "\"";
    }

    QString eventName;
    if (mEventName.isEmpty()) {
        eventName = i18nc("@item:intext", "(event not set)");
    } else {
        eventName = "\"" + mEventName + "\"";
    }

    return i18nc("@item", "When the event %1 is received by object %2",
                 eventName, receiverName);
}

//private:

void WaitForEventTreeItem::update(WaitFor* waitFor) {
    WaitForEvent* waitForEvent = static_cast<WaitForEvent*>(waitFor);
    mReceiverName = waitForEvent->receiverName();
    mEventName = waitForEvent->eventName();

    emit dataChanged(this);
}
