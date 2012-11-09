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

#include "RemoteObjectTreeItemUpdater.h"
#include "RemoteObjectTreeItem.h"
#include "../targetapplication/RemoteEventSpy.h"

//public:

RemoteObjectTreeItemUpdater::RemoteObjectTreeItemUpdater(QObject* parent):
        QObject(parent) {
}

void RemoteObjectTreeItemUpdater::setRemoteEventSpy(
                                            RemoteEventSpy* remoteEventSpy) {
    connect(remoteEventSpy, SIGNAL(eventReceived(RemoteObject*,QString)),
            this, SLOT(handleEventReceived(RemoteObject*,QString)));
}

void RemoteObjectTreeItemUpdater::registerRemoteObjectTreeItem(
                                                RemoteObjectTreeItem* item) {
    mRemoteObjectTreeItems.insert(item->remoteObject(), item);
}

//private slots:

void RemoteObjectTreeItemUpdater::handleEventReceived(
                                                    RemoteObject* remoteObject,
                                                    const QString& eventType) {
    if (!mRemoteObjectTreeItems.contains(remoteObject)) {
        return;
    }

    if (!mRemoteObjectTreeItems.value(remoteObject)) {
        //The tree item was deleted, so an entry is no longer needed for it
        mRemoteObjectTreeItems.remove(remoteObject);
        return;
    }

    if (eventType != "ChildAdded" && eventType != "ChildRemoved") {
        return;
    }

    mRemoteObjectTreeItems.value(remoteObject)->updateChildren();
}
