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

#include "ScriptedStep.h"

#include <kross/core/action.h>

#include "../WaitFor.h"

namespace ktutorial {
namespace scripting {

ScriptedStep::ScriptedStep(const QString& id): Step(id),
    mScriptAction(0) {
}

//protected:

void ScriptedStep::setup() {
    emit setup(this);
}

void ScriptedStep::tearDown() {
    emit tearDown(this);
}

void ScriptedStep::connectWaitFor(WaitFor* waitFor, QObject* receiver,
                          const QString& slot) {
    if (!qobject_cast<Kross::Action*>(receiver)) {
        Step::connectWaitFor(waitFor, receiver, slot);
        return;
    }

    Kross::Action* scriptAction = static_cast<Kross::Action*>(receiver);
    if (mScriptAction == 0) {
        mScriptAction = scriptAction;
    }
    //Check that the previously set Kross::Action is the same as the current one
    Q_ASSERT(mScriptAction == scriptAction);

    QString functionName = slot;
    int index = functionName.indexOf('(');
    if (index > -1) {
        functionName = functionName.left(index);
    }

    mFunctions.insert(waitFor, functionName);

    connect(waitFor, SIGNAL(waitEnded(WaitFor*)),
            this, SLOT(executeScriptFunctionFor(WaitFor*)));
}

void ScriptedStep::disconnectWaitFor(WaitFor* waitFor) {
    if (!mFunctions.contains(waitFor)) {
        Step::disconnectWaitFor(waitFor);
        return;
    }

    mFunctions.remove(waitFor);

    disconnect(waitFor, SIGNAL(waitEnded(WaitFor*)));
}

//private:

void ScriptedStep::executeScriptFunctionFor(WaitFor* waitFor) {
    mScriptAction->callFunction(mFunctions.value(waitFor).toLatin1());
}

}
}
