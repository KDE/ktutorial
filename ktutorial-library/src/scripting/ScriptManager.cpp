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

#include "ScriptManager.h"

#include <QDir>

#include <KDebug>
#include <KStandardDirs>

#include "ScriptedTutorial.h"
#include "../TutorialManager.h"

namespace ktutorial {
extern int debugArea();
}

namespace ktutorial {
namespace scripting {

//public:

ScriptManager::ScriptManager() {
}

ScriptManager::~ScriptManager() {
}

void ScriptManager::loadTutorials(TutorialManager* tutorialManager) {
    Q_ASSERT(tutorialManager);

    QStringList baseDirectories = KGlobal::dirs()->resourceDirs("appdata");
    if (baseDirectories.count() == 0) {
        kWarning(debugArea()) << "No directories found for \"appdata\""
                              << "resource. Tutorials can't be loaded";
        return;
    }

    for (int i=0; i<baseDirectories.count(); ++i) {
        loadTutorialsFromDirectory(tutorialManager,
                                   baseDirectories[i] + "tutorials/");
    }
}

//private:

void ScriptManager::loadTutorialsFromDirectory(TutorialManager* tutorialManager,
                                               const QString& directory) {
    QStringList entries = QDir(directory).entryList();
    for (int i=0; i<entries.count(); ++i) {
        QString fileName = directory + entries[i];
        ScriptedTutorial* scriptedTutorial = new ScriptedTutorial(fileName);

        if (scriptedTutorial->isValid()) {
            tutorialManager->registerTutorial(scriptedTutorial);
        } else {
            delete scriptedTutorial;
        }
    }
}

}
}
