/***************************************************************************
 *   Copyright (C) 2011 by Daniel Calviño Sánchez <danxuliu@gmail.com>     *
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

#include "RemoteObjectChooserFilterModel.h"

#include <QQueue>

#include "RemoteObjectTreeItem.h"
#include "../targetapplication/RemoteClass.h"
#include "../targetapplication/RemoteObject.h"

//public:

RemoteObjectChooserFilterModel::RemoteObjectChooserFilterModel(
                                                    QObject* parent /* = 0*/):
        QSortFilterProxyModel(parent),
    mNamedObjectFilterEnabled(false),
    mWidgetFilterEnabled(false) {
}

//public slots:

void RemoteObjectChooserFilterModel::setNamedObjectFilterEnabled(
                                                bool namedObjectFilterEnabled) {
    mNamedObjectFilterEnabled = namedObjectFilterEnabled;
    invalidateFilter();
}

void RemoteObjectChooserFilterModel::setWidgetFilterEnabled(
                                                    bool widgetFilterEnabled) {
    mWidgetFilterEnabled = widgetFilterEnabled;
    invalidateFilter();
}

//protected:

bool RemoteObjectChooserFilterModel::filterAcceptsRow(int sourceRow,
                                        const QModelIndex &sourceParent) const {
    QModelIndex index = sourceModel()->index(sourceRow, 0, sourceParent);
    RemoteObjectTreeItem* item = static_cast<RemoteObjectTreeItem*>(
                                                index.internalPointer());
    RemoteObject* remoteObject = item->remoteObject();
    Q_ASSERT(remoteObject);

    //Breadth-first traversal. No "scientific" reason to use it instead of
    //depth-first, just the "feeling" that it will find widgets and named
    //objects sooner than a depth-first approach.
    QQueue<RemoteObject*> objects;
    objects.append(remoteObject);

    while (!objects.isEmpty()) {
        remoteObject = objects.dequeue();

        try {
            if (filterNamedObject(remoteObject) && filterWidget(remoteObject)) {
                return true;
            }

            objects.append(remoteObject->children());
        } catch (DBusException) {
            return true;
        }
    }

    return false;
}

//private:

bool RemoteObjectChooserFilterModel::filterNamedObject(
                                            RemoteObject* remoteObject) const
throw (DBusException) {
    if (!mNamedObjectFilterEnabled || !remoteObject->name().isEmpty()) {
        return true;
    }

    return false;
}

bool RemoteObjectChooserFilterModel::filterWidget(
                                            RemoteObject* remoteObject) const
throw (DBusException) {
    if (!mWidgetFilterEnabled || isWidget(remoteObject->remoteClass())) {
        return true;
    }

    return false;
}

bool RemoteObjectChooserFilterModel::isWidget(RemoteClass* remoteClass) const
throw (DBusException) {
    if (!remoteClass) {
        return false;
    }

    if (remoteClass->className() == "QWidget") {
        return true;
    }

    if (isWidget(remoteClass->superClass())) {
        return true;
    }

    return false;
}
