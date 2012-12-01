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

#ifndef KTUTORIAL_STEP_H
#define KTUTORIAL_STEP_H

#include <QtCore/QHash>
#include <QtCore/QList>
#include <QtCore/QObject>
#include <QtCore/QString>

#include "ktutorial_export.h"

namespace ktutorial {
class Option;
class WaitFor;
}

namespace ktutorial {

/**
 * A step in a Tutorial.
 * It is each of the tasks that must be accomplished when following a Tutorial.
 * It must contain a text that explains what must be done by the user.
 *
 * Each of the Steps in a Tutorial must have a unique identifier. Two steps can
 * have the same identifier, but only if they aren't part of the same Tutorial.
 * The identifier must be an untranslated string, and it is used for easy
 * setting of the active Step.
 *
 * A Step is active if it is the Step the user is in. That is, if it is the one
 * currently being shown. Making a Step active or not is handled by KTutorial.
 * You shouldn't set a Step active yourself. In order to change the current
 * Step, you must use any of Tutorial::nextStep.
 *
 * Most of times, the text of the Step are instructions that, when accomplished,
 * activates the next Step. To do this, several conditions to wait for can be
 * added to the Step, and when a condition is met and the Step is active, a slot
 * specified when adding the condition is called. The conditions to wait for are
 * modelled using the hierarchy of classes derived from WaitFor.
 *
 * \code
 *   Step* step = new Step("someStep");
 *   step->setText(i18nc("@info", "Write something in the text area"));
 *
 *   QObject* textArea = KTutorial::self()->findObject<QObject*>("textArea");
 *   step->addWaitFor(new WaitForSignal(textArea, SIGNAL(textChanged())),
 *                    this, SLOT(startDone()));
 *
 *   addStep(startStep);
 * \endcode
 *
 * There will usually be just one condition in each Step (although it can be a
 * composed condition, see WaitForComposed). However, there is nothing wrong in
 * adding more than one WaitFor if needed. It will behave as it does with one
 * condition: when a condition is met, its associated slot will be called.
 *
 * However, you must be careful with the added WaitFors. You shouldn't add in
 * the same Step more than one WaitFor that wait for the same condition,
 * directly or indirectly. For example, waiting for textChanged() signal in a
 * text area and activating an action that clears the text area (as when the
 * action clears the text area, the textChanged() is emitted). Behaviour is
 * undefined in that case. Of course, there is no problem in adding WaitFors
 * that wait for the same condition in different Steps (but each WaitFor must be
 * a different object, they can't be reused).
 *
 * Also, sometimes is needed to show some Options to the user, for example to
 * head the Tutorial in one way or another. When an Option is selected by the
 * user, the slot specified when adding it is called, just as it happened with
 * WaitFors. You can have several Options and WaitFors added in the same Step,
 * although it is not very common.
 *
 * The slots called when an option is selected or when a condition in a WaitFor
 * is met can't have arguments. If you need further processing of the condition,
 * you must do it in the slot by your own means. For example, if you wait for a
 * clicked(bool checked) signal in a QAbstractButton and need to know if it is
 * checked, you should get the button in the slot code and look there whether it
 * is checked or not.
 *
 * \code
 * void SomeTutorial::clickedButton() {
 *     QAbstractButton* button = KTutorial::self()->findObject<QAbstractButton*>("myButton");
 *
 *     if (button->isChecked()) {
 *         nextStep("otherStep");
 *     }
 * }
 * \endcode
 *
 * Sometimes, when a condition to wait for is met or an option is selected you
 * may only want to change to another step, without further checks or actions.
 * In these cases, you can create a slot that just changes to the next step and
 * connect to it when adding the Option or WaitFor. But you can also use a
 * shortcut: there are special versions of addOption and addWaitFor methods that
 * receive the id of the next step to change to, which saves you the need to
 * create a slot just for that.
 *
 * When you need to perform something special when a Step is activated, you must
 * define a subclass of Step and redefine the setup() method in it. If something
 * must be performed when a Step is deactivated, you must redefine tearDown() 
 * instead.
 *
 * This may be needed, for example, when a Step needs to use an object that
 * didn't exist when the Tutorial was created (like something in a dialog
 * deleted every time it is closed). In this case, a subclass of Step should be
 * made with a setup() method adding the WaitFor for the "volatile" object.
 * Note, however, that you don't have to remove nor delete the WaitFor added in
 * the setup() method. It is done automatically when the step is deactivated.
 *
 * \code
 * void StepSubclass::setup() {
 *     QObject* textArea = KTutorial::self()->findObject<QObject*>("textAreaInDialog");
 *
 *     waitForAttribute = new WaitForSignal(textArea, SIGNAL(textChanged()));
 *     addWaitFor(waitForAttribute, this, SLOT(startDone()));
 * }
 * \endcode
 */
class KTUTORIAL_EXPORT Step: public QObject {
Q_OBJECT
public:

