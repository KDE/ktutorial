/***************************************************************************
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

#ifndef KTUTORIAL_STEP_P_H
#define KTUTORIAL_STEP_P_H

namespace ktutorial {

class StepPrivate {
public:

    /**
     * The identifier of this Step.
     */
    QString mId;

    /**
     * The text shown to the user.
     */
    QString mText;

    /**
     * Whether this Step is active or not
     */
    bool mActive;

    /**
     * When this flag is on, the conditions and conditions to wait for added are
     * removed and deleted the next time this Step is deactivated.
     */
    bool mDeleteAddedObjectsInTearDown;

    /**
     * The Options for this Step.
     */
    QList<Option*> mOptions;

    /**
     * The Options added in the setup to be deleted in the tearDown.
     */
    QList<Option*> mOptionsToBeDeletedInTearDown;

    /**
     * The conditions to wait for in each Option.
     * The order of both lists is the same, so the index in the Options list
     * is the index of it associated WaitFor.
     */
    QList<WaitFor*> mOptionsWaitsFor;

    /**
     * The conditions to wait for in this Step.
     */
    QList<WaitFor*> mWaitsFor;

    /**
     * The conditions to wait for added in the setup to be deleted in the
     * tearDown.
     */
    QList<WaitFor*> mWaitsForToBeDeletedInTearDown;

    /**
     * Associates a condition to wait for with the id of the step to execute
     * when the condition is met.
     */
    QHash<WaitFor*, QString> mNextStepForWaitFor;

};

}

#endif
