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

#ifndef KTUTORIAL_CUSTOMIZATION_DEFAULTKDECUSTOMIZATION_H
#define KTUTORIAL_CUSTOMIZATION_DEFAULTKDECUSTOMIZATION_H

#include <KXmlGuiWindow>

#include "../KTutorialCustomization.h"

class KAction;

namespace ktutorial {
namespace customization {

/**
 * Default KTutorial customization for KDE.
 * The default customization adds a "Tutorials" menu entry in the "Help" menu
 * that shows a TutorialManagerDialog; the tutorial UI shown is a StepWidget. It
 * also registers the default tutorial, "Using KTutorial".
 *
 * Note that the default customization can be used only with XMLGUI applications
 * (applications where the main window is a KXmlGuiWindow). In order to add the
 * menu entry in the proper place in the menu a change must be done in your
 * "applicationnameui.rc". Please refer to KTutorial manual for further
 * information.
 */
class DefaultKdeCustomization: public KTutorialCustomization {
Q_OBJECT
public:

    /**
     * Creates a new DefaultKdeCustomization for the given window.
     *
     * @param window The main window of the application.
     */
    explicit DefaultKdeCustomization(KXmlGuiWindow* window);

    /**
     * Sets up the KTutorial user interface.
     * It adds the Tutorials action and the menu entry for it in Help menu, and
     * prepares the TutorialManagerDialog and the StepWidget to be shown when
     * needed.
     * 
     * Besides setting up the user interface, the default "Using KTutorial"
     * tutorial is registered.
     *
     * @param tutorialManager The TutorialManager used by KTutorial.
     */
    virtual void setup(TutorialManager* tutorialManager);

    /**
     * Returns the main window of the application.
     *
     * @return The main window of the application.
     */
    virtual QWidget* mainApplicationWindow();

public slots:

    /**
     * Shows the tutorial UI.
     * A StepWidget is used.
     *
     * @param tutorial The tutorial to show its StepWidget.
     */
    virtual void showTutorialUI(Tutorial* tutorial);

private:

    /**
     * The main application window.
     */
    KXmlGuiWindow* mWindow;

    /**
     * The manager for the tutorials.
     */
    TutorialManager* mTutorialManager;

    /**
     * The KAction to show the TutorialManagerDialog.
     */
    KAction* mTutorialsAction;

private slots:

    /**
     * Shows a modal TutorialManagerDialog.
     * Called when the tutorials action is triggered.
     */
    void showTutorialManagerDialog() const;

    /**
     * Disables mTutorialsAction.
     * Just a wrapper to be connected with signals that can't pass false to the
     * setEnabled(bool) slot.
     */
    void disableTutorialsAction();

    /**
     * Enables mTutorialsAction.
     * Just a wrapper to be connected with signals that can't pass true to the 
     * setEnabled(bool) slot.
     */
    void enableTutorialsAction();

};

}
}

#endif
