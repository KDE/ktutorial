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

#include <QTest>

#include "RemoteObjectTreeSelectionManager.h"

#include <QSignalSpy>
#include <QSortFilterProxyModel>

#include "RemoteObjectTreeItem.h"
#include "TreeModel.h"
#include "../targetapplication/RemoteObject.h"
#include "../targetapplication/RemoteObjectMapper.h"

class RemoteObjectTreeSelectionManagerTest: public QObject {
Q_OBJECT

private slots:

    void initTestCase();
    void init();
    void cleanup();
    void cleanupTestCase();

    void testConstructor();

    void testSelectRemoteObjectClearingTheSelection();
    void testSelectRemoteObjectChangingBetweenRemoteObjects();

    void testSelectInProxyModel();

private:

    int mRemoteObjectStarType;

    class StubObjectRegister* mObjectRegister;
    RemoteObjectMapper* mMapper;

    RemoteObject* mRemoteObject;
    RemoteObject* mRemoteObject1;
    RemoteObject* mRemoteObject1_2;
    RemoteObject* mRemoteObject3;
    TreeModel* mTreeModel;

    QItemSelectionModel* mSelectionModel;
    RemoteObjectTreeSelectionManager* mSelectionManager;
    QSignalSpy* mRemoteObjectSelectedSpy;

    void select(const QModelIndex& index);

    void assertRemoteObjectSignal(const QSignalSpy* spy, int index,
                                  RemoteObject* remoteObject) const;

};

class StubObjectRegister: public QObject {
Q_OBJECT
Q_CLASSINFO("D-Bus Interface", "org.kde.ktutorial.ObjectRegister")
public:

    StubObjectRegister(QObject* parent = 0): QObject(parent) {
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
        for (int i=0; i<4; i++) {
            ids.append(objectId * 10 + i);
        }
        return ids;
    }

};

void RemoteObjectTreeSelectionManagerTest::initTestCase() {
    //RemoteObject* must be registered in order to be used with QSignalSpy
    mRemoteObjectStarType = qRegisterMetaType<RemoteObject*>("RemoteObject*");

    QVERIFY(QDBusConnection::sessionBus().isConnected());

    mObjectRegister = new StubObjectRegister(this);
    QDBusConnection::sessionBus().registerObject("/ktutorial/ObjectRegister",
                            mObjectRegister, QDBusConnection::ExportAllSlots);

    QString service = QDBusConnection::sessionBus().baseService();
    mMapper = new RemoteObjectMapper(service);

    mRemoteObject = mMapper->remoteObject(4);
    mRemoteObject1 = mRemoteObject->children()[0];
    mRemoteObject1_2 = mRemoteObject1->children()[1];
    mRemoteObject3 = mRemoteObject->children()[2];

    mTreeModel = new TreeModel(new RemoteObjectTreeItem(mRemoteObject));
}

void RemoteObjectTreeSelectionManagerTest::init() {
    mSelectionModel = new QItemSelectionModel(mTreeModel);
    mSelectionManager = new RemoteObjectTreeSelectionManager(mSelectionModel);

    mRemoteObjectSelectedSpy = new QSignalSpy(mSelectionManager,
                                SIGNAL(remoteObjectSelected(RemoteObject*)));
}

void RemoteObjectTreeSelectionManagerTest::cleanup() {
    delete mRemoteObjectSelectedSpy;
    delete mSelectionManager;
    delete mSelectionModel;
}

void RemoteObjectTreeSelectionManagerTest::cleanupTestCase() {
    delete mTreeModel;

    delete mMapper;

    QDBusConnection::sessionBus().unregisterObject("/ktutorial/ObjectRegister");
    delete mObjectRegister;
}

void RemoteObjectTreeSelectionManagerTest::testConstructor() {
    QObject parent;
    RemoteObjectTreeSelectionManager* selectionManager =
                    new RemoteObjectTreeSelectionManager(mSelectionModel, &parent);

    QCOMPARE(selectionManager->parent(), &parent);
}

