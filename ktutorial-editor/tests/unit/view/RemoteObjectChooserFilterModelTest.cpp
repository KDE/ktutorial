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

#include <QtTest>

#include "RemoteObjectChooserFilterModel.h"

#include "RemoteObjectTreeItem.h"
#include "TreeModel.h"
#include "../targetapplication/RemoteClassStubs.h"
#include "../targetapplication/RemoteObject.h"
#include "../targetapplication/RemoteObjectMapper.h"

class RemoteObjectChooserFilterModelTest: public QObject {
Q_OBJECT

private slots:

    void init();
    void cleanup();

    void testNamedObjectFilter();
    void testNamedObjectFilterWhenRemoteObjectsAreNotAvailable();

    void testWidgetFilter();
    void testWidgetFilterWhenRemoteObjectsAreNotAvailable();

    void testNamedObjectAndWidgetFilters();

private:

    StubObjectRegister* mObjectRegister;
    RemoteObjectMapper* mMapper;

    TreeModel* mTreeModel;

    RemoteObjectChooserFilterModel* mFilterModel;

    QModelIndex mappedFromSource(int row, int column, const QModelIndex& parent) const;

};

void RemoteObjectChooserFilterModelTest::init() {
    QVERIFY(QDBusConnection::sessionBus().isConnected());

    mObjectRegister = new StubObjectRegister();
    QDBusConnection::sessionBus().registerObject("/ktutorial/ObjectRegister",
                            mObjectRegister, QDBusConnection::ExportAdaptors);

    QString service = QDBusConnection::sessionBus().baseService();
    mMapper = new RemoteObjectMapper(service);

    RemoteObject* mainWindow = mMapper->remoteObject(42);
    RemoteObjectTreeItem* rootItem = new RemoteObjectTreeItem(mainWindow);
    mTreeModel = new TreeModel(rootItem, this);

    mFilterModel = new RemoteObjectChooserFilterModel(this);
    mFilterModel->setSourceModel(mTreeModel);
}

void RemoteObjectChooserFilterModelTest::cleanup() {
    delete mFilterModel;

    delete mTreeModel;

    delete mMapper;

    QDBusConnection::sessionBus().unregisterObject("/ktutorial/ObjectRegister");
    delete mObjectRegister;
}

void RemoteObjectChooserFilterModelTest::testNamedObjectFilter() {
    mFilterModel->setNamedObjectFilterEnabled(true);

    QModelIndex baseIndex = mFilterModel->index(7, 0);
    QModelIndex sourceBaseIndex = mTreeModel->index(7, 0);

    QCOMPARE(mFilterModel->rowCount(baseIndex), 3);
    QCOMPARE(mappedFromSource(0, 0, sourceBaseIndex),
             mFilterModel->index(0, 0, baseIndex));
    QCOMPARE(mappedFromSource(1, 0, sourceBaseIndex),
             QModelIndex());
    QCOMPARE(mappedFromSource(2, 0, sourceBaseIndex),
             mFilterModel->index(1, 0, baseIndex));
    QCOMPARE(mappedFromSource(3, 0, sourceBaseIndex),
             mFilterModel->index(2, 0, baseIndex));

    QModelIndex childBaseIndex = mFilterModel->index(0, 0, baseIndex);
    QModelIndex childSourceBaseIndex = mTreeModel->index(0, 0, sourceBaseIndex);

    QCOMPARE(mFilterModel->rowCount(childBaseIndex), 3);
    QCOMPARE(mappedFromSource(0, 0, childSourceBaseIndex),
             mFilterModel->index(0, 0, childBaseIndex));
    QCOMPARE(mappedFromSource(1, 0, childSourceBaseIndex),
             mFilterModel->index(1, 0, childBaseIndex));
    QCOMPARE(mappedFromSource(2, 0, childSourceBaseIndex),
             QModelIndex());
    QCOMPARE(mappedFromSource(3, 0, childSourceBaseIndex),
             mFilterModel->index(2, 0, childBaseIndex));

    childBaseIndex = mFilterModel->index(1, 0, baseIndex);

    QCOMPARE(mFilterModel->rowCount(childBaseIndex), 4);

    childBaseIndex = mFilterModel->index(2, 0, baseIndex);

    QCOMPARE(mFilterModel->rowCount(childBaseIndex), 4);

    mFilterModel->setNamedObjectFilterEnabled(false);

    baseIndex = mFilterModel->index(7, 0);
    sourceBaseIndex = mTreeModel->index(7, 0);

    QCOMPARE(mFilterModel->rowCount(baseIndex), 4);
    QCOMPARE(mappedFromSource(0, 0, sourceBaseIndex),
             mFilterModel->index(0, 0, baseIndex));
    QCOMPARE(mappedFromSource(1, 0, sourceBaseIndex),
             mFilterModel->index(1, 0, baseIndex));
    QCOMPARE(mappedFromSource(2, 0, sourceBaseIndex),
             mFilterModel->index(2, 0, baseIndex));
    QCOMPARE(mappedFromSource(3, 0, sourceBaseIndex),
             mFilterModel->index(3, 0, baseIndex));

    childBaseIndex = mFilterModel->index(0, 0, baseIndex);
    childSourceBaseIndex = mTreeModel->index(0, 0, sourceBaseIndex);

    QCOMPARE(mFilterModel->rowCount(childBaseIndex), 4);
    QCOMPARE(mappedFromSource(0, 0, childSourceBaseIndex),
             mFilterModel->index(0, 0, childBaseIndex));
    QCOMPARE(mappedFromSource(1, 0, childSourceBaseIndex),
             mFilterModel->index(1, 0, childBaseIndex));
    QCOMPARE(mappedFromSource(2, 0, childSourceBaseIndex),
             mFilterModel->index(2, 0, childBaseIndex));
    QCOMPARE(mappedFromSource(3, 0, childSourceBaseIndex),
             mFilterModel->index(3, 0, childBaseIndex));

    childBaseIndex = mFilterModel->index(1, 0, baseIndex);

    QCOMPARE(mFilterModel->rowCount(childBaseIndex), 4);

    childBaseIndex = mFilterModel->index(2, 0, baseIndex);

    QCOMPARE(mFilterModel->rowCount(childBaseIndex), 4);

    childBaseIndex = mFilterModel->index(3, 0, baseIndex);

    QCOMPARE(mFilterModel->rowCount(childBaseIndex), 4);
}

