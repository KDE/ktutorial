/***************************************************************************
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

#ifndef REMOTEOBJECTNAMEREGISTER_H
#define REMOTEOBJECTNAMEREGISTER_H

#include <QMultiHash>
#include <QObject>
#include <QPointer>

#include "../targetapplication/DBusException.h"

class RemoteObject;

/**
 * Provides information related to the names of the remote objects in the target
 * application.
 * RemoteObjectNameRegister can be used to know the names of all the remote
 * objects accessible by KTutorial in the target application, and it also
 * notifies the names of the remote objects added or removed in the target
 * application through the signals nameAdded(QString) and nameRemoved(QString).
 *
 * However, sometimes two or more remote objects may have the same name. In that
 * case, the names of one or more ancestors are required to differentiate
 * between those remote objects. The compound name that identifies a remote
 * object can be got using uniqueName(RemoteObject*). Also, given a name, the
 * remote object that it represents can be got using findObject(QString). An
 * unqualified name (without any ancestor names, just the name of the remote
 * object itself) can be used to get the best names of all the remote objects
 * represented by that unqualified name.
 *
 * The best name and the unique name of a remote object are tightly related.
 * When the name of a remote object is unique by itself, the best name of the
 * remote object is the name itself.
 * When there are other remote objects with the same name, but the remote object
 * has an ancestor with a unique name, and the remote object is the only
 * descendant of that ancestor with that name, its best name (which is also
 * unique) is the name of the ancestor and the name of the remote object. The
 * ancestor with a unique name used is the nearest one to the remote object
 * (parent preferred to grandparent, grandparent preferred to great-grandparent
 * and so on).
 * If there are more descendants of the ancestor with that name, or if there
 * is no ancestor with a unique name, the best name is composed by the name of
 * the remote object itself and the name of as many ancestors as needed to
 * obtain a unique name.
 * If there is no unique name for the remote object, the best name is composed
 * by the name of the remote object itself and the name of all of its ancestors
 * from the remote object to the deepest ancestor with a unique name, or all its
 * ancestors if none has a unique name.
 * A best name that is ambiguous, that is, identifies more than one remote
 * object, may become unique if the ambiguity solving rules from KTutorial are
 * taken into account.
 *
 * Note that a name and the remote object it represents depends on the state of
 * the target application. For example, if there is a dialog named
 * "Configuration dialog" with a button named "Ok button", its unique name could
 * be just "Ok button". However, if another dialog named "File information
 * dialog" were opened while the "Configuration dialog" was still opened, and
 * "File information dialog" had a also button called "Ok button", now the
 * unique name of the first button would be "Configuration dialog/Ok button". In
 * this situation, "Ok button" would not represent a specific remote object.
 * 
 * Also note that names are not registered when the objects are created in the
 * target application, but a slight delay is introduced instead. This prevents
 * registering objects too soon, before their name was even set in the target
 * application. While the update in the registered names is taking place the
 * methods described above should not be used. When a name update is started,
 * the signal nameUpdateStarted() is emitted; when the update is finished, the
 * signal nameUpdateFinished() is emitted. It can be checked if the register is
 * being updated or not using isBeingUpdated(). The "updating" state is only
 * enabled and the signals emitted when new objects are created; if an object is
 * deleted there is no delay in the update of the names.
 */
class RemoteObjectNameRegister: public QObject {
Q_OBJECT
public:

    /**
     * Creates a new RemoteObjectNameRegister.
     * Note that if the target application is already started when the
     * RemoteObjectNameRegister is created the RemoteObjects will be registered
     * and the nameUpdateStarted() signal will be emitted, even before being
     * able to connect to it. Thus, you may want to check whether the register
     * is being updated or not after the constructor to call the slot that will
     * be connected with nameUpdateStarted() signal.
     *
     * @param parent The parent QObject.
     */
    explicit RemoteObjectNameRegister(QObject* parent = 0);

    /**
     * Destroys this RemoteObjectNameRegister.
     * If necessary, disables the EventSpy used to update the remote object
     * names.
     */
    virtual ~RemoteObjectNameRegister();

    /**
     * Returns a list with all the names.
     *
     * @return A list with all the names.
     */
    QStringList names() const;

