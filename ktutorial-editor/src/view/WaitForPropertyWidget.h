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

#ifndef WAITFORPROPERTYWIDGET_H
#define WAITFORPROPERTYWIDGET_H

#include "EditionWidget.h"

#ifdef QT_QTDBUS_FOUND
#include "../targetapplication/DBusException.h"

class RemoteClass;
class RemoteObject;
class RemoteObjectNameWidget;
#endif

class WaitForProperty;

namespace Ui {
class WaitForPropertyWidget;
}

/**
 * Edition widget for the condition to wait for a property.
 */
class WaitForPropertyWidget: public EditionWidget {
Q_OBJECT
public:

    /**
     * Creates a new WaitForPropertyWidget for the given WaitForProperty.
     *
     * @param waitForProperty The WaitForProperty to set its data.
     * @param parent The parent QWidget.
     */
    explicit WaitForPropertyWidget(WaitForProperty* waitForProperty,
                                   QWidget* parent = 0);

    /**
     * Destroys this widget.
     */
    virtual ~WaitForPropertyWidget();

    /**
     * Saves the object name, the property name and the value in the
     * WaitForProperty.
     */
    virtual void saveChanges();

private:

    /**
     * The WaitForProperty to edit.
     */
    WaitForProperty* mWaitForProperty;

    /**
     * The Ui Designer generated class.
     */
    Ui::WaitForPropertyWidget* ui;

#ifdef QT_QTDBUS_FOUND
    /**
     * The widget to get the name of a remote object.
     */
    RemoteObjectNameWidget* mRemoteObjectNameWidget;

    /**
     * Sets the completion of the property line edit to the properties contained
     * in the given remote class and its super classes.
     *
     * @param remoteClass The class to get its property list.
     */
    void setPropertyCompletion(RemoteClass* remoteClass) throw (DBusException);
#endif

private slots:

#ifdef QT_QTDBUS_FOUND
    /**
     * Sets the completion for the property names based in the chosen remote
     * object.
     *
     * @param remoteObject The chosen RemoteObject.
     */
    void setChosenRemoteObject(RemoteObject* remoteObject);
#endif

};

#endif
