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

#include <QtTest>

#define protected public
#define private public
#include "RemoteObjectTreeItem.h"
#include "RemoteObjectTreeItemUpdater.h"
#undef private
#undef protected

#include <KLocalizedString>

#include "../targetapplication/RemoteObject.h"
#include "../targetapplication/RemoteObjectMapper.h"

class RemoteObjectTreeItemTest: public QObject {
Q_OBJECT

private slots:

    void init();
    void cleanup();

    void testConstructor();
    void testConstructorFullRemoteObject();
    void testConstructorWhenRemoteObjectIsNotAvailable();

    void testRemoteObjectAddChild();
    void testRemoteObjectRemoveChild();

    void testUpdateSingleChildAdded();
    void testUpdateSeveralChildrenAdded();
    void testUpdateSingleChildRemoved();
    void testUpdateSeveralChildrenRemoved();
    void testUpdateSeveralChildrenAddedAndRemoved();

    void testSetUpdater();
    void testSetUpdaterBeforeUpdating();

private:

    class StubObjectRegister* mObjectRegister;

    QString mService;
    RemoteObjectMapper* mMapper;

};

class StubTreeItem: public TreeItem {
public:
    virtual QString text() const {
        return "";
    }
};

class StubObjectRegister: public QObject {
Q_OBJECT
Q_CLASSINFO("D-Bus Interface", "org.kde.ktutorial.ObjectRegister")
public:

    int mNumberOfChildren;
    bool mFilterEvenChildren;

    StubObjectRegister(QObject* parent = 0): QObject(parent),
        mNumberOfChildren(4),
        mFilterEvenChildren(false) {
    }

public slots:

    QString objectName(int objectId) {
        if (objectId > 100) {
            return "";
        }

        return "The object name " + QString::number(objectId);
    }

    QString className(int objectId) {
        if (objectId > 100) {
            return "";
        }

        return "The class name " + QString::number(objectId);
    }

    QList<int> childObjectIds(int objectId) {
        if (objectId > 42) {
            return QList<int>();
        }

        QList<int> ids;
        for (int i=0; i<mNumberOfChildren; i++) {
            if (i % 2 || !mFilterEvenChildren) {
                ids.append(objectId * 10 + i);
            }
        }
        return ids;
    }

};

void RemoteObjectTreeItemTest::init() {
    QVERIFY(QDBusConnection::sessionBus().isConnected());

    mObjectRegister = new StubObjectRegister();
    QDBusConnection::sessionBus().registerObject("/ktutorial/ObjectRegister",
                            mObjectRegister, QDBusConnection::ExportAllSlots);

    mService = QDBusConnection::sessionBus().baseService();
    mMapper = new RemoteObjectMapper(mService);
}

void RemoteObjectTreeItemTest::cleanup() {
    delete mMapper;

    QDBusConnection::sessionBus().unregisterObject("/ktutorial/ObjectRegister");
    delete mObjectRegister;
}

void RemoteObjectTreeItemTest::testConstructor() {
    RemoteObject remoteObject(mService, mMapper, 108);

    StubTreeItem parent;
    RemoteObjectTreeItem item(&remoteObject, &parent);

    QCOMPARE(item.parent(), &parent);
    QCOMPARE(item.text(),
             i18nc("@item", "Object without name! (No class name!)"));
    QCOMPARE(item.remoteObject(), &remoteObject);
    QCOMPARE(item.mUpdater, (RemoteObjectTreeItemUpdater*)0);
    QCOMPARE(item.childCount(), 0);
}

