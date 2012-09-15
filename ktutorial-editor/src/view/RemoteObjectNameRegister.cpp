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

#include "RemoteObjectNameRegister.h"

#include <KDebug>

#include "../targetapplication/RemoteEditorSupport.h"
#include "../targetapplication/RemoteEventSpy.h"
#include "../targetapplication/RemoteObject.h"
#include "../targetapplication/TargetApplication.h"

//public:

RemoteObjectNameRegister::RemoteObjectNameRegister(QObject* parent /*= 0*/):
        QObject(parent),
    mIsBeingUpdated(false) {
    if (TargetApplication::self()->remoteEditorSupport()) {
        registerRemoteObjects();
    }

    connect(TargetApplication::self(), SIGNAL(started()),
            this, SLOT(registerRemoteObjects()));
    connect(TargetApplication::self(), SIGNAL(finished()),
            this, SLOT(deregisterRemoteObjects()));
}

RemoteObjectNameRegister::~RemoteObjectNameRegister() {
    if (TargetApplication::self()->remoteEditorSupport()) {
        TargetApplication::self()->remoteEditorSupport()->disableEventSpy();
    }
}

QStringList RemoteObjectNameRegister::names() const {
    return mRemoteObjectForName.keys();
}

QString RemoteObjectNameRegister::uniqueName(RemoteObject* remoteObject) const
throw (DBusException) {
    QStringList reversedPath = bestNameAsReversedPath(remoteObject);
    QString name = reversedPathAsName(reversedPath);

    QList<QStringList> reversedPathsToHomonyms =
                                        reversedPathsToHomonymsOf(remoteObject);
    if (isUniquePath(reversedPath, reversedPathsToHomonyms)) {
        return name;
    }

    //Although the name is ambiguous, it identifies the given remote object
    if (findRemoteObject(name) == remoteObject) {
        return name;
    }

    return "";
}

QStringList RemoteObjectNameRegister::bestNames(const QString& name) const
throw (DBusException) {
    Q_ASSERT(mRemoteObjectForName.values(name).count() > 0);

    QStringList bestNames;

    if (mRemoteObjectForName.values(name).count() <= 1) {
        bestNames.append(name);
        return bestNames;
    }

    foreach (RemoteObject* remoteObject, mRemoteObjectForName.values(name)) {
        bestNames.append(bestName(remoteObject));
    }

    return bestNames;
}


//As KTutorial applies several rules to resolve ambiguous names (a name that
//represents two or more objects), and querying the editor support is relatively
//quick (around 5-6 miliseconds on average for Showfoto on a computer from the
//year 2002), the target application itself is queried instead of reimplementing
//those rules in the editor, as they would be very cumbersome to be tested
//properly, and prevents having to duplicate the changes when some rule is
//modified in the library.
RemoteObject* RemoteObjectNameRegister::findRemoteObject(const QString& name)
                                                                        const
throw (DBusException) {
    if (TargetApplication::self()->remoteEditorSupport()) {
        return TargetApplication::self()->remoteEditorSupport()->
                                                            findObject(name);
    }

    return 0;
}

bool RemoteObjectNameRegister::isBeingUpdated() const {
    return mIsBeingUpdated;
}

//private:

void RemoteObjectNameRegister::registerRemoteObject(RemoteObject* remoteObject,
                                                    RemoteObject* parent)
throw (DBusException) {
    Q_ASSERT(remoteObject);

    mRemoteObjectForParent.insert(parent, remoteObject);

    mRemoteObjectsPendingNameRegister.append(
                                        QPointer<RemoteObject>(remoteObject));
    QTimer::singleShot(500, this, SLOT(deferredRegisterRemoteObjectName()));

    foreach (RemoteObject* child, remoteObject->children()) {
        registerRemoteObject(child, remoteObject);
    }
}

void RemoteObjectNameRegister::deregisterRemoteObject(
                                                    RemoteObject* remoteObject,
                                                    RemoteObject* parent)
throw (DBusException) {
    Q_ASSERT(remoteObject);

    //The remote object is no longer accessible, so name() can't be called
    QString name = mRemoteObjectForName.key(remoteObject);

    emit nameRemoved(name);

    mRemoteObjectForName.remove(name, remoteObject);
    mRemoteObjectForParent.remove(parent, remoteObject);

    foreach (RemoteObject* child, remoteObject->children()) {
        deregisterRemoteObject(child, remoteObject);
    }
}