void RemoteObjectTreeSelectionManagerTest::
                                testSelectRemoteObjectClearingTheSelection() {
    QModelIndex remoteObject1Index = mTreeModel->index(0, 0);
    select(remoteObject1Index);

    QCOMPARE(mRemoteObjectSelectedSpy->count(), 1);
    assertRemoteObjectSignal(mRemoteObjectSelectedSpy, 0, mRemoteObject1);

    select(mTreeModel->index(23, 42));

    QCOMPARE(mRemoteObjectSelectedSpy->count(), 2);
    assertRemoteObjectSignal(mRemoteObjectSelectedSpy, 1, 0);

    QModelIndex remoteObject1_2Index =
                                    mTreeModel->index(1, 0, remoteObject1Index);
    select(remoteObject1_2Index);

    QCOMPARE(mRemoteObjectSelectedSpy->count(), 3);
    assertRemoteObjectSignal(mRemoteObjectSelectedSpy, 2, mRemoteObject1_2);

    select(mTreeModel->index(23, 42));

    QCOMPARE(mRemoteObjectSelectedSpy->count(), 4);
    assertRemoteObjectSignal(mRemoteObjectSelectedSpy, 3, 0);

    QModelIndex remoteObject3Index = mTreeModel->index(2, 0);
    select(remoteObject3Index);

    QCOMPARE(mRemoteObjectSelectedSpy->count(), 5);
    assertRemoteObjectSignal(mRemoteObjectSelectedSpy, 4, mRemoteObject3);

    select(mTreeModel->index(23, 42));

    QCOMPARE(mRemoteObjectSelectedSpy->count(), 6);
    assertRemoteObjectSignal(mRemoteObjectSelectedSpy, 5, 0);
}

void RemoteObjectTreeSelectionManagerTest::
                        testSelectRemoteObjectChangingBetweenRemoteObjects() {
    QModelIndex remoteObject1Index = mTreeModel->index(0, 0);
    select(remoteObject1Index);

    QCOMPARE(mRemoteObjectSelectedSpy->count(), 1);
    assertRemoteObjectSignal(mRemoteObjectSelectedSpy, 0, mRemoteObject1);

    QModelIndex remoteObject1_2Index =
                                    mTreeModel->index(1, 0, remoteObject1Index);
    select(remoteObject1_2Index);

    QCOMPARE(mRemoteObjectSelectedSpy->count(), 2);
    assertRemoteObjectSignal(mRemoteObjectSelectedSpy, 1, mRemoteObject1_2);

    QModelIndex remoteObject3Index = mTreeModel->index(2, 0);
    select(remoteObject3Index);

    QCOMPARE(mRemoteObjectSelectedSpy->count(), 3);
    assertRemoteObjectSignal(mRemoteObjectSelectedSpy, 2, mRemoteObject3);
}

void RemoteObjectTreeSelectionManagerTest::testSelectInProxyModel() {
    QSortFilterProxyModel proxyModel;
    proxyModel.setSourceModel(mTreeModel);

    QItemSelectionModel selectionModel(&proxyModel);
    RemoteObjectTreeSelectionManager selectionManager(&selectionModel);

    QSignalSpy remoteObjectSelectedSpy(&selectionManager,
                                SIGNAL(remoteObjectSelected(RemoteObject*)));

    QModelIndex remoteObject1Index = proxyModel.index(0, 0);
    selectionModel.select(remoteObject1Index,
                          QItemSelectionModel::SelectCurrent);

    QCOMPARE(remoteObjectSelectedSpy.count(), 1);
    assertRemoteObjectSignal(&remoteObjectSelectedSpy, 0, mRemoteObject1);

    QModelIndex remoteObject1_2Index =
                                    proxyModel.index(1, 0, remoteObject1Index);
    selectionModel.select(remoteObject1_2Index,
                          QItemSelectionModel::SelectCurrent);

    QCOMPARE(remoteObjectSelectedSpy.count(), 2);
    assertRemoteObjectSignal(&remoteObjectSelectedSpy, 1, mRemoteObject1_2);
}

/////////////////////////////////// Helpers ////////////////////////////////////

void RemoteObjectTreeSelectionManagerTest::select(const QModelIndex& index) {
    mSelectionModel->select(index, QItemSelectionModel::SelectCurrent);
}

//RemoteObject* must be declared as a metatype to be used in qvariant_cast
Q_DECLARE_METATYPE(RemoteObject*);

void RemoteObjectTreeSelectionManagerTest::assertRemoteObjectSignal(
                                            const QSignalSpy* spy, int index,
                                            RemoteObject* remoteObject) const {
    QCOMPARE(spy->at(index).count(), 1);

    QVariant argument = spy->at(index).at(0);
    QCOMPARE(argument.userType(), mRemoteObjectStarType);
    QCOMPARE(qvariant_cast<RemoteObject*>(argument), remoteObject);
}

QTEST_MAIN(RemoteObjectTreeSelectionManagerTest)

#include "RemoteObjectTreeSelectionManagerTest.moc"
