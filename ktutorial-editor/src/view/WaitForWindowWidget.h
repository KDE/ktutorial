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

#ifndef WAITFORWINDOWWIDGET_H
#define WAITFORWINDOWWIDGET_H

#include "EditionWidget.h"

#ifdef QT_QTDBUS_FOUND
class RemoteObjectNameWidget;
#endif

class WaitForWindow;

namespace Ui {
class WaitForWindowWidget;
}

/**
 * Edition widget for the condition to wait for a window to be shown.
 */
class WaitForWindowWidget: public EditionWidget {
Q_OBJECT
public:

    /**
     * Creates a new WaitForWindowWidget for the given WaitForWindow.
     *
     * @param waitForWindow The WaitForWindow to set its data.
     * @param parent The parent QWidget.
     */
    explicit WaitForWindowWidget(WaitForWindow* waitForWindow,
                                 QWidget* parent = 0);

    /**
     * Destroys this widget.
     */
    virtual ~WaitForWindowWidget();

    /**
     * Saves the window object name in the WaitForWindow.
     */
    virtual void saveChanges();

private:

    /**
     * The WaitForWindow to edit.
     */
    WaitForWindow* mWaitForWindow;

    /**
     * The Ui Designer generated class.
     */
    Ui::WaitForWindowWidget* ui;

#ifdef QT_QTDBUS_FOUND
    /**
     * The widget to get the name of a remote object.
     */
    RemoteObjectNameWidget* mRemoteObjectNameWidget;
#endif

};

#endif
