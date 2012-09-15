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

#ifndef TARGETAPPLICATIONVIEW_H
#define TARGETAPPLICATIONVIEW_H

#include <QObject>

#include "../targetapplication/TargetApplication.h"

/**
 * Convenience class to interact with the user to start a target application.
 * It should be used whenever a target application has to be started from the
 * GUI.
 */
class TargetApplicationView: public QObject {
Q_OBJECT
public:

    /**
     * Creates a new TargetApplicationView for the given TargetApplication.
     *
     * @param targetApplication The target application to wrap.
     * @param parent The parent QWidget for the dialogs and message boxes.
     */
    explicit TargetApplicationView(TargetApplication* targetApplication,
                                   QWidget* parent = 0);

    /**
     * Convenience method to start the target application.
     * It asks for the executable to be executed if the target application does
     * not have one set yet. When the application is started, the cursor is set
     * to the busy shape until the target application has started or failed to
     * start.
     * If the application is already running nothing is done.
     */
    void start();

private:

    /**
     * The TargetApplication to provide interaction for.
     */
    TargetApplication* mTargetApplication;

    /**
     * The parent for dialogs and message boxes.
     */
    QWidget* mParent;

    /**
     * Asks the user for the target application executable.
     *
     * @return The full path to the executable.
     */
    QString askApplicationFilePath();

    /**
     * Connects to the TargetApplication signals and sets the cursor to the busy
     * shape.
     */
    void startExpectingTheTargetApplicationToStart();

private Q_SLOTS:

    /**
     * Shows a message box for the given error.
     * The user is asked whether he wants to set a new executable or not.
     *
     * @param error The TargetApplication error.
     */
    void showErrorMessage(TargetApplication::Error error);

    /**
     * Disconnects from the TargetApplication signals and restores the normal
     * cursor.
     */
    void stopExpectingTheTargetApplicationToStart();

};

#endif
