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

#include "ScriptedTutorial.h"

#include <kross/core/action.h>

#include "ScriptingModule.h"
#include "../TutorialInformation.h"

namespace ktutorial {
namespace scripting {

ScriptedTutorial::ScriptedTutorial(const QString& filename): 
    Tutorial(new TutorialInformation(filename)) {

    mScriptAction = new Kross::Action(this, filename);

    if (mScriptAction->setFile(filename)) {
        mScriptAction->addObject(this, "tutorial");
        mScriptAction->addObject(ScriptingModule::self(), "ktutorial");
        mScriptAction->trigger();
        mValid = !mScriptAction->hadError();
    } else {
        mValid = false;
    }
}

ScriptedTutorial::~ScriptedTutorial() {
}

QObject* ScriptedTutorial::tutorialInformationAsObject() const {
    return tutorialInformation();
}

bool ScriptedTutorial::isValid() const {
    return mValid;
}

//protected:

void ScriptedTutorial::setup() {
    emit setup(this);
}

void ScriptedTutorial::tearDown() {
    emit tearDown(this);
}

}
}
