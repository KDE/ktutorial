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

#ifndef REMOTEOBJECTCHOOSER_H
#define REMOTEOBJECTCHOOSER_H

#include <QWidget>

class RemoteObject;

namespace Ui {
class RemoteObjectChooser;
}

/**
 * Widget to choose a remote object from all the available remote objects in the
 * target application.
 * The RemoteObjectChooser shows a window with a list containing all the objects
 * that can be accesed by KTutorial in the target application. The target
 * application must be running, so it is started when the RemoteObjectChooser is
 * created. If the target application is closed, the RemoteObjectChooser is also
 * closed (after notifying the user).
 *
 * When the target application is successfully started, the RemoteObjectChooser
 * hides all its parent dialogs and windows. Only the RemoteObjectChooser itself
 * is kept shown to avoid the rest of windows of KTutorial editor to get in the
 * way of the user when he interacts with the target application. Before hiding
 * the windows an information message is shown to the user explaining this.
 *
 * When the user selects a remote object in the list and that object represents
 * a widget, the widget is highlighted in the target application.
 *
 * The list shows all the available remote objects in the target application.
 * Some of them may not be useful at all (for example, those without name), so
 * the user can select some filters to narrow the remote objects shown in the
 * list hiding those that do not pass the filters.
 *
 * The window contains an "Ok" and a "Cancel" button, like the ones found in
 * dialogs. A window is used instead of a dialog to be shown in the task bar (as
 * after hidding the rest of KTutorial editor windows no entry would be shown if
 * this were a dialog). When there is a remote object selected and the "Ok"
 * button is clicked, signal remoteObjectChosen(RemoteObject*) is emitted.
 *
 * The RemoteObjectChooser deletes itself once it has been closed, no matter the
 * reason why.
 */
class RemoteObjectChooser: public QWidget {
Q_OBJECT
public:

    /**
     * Creates a new RemoteObjectChooser with the given parent.
     * The target application is started.
     *
     * @param parent The parent QWidget.
     */
    explicit RemoteObjectChooser(QWidget* parent = 0);

    /**
     * Destroys this RemoteObjectChooser.
     * The highlighting in the current remote object is stopped, if any.
     */
    virtual ~RemoteObjectChooser();

Q_SIGNALS:

    /**
     * Emitted when the user chooses a RemoteObject.
     *
     * @param remoteObject The chosen RemoteObject.
     */
    void remoteObjectChosen(RemoteObject* remoteObject);

protected:

    /**
     * Executes KDialog handler, shows again the parent windows and schedules
     * this RemoteObjectChooser for deletion.
     * close() can't be redefined to restore the parent windows, as it doesn't
     * get a close triggered by the window manager (like using ALT+F4) (and it
     * is not even a virtual method). Moreover, close() itself sends a
     * QCloseEvent, which is handled by this method.
     * Likely, Qt::WA_DeleteOnClose isn't used as it will delete this
     * RemoteObjectChooser only through close(), but not through a QCloseEvent.
     * Instead, this RemoteObjectChooser is scheduled for deletion after
     * receiving a QCloseEvent.
     *
     * @param event The QCloseEvent.
     */
    virtual void closeEvent(QCloseEvent* event);

private:

    /**
     * The Ui Designer generated class.
     */
    Ui::RemoteObjectChooser* ui;

    /**
     * The RemoteObject currently selected in the list.
     */
    RemoteObject* mCurrentRemoteObject;

    /**
     * True if the target application has been successfully started, false
     * otherwise.
     */
    bool mSuccessfullyStarted;

    /**
     * Hide all the parent widgets of the given widget that are windows or
     * dialogs.
     *
     * @param widget The widget to hide its parents.
     */
    void hideParentWindows(QWidget* widget);

    /**
     * Shows all the parent widgets of the given widget that are windows or
     * dialogs.
     *
     * @param widget The widget to show its parents.
     */
    void showParentWindows(QWidget* widget);

    /**
     * A warning is shown to the user explaining that the target application
     * must be running and this RemoteObjectChooser is closed.
     */
    void warnAboutFinishedTargetApplicationBeforeClosing();

private Q_SLOTS:

    /**
     * Shows the information message about hiding the parent windows, hides the
     * parent windows and dialogs, and sets up the models for the tree view.
     */
    void handleTargetApplicationStarted();

    /**
     * If the target application was finished after starting successfully, a
     * warning is shown to the user and this RemoteObjectChooser is closed.
     */
    void handleTargetApplicationFinished();

    /**
     * Sets the current remote object.
     * The current remote object is highlighted, and the highlighting in the
     * previous one is stopped.
     *
     * @param remoteObject The current remote object to set.
     */
    void setCurrentRemoteObject(RemoteObject* remoteObject);

    /**
     * Emits remoteObjectChosen(RemoteObject*) and closes this
     * RemoteObjectChooser.
     */
    void accept();

    /**
     * Closes this RemoteObjectChooser.
     */
    void cancel();

};

#endif
