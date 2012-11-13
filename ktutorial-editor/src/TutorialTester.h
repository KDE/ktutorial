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

#ifndef TUTORIALTESTER_H
#define TUTORIALTESTER_H

#include <QObject>

class KTutorialEditor;

/**
 * Utility class to test a tutorial in the target application, starting it if
 * necessary.
 */
class TutorialTester: public QObject {
Q_OBJECT
public:

    /**
     * Creates a new TutorialTester with the given KTutorialEditor.
     *
     * @param tutorialEditor The parent KTutorialEditor.
     */
    explicit TutorialTester(KTutorialEditor* tutorialEditor);

    /**
     * Sets the step to test the tutorial from.
     * After starting the tutorial, it will be changed to the given step.
     *
     * @param stepId The id of the step to test the tutorial from.
     */
    void setStepToTestFrom(const QString& stepId);

    /**
     * Tests the current tutorial in the target application.
     * The tutorial is saved to a temporary file that will be removed when the
     * editor is closed.
     */
    void testTutorial();

private:

    /**
     * The KTutorialEditor to work with.
     */
    KTutorialEditor* mTutorialEditor;

    /**
     * The id of the step to test the tutorial from, if any.
     */
    QString mStepId;

private Q_SLOTS:

    /**
     * Exports the current tutorial to a temporary JavaScript file and sends the
     * file name (and, if set, the step id) to the target application to test
     * the tutorial.
     */
    void sendTutorialToTargetApplication();

};

#endif