void RemoteObjectTreeItemTest::testConstructorFullRemoteObject() {
    RemoteObject remoteObject(mService, mMapper, 4);

    StubTreeItem parent;
    RemoteObjectTreeItem item(&remoteObject, &parent);

    QCOMPARE(item.parent(), &parent);
    QCOMPARE(item.text(),
             i18nc("@item", "The object name 4 (The class name 4)"));
    QCOMPARE(item.remoteObject(), &remoteObject);
    QCOMPARE(item.mUpdater, (RemoteObjectTreeItemUpdater*)0);
    QCOMPARE(item.childCount(), 4);
    RemoteObjectTreeItem* child =
                            qobject_cast<RemoteObjectTreeItem*>(item.child(0));
    QVERIFY(child);
    QCOMPARE(child->parent(), &item);
    QCOMPARE(child->text(),
             i18nc("@item", "The object name 40 (The class name 40)"));
    QCOMPARE(child->remoteObject(), remoteObject.children()[0]);
    QCOMPARE(child->mUpdater, (RemoteObjectTreeItemUpdater*)0);
    QCOMPARE(child->childCount(), 4);
    child = qobject_cast<RemoteObjectTreeItem*>(item.child(1));
    QVERIFY(child);
    QCOMPARE(child->parent(), &item);
    QCOMPARE(child->text(),
             i18nc("@item", "The object name 41 (The class name 41)"));
    QCOMPARE(child->remoteObject(), remoteObject.children()[1]);
    QCOMPARE(child->mUpdater, (RemoteObjectTreeItemUpdater*)0);
    QCOMPARE(child->childCount(), 4);
    child = qobject_cast<RemoteObjectTreeItem*>(item.child(2));
    QVERIFY(child);
    QCOMPARE(child->parent(), &item);
    QCOMPARE(child->text(),
             i18nc("@item", "The object name 42 (The class name 42)"));
    QCOMPARE(child->remoteObject(), remoteObject.children()[2]);
    QCOMPARE(child->mUpdater, (RemoteObjectTreeItemUpdater*)0);
    QCOMPARE(child->childCount(), 4);
    child = qobject_cast<RemoteObjectTreeItem*>(item.child(3));
    QVERIFY(child);
    QCOMPARE(child->parent(), &item);
    QCOMPARE(child->text(),
             i18nc("@item", "The object name 43 (The class name 43)"));
    QCOMPARE(child->remoteObject(), remoteObject.children()[3]);
    QCOMPARE(child->mUpdater, (RemoteObjectTreeItemUpdater*)0);
    QCOMPARE(child->childCount(), 0);
}

void RemoteObjectTreeItemTest::testConstructorWhenRemoteObjectIsNotAvailable() {
    RemoteObject remoteObject(mService, mMapper, 4);

    QDBusConnection::sessionBus().unregisterObject("/ktutorial/ObjectRegister");

    StubTreeItem parent;
    RemoteObjectTreeItem item(&remoteObject, &parent);

    QCOMPARE(item.parent(), &parent);
    QCOMPARE(item.text(), i18nc("@item", "D-Bus Error! (D-Bus Error!)"));
    QCOMPARE(item.remoteObject(), &remoteObject);
    QCOMPARE(item.mUpdater, (RemoteObjectTreeItemUpdater*)0);
    QCOMPARE(item.childCount(), 0);
}

void RemoteObjectTreeItemTest::testRemoteObjectAddChild() {
    RemoteObject remoteObject(mService, mMapper, 4);
    RemoteObjectTreeItem item(&remoteObject);

    //The child is not really added to the remoteObject. The tree item is misled
    //to think that
    RemoteObject fakeChild(mService, mMapper, 16);
    item.addChildRemoteObject(&fakeChild);

    QCOMPARE(item.childCount(), 5);
    RemoteObjectTreeItem* child =
                            qobject_cast<RemoteObjectTreeItem*>(item.child(0));
    QVERIFY(child);
    QCOMPARE(child->remoteObject(), remoteObject.children()[0]);
    child = qobject_cast<RemoteObjectTreeItem*>(item.child(1));
    QVERIFY(child);
    QCOMPARE(child->remoteObject(), remoteObject.children()[1]);
    child = qobject_cast<RemoteObjectTreeItem*>(item.child(2));
    QVERIFY(child);
    QCOMPARE(child->remoteObject(), remoteObject.children()[2]);
    child = qobject_cast<RemoteObjectTreeItem*>(item.child(3));
    QVERIFY(child);
    QCOMPARE(child->remoteObject(), remoteObject.children()[3]);
    child = qobject_cast<RemoteObjectTreeItem*>(item.child(4));
    QVERIFY(child);
    QCOMPARE(child->parent(), &item);
    QCOMPARE(child->text(),
             i18nc("@item", "The object name 16 (The class name 16)"));
    QCOMPARE(child->remoteObject(), &fakeChild);
    QCOMPARE(child->childCount(), 4);
}

void RemoteObjectTreeItemTest::testRemoteObjectRemoveChild() {
    RemoteObject remoteObject(mService, mMapper, 4);
    RemoteObjectTreeItem item(&remoteObject);

    //The child is not really removed from the remoteObject. The tree item is
    //misled to think that
    item.removeChildRemoteObject(remoteObject.children()[2]);

    QCOMPARE(item.childCount(), 3);
    RemoteObjectTreeItem* child =
                            qobject_cast<RemoteObjectTreeItem*>(item.child(0));
    QVERIFY(child);
    QCOMPARE(child->remoteObject(), remoteObject.children()[0]);
    child = qobject_cast<RemoteObjectTreeItem*>(item.child(1));
    QVERIFY(child);
    QCOMPARE(child->remoteObject(), remoteObject.children()[1]);
    child = qobject_cast<RemoteObjectTreeItem*>(item.child(2));
    QVERIFY(child);
    QCOMPARE(child->remoteObject(), remoteObject.children()[3]);
}

