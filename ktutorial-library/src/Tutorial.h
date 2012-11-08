/***************************************************************************
 *   Copyright (C) 2008 by Daniel Calviño Sánchez <danxuliu@gmail.com>     *
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

#ifndef KTUTORIAL_TUTORIAL_H
#define KTUTORIAL_TUTORIAL_H

#include <QMap>
#include <QObject>

#include "ktutorial_export.h"

namespace ktutorial {
class Step;
class TutorialInformation;
}

namespace ktutorial {

/**
 * A tutorial.
 * A tutorial is composed by some information about it and several steps. The
 * steps are shown to the user, one at a time, and the tutorial advances as the
 * user completes each Step.
 *
 * The information is stored in a TutorialInformation object associated with the
 * Tutorial. It must be provided when the Tutorial is created. Subclasses can
 * set it to 0, but must initialize it in their own constructor code. Don't
 * delete the TutorialInformation in subclasses, as it is deleted when the
 * Tutorial is destroyed.
 *
 * Steps are the main elements in a Tutorial. They are each of the phases the
 * user must complete when following a Tutorial. A Tutorial will have several
 * Steps (each with a unique identifier), but it must contain at least a Step
 * with "start" id. This will be the starting point of the Tutorial, and the
 * first one to be activated.
 *
 * To activate a Step, any of Tutorial::nextStep must be called. You are advised
 * to use the QString version for convenience. Steps can activate another step
 * when a condition is met or an option selected using an overloaded version of
 * addOption and addWaitFor methods provided for convenience.
 *
 * The Tutorial finishes when the user closes it. It can happen in an
 * intermediate Step or when there are no more Steps. When making a Tutorial,
 * add a final Step which notifies the user that the Tutorial is done and that
 * it can be closed.
 *
 * To create a Tutorial embedded in the application code the easiest way is
 * subclassing Tutorial. In the constructor of the subclass, initialize the
 * information about the Tutorial and add all the Steps with their WaitFors and
 * Options. Add as many slots as needed to the class to connect them with the
 * WaitFors and Options when added to their Steps. If something must be set
 * before starting the Tutorial redefine setup(), if something must be cleaned
 * after finishing the tutorial redefine tearDown(), and you are done.
 */
class KTUTORIAL_EXPORT Tutorial: public QObject {
Q_OBJECT
public:

    /**
     * Creates a new Tutorial with the specified TutorialInformation.
     * When deriving from this class you may need to set the TutorialInformation
     * to 0. In that case, remember to set the TutorialInformation in the
     * constructor body!
     *
     * @param tutorialInformation The TutorialInformation for this Tutorial.
     */
    explicit Tutorial(TutorialInformation* tutorialInformation): QObject(),
        mTutorialInformation(tutorialInformation),
        mCurrentStep(0) {
    }

    /**
     * Destroys this Tutorial.
     * It also destroys the TutorialInformation.
     */
    virtual ~Tutorial();

    /**
     * Returns the TutorialInformation of this Tutorial.
     * This method can be invoked from a script.
     *
     * @return The TutorialInformation of this Tutorial.
     */
    Q_INVOKABLE TutorialInformation* tutorialInformation() const {
        return mTutorialInformation;
    }

    /**
     * Adds a new Step to this Tutorial.
     * The order in which Steps are added has no relevance. Changing the active
     * Step to another Step must be done with any of Tutorial::nextStep.
     *
     * The Step is reparented to this Tutorial, and thus deleted when this
     * Tutorial is deleted.
     *
     * This method can be invoked from a script.
     *
     * @param step The Step to add.
     */
    Q_INVOKABLE void addStep(Step* step);

    /**
     * Starts this Tutorial.
     * It sets up the tutorial and activates the start step.
     *
     * This method is used internally. Do not call this method yourself.
     */
    void start();

    /**
     * Activates the next step in the Tutorial.
     * Call this method when the Tutorial has to pass to another Step. Consider
     * using nextStep(const QString&) instead for convenience.
     *
     * @param step The Step to change to.
     */
    void nextStep(Step* step);

public Q_SLOTS:

    /**
     * Activates the next Step in the Tutorial.
     * The identifier must be of one of the Steps added to this Tutorial.
     *
     * Call this method when the Tutorial has to pass to another Step.
     *
     * This method can be invoked from a script.
     *
     * @param id The identifier of the next Step to set.
     * @see nextStep(Step*)
     */
    void nextStep(const QString& id);

    /**
     * Finishes this Tutorial.
     * The current step is deactivated, this tutorial is cleaned, and finished
     * signal is emitted.
     *
     * If a KTutorial customization is used, this method must be called when the
     * user finished the tutorial from the UI.
     */
    void finish();

Q_SIGNALS:

    /**
     * This signal is emitted when this Tutorial finishes.
     *
     * If a KTutorial customization is used, you may connect to this signal to
     * know when the tutorial UI is no longer needed.
     *
     * @param tutorial This tutorial.
     */
    void finished(Tutorial* tutorial);

    /**
     * This signal is emitted when a Step is activated.
     *
     * If a KTutorial customization is used, you must connect to this signal to
     * know when the tutorial UI has to show a new step.
     *
     * @param step The activated Step.
     */
    void stepActivated(Step* step);

protected:

    /**
     * The information about this Tutorial.
     * Remember to initialize it if you derive from Tutorial class.
     */
    TutorialInformation* mTutorialInformation;

    /**
     * Sets up the Tutorial before activating start Step.
     * Tutorial subclasses can redefine it if they need to set up something
     * before start.
     */
    virtual void setup() {
    }

    /**
     * Sets up the Tutorial before activating start Step.
     * Tutorial subclasses can redefine it if they need to set up something
     * before start.
     */
    virtual void tearDown() {
    }

private:

    /**
     * All the added Steps, indexed by their identifier.
     */
    QMap<QString, Step*> mSteps;

    /**
     * The step currently active, if any.
     */
    Step* mCurrentStep;

    /**
     * The steps to change to that were not activated yet.
     */
    QList<Step*> mQueuedSteps;

    /**
     * Activates the given step.
     * When calling nextStep(Step*), the tutorial queues the steps to change to.
     * The real activation of the step is done by this method.
     *
     * @param step The Step to activate.
     */
    void changeToStep(Step* step);

};

}

#endif