QString RemoteObjectNameRegister::bestName(RemoteObject* remoteObject) const
throw (DBusException) {
    return reversedPathAsName(bestNameAsReversedPath(remoteObject));
}

QStringList RemoteObjectNameRegister::bestNameAsReversedPath(
                                            RemoteObject* remoteObject) const
throw (DBusException) {
    QStringList reversedUniquePath;

    QString name = remoteObject->name();
    if (name.isEmpty()) {
        return reversedUniquePath;
    }

    reversedUniquePath.append(name);

    QList<QStringList> reversedPathsToHomonyms =
                                        reversedPathsToHomonymsOf(remoteObject);
    if (reversedPathsToHomonyms.count() == 0) {
        return reversedUniquePath;
    }

    QStringList reversedFullPath = reversedPathTo(remoteObject);

    //Get deepest ancestor with unique name
    int uniqueAncestorIndex = -1;
    for (int i=1; i<reversedFullPath.size() && uniqueAncestorIndex == -1; ++i) {
        if (mRemoteObjectForName.values(reversedFullPath[i]).count() == 1) {
            uniqueAncestorIndex = i;
        }
    }

    //If there is an ancestor with a unique name and it has only one descendant
    //with the name of the remote object (that is, the remote object itself),
    //return "ancestor/object".
    if (uniqueAncestorIndex != -1 && 
        ancestorNotInPaths(reversedFullPath[uniqueAncestorIndex],
                           reversedPathsToHomonyms)) {
        reversedUniquePath.append(reversedFullPath[uniqueAncestorIndex]);
        return reversedUniquePath;
    }

    //If there is more than one descendant with that name, mark the ancestor
    //with the unique name as the limit to descend when looking for a unique
    //path
    int limit = uniqueAncestorIndex;
    if (limit == -1) {
        //If no ancestor has a unique name, mark the shallowest ancestor as the
        //limit to descend
        limit = reversedFullPath.size() - 1;
    }

    //Add ancestors to the path until limit or until a unique path is found
    for (int i=1; i <= limit; ++i) {
        reversedUniquePath.append(reversedFullPath[i]);

        if (isUniquePath(reversedUniquePath, reversedPathsToHomonyms)) {
            return reversedUniquePath;
        }
    }

    return reversedUniquePath;
}

QStringList RemoteObjectNameRegister::reversedPathTo(
                                            RemoteObject* remoteObject) const
throw (DBusException) {
    QStringList reversedPath;

    RemoteObject* parent = remoteObject;
    while (parent && mRemoteObjectForParent.key(parent)) {
        QString name = parent->name();
        if (!name.isEmpty() && !name.startsWith(QLatin1String("qt_"))) {
            reversedPath.append(name);
        }
        parent = mRemoteObjectForParent.key(parent);
    }

    return reversedPath;
}

QList<QStringList> RemoteObjectNameRegister::reversedPathsToHomonymsOf(
                                            RemoteObject* remoteObject) const
throw (DBusException) {
    QList<QStringList> reversedPaths;

    QList<RemoteObject*> homonymRemoteObjects =
                            mRemoteObjectForName.values(remoteObject->name());
    homonymRemoteObjects.removeOne(remoteObject);

    foreach (RemoteObject* homonymRemoteObject, homonymRemoteObjects) {
        reversedPaths.append(reversedPathTo(homonymRemoteObject));
    }

    return reversedPaths;
}

bool RemoteObjectNameRegister::ancestorNotInPaths(const QString& ancestor,
                                const QList<QStringList>& reversedPaths) const {
    foreach (const QStringList& reversedPath, reversedPaths) {
        if (reversedPath.contains(ancestor)) {
            return false;
        }
    }

    return true;
}

bool RemoteObjectNameRegister::isUniquePath(
                                const QStringList& reversedPathToCheck,
                                const QList<QStringList>& reversedPaths) const {
    for (int i=0; i<reversedPaths.size(); ++i) {
        if (!isUniquePath(reversedPathToCheck, reversedPaths[i])) {
            return false;
        }
    }

    return true;
}

