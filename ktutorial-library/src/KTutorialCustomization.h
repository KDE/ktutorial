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

#ifndef KTUTORIAL_KTUTORIALCUSTOMIZATION_H
#define KTUTORIAL_KTUTORIALCUSTOMIZATION_H

#include <QtCore/QObject>

#include "ktutorial_export.h"

namespace ktutorial {
class Tutorial;
class TutorialManager;
}

namespace ktutorial {

/**
 * Interface to customize the UI of KTutorial.
 * The classes that implement this interface are used by KTutorial to set up its
 * user interface. You can provide your own class to KTutorial through
 * KTutorial::setup(KTutorialCustomization*) to control the user interface for
 * the tutorial manager and the tutorials.
 *
 * As this is an interface, all its methods must be implemented.
 *
 * Besides implementing the methods, you must provide your own classes for the
 * tutorial manager and tutorial UIs. The tutorial manager UI must call the
 * TutorialManager::start(QString) with the id of the tutorial to be started. If
 * you need to know when the tutorial finishes (for example, to show again the
 * tutorial manager UI, or to know when the tutorial UI is no longer needed),
 * you can connect to the TutorialManager::finished() or the
 * Tutorial::finished(Tutorial*) signals. Anyway, the tutorial UI must call
 * Tutorial::finish() when the user finishes the tutorial from the UI. The
 * tutorial UI must also emit the appropriate Option::selected() (connecting it
 * to another signal) when the user selects an option in the UI. Finally, the
 * tutorial UI must connect to the Tutorial::stepActivated(Step*) to know when
 * a new step has to be shown.
 *
 * If you do not want to implement your own KTutorial user interface just use
 * the default one for KDE through KTutorial::setup(KXmlGuiWindow*).
 */
class KTUTORIAL_EXPORT KTutorialCustomization: public QObject {
Q_OBJECT
public:

    /**
     * Sets up the KTutorial user interface.
     * This method will be called before loading the scripted tutorials, and
     * before setting up the editor support (if available).
     *
     * Two things must be done in this method: preparing the user interface for
     * the tutorial manager, and preparing the user interface for the tutorials.
     * Note that preparing the UI for the tutorial manager also implies setting
     * up a method to show the UI itself, for example, through a menu item.
     * Also, to show the tutorial UI when a tutorial is started you can just
     * connect the TutorialManager::started(Tutorial*) signal with the
     * showTutorialUI(Tutorial*) slot in the KTutorialCustomization implementing
     * class.
     * 
     * Besides setting up the user interface, you may also register meta objects
     * and tutorials here if needed.
     *
     * @param tutorialManager The TutorialManager used by KTutorial.
     */
    virtual void setup(TutorialManager* tutorialManager) = 0;

    /**
     * Returns the main window of the application.
     *
     * @return The main window of the application.
     */
    virtual QWidget* mainApplicationWindow() = 0;

public Q_SLOTS:

    /**
     * Shows the tutorial UI.
     * This method is used by KTutorial to show the UI of a tutorial when tested
     * from the editor.
     *
     * Of course, it can be used when showing regular tutorials too, but it is
     * the KTutorialCustomization implementing class the one which must call
     * this method when needed, for example, connecting the slot to the
     * started(Tutorial*) signal of the TutorialManager.
     *
     * @param tutorial The tutorial to show its UI.
     */
    virtual void showTutorialUI(Tutorial* tutorial) = 0;

};

}

#endif
