/***************************************************************************
 *   Copyright (C) 2008 by Daniel Calviño Sánchez <danxuliu@gmail.com>     *
 *   Copyright (C) 2009 by Daniel Calviño Sánchez <danxuliu@gmail.com>     *
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

#ifndef KTUTORIAL_WAITFORSIGNAL_H
#define KTUTORIAL_WAITFORSIGNAL_H

#include "ktutorial_export.h"

#include "WaitFor.h"

namespace ktutorial {

/**
 * Waits for a signal to be emitted.
 * When the signal is emitted and the WaitForSignal is active, the wait ends.
 *
 * Note that if the signal is emitted while the WaitFor isn't active, it won't
 * be registered and the condition won't be met. In order to met the condition,
 * the signal must be emitted while the WaitForSignal is active.
 */
class KTUTORIAL_EXPORT WaitForSignal: public WaitFor {
Q_OBJECT
public:

    /**
     * Creates a new WaitForSignal.
     * This constructor is needed to dynamically create WaitForSignal objects in
     * scripts using ScriptingModule::newWaitFor(const QString&). Method
     * setSignal(QObject*, const QString&) must be called to finish setting up
     * the object. For C++ tutorials, use
     * WaitForSignal(QObject*, const QString&) constructor instead of this one.
     */
    Q_INVOKABLE WaitForSignal();

    /**
     * Creates a new WaitForSignal.
     * Note that the signal name can be set with or without using the SIGNAL
     * macro.
     *
     * @param sender The sender of the signal.
     * @param signal The signal to wait for.
     */
    WaitForSignal(QObject* sender, const QString& signal);

    /**
     * Destroys this WaitForSignal.
     */
    virtual ~WaitForSignal();

    /**
     * Sets the signal to wait for.
     * Note that the signal name can be set with or without using the SIGNAL
     * macro (in fact, in a script you will not have a SIGNAL macro).
     * This method can be invoked from a script.
     *
     * In fact, you should only invoke this method from a script, and only once,
     * to set up the object. For C++ tutorials, use
     * WaitForSignal(QObject*, const QString&) constructor when creating this
     * WaitForSignal.
     *
     * @param sender The sender of the signal.
     * @param signal The signal to wait for.
     */
    Q_INVOKABLE void setSignal(QObject* sender, const QString& signal);

    /**
     * Returns true if the connected signal was emitted while active, false
     * otherwise.
     *
     * @return True if the connected signal was emitted while active, false
     * otherwise.
     */
    virtual bool conditionMet() const;

    /**
     * Sets this WaitForSignal active or inactive.
     * Activating it resets its condition.
     *
     * @param active True to set it active, false otherwise.
     */
    virtual void setActive(bool active);

public Q_SLOTS:

    /**
     * Notifies that the wait for the signal ended.
     * If active, it sets the condition as met and ends its wait.
     */
    void signalWaitEnd();

private:

    class WaitForSignalPrivate* d;

};

}

#endif
