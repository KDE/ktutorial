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

#ifndef WAITFOREVENTWIDGET_H
#define WAITFOREVENTWIDGET_H

#include "EditionWidget.h"

#ifdef QT_QTDBUS_FOUND
class RemoteObject;
class RemoteObjectNameWidget;
#endif

class WaitForEvent;

namespace Ui {
class WaitForEventWidget;
}

/**
 * Edition widget for the condition to wait for an event.
 */
class WaitForEventWidget: public EditionWidget {
Q_OBJECT
public:

    /**
     * Creates a new WaitForEventWidget for the given WaitForEvent.
     *
     * @param waitForEvent The WaitForEvent to set its data.
     * @param parent The parent QWidget.
     */
    explicit WaitForEventWidget(WaitForEvent* waitForEvent,
                                QWidget* parent = 0);

    /**
     * Destroys this widget.
     */
    virtual ~WaitForEventWidget();

    /**
     * Saves the receiver name and the event name in the WaitForEvent.
     */
    virtual void saveChanges();

private:

    /**
     * The WaitForEvent to edit.
     */
    WaitForEvent* mWaitForEvent;

    /**
     * The Ui Designer generated class.
     */
    Ui::WaitForEventWidget* ui;

#ifdef QT_QTDBUS_FOUND
    /**
     * The widget to get the name of a remote object.
     */
    RemoteObjectNameWidget* mRemoteObjectNameWidget;
#endif

    /**
     * Adds all the event names to the completion object of the event name line
     * edit.
     */
    void addItemsToEventNameCompletion();

};

#endif