void RemoteObjectChooserFilterModelTest::
                    testNamedObjectFilterWhenRemoteObjectsAreNotAvailable() {
    QDBusConnection::sessionBus().unregisterObject("/ktutorial/ObjectRegister");

    mFilterModel->setNamedObjectFilterEnabled(true);

    QModelIndex baseIndex = mFilterModel->index(7, 0);

    QCOMPARE(mFilterModel->rowCount(baseIndex), 4);

    mFilterModel->setNamedObjectFilterEnabled(false);

    QCOMPARE(mFilterModel->rowCount(baseIndex), 4);
}

void RemoteObjectChooserFilterModelTest::testWidgetFilter() {
    mFilterModel->setWidgetFilterEnabled(true);

    QModelIndex baseIndex = mFilterModel->index(0, 0);
    QModelIndex sourceBaseIndex = mTreeModel->index(7, 0);

    QCOMPARE(mFilterModel->rowCount(baseIndex), 3);
    QCOMPARE(mappedFromSource(0, 0, sourceBaseIndex),
             QModelIndex());
    QCOMPARE(mappedFromSource(1, 0, sourceBaseIndex),
             mFilterModel->index(0, 0, baseIndex));
    QCOMPARE(mappedFromSource(2, 0, sourceBaseIndex),
             mFilterModel->index(1, 0, baseIndex));
    QCOMPARE(mappedFromSource(3, 0, sourceBaseIndex),
             mFilterModel->index(2, 0, baseIndex));

    QModelIndex childBaseIndex = mFilterModel->index(0, 0, baseIndex);

    QCOMPARE(mFilterModel->rowCount(childBaseIndex), 0);

    childBaseIndex = mFilterModel->index(1, 0, baseIndex);

    QCOMPARE(mFilterModel->rowCount(childBaseIndex), 0);

    childBaseIndex = mFilterModel->index(2, 0, baseIndex);
    QModelIndex childSourceBaseIndex = mTreeModel->index(3, 0, sourceBaseIndex);

    QCOMPARE(mFilterModel->rowCount(childBaseIndex), 1);
    QCOMPARE(mappedFromSource(0, 0, childSourceBaseIndex),
             mFilterModel->index(0, 0, childBaseIndex));

    mFilterModel->setWidgetFilterEnabled(false);

    baseIndex = mFilterModel->index(7, 0);
    sourceBaseIndex = mTreeModel->index(7, 0);

    QCOMPARE(mFilterModel->rowCount(baseIndex), 4);
    QCOMPARE(mappedFromSource(0, 0, sourceBaseIndex),
             mFilterModel->index(0, 0, baseIndex));
    QCOMPARE(mappedFromSource(1, 0, sourceBaseIndex),
             mFilterModel->index(1, 0, baseIndex));
    QCOMPARE(mappedFromSource(2, 0, sourceBaseIndex),
             mFilterModel->index(2, 0, baseIndex));
    QCOMPARE(mappedFromSource(3, 0, sourceBaseIndex),
             mFilterModel->index(3, 0, baseIndex));

    childBaseIndex = mFilterModel->index(0, 0, baseIndex);

    QCOMPARE(mFilterModel->rowCount(childBaseIndex), 4);

    childBaseIndex = mFilterModel->index(1, 0, baseIndex);

    QCOMPARE(mFilterModel->rowCount(childBaseIndex), 4);

    childBaseIndex = mFilterModel->index(2, 0, baseIndex);

    QCOMPARE(mFilterModel->rowCount(childBaseIndex), 4);

    childBaseIndex = mFilterModel->index(3, 0, baseIndex);
    childSourceBaseIndex = mTreeModel->index(3, 0, sourceBaseIndex);

    QCOMPARE(mFilterModel->rowCount(childBaseIndex), 4);
    QCOMPARE(mappedFromSource(0, 0, childSourceBaseIndex),
             mFilterModel->index(0, 0, childBaseIndex));
    QCOMPARE(mappedFromSource(1, 0, childSourceBaseIndex),
             mFilterModel->index(1, 0, childBaseIndex));
    QCOMPARE(mappedFromSource(2, 0, childSourceBaseIndex),
             mFilterModel->index(2, 0, childBaseIndex));
    QCOMPARE(mappedFromSource(3, 0, childSourceBaseIndex),
             mFilterModel->index(3, 0, childBaseIndex));
}

