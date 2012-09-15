/***************************************************************************
 *   Copyright (C) 2010 by Daniel Calviño Sánchez <danxuliu@gmail.com>     *
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

#ifndef FILEACTIONS_H
#define FILEACTIONS_H

#include <QObject>

#include <KUrl>

class KRecentFilesAction;
class KTutorialEditor;

/**
 * File related actions.
 * FileActions provide the actions that create a new tutorial, open an existing
 * tutorial, open one of the recently opened tutorials, save the tutorial or
 * export the tutorial.
 *
 * Note, however, that it does not provide a quit action (as it has more to do
 * with the application as a whole than with the files).
 *
 * FileActions provides also the URL of the current tutorial, and a way to check
 * whether it can be closed or not, warning the user if needed.
 *
 * FileActions works closely with KTutorialEditor. KTutorialEditor is used to
 * know the tutorial and whether it is clean or not. FileActions provides the
 * tutorial URL and methods to create a new tutorial, load a tutorial from an
 * URL and check whether the current tutorial was modified after the last time
 * it was saved or not.
 *
 * The KTutorialEditor window is also used as the parent for every dialog shown
 * by the actions.
 */
class FileActions: public QObject {
Q_OBJECT
public:

    /**
     * Creates a new FileActions for the given KTutorialEditor.
     * All the actions are set up and added to the KTutorialEditor.
     *
     * @param tutorialEditor The KTutorialEditor to work with.
     */
    explicit FileActions(KTutorialEditor* tutorialEditor);

    /**
     * Destroys this FileActions.
     * The entries in the recent files action are saved in the configuration.
     */
    virtual ~FileActions();

    /**
     * Returns the tutorial URL.
     *
     * @return The tutorial URL.
     */
    const KUrl& tutorialUrl() const;

    /**
     * Checks whether the current tutorial can be closed or not.
     * If the tutorial is not clean, the user is asked if it has to be saved or
     * not, or if the close operation should be cancelled.
     *
     * @return True if the tutorial can be closed, false otherwise.
     */
    bool queryCloseTutorial();

public Q_SLOTS:

    /**
     * Loads the tutorial to be edited from the given URL.
     * The tutorial URL is updated and added to the recent files action, and a
     * clean state is forced.
     * An error message is shown if the tutorial could not be opened.
     *
     * Nothing is done if the current tutorial, if any, should not be closed.
     *
     * @param url The URL to load the tutorial from.
     */
    void loadTutorialFromUrl(KUrl url);

    /**
     * Creates a new empty tutorial replacing the current one, if any.
     * The tutorial URL is cleared.
     *
     * Nothing is done if the current tutorial, if any, should not be closed.
     */
    void newTutorial();

    /**
     * Enables or disables Save action based on the clean state of the stack.
     * When the stack is clean and the tutorial has an associated URL, the Save
     * action is disabled. Otherwise, it is enabled, so if there is no
     * associated URL the Save action is always kept enabled, even if the stack
     * is clean.
     *
     * @param clean Whether the undo stack entered clean state or not.
     */
    void handleUndoStackCleanChanged(bool clean);

private:

    /**
     * The KTutorialEditor to work with.
     */
    KTutorialEditor* mTutorialEditor;

    /**
     * The URL to save the tutorial to.
     */
    KUrl mTutorialUrl;

    /**
     * True if the tutorial was not modified since the last time it was saved,
     * false otherwise.
     */
    bool mClean;

    /**
     * The "Open Recent" action.
     */
    KRecentFilesAction* mRecentFilesAction;

    /**
     * Sets up all the file related actions.
     */
    void setupActions();

private Q_SLOTS:

    /**
     * Shows a KFileDialog to select the file to open the tutorial from.
     * The tutorial is loaded from the URL selected by the user.
     */
    void openTutorial();

    /**
     * Saves the tutorial to the tutorial URL.
     * A clean state is set. If there is no tutorial URL it behaves like
     * saveTutorialAs().
     * An error message is shown if the tutorial could not be saved.
     *
     * @return True if the tutorial was successfully saved, false otherwise.
     */
    bool saveTutorial();

    /**
     * Shows a KFileDialog to select the file to save the tutorial to.
     * The tutorial URL is updated and added to the recent files action, and a
     * clean state is forced.
     * An error message is shown if the tutorial could not be saved.
     *
     * @return True if the tutorial was successfully saved, false otherwise.
     */
    bool saveTutorialAs();

    /**
     * Shows a KFileDialog to select the file to save the exported tutorial in.
     * An error message is shown if the tutorial could not be saved.
     */
    void exportTutorial();

};

#endif
