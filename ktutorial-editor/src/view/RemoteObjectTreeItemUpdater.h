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

#ifndef REMOTEOBJECTTREEITEMUPDATER_H
#define REMOTEOBJECTTREEITEMUPDATER_H

#include <QHash>
#include <QObject>
#include <QPointer>

class RemoteEventSpy;
class RemoteObject;
class RemoteObjectTreeItem;

/**
 * Helper class for RemoteObjectTreeItem to notify when it should update its
 * children tree items.
 * RemoteObjectTreeItems can not update their list of children tree items by
 * themselves, as RemoteObject does not provide that information. The
 * RemoteEventSpy has to be used to watch events in all the RemoteObjects and
 * update the tree items when a ChildAdded or a ChildRemoved event is received.
 *
 * RemoteObjectTreeItems should not be registered directly with the updater. The
 * updater must be set in the root RemoteObjectTreeItem to update. The tree item
 * will take care of registering its children with the updater when necessary.
 * Thus, the whole tree is updated, instead of only the registered item.
 *
 * @see RemoteObjectTreeItem
 */
class RemoteObjectTreeItemUpdater: public QObject {
Q_OBJECT
public:

    /**
     * Creates a new RemoteObjectTreeItemUpdater.
     *
     * @param parent The parent object.
     */
    explicit RemoteObjectTreeItemUpdater(QObject* parent = 0);

    /**
     * Sets the RemoteEventSpy to check its events.
     *
     * @param remoteEventSpy The RemoteEventSpy.
     */
    void setRemoteEventSpy(RemoteEventSpy* remoteEventSpy);

    /**
     * Registers a new RemoteObjectTreeItem to be updated.
     *
     * @param remoteObjectTreeItem The RemoteObjectTreeItem to register.
     */
    void registerRemoteObjectTreeItem(
                                    RemoteObjectTreeItem* remoteObjectTreeItem);

private:

    /**
     * The RemoteObjectTreeItems to update, identified by their RemoteObject.
     */
    QHash<RemoteObject*, QPointer<RemoteObjectTreeItem> >
                                                        mRemoteObjectTreeItems;

private Q_SLOTS:

    /**
     * If the event is a ChildAdded or a ChildRemoved, the RemoteObjectTreeItem
     * that represents the remoteObject is told to update its children.
     *
     * @param remoteObject The RemoteObject to update.
     * @param eventType The type of the event received.
     */
    void handleEventReceived(RemoteObject* remoteObject,
                             const QString& eventType);

};

#endif
