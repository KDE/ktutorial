/***************************************************************************
 *   Copyright (C) 2009 by Daniel Calviño Sánchez <danxuliu@gmail.com>     *
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

#ifndef KTUTORIAL_SCRIPTING_SCRIPTINGMODULE_H
#define KTUTORIAL_SCRIPTING_SCRIPTINGMODULE_H

#include <QHash>
#include <QObject>

class Option;
class WaitFor;

namespace ktutorial {
namespace scripting {
class ScriptedStep;
}
}

namespace ktutorial {
namespace scripting {

/**
 * Main KTutorial interface for scripts.
 * It acts mainly as a factory for several KTutorial objects, as they can't be
 * created on the scripts themselves. It also provides access to some KTutorial
 * class methods needed to build tutorials, like findObject(const QString&).
 * Note, however, that ScriptingModule isn't actually a Kross module, but an
 * object exposed in Kross Actions.
 *
 * The objects created through ScriptingModule can be from fixed classes, using
 * newOption(const QString&) and newStep(const QString&), or from dynamically
 * specified classes using a type name in newWaitFor(const QString&).
 *
 * In any case, the objects created must be deleted. So if they are not deleted
 * by their parent object (for example, a Step added to a Tutorial will be
 * automatically deleted when the Tutorial is deleted), they must be deleted
 * using QObject::deleteLater() method in the scripts.
 *
 * New objects of the default KTutorial WaitFor classes can be created with
 * newWaitFor(const QString&) using their name (without any namespace
 * qualifier). Moreover, custom WaitFor classes can be registered with
 * registerWaitForMetaObject(const QMetaObject&, const QString&) and new
 * instances of those classes can be created since that moment, provided they
 * have an invokable public default constructor.
 *
 * ScriptingModule is a singleton, so all the scripts can use each registered
 * WaitFor type.
 */
class ScriptingModule: public QObject {
Q_OBJECT
public:

    /**
     * Returns the only instance of this class.
     *
     * @return The only instance of this class.
     */
    static ScriptingModule* self();

    /**
     * Destroys this ScriptingModule.
     */
    virtual ~ScriptingModule();

    /**
     * Registers a WaitFor type to create instances of it using
     * newWaitFor(const QString&).
     * Only WaitFor subclasses that contain an invokable public default
     * constructor can be registered.
     *
     * When new instances are created with newWaitFor(const QString&), the types
     * are identified by the name used to register them, not their true C++
     * name. The same type can be registered with several different names if
     * needed (although once a name is used, no other types can be registered
     * with that name). However, if no name is specified, the full name of the
     * class is used (including namespaces and :: separators).
     *
     * @param waitForMetaObject The QMetaObject to create instances of.
     * @param typeName A specific name to be used for the QMetaObject.
     * @return True if the type was successfully registered, false otherwise.
     */
    bool registerWaitForMetaObject(const QMetaObject& waitForMetaObject,
                                   const QString& typeName = QString());

    /**
     * Returns the object with the specified name, if any.
     * This method can be invoked from a script.
     *
     * @param name The name of the object to find.
     * @return The object with the specified name, or null if there is none.
     * @see KTutorial::findObject(const QString&)
     */
    Q_INVOKABLE QObject* findObject(const QString& name);

    /**
     * Returns a new Option with the given name.
     * This method can be invoked from a script.
     *
     * @param name The name of the Option.
     * @return The new Option.
     * @todo Change return type to Option* once all major Kross scripting
     *       backends support classes wrapped through WrapperInterface
     */
    Q_INVOKABLE QObject* newOption(const QString& name);

    /**
     * Returns a new ScriptedStep with the given id.
     * This method can be invoked from a script.
     *
     * @param id The id of the ScriptedStep.
     * @return The new ScriptedStep.
     * @todo Change return type to ScriptedStep* once all major Kross scripting
     *       backends support classes wrapped through WrapperInterface
     */
    Q_INVOKABLE QObject* newStep(const QString& id);

    /**
     * Returns a new WaitFor of the given type name.
     * The type can be any of the default WaitFor included in KTutorial
     * (WaitForSignal, WaitForAnd...) or the name of a custom WaitFor type
     * registered with
     * registerWaitForMetaObject(const QMetaObject&, const QString&).
     * This method can be invoked from a script.
     *
     * @param typeName The type name of the WaitFor.
     * @return The new WaitFor, or a null pointer if the type name isn't
     *         registered.
     * @todo Change return type to WaitFor* once all major Kross scripting
     *       backends support classes wrapped through WrapperInterface
     */
    Q_INVOKABLE QObject* newWaitFor(const QString& typeName);

private:

    /**
     * The hash to store the registered WaitFor QMetaObjects.
     */
    QHash<QString, QMetaObject> mWaitForMetaObjects;

    /**
     * The instance of this class.
     */
    static ScriptingModule* sSelf;

    /**
     * Creates a new ScriptingModule.
     * Private to avoid classes other than itself to create them.
     */
    ScriptingModule();

    /**
     * Check if the given QMetaObject is from a WaitFor subclass.
     *
     * @param metaObject The QMetaObject to check.
     * @return True if it is from a WaitFor subclass, false otherwise.
     */
    bool inheritsWaitFor(const QMetaObject& metaObject) const;

    /**
     * Check if the class represented by the given QMetaObject has an invokable
     * public default constructor.
     *
     * @param metaObject The QMetaObject to check.
     * @return True if it has an invokable public default constructor, false
     *         otherwise.
     */
    bool hasInvokableDefaultConstructor(const QMetaObject& metaObject) const;

};

}
}

#endif
