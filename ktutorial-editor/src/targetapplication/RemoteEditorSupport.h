/***************************************************************************
 *   Copyright (C) 2010 by Daniel Calviño Sánchez <danxuliu@gmail.com>     *
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

#ifndef REMOTEEDITORSUPPORT_H
#define REMOTEEDITORSUPPORT_H

#include <QDBusAbstractInterface>

#include "DBusException.h"

class RemoteEventSpy;
class RemoteObject;
class RemoteObjectMapper;

/**
 * Proxy for the remote EditorSupport exposed by KTutorial editor support module
 * in KTutorial library.
 * RemoteEditorSupport represents a remote EditorSupport exposed through DBus.
 * Its purpose is provide an API to use the remote EditorSupport like any other
 * local object, hiding the DBus complexity behind it.
 *
 * RemoteEditorSupport makes DBus calls to "org.kde.ktutorial.EditorSupport"
 * interface in the DBus service specified in the constructor.
 *
 * Although the idea is let other objects use it like a local object, it has to
 * communicate with the remote EditorSupport through DBus anyway, so the methods
 * may throw a DBusException if something goes wrong.
 */
class RemoteEditorSupport: public QDBusAbstractInterface {
Q_OBJECT
public:

    /**
     * Creates a new RemoteEditorSupport to represent a remote EditorSupport in
     * the given DBus service.
     *
     * @param service The DBus service name.
     * @param mapper The RemoteObjectMapper to get RemoteObjects from.
     */
    RemoteEditorSupport(const QString& service, RemoteObjectMapper* mapper);

    /**
     * Destroys this RemoteEditorSupport.
     * The RemoteEventSpy is also destroyed, if any.
     */
    virtual ~RemoteEditorSupport();

    /**
     * Returns the RemoteObject that represents the main window of the
     * application.
     *
     * @return The RemoteObject for the main window.
     * @throws DBusException If a DBus error happens.
     */
    RemoteObject* mainWindow() throw (DBusException);

    /**
     * Returns the RemoteObject with the given name.
     * The name can include ancestor names.
     * 
     * @param name The name of the RemoteObject to find.
     * @return The RemoteObject with the given name, or 0 if there is none.
     */
    RemoteObject* findObject(const QString& name) throw (DBusException);

    /**
     * Highlights the widget represented by the given remote object.
     * If the object does not represent a widget nothing is highlighted.
     *
     * @param remoteWidget The RemoteObject for the widget to highlight.
     * @throws DBusException If a DBus error happens.
     */
    void highlight(RemoteObject* remoteWidget) throw (DBusException);

    /**
     * Stops highlighting the widget represented by the given remote object.
     * If the object does not represent a widget no highlighting is stopped.
     *
     * @param remoteWidget The RemoteObject for the widget to stop highlighting.
     * @throws DBusException If a DBus error happens.
     */
    void stopHighlighting(RemoteObject* remoteWidget) throw (DBusException);

    /**
     * Enables the EventSpy in the remote EditorSupport and returns a proxy for
     * it.
     * If the EventSpy was already enabled, the already created proxy is
     * returned again.
     * The RemoteEventSpy is destroyed when the EventSpy is disabled or this
     * RemoteEditorSupport destroyed, so consider using QPointer to store it.
     *
     * @return A proxy for the remote EventSpy.
     * @throws DBusException If a DBus error happens.
     */
    RemoteEventSpy* enableEventSpy() throw (DBusException);

    /**
     * Disables the EventSpy in the remote EditorSupport and destroys the proxy
     * for it.
     * The EventSpy is not disabled until the same number of calls to the
     * enableEventSpy() method are made to this method. It makes possible to
     * share the same RemoteEventSpy between objects that know nothing one of
     * each other: each object enable and disable the RemoteEventSpy as they
     * need, but it is not actually disabled until it is disabled by the last
     * object using it.
     *
     * @throws DBusException If a DBus error happens.
     */
    void disableEventSpy() throw (DBusException);

    /**
     * Tests the scripted tutorial stored in the file with the given name.
     * The target application just starts the tutorial and shows it to the user.
     * If a step id is given, the tutorial is changed to that step after
     * starting.
     *
     * @param filename The name of the file that contains the tutorial to test.
     * @param stepId The id of the step to change to after starting.
     */
    void testScriptedTutorial(const QString& filename,
                              const QString& stepId = QString())
    throw (DBusException);

private:

    /**
     * The mapper that associates a RemoteObject with its object id.
     */
    RemoteObjectMapper* mMapper;

    /**
     * The RemoteEventSpy, if enabled.
     */
    RemoteEventSpy* mRemoteEventSpy;

    /**
     * The number of enableEventSpy calls that didn't get a disableEventSpy
     * counterpart yet.
     */
    int mNumberOfPendingEnableEventSpyCalls;

};

#endif