    /**
     * Returns a unique name for the given remote object.
     * If the best name of the given remote object is unique, or if it is not
     * unique, but the object found when applying the ambiguity solving rules is
     * the given remote object, the best name is returned.
     * Otherwise, there is no unique name for the given remote object, and an
     * empty string is returned instead.
     * 
     * @param remoteObject The remote object to get its unique name.
     * @return A unique name for the given remote object, if any.
     * @throws DBusException If a DBus error happens.
     */
    QString uniqueName(RemoteObject* remoteObject) const throw (DBusException);

    /**
     * Returns the best names of the remote objects that have the given name.
     * The given name can not contain any ancestor names, and it must be a
     * registered name.
     *
     * @param name The name of the remote objects to get their best names.
     * @return The best names of the remote objects that have the given name.
     * @throws DBusException If a DBus error happens.
     */
    QStringList bestNames(const QString& name) const throw (DBusException);

    /**
     * Returns the remote object with the given name, if any.
     * The editor support module in the target application is queried to find
     * the object with the given name.
     *
     * @param name The name of the remote object to find.
     * @return The remote object with the specified name, or a null pointer if
     *         there is none.
     */
    RemoteObject* findRemoteObject(const QString& name) const
    throw (DBusException);

    /**
     * Returns whether the names are being registered or not.
     * 
     * @return True if the names are being registered, false otherwise.
     */
    bool isBeingUpdated() const;

Q_SIGNALS:

    /**
     * Emitted when the name of a remote object is added to the register.
     *
     * @param name The name added.
     */
    void nameAdded(const QString& name);

    /**
     * Emitted when the name of a remote object is removed from the register.
     *
     * @param name The name removed.
     */
    void nameRemoved(const QString& name);

    /**
     * Emitted when one or more remote objects are created in the target
     * application, and thus queued for their names to be registered.
     * Note that this signal can be emitted several times before
     * nameUpdateFinished() signal is emitted.
     */
    void nameUpdateStarted();
    
    /**
     * Emitted when there are no more remote objects pending to be registered.
     * Note that this signal may be emitted just once after several
     * nameUpdateStarted() signals are emitted.
     */
    void nameUpdateFinished();

private:

    /**
     * The RemoteObjects with a name indexed by their name.
     */
    QMultiHash<QString, RemoteObject*> mRemoteObjectForName;

    /**
     * The known RemoteObjects (with and without name) indexed by their parent.
     */
    QMultiHash<RemoteObject*, RemoteObject*> mRemoteObjectForParent;

    /**
     * Whether the names are being registered or not.
     */
    bool mIsBeingUpdated;

    /**
     * The list of RemoteObjects queued to register their name.
     * A guarded pointer is needed, as the RemoteObjects may be deleted if the
     * target application is closed before finishing the update of the names.
     */
    QList< QPointer<RemoteObject> > mRemoteObjectsPendingNameRegister;

    /**
     * Registers the given RemoteObject and all its children.
     * The name itself is not registered yet, but queued to be registered after
     * a little time.
     *
     * @param remoteObject The RemoteObject to register.
     * @param parent The parent of the RemoteObject to register.
     * @throws DBusException If a DBus error happens.
     */
    void registerRemoteObject(RemoteObject* remoteObject, RemoteObject* parent)
    throw (DBusException);

    /**
     * Deregisters the given RemoteObject and all its children.
     *
     * @param remoteObject The RemoteObject to deregister.
     * @param parent The parent of the RemoteObject to deregister.
     * @throws DBusException If a DBus error happens.
     */
    void deregisterRemoteObject(RemoteObject* remoteObject,
                                RemoteObject* parent)
    throw (DBusException);

    /**
     * Returns the best name for the given remote object.
     *
     * @param remoteObject The remote object to get its best name.
     * @return The best name of the given remote object.
     * @throws DBusException If a DBus error happens.
     */
    QString bestName(RemoteObject* remoteObject) const
    throw (DBusException);

    /**
     * Returns the best name for the given remote object as a reversed path.
     *
     * @param remoteObject The remote object to get its best name.
     * @return The reversed path for the best name of the given remote object.
     * @throws DBusException If a DBus error happens.
     */
    QStringList bestNameAsReversedPath(RemoteObject* remoteObject) const
    throw (DBusException);