void RemoteObjectChooserFilterModelTest::
                            testWidgetFilterWhenRemoteObjectsAreNotAvailable() {
    QDBusConnection::sessionBus().unregisterObject("/ktutorial/ObjectRegister");

    mFilterModel->setWidgetFilterEnabled(true);

    QModelIndex baseIndex = mFilterModel->index(7, 0);

    QCOMPARE(mFilterModel->rowCount(baseIndex), 4);

    mFilterModel->setWidgetFilterEnabled(false);

    QCOMPARE(mFilterModel->rowCount(baseIndex), 4);
}

void RemoteObjectChooserFilterModelTest::testNamedObjectAndWidgetFilters() {
    mFilterModel->setNamedObjectFilterEnabled(true);
    mFilterModel->setWidgetFilterEnabled(true);

    QModelIndex baseIndex = mFilterModel->index(0, 0);
    QModelIndex sourceBaseIndex = mTreeModel->index(7, 0);

    QCOMPARE(mFilterModel->rowCount(baseIndex), 2);
    QCOMPARE(mappedFromSource(0, 0, sourceBaseIndex),
             QModelIndex());
    QCOMPARE(mappedFromSource(1, 0, sourceBaseIndex),
             QModelIndex());
    QCOMPARE(mappedFromSource(2, 0, sourceBaseIndex),
             mFilterModel->index(0, 0, baseIndex));
    QCOMPARE(mappedFromSource(3, 0, sourceBaseIndex),
             mFilterModel->index(1, 0, baseIndex));

    QModelIndex childBaseIndex = mFilterModel->index(0, 0, baseIndex);

    QCOMPARE(mFilterModel->rowCount(childBaseIndex), 0);

    childBaseIndex = mFilterModel->index(1, 0, baseIndex);
    QModelIndex childSourceBaseIndex = mTreeModel->index(3, 0, sourceBaseIndex);

    QCOMPARE(mFilterModel->rowCount(childBaseIndex), 1);
    QCOMPARE(mappedFromSource(0, 0, childSourceBaseIndex),
             mFilterModel->index(0, 0, childBaseIndex));

    mFilterModel->setNamedObjectFilterEnabled(false);
    mFilterModel->setWidgetFilterEnabled(false);

    baseIndex = mFilterModel->index(7, 0);
    sourceBaseIndex = mTreeModel->index(7, 0);

    QCOMPARE(mFilterModel->rowCount(baseIndex), 4);
    QCOMPARE(mappedFromSource(0, 0, sourceBaseIndex),
             mFilterModel->index(0, 0, baseIndex));
    QCOMPARE(mappedFromSource(1, 0, sourceBaseIndex),
             mFilterModel->index(1, 0, baseIndex));
    QCOMPARE(mappedFromSource(2, 0, sourceBaseIndex),
             mFilterModel->index(2, 0, baseIndex));
    QCOMPARE(mappedFromSource(3, 0, sourceBaseIndex),
             mFilterModel->index(3, 0, baseIndex));

    childBaseIndex = mFilterModel->index(0, 0, baseIndex);

    QCOMPARE(mFilterModel->rowCount(childBaseIndex), 4);

    childBaseIndex = mFilterModel->index(1, 0, baseIndex);

    QCOMPARE(mFilterModel->rowCount(childBaseIndex), 4);

    childBaseIndex = mFilterModel->index(2, 0, baseIndex);

    QCOMPARE(mFilterModel->rowCount(childBaseIndex), 4);

    childBaseIndex = mFilterModel->index(3, 0, baseIndex);

    QCOMPARE(mFilterModel->rowCount(childBaseIndex), 4);
}

/////////////////////////////////// Helpers ////////////////////////////////////

QModelIndex RemoteObjectChooserFilterModelTest::mappedFromSource(int row,
                                int column, const QModelIndex& parent) const {
    return mFilterModel->mapFromSource(mTreeModel->index(row, column, parent));
}

QTEST_MAIN(RemoteObjectChooserFilterModelTest)

#include "../targetapplication/moc_RemoteClassStubs.cxx"
#include "RemoteObjectChooserFilterModelTest.moc"
