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

#ifndef KTUTORIAL_WAITFORPROPERTY_H
#define KTUTORIAL_WAITFORPROPERTY_H

#include <QtCore/QVariant>

#include "ktutorial_export.h"

#include "WaitFor.h"

namespace ktutorial {

/**
 * Waits for a property to have a specific value.
 * When the property changes its value to the specified one and the
 * WaitForProperty is active, the wait ends.
 *
 * Note that the condition can be met even if the wait did not end. The
 * condition is met if the property has the expected value. However, the wait
 * ends only when the property changes to the expected value while the WaitFor
 * is active. That is, in an active WaitFor, the condition would be met even if
 * the expected value was set in the property when the WaitFor was inactive, but
 * the wait would not end until the expected value was set again in the property
 * after the WaitFor was activated.
 *
 * Not every property can be used to wait for its value to change. Only
 * properties that have a notify signal can be used with that purpose.
 *
 * WaitForProperty with properties that do not have a notify signal can still be
 * used to enrich other WaitFors (for example, a WaitForAnd containing a
 * WaitForSignal and a WaitForProperty: waiting for a signal to be emitted but
 * only changing to the next step if, in addition, some property has some
 * value), but they can not be used alone as single WaitFors (for example,
 * waiting just for the property "visible" of some widget to have some value
 * would never end).
 *
 * Using a WaitForProperty just to perform further checks when other WaitFor
 * ends its waiting should be done only with properties that do not have a
 * notify signal. For example, if a WaitForAnd contains a WaitForSignal and a
 * WaitForProperty, and the WaitForProperty uses a property with a notify
 * signal, it may happen that the property changes to the expected value after
 * the signal was emitted. If that happens, the WaitForProperty would end its
 * waiting, and so the WaitForAnd as the WaitForSignal had already ended its
 * waiting too. It may not be the desired behavior if the WaitForProperty was
 * there to be checked only when the WaitForSignal ended. In that case, instead
 * of using a WaitForProperty, the proper way to do it is executing a custom
 * slot when the WaitForSignal ends its waiting, and checking the value of the
 * property in a "if" construction in that slot.
 */
class KTUTORIAL_EXPORT WaitForProperty: public WaitFor {
Q_OBJECT
public:

    /**
     * Creates a new WaitForProperty.
     * This constructor is needed to dynamically create WaitForProperty objects
     * in scripts using ScriptingModule::newWaitFor(const QString&). Method
     * setProperty(QObject*, const QString&, const QVariant&) must be called to
     * finish setting up the object. For C++ tutorials, use
     * WaitForProperty(QObject*, const QString&, const QVariant&) constructor
     * instead of this one.
     */
    Q_INVOKABLE WaitForProperty();

    /**
     * Creates a new WaitForProperty.
     *
     * @param object The object that contains the property.
     * @param propertyName The name of the property.
     * @param value The value of the property to wait for.
     */
    WaitForProperty(QObject* object, const QString& propertyName,
                    const QVariant& value);

    /**
     * Destroys this WaitForProperty.
     */
    virtual ~WaitForProperty();

    /**
     * Sets the property to wait for.
     * This method can be invoked from a script.
     *
     * In fact, you should only invoke this method from a script, and only once,
     * to set up the object. For C++ tutorials, use
     * WaitForProperty(QObject*, const QString&, const QVariant&) constructor
     * when creating this WaitForProperty.
     *
     * @param object The object that contains the property.
     * @param propertyName The name of the property.
     * @param value The value of the property to wait for.
     */
    Q_INVOKABLE void setProperty(QObject* object, const QString& propertyName,
                                 const QVariant& value);

    /**
     * setProperty(QObject*, const QString&, const QVariant&) version to be used
     * in Ruby scripts.
     * Ruby backend calls setProperty(const char*, const QVariant&) method
     * instead of the aforementioned one. This method just offers a non
     * overloaded name and delegates the call in the method from
     * WaitForProperty.
     *
     * @param object The object that contains the property.
     * @param propertyName The name of the property.
     * @param value The value of the property to wait for.
     * @todo Remove when Ruby backend gets proper support for overloaded
     *       methods.
     */
    Q_INVOKABLE void setPropertyToWaitFor(QObject* object,
                                          const QString& propertyName,
                                          const QVariant& value);

    /**
     * Returns true if the property has the expected value, false otherwise.
     * Note that it will return true even if the property got the expected value
     * when this WaitForProperty was inactive. That is, as long as the property
     * has the expected value, it will return true, no matter when that value
     * was set.
     *
     * @return True if the property has the expected value, false otherwise.
     */
    virtual bool conditionMet() const;

private:

    class WaitForPropertyPrivate* d;

private Q_SLOTS:

    /**
     * When the property value changes to the expected one, this method notifies
     * that the wait for the property ended.
     * The wait is only ended if this WaitForProperty is active.
     */
    void checkPropertyValueToEndTheWait();

};

}

#endif
