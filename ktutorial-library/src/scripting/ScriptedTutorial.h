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

#ifndef KTUTORIAL_SCRIPTING_SCRIPTEDTUTORIAL_H
#define KTUTORIAL_SCRIPTING_SCRIPTEDTUTORIAL_H

#include "../Tutorial.h"

namespace Kross {
class Action;
}

namespace ktutorial {
namespace scripting {

/**
 * Tutorial to be used in scripts.
 * This class acts as a bridge between scripts and tutorials. The tutorial is
 * written in a script, which is interpreted by the ScriptedTutorial through
 * Kross. The script is executed when the ScriptedTutorial is created.
 *
 * Not every script file is valid, as the filename may be wrong, it could be
 * written in a language not manageable by current Kross environment, or it may
 * contain errors. In those cases, a ScriptedTutorial is useless. Method
 * isValid() can be used to check whether the ScriptedTutorial is valid or not.
 *
 * The ScriptedTutorial is exposed to the script as "tutorial" object. The
 * ScriptingModule is exposed to the script as "ktutorial" object.
 *
 * If something must be set before starting the Tutorial, implement a function
 * in the script and connect it to setup(QObject*) signal. Note that, although
 * the argument in the signal is declared as QObject*, it is in fact the
 * ScriptedStep that emitted the signal. It is a Kross limitation.
 *
 * You can do the same if something has to be cleaned after the Tutorial
 * finishes using tearDown(QObject*) signal.
 */
class ScriptedTutorial: public Tutorial {
Q_OBJECT
public:

    /**
     * Creates a new ScriptedTutorial for the given filename.
     * The id of its TutorialInformation is set to the filename.
     *
     * @param filename The name of the file containing the script.
     */
    ScriptedTutorial(const QString& filename);

    /**
     * Destroys this ScriptedTutorial.
     */
    virtual ~ScriptedTutorial();

    /**
     * Returns the TutorialInformation of this Tutorial.
     * This method can be invoked from a script.
     *
     * This is a temporal method to be used instead of
     * Tutorial::tutorialInformation().
     *
     * @return The TutorialInformation of this Tutorial.
     * @todo Remove this method once all major Kross scripting backends support
     *       classes wrapped through WrapperInterface
     */
    Q_INVOKABLE QObject* tutorialInformationAsObject() const;

    /**
     * Whether the file used in the constructor contained a valid script or not.
     * A file is valid if it exists, contains an script written in a language
     * that can be managed by the current Kross environment and the script has
     * no errors. That is, if the script could be executed.
     *
     * @return True if the file is valid, false otherwise.
     */
    bool isValid() const;

signals:

    /**
     * Emitted when this ScriptedTutorial has to be set up.
     *
     * @param scriptedTutorial A pointer to this ScriptedTutorial.
     * @see Tutorial::setup()
     * @todo Change argument type to scripting::ScriptedTutorial* once all major
     *       Kross scripting backends support classes wrapped through
     *       WrapperInterface
     */
    void setup(QObject* scriptedTutorial);

    /**
     * Emitted when this ScriptedTutorial has to be cleaned.
     *
     * @param scriptedTutorial A pointer to this ScriptedTutorial.
     * @see Tutorial::tearDown()
     * @todo Change argument type to scripting::ScriptedTutorial* once all major
     *       Kross scripting backends support classes wrapped through
     *       WrapperInterface
     */
    void tearDown(QObject* scriptedTutorial);

protected:

    /**
     * Emits setup(scripting::ScriptedTutorial*) signal using this
     * ScriptedTutorial as parameter.
     */
    virtual void setup();

    /**
     * Emits tearDown(scripting::ScriptedTutorial*) signal using this
     * ScriptedTutorial as parameter.
     */
    virtual void tearDown();

private:

    /**
     * The Kross action that manages and executes the script.
     */
    Kross::Action* mScriptAction;

    /**
     * Whether the script is valid or not.
     */
    bool mValid;

};

}
}

#endif
