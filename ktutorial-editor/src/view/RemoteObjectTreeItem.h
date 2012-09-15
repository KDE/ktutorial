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

#ifndef REMOTEOBJECTTREEITEM_H
#define REMOTEOBJECTTREEITEM_H

#include "TreeItem.h"

class RemoteObject;
class RemoteObjectTreeItemUpdater;

/**
 * A TreeItem that represents a RemoteObject.
 * The tree representation of a RemoteObject is:
 * The object name (The class name)
 *   |-The child 1 name (The child 1 class name)
 *   |   ...
 *   |-The child 2 name (The child 2 class name)
 *   |   ...
 *   ...
 *
 * If the RemoteObject returns an empty object name or an empty class name
 * (for example, because the id is wrong), a placeholder is used instead. Name
 * placeholder is "Object without name!", and the class name placeholder is
 * "No class name!".
 *
 * If a D-Bus error happens, the placeholder for each element is "D-Bus Error!".
 * No children are shown if a D-Bus error happened.
 *
 * RemoteObjects does not provide information about changes in their children. A
 * helper class, RemoteObjectTreeItemUpdater, is used for this.
 * RemoteObjectTreeItem can work without an updater, but changes in the children
 * will not be shown if updateChildren() is not called explicitly.
 *
 * @see RemoteObjectTreeItemUpdater
 */
class RemoteObjectTreeItem: public TreeItem {
Q_OBJECT
public:

    /**
     * Creates a new RemoteObjectTreeItem for the given RemoteObject and with
     * the given parent.
     *
     * @param remoteObject The RemoteObject to represent.
     * @param parent The parent TreeItem.
     */
    explicit RemoteObjectTreeItem(RemoteObject* remoteObject,
                                  TreeItem* parent = 0);

    /**
     * Returns "The class name (The class name)", or a placeholder if the names
     * are empty or can not be got.
     *
     * @return The text for this TreeItem.
     */
    virtual QString text() const;

    /**
     * Returns the RemoteObject.
     *
     * @return The RemoteObject.
     */
    RemoteObject* remoteObject() const;

    /**
     * Sets the updater to register the children tree items in.
     *
     * @param updater The updater to register the children tree items in.
     */
    void setUpdater(RemoteObjectTreeItemUpdater* updater);

    /**
     * Updates the children tree items based on the current children of the
     * remote object, adding or removing them as necessary.
     */
    void updateChildren();

private:

    /**
     * The RemoteObject.
     */
    RemoteObject* mRemoteObject;

    /**
     * The updater for this RemoteObjectTreeItem and all its children.
     */
    RemoteObjectTreeItemUpdater* mUpdater;

    /**
     * The name of the RemoteObject.
     */
    QString mName;

    /**
     * The class name of the RemoteObject.
     */
    QString mClassName;

    /**
     * The RemoteObjectTreeItems for each child RemoteObject in the
     * RemoteObject.
     */
    QList<RemoteObjectTreeItem*> mChildRemoteObjectTreeItems;

    /**
     * Returns the RemoteObjectTreeItem for the given child RemoteObject.
     *
     * @param child The child RemoteObject to get its RemoteObjectTreeItem.
     * @return The RemoteObjectTreeItem.
     */
    RemoteObjectTreeItem* remoteObjectTreeItemForRemoteObject(
                                                    RemoteObject* child) const;

    /**
     * Adds a new RemoteObjectTreeItem when a child RemoteObject is added in the
     * RemoteObject.
     *
     * @param child The child RemoteObject added in the RemoteObject.
     */
    void addChildRemoteObject(RemoteObject* child);

    /**
     * Removes the RemoteObjectTreeItem for the child RemoteObject removed in
     * the RemoteObject.
     *
     * @param child The child RemoteObject removed in the RemoteObject.
     */
    void removeChildRemoteObject(RemoteObject* child);

};

#endif
