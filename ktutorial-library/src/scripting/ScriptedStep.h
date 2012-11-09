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

#ifndef KTUTORIAL_SCRIPTING_SCRIPTEDSTEP_H
#define KTUTORIAL_SCRIPTING_SCRIPTEDSTEP_H

#include <QHash>

#include "../Step.h"

namespace Kross {
class Action;
}

namespace ktutorial {
namespace scripting {

/**
 * Step to be used in scripts.
 * It is just a special subclass of Step that makes possible to perform actions
 * when the Step is activated or deactivated connecting to a signal instead of
 * redefining C++ methods (as that can't be done in scripts). It also makes
 * possible to execute functions in the script when a WaitFor waiting ends or an
 * Option is selected.
 *
 * If something must be performed when a Step is activated, implement a function
 * in the script and connect it to setup(QObject*) signal. If something must be
 * performed when a Step is deactivated, do the same with tearDown(QObject*)
 * signal instead. Note that, although the argument in those signals is declared
 * as QObject*, it is in fact the ScriptedStep that emitted the signals. It is a
 * Kross limitation.
 *
 * If a function of the script must be executed when an Option is selected or
 * when the waiting of a WaitFor ends, just use addOption(Option*, QObject*,
 * const QString&) or addWaitFor(WaitFor*, QObject*, const QString&) like you
 * would do with a normal slot, but using "self" as the receiver and the
 * function name (with or without parenthesis, at your option) as the slot.
 */
class ScriptedStep: public Step {
Q_OBJECT
public:

    /**
     * Creates a new ScriptedStep with the given identifier.
     *
     * @param id The identifier of this Step.
     */
    explicit ScriptedStep(const QString& id);

signals:

    /**
     * Emitted when this ScriptedStep has to be set up.
     *
     * @param scriptedStep A pointer to this ScriptedStep.
     * @see Step::setup()
     * @todo Change argument type to scripting::ScriptedStep* once all major
     *       Kross scripting backends support classes wrapped through
     *       WrapperInterface
     */
    void setup(QObject* scriptedStep);

    /**
     * Emitted when this ScriptedStep has to be tear down.
     *
     * @param scriptedStep A pointer to this ScriptedStep.
     * @see Step::tearDown()
     * @todo Change argument type to scripting::ScriptedStep* once all major
     *       Kross scripting backends support classes wrapped through
     *       WrapperInterface
     */
    void tearDown(QObject* scriptedStep);

protected:

    /**
     * Emits setup(scripting::ScriptedStep*) signal using this ScriptedStep as
     * parameter.
     */
    virtual void setup();

    /**
     * Emits tearDown(scripting::ScriptedStep*) signal using this ScriptedStep
     * as parameter.
     */
    virtual void tearDown();

    /**
     * Connects the given WaitFor with the slot in the receiver.
     * If the receiver is a Kross::Action, the slot must be the name of a
     * callable function (with or without parenthesis) instead of a true slot.
     * When the WaitFor::waitEnded(WaitFor*) signal is emitted, the function
     * will be called (thus behaving like a slot).
     *
     * If the receiver is not a Kross::Action, the parent method is called.
     *
     * @param waitFor The WaitFor to connect.
     * @param receiver The object to connect to.
     * @param slot The slot to connect to.
     */
    virtual void connectWaitFor(WaitFor* waitFor, QObject* receiver,
                                const QString& slot);

    /**
     * Disconnects the given WaitFor.
     * If the WaitFor was connected to a Kross::Action function, the function
     * will be "disconnected" from the WaitFor.
     *
     * If the WaitFor was not connected to a Kross::Action function, the parent
     * method is called.
     *
     * @param waitFor The WaitFor to disconnect.
     * @see connectWaitFor(WaitFor*, QObject*, const QString&)
     */
    virtual void disconnectWaitFor(WaitFor* waitFor);

private:

    /**
     * The Kross::Action that contains the functions to be called.
     * All the functions must belong to the same Kross::Action.
     */
    Kross::Action* mScriptAction;

    /**
     * Maps the WaitFors with the name of the functions to call when the waiting
     * ends.
     */
    QHash<WaitFor*, QString> mFunctions;

private slots:

    /**
     * Calls the function associated with the given WaitFor.
     *
     * @param waitFor The WaitFor.
     */
    void executeScriptFunctionFor(WaitFor* waitFor);

};

}
}

#endif
