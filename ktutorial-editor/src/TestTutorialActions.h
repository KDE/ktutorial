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

#ifndef TESTTUTORIALACTIONS_H
#define TESTTUTORIALACTIONS_H

#include <QObject>

class KTutorialEditor;
class Step;

/**
 * Test tutorial related actions.
 * TestTutorialActions provide the actions to test the current tutorial in the
 * target application (from the start or from the selected step).
 *
 * KTutorialEditor notifies TestTutorialActions when a step is selected, so it
 * can know from which step test the tutorial.
 */
class TestTutorialActions: public QObject {
Q_OBJECT
public:

    /**
     * Creates a new TestTutorialActions for the given KTutorialEditor.
     * All the actions are set up and added to the KTutorialEditor.
     *
     * @param tutorialEditor The KTutorialEditor to work with.
     */
    explicit TestTutorialActions(KTutorialEditor* tutorialEditor);

public Q_SLOTS:

    /**
     * Sets the current step and enables or disables the actions that depend on
     * a step as needed.
     *
     * @param step The step to select, or null to deselect the current one.
     */
    void selectStep(Step* step);

private:

    /**
     * The KTutorialEditor to work with.
     */
    KTutorialEditor* mTutorialEditor;

    /**
     * The currently selected step.
     */
    Step* mCurrentStep;

    /**
     * Sets up all the edit related actions.
     */
    void setupActions();

private Q_SLOTS:

    /**
     * Enables or disables the "test tutorial from current step" action based on
     * the current step.
     */
    void updateTestTutorialFromCurrentStepActionState();

    /**
     * Tests the current tutorial in the target application.
     */
    void testTutorial();

    /**
     * Tests the current tutorial from the current step in the target
     * application.
     */
    void testTutorialFromCurrentStep();

};

#endif
