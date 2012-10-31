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

#include "RemoteObjectTreeItem.h"

#include <KDebug>
#include <KLocalizedString>

#include "RemoteObjectTreeItemUpdater.h"
#include "../targetapplication/RemoteClass.h"
#include "../targetapplication/RemoteObject.h"

//public:

RemoteObjectTreeItem::RemoteObjectTreeItem(RemoteObject* remoteObject,
                                           TreeItem* parent):
        TreeItem(parent),
    mRemoteObject(remoteObject),
    mUpdater(0) {
    Q_ASSERT(remoteObject);

    try {
        mName = remoteObject->name();
        mClassName = remoteObject->remoteClass()->className();
    } catch (DBusException e) {
        mName = i18nc("@item:intext", "D-Bus Error!");
        mClassName = i18nc("@item:intext", "D-Bus Error!");
    }

    if (mName.isEmpty()) {
        mName = i18nc("@item:intext", "Object without name!");
    }

    if (mClassName.isEmpty()) {
        mClassName = i18nc("@item:intext", "No class name!");
    }

    updateChildren();
}

QString RemoteObjectTreeItem::text() const {
    return i18nc("@item Object name (Class name)", "%1 (%2)", mName,
                 mClassName);
}

RemoteObject* RemoteObjectTreeItem::remoteObject() const {
    return mRemoteObject;
}

void RemoteObjectTreeItem::setUpdater(RemoteObjectTreeItemUpdater* updater) {
    Q_ASSERT(updater);

    mUpdater = updater;

    updater->registerRemoteObjectTreeItem(this);

    foreach (RemoteObjectTreeItem* child, mChildRemoteObjectTreeItems) {
        child->setUpdater(updater);
    }
}

void RemoteObjectTreeItem::updateChildren() {
    QList<RemoteObject*> children;
    try {
        children = mRemoteObject->children();
    } catch (DBusException e) {
        kWarning() << "The children for the remote object with id "
                   << mRemoteObject->objectId() << " could not be updated ("
                   << e.message() << ").";
        return;
    }

    int i=0;
    while (i < mChildRemoteObjectTreeItems.count()) {
        if (i >= children.count() ||
                mChildRemoteObjectTreeItems[i]->remoteObject() != children[i]) {
            removeChildRemoteObject(
                                mChildRemoteObjectTreeItems[i]->remoteObject());
        } else {
            i++;
        }
    }

    while (i < children.count()) {
        addChildRemoteObject(children[i]);
        i++;
    }
}

//private:

RemoteObjectTreeItem* RemoteObjectTreeItem::remoteObjectTreeItemForRemoteObject(
                                                    RemoteObject* child) const {
    foreach (RemoteObjectTreeItem* remoteObjectTreeItem,
                                                mChildRemoteObjectTreeItems) {
        if (remoteObjectTreeItem->remoteObject() == child) {
            return remoteObjectTreeItem;
        }
    }

    return 0;
}

//private slots:

void RemoteObjectTreeItem::addChildRemoteObject(RemoteObject* child) {
    RemoteObjectTreeItem* remoteObjectTreeItem =
                                new RemoteObjectTreeItem(child, this);
    appendChild(remoteObjectTreeItem);
    mChildRemoteObjectTreeItems.append(remoteObjectTreeItem);

    if (mUpdater) {
        remoteObjectTreeItem->setUpdater(mUpdater);
    }
}

void RemoteObjectTreeItem::removeChildRemoteObject(RemoteObject* child) {
    RemoteObjectTreeItem* remoteObjectTreeItem =
                                    remoteObjectTreeItemForRemoteObject(child);

    removeChild(remoteObjectTreeItem);
    mChildRemoteObjectTreeItems.removeOne(remoteObjectTreeItem);
    delete remoteObjectTreeItem;
}
