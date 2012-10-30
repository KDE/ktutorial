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

#ifndef DIALOGRUNNER_H
#define DIALOGRUNNER_H

#include <QObject>
#include <QPointer>

class QDialog;
class QEventLoop;

/**
 * Convenience class to execute modal dialogs that can be hidden without ending
 * their execution.
 * DialogRunner just shows the given dialog as a modal dialog and executes an
 * event loop to block in the exec() method until the user closes the dialog or
 * the dialog is destroyed. The dialog can be hidden and shown again without
 * causing the exec method to return (in contrast with QDialog::exec() that
 * returns when the dialog is hidden).
 *
 * Once the event loop ends, the dialog is scheduled for deletion.
 */
class DialogRunner: public QObject {
Q_OBJECT
public:

    /**
     * Creates a new DialogRunner for the given dialog and with the given
     * parent.
     *
     * @param dialog The dialog to execute.
     * @param parent The parent object of this DialogRunner.
     */
    explicit DialogRunner(QDialog* dialog, //krazy:exclude=qclasses
                          QObject* parent = 0);

    /**
     * Shows the dialog as a modal dialog, blocking until the user closes it.
     */
    int exec();

private:

    /**
     * The dialog to execute.
     */
    QPointer<QDialog> mDialog;

    /**
     * The event loop to block exec method in.
     */
    QEventLoop* mEventLoop;

private Q_SLOTS:

    /**
     * Exits the event loop.
     * When the event loop exits, exec method is no longer blocked and returns.
     */
    void exitEventLoop();

};

#endif