    /**
     * Creates a new Step with the specified identifier.
     * The identifier must be unique in the Tutorial this Step belongs to.
     *
     * @param id The identifier of this Step.
     */
    explicit Step(const QString& id);

    /**
     * Destroys this Step.
     */
    virtual ~Step();

    /**
     * Returns the identifier of this Step.
     *
     * @return The identifier of this Step.
     */
    QString id() const;

    /**
     * Returns the Options added to this Step.
     *
     * @return The Options added to this Step.
     */
    QList<Option*> options() const;

    /**
     * Returns the text to be shown to the user.
     *
     * @return The text to be shown to the user.
     */
    QString text() const;

    /**
     * Sets the text to be shown to the user.
     * Note that if the text is set while this Step is being shown it won't
     * be updated in the GUI. The text will be updated in the GUI the next time
     * this Step is activated.
     *
     * This method can be invoked from a script.
     *
     * @param text The text to set.
     */
    Q_INVOKABLE void setText(const QString& text);

    /**
     * Returns true if this Step is active, false otherwise.
     *
     * @return True if this Step is active, false otherwise.
     */
    bool isActive() const;

    /**
     * Makes this Step active or inactive.
     * This method is used internally. Do not call this method yourself.
     *
     * @param active True if the Step is active, false otherwise.
     */
    void setActive(bool active);

    /**
     * Adds an Option to this Step.
     * When the Option is selected by the user, the slot of the receiver object
     * is called.
     * Note that the slot name can be set with or without using the SLOT macro.
     *
     * The Option is reparented to this Step, and thus deleted when this Step
     * is deleted. However, if the Option is added in the setup() method of a
     * Step derived class, the Option is automatically removed and deleted when
     * the Step is deactivated (that is, you don't have to do it explicitly in
     * the tearDown() method).
     *
     * If you try to add the same Option (or different ones with the same name)
     * twice, nothing will happen. Only the Option added in first place and its
     * associated slot will be taken in consideration.
     *
     * This method can be invoked from a script.
     *
     * @param option The Option to add.
     * @param receiver The object to call its slot.
     * @param slot The slot name of the object to call.
     */
    Q_INVOKABLE void addOption(Option* option, QObject* receiver,
                               const QString& slot);

    /**
     * Adds an Option to this Step.
     * When the Option is selected by the user, the tutorial this Step is part
     * of will activate the step identified by nextStepId.
     *
     * This method only provides a shortcut to avoid creating an slot that
     * just calls tutorial.nextStep(nextStepId).
     *
     * In evey other aspect, it behaves like
     * addOption(Option*, QObject*, const QString&). See its documentation for
     * further details.
     *
     * This method can be invoked from a script.
     *
     * @param option The Option to add.
     * @param nextStepId The id of the step to change to.
     * @see addOption(Option*, QObject*, const QString&)
     */
    Q_INVOKABLE void addOption(Option* option, const QString& nextStepId);

    /**
     * Adds a condition to wait for to this Step.
     * When the condition is met and this Step is active, the slot of the
     * receiver object is called.
     * Note that the slot name can be set with or without using the SLOT macro.
     *
     * The WaitFor is reparented to this Step, and thus deleted when this Step
     * is deleted. However, if the WaitFor is added in the setup() method of a
     * Step derived class, the WaitFor is automatically removed and deleted when
     * the Step is deactivated (that is, you don't have to do it explicitly in
     * the tearDown() method).
     *
     * If you try to add the same WaitFor twice, nothing will happen. Only the
     * WaitFor added in first place and its associated slot will be taken in
     * consideration.
     *
     * This method can be invoked from a script.
     *
     * @param waitFor The condition to wait for.
     * @param receiver The object to call its slot.
     * @param slot The slot name of the object to call.
     */
    Q_INVOKABLE void addWaitFor(WaitFor* waitFor, QObject* receiver,
                                const QString& slot);

