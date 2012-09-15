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

#ifndef KTUTORIAL_EDITORSUPPORT_CLASSREGISTERADAPTOR_H
#define KTUTORIAL_EDITORSUPPORT_CLASSREGISTERADAPTOR_H

#include <QDBusAbstractAdaptor>
#include <QStringList>

namespace ktutorial {
namespace editorsupport {
class ObjectRegister;
}
}

namespace ktutorial {
namespace editorsupport {

/**
 * Adaptor to expose an ObjectRegister through DBus.
 * It provides methods to get information about the registered QMetaObjects.
 *
 * @see EditorSupport
 */
class ClassRegisterAdaptor: public QDBusAbstractAdaptor {
Q_OBJECT
Q_CLASSINFO("D-Bus Interface", "org.kde.ktutorial.ClassRegister")
public:

    /**
     * Creates a new ClassRegisterAdaptor for the given ObjectRegister.
     *
     * @param objectRegister The ObjectRegister to adapt.
     */
    explicit ClassRegisterAdaptor(ObjectRegister* objectRegister);

public Q_SLOTS:

    /**
     * Returns super class name of the class with the given name.
     * If the class name is not registered, or it has no super class, an empty
     * string is returned.
     *
     * @param className The name of the class.
     * @return The name of the super class.
     */
    QString superClass(const QString& className) const;

    /**
     * Returns the properties of the class with the given name.
     * If the class is not registered, an empty list is returned.
     *
     * Only the properties defined in the given class are included in the list.
     * Properties from parent classes must be got using the parent class name.
     *
     * @param className The name of the class.
     * @return The list of properties.
     */
    QStringList propertyList(const QString& className) const;

    /**
     * Returns the signals of the class with the given name.
     * If the class is not registered, an empty list is returned.
     *
     * Only the signals defined in the given class are included in the list.
     * Signals from parent classes must be got using the parent class name.
     *
     * @param className The name of the class.
     * @return The list of signals.
     */
    QStringList signalList(const QString& className) const;

private:

    /**
     * The ObjectRegister to adapt.
     */
    ObjectRegister* mObjectRegister;

    /**
     * Checks whether the given meta property is a property defined in the given
     * meta object.
     * If the property is inherited in the given meta object instead of defined
     * in it, false is also returned.
     *
     * @param metaProperty The meta property to check.
     * @param metaObject The meta object to check the meta property with.
     * @return True if the meta property is a property defined in the meta
     *         object, false otherwise.
     */
    bool isPropertyDefinedInClass(const QMetaProperty& metaProperty,
                                  const QMetaObject* metaObject) const;

    /**
     * Checks whether the given meta method is a signal defined in the given
     * meta object.
     * If the signal is inherited in the given meta object instead of defined in
     * it, false is also returned.
     *
     * @param metaMethod The meta method to check.
     * @param metaObject The meta object to check the meta method with.
     * @return True if the meta method is a signal defined in the meta object,
     *         false otherwise.
     */
    bool isSignalDefinedInClass(const QMetaMethod& metaMethod,
                                const QMetaObject* metaObject) const;

};

}
}

#endif
