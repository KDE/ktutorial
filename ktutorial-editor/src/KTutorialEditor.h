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

#ifndef KTUTORIALEDITOR_H
#define KTUTORIALEDITOR_H

#include <KXmlGuiWindow>

class KUrl;

class EditActions;
class FileActions;
class QTreeView;
class Tutorial;

#ifdef QT_QTDBUS_FOUND
class TestTutorialActions;
#endif

/**
 * KTutorial editor main window.
 * It wires up all the components in the application.
 */
class KTutorialEditor: public KXmlGuiWindow {
Q_OBJECT
public:

    /**
     * Creates a new KTutorialEditor.
     */
    KTutorialEditor();

    /**
     * Destroys this KTutorialEditor.
     * The target application is also killed if it is running.
     */
    virtual ~KTutorialEditor();

    /**
     * Returns the tutorial being edited.
     *
     * @return The tutorial being edited.
     */
    Tutorial* tutorial();

    /**
     * Sets the tutorial as clean (not modified after the last time it was
     * saved).
     */
    void setClean();

    /**
     * Loads the tutorial to be edited from the given URL.
     *
     * @param url The URL to load the tutorial from.
     * @see FileActions::loadTutorialFromUrl(KUrl)
     */
    void loadTutorialFromUrl(const KUrl& url);

    /**
     * Sets the tutorial to be edited.
     * It creates a new tutorial, prepares the tree view to represent it and
     * handles the selection of items.
     *
     * If the tutorial is null, a new empty tutorial is set.
     *
     * @param tutorial The tutorial to set.
     */
    void setTutorialToBeEdited(Tutorial* tutorial = 0);

Q_SIGNALS:

    /**
     * Emitted when the clean state changes.
     *
     * @param clean True if the tutorial is clean, false otherwise.
     */
    void cleanChanged(bool clean);

protected:

    /**
     * Called before the window is closed, either by the user or indirectly by
     * the session manager.
     * It checks whether the tutorial can be closed or not.
     *
     * Reimplemented from KMainWindow::queryClose().
     *
     * @return True if the window can be closed, false otherwise.
     */
    virtual bool queryClose();

    /**
     * Restores the session based on the saved state.
     * If "TutorialURL" is not empty, the tutorial is loaded from that URL.
     *
     * Reimplemented from KMainWindow::readProperties(const KConfigGroup&).
     *
     * @param configGroup The KConfigGroup to read the state from.
     */
    virtual void readProperties(const KConfigGroup& configGroup);

    /**
     * Saves the state to restore the session.
     * The tutorial URL is saved as "TutorialURL".
     *
     * Reimplemented from KMainWindow::saveProperties(KConfigGroup&).
     *
     * @param configGroup The KConfigGroup to save the state to.
     */
    virtual void saveProperties(KConfigGroup& configGroup);

private:

    /**
     * The main tree view that shows the tutorial.
     */
    QTreeView* mTreeView;

    /**
     * Dock with the actions to edit a tutorial.
     */
    QDockWidget* mTutorialActionDock;

    /**
     * Dock with the actions to edit a step.
     */
    QDockWidget* mStepActionDock;

    /**
     * Dock with the actions to edit a reaction.
     */
    QDockWidget* mReactionActionDock;

    /**
     * The file related actions and data.
     */
    FileActions* mFileActions;

    /**
     * The edit related actions and data.
     */
    EditActions* mEditActions;

#ifdef QT_QTDBUS_FOUND
    /**
     * The test tutorial related actions and data.
     */
    TestTutorialActions* mTestTutorialActions;
#endif

    /**
     * The tutorial being edited.
     */
    Tutorial* mTutorial;

    /**
     * Sets up the dock widgets.
     */
    void setupDocks();

    /**
     * Sets up all the actions used in the application.
     */
    void setupActions();

    /**
     * Sets up the widgets to show the actions in the docks.
     */
    void setupActionListWidgets();

    /**
     * Returns a caption (window title) string based on the tutorial URL.
     * The caption contains the URL, which is truncated if it is too lengthy. If
     * the URL is empty, "New file" is returned.
     *
     * @return A caption string based on the tutorial URL.
     */
    QString captionFromTutorialUrl();

private Q_SLOTS:

    /**
     * Modifies the caption based on the clean state of the stack.
     * When the stack is not clean, "[not saved]" is added after the caption
     * (the URL or "New file", depending on the case). Otherwise, the caption is
     * the URL associated to the tutorial.
     *
     * @param clean Whether the undo stack entered clean state or not.
     */
    void handleUndoStackCleanChanged(bool clean);

};

#endif
