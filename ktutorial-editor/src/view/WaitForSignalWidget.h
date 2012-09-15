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

#ifndef WAITFORSIGNALWIDGET_H
#define WAITFORSIGNALWIDGET_H

#include "EditionWidget.h"

#ifdef QT_QTDBUS_FOUND
#include "../targetapplication/DBusException.h"

class RemoteClass;
class RemoteObject;
class RemoteObjectNameWidget;
#endif

class WaitForSignal;

namespace Ui {
class WaitForSignalWidget;
}

/**
 * Edition widget for the condition to wait for a signal.
 */
class WaitForSignalWidget: public EditionWidget {
Q_OBJECT
public:

    /**
     * Creates a new WaitForSignalWidget for the given WaitForSignal.
     *
     * @param waitForSignal The WaitForSignal to set its data.
     * @param parent The parent QWidget.
     */
    explicit WaitForSignalWidget(WaitForSignal* waitForSignal,
                                 QWidget* parent = 0);

    /**
     * Destroys this widget.
     */
    virtual ~WaitForSignalWidget();

    /**
     * Saves the emitter name and the signal name in the WaitForSignal.
     */
    virtual void saveChanges();

private:

    /**
     * The WaitForSignal to edit.
     */
    WaitForSignal* mWaitForSignal;

    /**
     * The Ui Designer generated class.
     */
    Ui::WaitForSignalWidget* ui;

#ifdef QT_QTDBUS_FOUND
    /**
     * The widget to get the name of a remote object.
     */
    RemoteObjectNameWidget* mRemoteObjectNameWidget;

    /**
     * Sets the completion of the signal line edit to the signals emitted by the
     * given remote class and its super classes.
     *
     * @param remoteClass The class to get its signal list.
     */
    void setSignalCompletion(RemoteClass* remoteClass) throw (DBusException);
#endif

private Q_SLOTS:

#ifdef QT_QTDBUS_FOUND
    /**
     * Sets the signal completion based on the chosen remote object.
     *
     * @param remoteObject The chosen RemoteObject.
     */
    void setChosenRemoteObject(RemoteObject* remoteObject);
#endif

};

#endif