bool RemoteObjectNameRegister::isUniquePath(
                                        const QStringList& reversedPathToCheck,
                                        const QStringList& reversedPath) const {
    int commonAncestorIndex = -1;
    for (int i=reversedPathToCheck.size()-1; i>0; --i) {
        commonAncestorIndex = reversedPath.lastIndexOf(reversedPathToCheck[i],
                                                       commonAncestorIndex);
        if (commonAncestorIndex == -1) {
            return true;
        }
    }

    return false;
}

QString RemoteObjectNameRegister::reversedPathAsName(
                                        const QStringList& reversedPath) const {
    if (reversedPath.isEmpty()) {
        return "";
    }

    QString name = reversedPath[0];
    for (int i=1; i<reversedPath.size(); ++i) {
        name = reversedPath[i] + '/' + name;
    }

    return name;
}

void RemoteObjectNameRegister::startNameUpdate() {
    mIsBeingUpdated = true;
    emit nameUpdateStarted();
}

void RemoteObjectNameRegister::finishNameUpdate() {
    mIsBeingUpdated = false;
    emit nameUpdateFinished();
}

//private slots:

void RemoteObjectNameRegister::registerRemoteObjects() {
    startNameUpdate();

    try {
        registerRemoteObject(TargetApplication::self()->
                                        remoteEditorSupport()->mainWindow(), 0);
    } catch (DBusException e) {
        kWarning() << "The remote objects could not be registered to provide "
                   << "name completion (" << e.message() << ").";
    }
    
    if (mRemoteObjectsPendingNameRegister.isEmpty()) {
        finishNameUpdate();
    }
    
    try {
        RemoteEventSpy* remoteEventSpy =
            TargetApplication::self()->remoteEditorSupport()->enableEventSpy();
        connect(remoteEventSpy, SIGNAL(eventReceived(RemoteObject*,QString)),
                this, SLOT(updateRemoteObjects(RemoteObject*,QString)));
    } catch (DBusException e) {
        kWarning() << "The remote event spy could not be connected to provide "
                   << "name completion updates (" << e.message() << ").";
    }
}

void RemoteObjectNameRegister::deregisterRemoteObjects() {
    mRemoteObjectForName.clear();
    mRemoteObjectForParent.clear();
}

void RemoteObjectNameRegister::updateRemoteObjects(RemoteObject* remoteObject,
                                                   const QString& eventType) {
    if (eventType != "ChildAdded" && eventType != "ChildRemoved") {
        return;
    }

    if (eventType == "ChildAdded") {
        startNameUpdate();
    }

    try {
        QList<RemoteObject*> children = remoteObject->children();

        QList<RemoteObject*> knownChildren =
                                    mRemoteObjectForParent.values(remoteObject);

        if (eventType == "ChildAdded") {
            foreach (RemoteObject* child, children) {
                if (!knownChildren.contains(child)) {
                    registerRemoteObject(child, remoteObject);
                }
            }
        }

        if (eventType == "ChildRemoved") {
            foreach (RemoteObject* child, knownChildren) {
                if (!children.contains(child)) {
                    deregisterRemoteObject(child, remoteObject);
                }
            }
        }
    } catch (DBusException e) {
        kWarning() << "There was a problem querying the remote objects, the "
                   << "name completion could not be updated ("
                   << e.message() << ").";
    }

    if (eventType == "ChildAdded" &&
            mRemoteObjectsPendingNameRegister.isEmpty()) {
        finishNameUpdate();
    }
}

void RemoteObjectNameRegister::deferredRegisterRemoteObjectName() {
    QPointer<RemoteObject> remoteObject =
                                mRemoteObjectsPendingNameRegister.takeFirst();
    if (!remoteObject) {
        if (mRemoteObjectsPendingNameRegister.isEmpty()) {
            finishNameUpdate();
        }
    
        return;
    }

    QString name;
    try {
        name = remoteObject->name();
    } catch (DBusException e) {
        kWarning() << "There was a problem getting the name of the remote"
                   << "object (" << e.message() << ").";
    }

    if (!name.isEmpty()) {
        emit nameAdded(name);

        mRemoteObjectForName.insert(name, remoteObject);
    }

    if (mRemoteObjectsPendingNameRegister.isEmpty()) {
        finishNameUpdate();
    }
}
