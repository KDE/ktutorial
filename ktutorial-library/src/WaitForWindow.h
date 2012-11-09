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

#ifndef KTUTORIAL_WAITFORWINDOW_H
#define KTUTORIAL_WAITFORWINDOW_H

#include "ktutorial_export.h"

#include "WaitFor.h"

namespace ktutorial {

/**
 * Waits for a specific window to be shown.
 * When the window with the expected object name is shown and the WaitForEvent
 * is active, the wait ends.
 *
 * Note that if the window is shown while the WaitFor isn't active, it won't
 * be registered and the condition won't be met. In order to met the condition,
 * the window must be shown while the WaitForEvent is active.
 *
 * The term "window" is used here in a general sense: WaitForWindow can wait
 * either for true windows or for dialogs. In fact, WaitForWindow must be used
 * whenever there is a modal dialog; waiting for the user to click a button that
 * causes a modal dialog to appear will not work, as the modal dialog will
 * halt further processing of the clicked() signal until it is closed (if the
 * modal dialog was created in a slot connected to the clicked() signal, which
 * is very likely).
 */
class KTUTORIAL_EXPORT WaitForWindow: public WaitFor {
Q_OBJECT
public:

    /**
     * Creates a new WaitForWindow.
     * This constructor is needed to dynamically create WaitForWindow objects in
     * scripts using ScriptingModule::newWaitFor(const QString&). Method
     * setWindowObjectName(const QString&) must be called to finish setting up
     * the object. For C++ tutorials, use WaitForWindow(const QString&)
     * constructor instead of this one.
     */
    Q_INVOKABLE WaitForWindow();

    /**
     * Creates a new WaitForWindow.
     * Note that the name is the object name, not the window title.
     *
     * @param windowObjectName The object name of the window to wait for.
     */
    explicit WaitForWindow(const QString& windowObjectName);

    /**
     * Sets the object name of the window to wait for.
     * Note that the name is the object name, not the window title.
     * This method can be invoked from a script.
     *
     * In fact, you should only invoke this method from a script, and only once,
     * to set up the object. For C++ tutorials, use
     * WaitForWindow(const QString&) constructor when creating this
     * WaitForWindow.
     *
     * @param windowObjectName The object name of the window to wait for.
     */
    Q_INVOKABLE void setWindowObjectName(const QString& windowObjectName);

    /**
     * Returns true if the window was shown while active, false otherwise.
     *
     * @return True if the window was shown while active, false otherwise.
     */
    virtual bool conditionMet() const;

    /**
     * Sets this WaitForWindow active or inactive.
     * Activating it resets its condition.
     *
     * @param active True to set it active, false otherwise.
     */
    virtual void setActive(bool active);

private:

    /**
     * Whether the window with the expected object name was shown when active or
     * not.
     */
    bool mConditionMet;

    /**
     * The object name of the window to wait for.
     */
    QString mWindowObjectName;

private Q_SLOTS:

    /**
     * Checks whether the window that has been shown is the expected one.
     * If the WaitFor is active and the object name of the window is the
     * expected one, the condition is met and the wait ended.
     *
     * @param window A window that has been shown.
     */
    void checkWindowShown(QWidget* window);

};

}

#endif
