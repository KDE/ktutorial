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

#ifndef EDITACTIONS_H
#define EDITACTIONS_H

#include <QObject>

class CommandWidget;
class KTutorialEditor;
class KUndoStack;
class Reaction;
class Step;

/**
 * Edition related actions.
 * EditActions provide the actions to edit a tutorial (set the setup code, add a
 * step...), and actions to undo and redo the edition.
 *
 * KTutorialEditor notifies EditActions when a step or reaction is selected, so
 * it can know which step or reaction have to be edited. EditActions provides
 * information about whether the tutorial is clean (not modified after the last
 * time it was saved) or not, and allows to clear the command stack (for
 * example, when a new file is opened) or set it as clean.
 *
 * The KTutorialEditor window is also used as the parent for every dialog shown
 * by the actions.
 */
class EditActions: public QObject {
Q_OBJECT
public:

    /**
     * Creates a new EditActions for the given KTutorialEditor.
     * All the actions are set up and added to the KTutorialEditor.
     *
     * @param tutorialEditor The KTutorialEditor to work with.
     */
    explicit EditActions(KTutorialEditor* tutorialEditor);

public:

    /**
     * Clears the stack of undoable commands.
     */
    void clearCommands();

    /**
     * Sets the tutorial as clean.
     */
    void setClean();

public Q_SLOTS:

    /**
     * Sets the current step and enables or disables the actions that depend on
     * a step as needed.
     *
     * @param step The step to select, or null to deselect the current one.
     */
    void selectStep(Step* step);

    /**
     * Sets the current reaction and enables or disables the actions that depend
     * on a reaction as needed.
     *
     * @param reaction The reaction to select, or null to deselect the current
     *                 one.
     */
    void selectReaction(Reaction* reaction);

Q_SIGNALS:

    /**
     * Emitted when the clean state changes.
     *
     * @param clean True if the tutorial is clean, false otherwise.
     */
    void cleanChanged(bool clean);

private:

    /**
     * The KTutorialEditor to work with.
     */
    KTutorialEditor* mTutorialEditor;

    /**
     * The stack of undoable commands.
     */
    KUndoStack* mUndoStack;

    /**
     * The currently selected step.
     */
    Step* mCurrentStep;

    /**
     * The currently selected reaction.
     */
    Reaction* mCurrentReaction;

    /**
     * Sets up all the edit related actions.
     */
    void setupActions();

    /**
     * Shows an EditionDialog for the given CommandWidget.
     * The undo stack used in the CommandWidget is mUndoStack.
     *
     * @param commandWidget The CommandWidget to wrap.
     * @return QDialog::Accepted if the dialog was accepted, or
     *         QDialog::Rejected if the dialog was rejected.
     */
    int showEditionDialog(CommandWidget* commandWidget);

private Q_SLOTS:

    /**
     * Shows a TutorialInformationWidget for the tutorial.
     */
    void setTutorialInformation();

    /**
     * Shows a LicenseWidget for the tutorial.
     */
    void setTutorialLicense();

    /**
     * Shows a TutorialCustomCodeWidget for the setup code of the tutorial.
     */
    void setTutorialSetup();

    /**
     * Shows a TutorialCustomCodeWidget for the tear down code of the tutorial.
     */
    void setTutorialTearDown();

    /**
     * Adds a new step to the tutorial after showing a StepDataWidget for it.
     * The step isn't added if the dialog is cancelled.
     */
    void addStep();

    /**
     * Shows a StepDataWidget for the current step.
     */
    void setStepData();

    /**
     * Shows a StepCustomCodeWidget for the setup code of the current step.
     */
    void setStepSetup();

    /**
     * Shows a StepCustomCodeWidget for the tear down code of the current step.
     */
    void setStepTearDown();

    /**
     * Removes the current step from the tutorial.
     */
    void removeStep();

    /**
     * Adds a new reaction to the current step after showing a ReactionWidget
     * for it.
     * The reaction isn't added if the dialog is cancelled.
     */
    void addReaction();

    /**
     * Shows a ReactionWidget for the current reaction.
     */
    void setReactionData();

    /**
     * Removes the current reaction from its step.
     */
    void removeReaction();

};

#endif