void RemoteObjectTreeItemTest::testUpdateSingleChildAdded() {
    mObjectRegister->mNumberOfChildren = 2;

    RemoteObject remoteObject(mService, mMapper, 4);
    RemoteObjectTreeItem item(&remoteObject);

    mObjectRegister->mNumberOfChildren = 3;

    item.updateChildren();

    QCOMPARE(item.childCount(), 3);
    RemoteObjectTreeItem* child =
                            qobject_cast<RemoteObjectTreeItem*>(item.child(0));
    QVERIFY(child);
    QCOMPARE(child->remoteObject(), remoteObject.children()[0]);
    child = qobject_cast<RemoteObjectTreeItem*>(item.child(1));
    QVERIFY(child);
    QCOMPARE(child->remoteObject(), remoteObject.children()[1]);
    child = qobject_cast<RemoteObjectTreeItem*>(item.child(2));
    QVERIFY(child);
    QCOMPARE(child->parent(), &item);
    QCOMPARE(child->text(),
             i18nc("@item", "The object name 42 (The class name 42)"));
    QCOMPARE(child->remoteObject(), remoteObject.children()[2]);
}

void RemoteObjectTreeItemTest::testUpdateSeveralChildrenAdded() {
    mObjectRegister->mNumberOfChildren = 1;

    RemoteObject remoteObject(mService, mMapper, 4);
    RemoteObjectTreeItem item(&remoteObject);

    mObjectRegister->mNumberOfChildren = 4;

    item.updateChildren();

    QCOMPARE(item.childCount(), 4);
    RemoteObjectTreeItem* child =
                            qobject_cast<RemoteObjectTreeItem*>(item.child(0));
    QVERIFY(child);
    QCOMPARE(child->remoteObject(), remoteObject.children()[0]);
    child = qobject_cast<RemoteObjectTreeItem*>(item.child(1));
    QVERIFY(child);
    QCOMPARE(child->parent(), &item);
    QCOMPARE(child->text(),
             i18nc("@item", "The object name 41 (The class name 41)"));
    QCOMPARE(child->remoteObject(), remoteObject.children()[1]);
    child = qobject_cast<RemoteObjectTreeItem*>(item.child(2));
    QVERIFY(child);
    QCOMPARE(child->parent(), &item);
    QCOMPARE(child->text(),
             i18nc("@item", "The object name 42 (The class name 42)"));
    QCOMPARE(child->remoteObject(), remoteObject.children()[2]);
    child = qobject_cast<RemoteObjectTreeItem*>(item.child(3));
    QVERIFY(child);
    QCOMPARE(child->parent(), &item);
    QCOMPARE(child->text(),
             i18nc("@item", "The object name 43 (The class name 43)"));
    QCOMPARE(child->remoteObject(), remoteObject.children()[3]);
}

void RemoteObjectTreeItemTest::testUpdateSingleChildRemoved() {
    mObjectRegister->mNumberOfChildren = 2;

    RemoteObject remoteObject(mService, mMapper, 4);
    RemoteObjectTreeItem item(&remoteObject);

    mObjectRegister->mFilterEvenChildren = true;

    item.updateChildren();

    QCOMPARE(item.childCount(), 1);
    RemoteObjectTreeItem* child =
                            qobject_cast<RemoteObjectTreeItem*>(item.child(0));
    QVERIFY(child);
    QCOMPARE(child->parent(), &item);
    QCOMPARE(child->text(),
             i18nc("@item", "The object name 41 (The class name 41)"));
    QCOMPARE(child->remoteObject(), remoteObject.children()[0]);
}

void RemoteObjectTreeItemTest::testUpdateSeveralChildrenRemoved() {
    mObjectRegister->mNumberOfChildren = 5;

    RemoteObject remoteObject(mService, mMapper, 4);
    RemoteObjectTreeItem item(&remoteObject);

    mObjectRegister->mFilterEvenChildren = true;

    item.updateChildren();

    QCOMPARE(item.childCount(), 2);
    RemoteObjectTreeItem* child =
                            qobject_cast<RemoteObjectTreeItem*>(item.child(0));
    QVERIFY(child);
    QCOMPARE(child->parent(), &item);
    QCOMPARE(child->text(),
             i18nc("@item", "The object name 41 (The class name 41)"));
    QCOMPARE(child->remoteObject(), remoteObject.children()[0]);
    child = qobject_cast<RemoteObjectTreeItem*>(item.child(1));
    QVERIFY(child);
    QCOMPARE(child->parent(), &item);
    QCOMPARE(child->text(),
             i18nc("@item", "The object name 43 (The class name 43)"));
    QCOMPARE(child->remoteObject(), remoteObject.children()[1]);
}

