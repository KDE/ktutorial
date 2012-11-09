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

#ifndef KTUTORIAL_WAITFORNOT_H
#define KTUTORIAL_WAITFORNOT_H

#include "ktutorial_export.h"

#include "WaitFor.h"

namespace ktutorial {

/**
 * Special WaitFor class to negate other WaitFor objects.
 * This WaitFor is intended to be used only as a child of WaitForComposed
 * objects. It negates the condition of the WaitFor object added to the
 * WaitForNot.
 *
 * WaitForNot can't be added directly in Steps, as they don't emit
 * waitEnded(WaitFor*) signal. The signal isn't emitted because it is intended
 * to notify when the wait ends. However, in this class, the wait doesn't really
 * ends, because it is ended since the beginning.
 *
 * If you need to use WaitForNot, it will be likely be in a WaitForAnd. For
 * example, to wait for a signal only if another signal wasn't emitted. Note
 * that, in this case, you will need to add a fallback WaitFor or an Option to
 * avoid a Step to hang.
 *
 * \code
 *   WaitForSignal* someSignal = new WaitForSignal(someObject, SIGNAL(someSignal()));
 *   WaitForSignal* offendingSignal = new WaitForSignal(someObject, SIGNAL(offendingSignal()));
 *   WaitForNot* notOffendingSignal = new WaitForNot(offendingSignal);
 *
 *   WaitForAnd* someSignalButNotOffendingSignal = new WaitForAnd();
 *   someSignalButNotOffendingSignal->add(someSignal);
 *   someSignalButNotOffendingSignal->add(notOffendingSignal);
 *
 *   step->addWaitFor(someSignalButNotOffendingSignal, someObject, SLOT(someSlot));
 *   ...
 *   step->addWaitFor(fallbackWaitFor, someObject, SLOT(anotherSlot));
 * \endcode
 */
class KTUTORIAL_EXPORT WaitForNot: public WaitFor {
Q_OBJECT
public:

    /**
     * Creates a new WaitForNot.
     * This constructor is needed to dynamically create WaitForNot objects in
     * scripts using ScriptingModule::newWaitFor(const QString&). Method
     * setNegatedWaitFor(WaitFor*) must be called to finish setting up the
     * object. For C++ tutorials, use WaitForNot(WaitFor*) constructor instead
     * of this one.
     */
    Q_INVOKABLE WaitForNot();

    /**
     * Creates a new WaitForNot.
     * The WaitFor is reparented to this WaitForNot, and thus deleted when this
     * WaitForNot is deleted.
     *
     * @param waitFor The child WaitFor.
     */
    explicit WaitForNot(WaitFor* waitFor);

    /**
     * Sets the WaitFor to be negated.
     * The WaitFor is reparented to this WaitForNot, and thus deleted when this
     * WaitForNot is deleted.
     * This method can be invoked from a script.
     *
     * In fact, you should only invoke this method from a script, and only once,
     * to set up the object. For C++ tutorials, use WaitForNot(WaitFor*)
     * constructor when creating this WaitForNot.
     *
     * @param waitFor The child WaitFor.
     */
    Q_INVOKABLE void setNegatedWaitFor(WaitFor* waitFor);

    /**
     * Returns true if the child WaitFor condition isn't met, false otherwise.
     *
     * @return True if the child WaitFor condition isn't met, false otherwise.
     */
    virtual bool conditionMet() const;

    /**
     * Sets this WaitForNot and its child as active or inactive.
     *
     * @param active True to set it active, false otherwise.
     */
    virtual void setActive(bool active);

protected:

    /**
     * The WaitFor to check if its condition isn't met.
     */
    WaitFor* mWaitFor;

};

}

#endif