    /**
     * Returns the reversed path to the given remote object.
     * The path contains the name of the remote object itself and all its named
     * ancestors (except for the root ancestor). The order of the items is
     * reversed from the natural path, that is, the first item in the returned
     * path is the remote object name, the second item is the name of its
     * parent, the third is the name of its grandparent...
     * Empty names and Qt default names (like "qt_scrollarea_viewport") are not
     * included in the path.
     *
     * @param remoteObject The remote object to get its path.
     * @return The reversed path to the given remote object.
     * @throws DBusException If a DBus error happens.
     */
    QStringList reversedPathTo(RemoteObject* remoteObject) const
    throw (DBusException);

    /**
     * Returns the reversed paths to the remote objects with the same name as
     * the given one.
     *
     * @param remoteObject The remote object to get the paths to its homonyms.
     * @return The reversed paths to the homonyms of the given remote object.
     * @throws DBusException If a DBus error happens.
     * @see reversedPathTo(RemoteObject*)
     */
    QList<QStringList> reversedPathsToHomonymsOf(RemoteObject* remoteObject)
                                                                        const
    throw (DBusException);

    /**
     * Checks if the ancestor is unique, that is, if the paths do not contain
     * the given ancestor.
     *
     * @param ancestor The ancestor to check.
     * @param reversedPaths The list with the paths to check.
     * @return True if the paths do not contain the given ancestor, false
     *         otherwise.
     */
    bool ancestorNotInPaths(const QString& ancestor,
                            const QList<QStringList>& reversedPaths) const;

    /**
     * Checks if the path represents a remote object that is unique regarding
     * the reference paths.
     *
     * @param reversedPathToCheck The path to check.
     * @param reversedPaths The other paths to check against.
     * @return True if the path is unique, false otherwise.
     * @see isUniquePath(const QStringList&, const QStringList&)
     */
    bool isUniquePath(const QStringList& reversedPathToCheck,
                      const QList<QStringList>& reversedPaths) const;

    /**
     * Checks if the path represents a remote object that is unique regarding
     * the reference path.
     * Note that the argument order matters. The path to check can be unique
     * regarding the reference path and, at the same time, the reference path
     * can be used to find the same remote object as the path to check. Take,
     * for example, "Common ancestor/Another ancestor/Duplicated object name" as
     * the path to check and "Common ancestor/Duplicated object name" as the
     * reference path. The reference path may represent a remote object named
     * "Duplicated object name" that is child of the remote object named
     * "Common ancestor", but when using that path to find the object it
     * represents it would represent both that object and the one represented by
     * the path to check (as ANY remote object named "Duplicated object name"
     * that is descendant of the remote object named "Common ancestor" would
     * match). The path to check, on the other hand, would represent only its
     * own object thanks to its extra ancestor.
     *
     * @param reversedPathToCheck The path to check.
     * @param reversedPath The reference path to check against.
     * @return True if the path is unique, false otherwise.
     */
    bool isUniquePath(const QStringList& reversedPathToCheck,
                      const QStringList& reversedPath) const;

    /**
     * Returns the name equivalent to the given reversed path.
     * The name contains all the items in the path separated by "/". The order
     * of the items in the name is their natural order, that is, the last item
     * is the first one, the penultimate is the second one...
     * If the given path is empty an empty string is returned.
     *
     * @param path The reversed path to get its equivalent name.
     * @return The name equivalent to the given reversed path.
     */
    QString reversedPathAsName(const QStringList& reversedPath) const;

    /**
     * Sets this RemoteObjectNameRegister as being updated and emits the
     * nameUpdateStarted() signal.
     */
    void startNameUpdate();

    /**
     * Sets this RemoteObjectNameRegister as not being updated and emits the
     * nameUpdateFinished() signal.
     */
    void finishNameUpdate();

private Q_SLOTS:

    /**
     * Registers all the remote objects.
     * The name update is started (and finished if no names are queued to be
     * registered).
     */
    void registerRemoteObjects();

    /**
     * Deregisters all the remote objects.
     */
    void deregisterRemoteObjects();

    /**
     * Updates the registered remote objects if they received a ChildAdded or
     * ChildRemoved event.
     * If the ChildAdded event is received, the name update is started (and
     * finished if no names are queued to be registered).
     *
     * @param remoteObject The RemoteObject that received the event.
     * @param eventType The type of the event received.
     */
    void updateRemoteObjects(RemoteObject* remoteObject,
                             const QString& eventType);

    /**
     * Registers, if it is still available, the name of the remote object
     * pending since the longest time ago.
     * If there are no more names pending to be registered, the name update is
     * finished.
     */
    void deferredRegisterRemoteObjectName();

};

#endif
