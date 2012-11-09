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

#ifndef REMOTEOBJECTCHOOSERFILTERMODEL_H
#define REMOTEOBJECTCHOOSERFILTERMODEL_H

#include <QSortFilterProxyModel>

#include "../targetapplication/DBusException.h"

class RemoteClass;
class RemoteObject;

/**
 * Proxy model to filter RemoteObjecTreeItem tree models.
 * When applied on a TreeModel composed by RemoteObjectTreeItems it hides those
 * items that do not pass the enabled filters. Note that the TreeModel must
 * contain only RemoteObjectTreeItems; no other TreeItem types can appear in the
 * source model.
 *
 * There are two available filters: named objects and widgets. Both filters
 * check the RemoteObjects referenced by the RemoteObjectTreeItems. The first
 * checks if it has a name, and the second checks if it is a widget.
 *
 * The filters can be enabled and disabled using
 * setNamedObjectFilterEnabled(bool) and setWidgetFilterEnabled(bool). When both
 * filters are disabled every item in the model is shown.
 *
 * When there are filters enabled, an item may be shown even if it does not pass
 * them if any of its descendants does.
 */
class RemoteObjectChooserFilterModel: public QSortFilterProxyModel {
Q_OBJECT
public:

    /**
     * Creates a new RemoteObjectChooserFilterModel.
     *
     * @param parent The parent object.
     */
    explicit RemoteObjectChooserFilterModel(QObject* parent = 0);

public Q_SLOTS:

    /**
     * Enables or disables the named object filter.
     *
     * @param namedObjectFilterEnabled If the filter has to be enabled or
     *        disabled.
     */
    void setNamedObjectFilterEnabled(bool namedObjectFilterEnabled);

    /**
     * Enables or disables the widget filter.
     *
     * @param widgetFilterEnabled If the filter has to be enabled or disabled.
     */
    void setWidgetFilterEnabled(bool widgetFilterEnabled);

protected:

    /**
     * Returns true if the item indicated by the given sourceRow and
     * sourceParent should be included in the model, false otherwise.
     * An item is included if the item, or any of its children, passes the
     * enabled filters. If no filter is enabled, every item is included in the
     * model.
     * Also, if there was a problem querying the remote objects the item is also
     * included in the model.
     *
     * @param sourceRow The row of the index in the source model.
     * @param sourceParent The parent of the index in the source model.
     * @return True if the item passes the filter, false otherwise.
     */
    virtual bool filterAcceptsRow(int sourceRow,
                                  const QModelIndex &sourceParent) const;

private:

    /**
     * Whether the named object filter is enabled or not.
     */
    bool mNamedObjectFilterEnabled;

    /**
     * Whether the widget filter is enabled or not.
     */
    bool mWidgetFilterEnabled;

    /**
     * Checks if the given remote object passes the name object filter.
     * The remote object passes the filter if the filter is not enabled, or if
     * the filter is enabled and the remote object has a name.
     *
     * @param remoteObject The remote object to check.
     * @return True if the remote object passes the named object filter, false
     *         otherwise.
     * @throws DBusException If a DBus error happens.
     */
    bool filterNamedObject(RemoteObject* remoteObject) const
    throw (DBusException);

    /**
     * Checks if the given remote object passes the widget filter.
     * The remote object passes the filter if the filter is not enabled, or if
     * the filter is enabled and the remote object is a widget.
     *
     * @param remoteObject The remote object to check.
     * @return True if the remote object passes the widget filter, false
     *         otherwise.
     * @throws DBusException If a DBus error happens.
     */
    bool filterWidget(RemoteObject* remoteObject) const throw (DBusException);

    /**
     * Returns true if the remote class is a widget, false otherwise.
     *
     * @param remoteClass The remote class to check.
     * @return True if the remote class is a widget, false otherwise.
     * @throws DBusException If a DBus error happens.
     */
    bool isWidget(RemoteClass* remoteClass) const throw (DBusException);

};

#endif
