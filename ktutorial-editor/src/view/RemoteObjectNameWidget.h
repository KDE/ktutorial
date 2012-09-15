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

#ifndef REMOTEOBJECTNAMEWIDGET_H
#define REMOTEOBJECTNAMEWIDGET_H

#include <QPointer>
#include <QWidget>

class RemoteObject;
class RemoteObjectNameRegister;

namespace Ui {
class RemoteObjectNameWidget;
}

/**
 * Widget to choose the name of a RemoteObject.
 * The widget is composed by a line edit and a button to show a
 * RemoteObjectChooser. The name can be set directly in the line edit or with
 * the aid of the RemoteObjectChooser.
 *
 * The line edit provides text completion for the names of the remote objects
 * in the target application. If there is no target application running, no
 * completion is provided. As soon as it starts, the completion is enabled.
 * When the completion matches contain a name that represents several remote
 * objects that match is replaced by the best name of each remote object.
 *
 * When a name is set in the line edit the signal
 * remoteObjectChosen(RemoteObject*) is emitted.
 *
 * To provide name completion, name substring completion, setting the name based
 * on the RemoteObject chosen or emitting the signal
 * remoteObjectChosen(RemoteObject*) based on the current name, the widget uses
 * a register with the names of the remote objects. During the updates of this
 * register it should not be queried (as the information provided will not be
 * accurate), so if any of the aforementioned operations is requested during a
 * register update, the operation is queued and executed once the update
 * finishes.The cursor shape is changed to busy during register updates to
 * reflect this fact.
 */
class RemoteObjectNameWidget: public QWidget {
Q_OBJECT
public:

    /**
     * Creates a new RemoteObjectNameWidget.
     *
     * @param parent The parent QWidget.
     */
    explicit RemoteObjectNameWidget(QWidget* parent = 0);

    /**
     * Destroys this widget.
     */
    virtual ~RemoteObjectNameWidget();

    /**
     * Returns the chosen remote object name.
     *
     * @return The chosen remote object name.
     */
    QString name() const;

    /**
     * Sets the chosen remote object name.
     *
     * @param name The chosen remote object name.
     */
    void setName(const QString& name);

Q_SIGNALS:

    /**
     * Emitted when the name of a remote object is set.
     * It does not matter how the name was set (using setName(QString), written
     * by the user or chosen from the RemoteObjectChooser).
     * If there is no remote object with the name set or the target application
     * is not running, the parameter of this signal is a null pointer.
     *
     * @param remoteObject The RemoteObject with the name set.
     */
    void remoteObjectChosen(RemoteObject* remoteObject);

private:

    /**
     * The register that provides the names of the remote objects and finds them
     * based on their name.
     */
    RemoteObjectNameRegister* mRemoteObjectNameRegister;

    /**
     * The Ui Designer generated class.
     */
    Ui::RemoteObjectNameWidget* ui;

    /**
     * Whether the remote object was chosen while the register was being updated
     * or not.
     */
    bool mIsRemoteObjectChosenPending;

    /**
     * The last remote object chosen while the register was being updated, if
     * any.
     * A guarded pointer is needed, as the RemoteObject may be deleted if the
     * target application is closed before finishing the update of the names.
     */
    QPointer<RemoteObject> mPendingRemoteObjectChosen;

    /**
     * Whether the name was set (programatically or by the user) while the
     * register was being updated or not.
     */
    bool mIsNameChangedPending;

    /**
     * Whether the name completion was triggered while the register was being
     * updated or not.
     */
    bool mIsCompletionPending;

    /**
     * Whether the name substring completion was triggered while the register
     * was being updated or not.
     */
    bool mIsSubstringCompletionPending;

    /**
     * The cursor shape used by default by the object name line edit.
     */
    Qt::CursorShape mObjectNameLineEditCursorShape;

private Q_SLOTS:

    /**
     * Creates and shows a new RemoteObjectChooser.
     */
    void showRemoteObjectChooser();

    /**
     * Sets the object name based in the chosen remote object.
     * If the name register is being updated, nothing will be done, but marking
     * setting the chosen remote object as a pending operation.
     * If the given remote object is null no name is set.
     * If the given remote object has no unique name, no name is set and a
     * message box is shown to inform the user that the target application must
     * be modified in order to be able to use that object in a tutorial.
     *
     * @param remoteObject The chosen RemoteObject.
     */
    void setChosenRemoteObject(RemoteObject* remoteObject);

    /**
     * Emits remoteObjectChosen(RemoteObject*) with the remote object found with
     * the given name (which can include ancestor names).
     * If the name register is being updated, nothing will be done, but marking
     * handling the name change as a pending operation.
     *
     * @param name The name set.
     */
    void handleNameChanged(const QString& name);

    /**
     * If the name register is being updated, marks the name completion as a
     * pending operation.
     */
    void handleCompletion();

    /**
     * If the name register is being updated, marks the name substring
     * completion as a pending operation.
     */
    void handleSubstringCompletion();

    /**
     * Enters the name register update state.
     * Changes the cursor to busy and disables the completion.
     */
    void startNameUpdate();

    /**
     * Exists the name register update state.
     * Executes all the pending operations, enables again the completion and
     * restores the cursor.
     */
    void finishNameUpdate();

};

#endif
