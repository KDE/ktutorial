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

#ifndef KTUTORIAL_WAITFORSTEPACTIVATION_H
#define KTUTORIAL_WAITFORSTEPACTIVATION_H

#include "ktutorial_export.h"

#include "WaitFor.h"

namespace ktutorial {
class Step;
class Tutorial;
}

namespace ktutorial {

/**
 * Waits for the activation of a step.
 * When the step is activated, the wait ends.
 *
 * The purpose of WaitForStepActivations is to be combined with one or more
 * WaitForProperties using a WaitForAnd. If the properties have the expected
 * value when the step is activated, the associated slot is executed or the next
 * step is requested (depending on the Step::addWaitFor() method used). It can
 * be used to set "preconditions" for a step, and change to another step if
 * some things that should have been done before that step were not done yet.
 *
 * Note that the condition is met only while the step is being activated. Once
 * the step is active, the condition is no longer met (until the next time that
 * the step is being activated again). Due to this, it only makes sense to use
 * WaitForStepActivation with the same step that the WaitForStepActivation is
 * going to be added to (if used to wait for the activation of another step, the
 * WaitForStepActivation will always be inactive when that step is being
 * activated).
 *
 * Despite waiting for the step activation, WaitForStepActivation can be safely
 * created and added to a step in its setup, and several WaitForStepActivation
 * can be added to the same step without problems.
 *
 * @see WaitForAnd
 * @see WaitForProperty
 */
class KTUTORIAL_EXPORT WaitForStepActivation: public WaitFor {
Q_OBJECT
public:

    /**
     * Creates a new WaitForStepActivation.
     * This constructor is needed to dynamically create WaitForStepActivation
     * objects in scripts using ScriptingModule::newWaitFor(const QString&).
     * Method setStep(const Tutorial*, const Step*) must be called to finish
     * setting up the object. For C++ tutorials, use
     * WaitForStepActivation(const Tutorial*, const Step*) constructor instead
     * of this one.
     */
    Q_INVOKABLE WaitForStepActivation();

    /**
     * Creates a new WaitForStepActivation.
     *
     * @param tutorial The tutorial that contains the step.
     * @param step The step to wait for its activation.
     */
    WaitForStepActivation(const Tutorial* tutorial, const Step* step);

    /**
     * Sets the step to wait for its activation.
     * This method can be invoked from a script.
     *
     * In fact, you should only invoke this method from a script, and only once,
     * to set up the object. For C++ tutorials, use
     * WaitForStepActivation(const Tutorial*, const Step*) constructor when
     * creating this WaitForStepActivation.
     *
     * @param tutorial The tutorial that contains the step.
     * @param step The step to wait for its activation.
     */
    Q_INVOKABLE void setStep(const Tutorial* tutorial, const Step* step);

    /**
     * Returns true if the step is being activated.
     * Note that true is only returned during the step activation, but not once
     * it is already active.
     *
     * @return True if the step is being activated, false otherwise.
     */
    virtual bool conditionMet() const;

private:

    /**
     * Whether the step is being activated or not.
     */
    bool mDuringStepActivation;

private Q_SLOTS:

    /**
     * When the step is activated, this method notifies that the wait for the
     * activation ended.
     * The wait is only ended if this WaitForStepActivation is active.
     */
    void checkStepActivatedToEndTheWait(Step* step);

};

}

#endif