    /**
     * Adds a condition to wait for to this Step.
     * When the condition is met and this Step is active, the tutorial this Step
     * is part of will activate the step identified by nextStepId.
     *
     * This method only provides a shortcut to avoid creating an slot that
     * just calls tutorial.nextStep(nextStepId).
     *
     * In evey other aspect, it behaves like
     * addWaitFor(WaitFor*, QObject*, const QString&). See its documentation for
     * further details.
     *
     * This method can be invoked from a script.
     *
     * @param waitFor The condition to wait for.
     * @param nextStepId The id of the step to change to.
     * @see addOption(Option*, QObject*, const QString&)
     */
    Q_INVOKABLE void addWaitFor(WaitFor* waitFor, const QString& nextStepId);

    /**
     * Removes an Option from this Step.
     * The Option is reparented to null, so you must delete it explicitly.
     *
     * If you try to remove an Option that wasn't added, nothing will happen.
     *
     * This method can be invoked from a script.
     *
     * @param option The Option to remove.
     */
    Q_INVOKABLE void removeOption(Option* option);

    /**
     * Removes a condition to wait for from this Step.
     * The slot of the receiver object associated when adding the WaitFor will
     * not be notified anymore when the condition is met (note that all the
     * slots connected with waitEnded(WaitFor*) will be disconnected). If the
     * WaitFor was associated with a step id, the tutorial won't change to it
     * anymore when the condition is met. In any case, the WaitFor will be also
     * deactivated.
     *
     * The WaitFor is reparented to null, so you must delete it explicitly.
     *
     * If you try to remove a WaitFor that wasn't added, nothing will happen.
     *
     * This method can be invoked from a script.
     *
     * @param waitFor The WaitFor to remove.
     */
    Q_INVOKABLE void removeWaitFor(WaitFor* waitFor);

Q_SIGNALS:

    /**
     * Request a change to the next step.
     * Don't connect nor emit this signal yourself. It is connected
     * automatically by KTutorial.
     *
     * @param nextStepId The id of the Step to request a change to.
     */
    void nextStepRequested(const QString& nextStepId);

protected:

    /**
     * Sets up the Step when it is activated.
     * Step subclasses can redefine it if they need to perform something when 
     * they are activated.
     */
    virtual void setup();

    /**
     * Tears down the Step when it is deactivated.
     * Step subclasses can redefine it if they need to perform something when 
     * they are deactivated.
     */
    virtual void tearDown();

    /**
     * Connects the given WaitFor with the slot in the receiver.
     * This is a helper method for ScriptedStep, as it has an special behavior
     * when connecting and disconnecting slots. You should not redefine this
     * method in your Step subclasses.
     * Note that the slot name can be set with or without using the SLOT macro.
     *
     * @param waitFor The WaitFor to connect.
     * @param receiver The object to connect to.
     * @param slot The slot to connect to.
     */
    virtual void connectWaitFor(WaitFor* waitFor, QObject* receiver,
                                const QString& slot);

    /**
     * Disconnects the given WaitFor.
     * This is a helper method for ScriptedStep, as it has an special behavior
     * when connecting and disconnecting slots. You should not redefine this
     * method in your Step subclasses.
     *
     * @param waitFor The WaitFor to disconnect.
     */
    virtual void disconnectWaitFor(WaitFor* waitFor);

private:

    class StepPrivate* d;

    /**
     * Wraps setup method to ensure that some code is executed before and after
     * inherited setup method.
     * It follows a Template Design Pattern.
     */
    void setupWrapper();

    /**
     * Wraps tearDown method to ensure that some code is executed before and
     * after inherited tearDown method.
     * It follows a Template Design Pattern.
     */
    void tearDownWrapper();

    /**
     * Adds a new Option.
     * The Option is refused to be added if it is already added, or there is
     * another Option with the same name.
     *
     * @param option The Option to add.
     * @return True if the Option was added, false otherwise.
     */
    bool addOption(Option* option);

    /**
     * Adds a new WaitFor.
     * The WaitFor is refused to be added if it is already added.
     *
     * @param waitFor The WaitFor to add.
     * @return True if the WaitFor was added, false otherwise.
     */
    bool addWaitFor(WaitFor* waitFor);

private Q_SLOTS:

    /**
     * Emits nextStepRequested(const QString&) with the id of the step
     * associated to the given WaitFor.
     *
     * @param waitFor The WaitFor to request its associated step.
     */
    void requestNextStepForWaitFor(WaitFor* waitFor);

};

}

#endif
