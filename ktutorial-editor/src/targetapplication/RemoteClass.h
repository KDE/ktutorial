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

#ifndef REMOTECLASS_H
#define REMOTECLASS_H

#include <QDBusAbstractInterface>

#include "DBusException.h"

class RemoteObjectMapper;

/**
 * Proxy for remote classes exposed by KTutorial editor support module in
 * KTutorial library.
 * RemoteClass represents a remote class exposed by ObjectRegister in KTutorial
 * library through DBus. Its purpose is provide an API to use the remote
 * class/QMetaObject like any other local object, hiding the DBus complexity
 * behind it.
 *
 * To get the data, RemoteClass makes DBus calls to the
 * "org.kde.ktutorial.ClassRegistry" interface in the DBus service specified in
 * the constructor.
 *
 * Although the idea is let other objects use it like a local object, it has to
 * communicate with the remote ObjectRegistry through DBus anyway, so the
 * methods may throw a DBusException if something goes wrong.
 */
class RemoteClass: public QDBusAbstractInterface {
Q_OBJECT
public:

    /**
     * Creates a new RemoteClass to represent the remote class with the given
     * class name in the given DBus service name.
     *
     * @param service The DBus service name.
     * @param mapper The RemoteObjectMapper to get RemoteClasses from.
     * @param className The name of the remote class.
     */
    RemoteClass(const QString& service, RemoteObjectMapper* mapper,
                const QString& className);

    /**
     * Returns the class name.
     *
     * @return The class name.
     */
    QString className() const;

    /**
     * Returns the remote super class.
     * If it has no super class, a null pointer is returned.
     *
     * @return The remote super class.
     * @throws DBusException If a DBus error happens.
     */
    RemoteClass* superClass() throw (DBusException);

    /**
     * Returns a list with the properties defined in the remote class.
     * The list only includes the properties from the class itself, but not its
     * super classes.
     *
     * @return The properties.
     * @throws DBusException If a DBus error happens.
     */
    QStringList propertyList() throw (DBusException);

    /**
     * Returns a list with the signals defined in the remote class.
     * The list only includes the signals from the class itself, but not its
     * super classes.
     *
     * @return The signals.
     * @throws DBusException If a DBus error happens.
     */
    QStringList signalList() throw (DBusException);

private:

    /**
     * The mapper that associates a RemoteClass with its name.
     */
    RemoteObjectMapper* mMapper;

    /**
     * The name of the remote class.
     */
    QString mClassName;

};

#endif
