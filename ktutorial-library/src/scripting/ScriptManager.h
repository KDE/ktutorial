/***************************************************************************
 *   Copyright (C) 2009 by Daniel Calviño Sánchez <danxuliu@gmail.com>     *
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

#ifndef KTUTORIAL_SCRIPTING_SCRIPTMANAGER_H
#define KTUTORIAL_SCRIPTING_SCRIPTMANAGER_H

class QString;

namespace ktutorial {
class TutorialManager;
}

namespace ktutorial {
namespace scripting {

/**
 * Manager for scripted tutorials.
 * The ScriptManager task is loading ScriptedTutorials from their script. The
 * scripts containing tutorials are stored in "tutorials" subdirectory of each
 * application standard directories. The ScriptManager can load all the valid
 * tutorials for the current KApplication in a TutorialManager using
 * loadTutorials(TutorialManager*).
 *
 * @see KStandardDirs
 */
class ScriptManager {
public:

    /**
     * Creates a new ScriptManager.
     */
    ScriptManager();

    /**
     * Destroys this ScriptManager.
     */
    virtual ~ScriptManager();

    /**
     * Loads all the valid scripted tutorials from the application standard
     * directories.
     *
     * @param tutorialManager The TutorialManager to load the tutorials into.
     */
    void loadTutorials(TutorialManager* tutorialManager);

private:

    /**
     * Loads all the valid scripted tutorials from the given directory.
     *
     * @param tutorialManager The TutorialManager to load the tutorials into.
     * @param directory The name of the directory to load the tutorials from.
     */
    void loadTutorialsFromDirectory(TutorialManager* tutorialManager,
                                    const QString& directory);

};

}
}

#endif
