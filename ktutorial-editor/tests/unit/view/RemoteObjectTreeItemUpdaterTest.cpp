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

#include <QTest>

#include "RemoteObjectTreeItemUpdater.h"
#include "RemoteObjectTreeItem.h"
#include "../targetapplication/RemoteEventSpy.h"
#include "../targetapplication/RemoteObject.h"
#include "../targetapplication/RemoteObjectMapper.h"

class RemoteObjectTreeItemUpdaterTest: public QObject {
Q_OBJECT

private slots:

    void init();
    void cleanup();

    void testConstructor();

    void testEventReceived();
    void testEventReceivedEventTypeUnknown();
    void testEventReceivedObjectNotRegistered();
    void testEventReceivedTreeItemAlreadyDestroyed();

private:

    class StubEventSpy* mEventSpy;
    class StubObjectRegister* mObjectRegister;

    QString mService;
    RemoteObjectMapper* mMapper;

};

class StubEventSpy: public QObject {
Q_OBJECT
Q_CLASSINFO("D-Bus Interface", "org.kde.ktutorial.EventSpy")
public:

    StubEventSpy(QObject* parent = 0): QObject(parent) {
    }

    void emitEventReceived(int objectId, const QString& eventType) {
        emit eventReceived(objectId, eventType);
    }

signals:

    void eventReceived(int objectId, const QString& eventType);

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

void RemoteObjectTreeItemUpdaterTest::init() {
    QVERIFY(QDBusConnection::sessionBus().isConnected());

    mEventSpy = new StubEventSpy();
    QDBusConnection::sessionBus().registerObject("/ktutorial/EventSpy",
                            mEventSpy, QDBusConnection::ExportAllSignals);

    mObjectRegister = new StubObjectRegister();
    QDBusConnection::sessionBus().registerObject("/ktutorial/ObjectRegister",
                            mObjectRegister, QDBusConnection::ExportAllSlots);

    mService = QDBusConnection::sessionBus().baseService();
    mMapper = new RemoteObjectMapper(mService);
}

void RemoteObjectTreeItemUpdaterTest::cleanup() {
    delete mMapper;

    QDBusConnection::sessionBus().unregisterObject("/ktutorial/EventSpy");
    delete mEventSpy;

    QDBusConnection::sessionBus().unregisterObject("/ktutorial/ObjectRegister");
    delete mObjectRegister;
}

void RemoteObjectTreeItemUpdaterTest::testConstructor() {
    QObject parent;
    RemoteObjectTreeItemUpdater* updater =
                                    new RemoteObjectTreeItemUpdater(&parent);

    QCOMPARE(updater->parent(), &parent);
}

void RemoteObjectTreeItemUpdaterTest::testEventReceived() {
    mObjectRegister->mNumberOfChildren = 0;

    RemoteObjectTreeItem item(mMapper->remoteObject(4));

    QCOMPARE(item.childCount(), 0);

    RemoteObjectTreeItemUpdater updater;
    RemoteEventSpy remoteEventSpy(mService, mMapper);
    updater.setRemoteEventSpy(&remoteEventSpy);
    updater.registerRemoteObjectTreeItem(&item);

    mObjectRegister->mNumberOfChildren = 1;
    mEventSpy->emitEventReceived(4, "ChildAdded");

    //Give D-Bus time to deliver the signal
    QTest::qWait(100);

    QCOMPARE(item.childCount(), 1);

    mObjectRegister->mNumberOfChildren = 0;
    mEventSpy->emitEventReceived(4, "ChildRemoved");

    //Give D-Bus time to deliver the signal
    QTest::qWait(100);

    QCOMPARE(item.childCount(), 0);
}

void RemoteObjectTreeItemUpdaterTest::testEventReceivedEventTypeUnknown() {
    mObjectRegister->mNumberOfChildren = 0;

    RemoteObjectTreeItem item(mMapper->remoteObject(4));

    QCOMPARE(item.childCount(), 0);

    RemoteObjectTreeItemUpdater updater;
    RemoteEventSpy remoteEventSpy(mService, mMapper);
    updater.setRemoteEventSpy(&remoteEventSpy);
    updater.registerRemoteObjectTreeItem(&item);

    mObjectRegister->mNumberOfChildren = 1;
    mEventSpy->emitEventReceived(4, "AnyOtherEvent");

    //Give D-Bus time to deliver the signal
    QTest::qWait(100);

    QCOMPARE(item.childCount(), 0);
}

void RemoteObjectTreeItemUpdaterTest::testEventReceivedObjectNotRegistered() {
    mObjectRegister->mNumberOfChildren = 0;

    RemoteObjectTreeItem item(mMapper->remoteObject(4));

    QCOMPARE(item.childCount(), 0);

    RemoteObjectTreeItemUpdater updater;
    RemoteEventSpy remoteEventSpy(mService, mMapper);
    updater.setRemoteEventSpy(&remoteEventSpy);
    updater.registerRemoteObjectTreeItem(&item);

    mObjectRegister->mNumberOfChildren = 1;
    mEventSpy->emitEventReceived(8, "ChildAdded");

    //Give D-Bus time to deliver the signal
    QTest::qWait(100);

    QCOMPARE(item.childCount(), 0);
}

void RemoteObjectTreeItemUpdaterTest::
                                testEventReceivedTreeItemAlreadyDestroyed() {
    mObjectRegister->mNumberOfChildren = 0;

    QPointer<RemoteObjectTreeItem> item =
                            new RemoteObjectTreeItem(mMapper->remoteObject(4));

    QCOMPARE(item->childCount(), 0);

    RemoteObjectTreeItemUpdater updater;
    RemoteEventSpy remoteEventSpy(mService, mMapper);
    updater.setRemoteEventSpy(&remoteEventSpy);
    updater.registerRemoteObjectTreeItem(item);

    delete item;

    mObjectRegister->mNumberOfChildren = 1;
    mEventSpy->emitEventReceived(4, "ChildAdded");

    //Give D-Bus time to deliver the signal
    QTest::qWait(100);

    //Ensure that the item was really destroyed. Nothing else can be explicitly
    //checked, if the test does not crash everything is fine
    QVERIFY(!item);
}

QTEST_MAIN(RemoteObjectTreeItemUpdaterTest)

#include "RemoteObjectTreeItemUpdaterTest.moc"