void RemoteObjectTreeItemTest::testUpdateSeveralChildrenAddedAndRemoved() {
    mObjectRegister->mNumberOfChildren = 5;

    RemoteObject remoteObject(mService, mMapper, 4);
    RemoteObjectTreeItem item(&remoteObject);

    mObjectRegister->mNumberOfChildren = 9;
    mObjectRegister->mFilterEvenChildren = true;

    item.updateChildren();

    QCOMPARE(item.childCount(), 4);
    RemoteObjectTreeItem* child =
                            qobject_cast<RemoteObjectTreeItem*>(item.child(0));
    QVERIFY(child);
    QCOMPARE(child->parent(), &item);
    QCOMPARE(child->text(),
             i18nc("@item", "The object name 41 (The class name 41)"));
    QCOMPARE(child->remoteObject(), remoteObject.children()[0]);
    child = qobject_cast<RemoteObjectTreeItem*>(item.child(1));
    QVERIFY(child);
    QCOMPARE(child->parent(), &item);
    QCOMPARE(child->text(),
             i18nc("@item", "The object name 43 (The class name 43)"));
    QCOMPARE(child->remoteObject(), remoteObject.children()[1]);
    child = qobject_cast<RemoteObjectTreeItem*>(item.child(2));
    QVERIFY(child);
    QCOMPARE(child->parent(), &item);
    QCOMPARE(child->text(),
             i18nc("@item", "The object name 45 (The class name 45)"));
    QCOMPARE(child->remoteObject(), remoteObject.children()[2]);
    child = qobject_cast<RemoteObjectTreeItem*>(item.child(3));
    QVERIFY(child);
    QCOMPARE(child->parent(), &item);
    QCOMPARE(child->text(),
             i18nc("@item", "The object name 47 (The class name 47)"));
    QCOMPARE(child->remoteObject(), remoteObject.children()[3]);
}

void RemoteObjectTreeItemTest::testSetUpdater() {
    mObjectRegister->mNumberOfChildren = 2;

    RemoteObject remoteObject(mService, mMapper, 4);

    RemoteObjectTreeItem item(&remoteObject);
    RemoteObjectTreeItemUpdater updater;
    item.setUpdater(&updater);

    QCOMPARE(item.mUpdater, &updater);
    QCOMPARE(updater.mRemoteObjectTreeItems.value(item.remoteObject()).data(),
             &item);
    QCOMPARE(item.childCount(), 2);
    RemoteObjectTreeItem* child =
                            qobject_cast<RemoteObjectTreeItem*>(item.child(0));
    QVERIFY(child);
    QCOMPARE(child->mUpdater, &updater);
    QCOMPARE(updater.mRemoteObjectTreeItems.value(child->remoteObject()).data(),
             child);
    child = qobject_cast<RemoteObjectTreeItem*>(item.child(1));
    QVERIFY(child);
    QCOMPARE(child->mUpdater, &updater);
    QCOMPARE(updater.mRemoteObjectTreeItems.value(child->remoteObject()).data(),
             child);
}

void RemoteObjectTreeItemTest::testSetUpdaterBeforeUpdating() {
    mObjectRegister->mNumberOfChildren = 0;

    RemoteObject remoteObject(mService, mMapper, 4);
    RemoteObjectTreeItem item(&remoteObject);
    RemoteObjectTreeItemUpdater updater;
    item.setUpdater(&updater);

    mObjectRegister->mNumberOfChildren = 1;

    item.updateChildren();

    QCOMPARE(item.mUpdater, &updater);
    QCOMPARE(updater.mRemoteObjectTreeItems.value(item.remoteObject()).data(),
             &item);
    QCOMPARE(item.childCount(), 1);
    RemoteObjectTreeItem* child =
                            qobject_cast<RemoteObjectTreeItem*>(item.child(0));
    QVERIFY(child);
    QCOMPARE(child->mUpdater, &updater);
    QCOMPARE(updater.mRemoteObjectTreeItems.value(child->remoteObject()).data(),
             child);
}

QTEST_MAIN(RemoteObjectTreeItemTest)

#include "RemoteObjectTreeItemTest.moc"
