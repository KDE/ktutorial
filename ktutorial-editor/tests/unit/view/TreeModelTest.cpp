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

#include "TreeModel.h"

#include "TreeItem.h"

class TreeModelTest: public QObject {
Q_OBJECT

private slots:

    void initTestCase();

    void init();
    void cleanup();

    void testConstructorEmptyRootItem();
    void testConstructorSingleItem();
    void testConstructorSeveralFlatItems();
    void testConstructorSingleNestedItem();
    void testConstructorSeveralNestedItems();

    void testAppendTopLevelItem();
    void testInsertTopLevelItem();
    void testRemoveTopLevelItem();

    void testChangeChildrenOfNestedItemAddedBeforeConstructor();
    void testChangeChildrenOfNestedItemAddedAfterConstructor();

    void testChangeChildrenOfRemovedItem();
    void testChangeChildrenOfRemovedNestedItem();

    void testChangeItemData();
    void testChangeRootItemData();

    void testDataWithInvalidIndex();
    void testDataWithInvalidRole();

    void testFlagsWithInvalidIndex();

    void testHeaderDataWithInvalidSection();
    void testHeaderDataWithInvalidOrientation();
    void testHeaderDataWithInvalidRole();

    void testIndexOutOfBounds();

    void testParentWithInvalidIndex();

private:

    int mModelIndexType;

    TreeItem* mEmptyRootItem;
    TreeItem* mSingleItem;
    TreeItem* mSeveralFlatItems;
    TreeItem* mSingleNestedItem;
    TreeItem* mSeveralNestedItems;

    void assertItem(const QModelIndex& index, const QString& displayRoleData,
                    int childrenCount, const QModelIndex& parent) const;

    void assertSignal(const QSignalSpy& spy, int signalIndex,
                      const QModelIndex& index, int row) const;

};

class StubTreeItem: public TreeItem {
Q_OBJECT
public:

    QString mText;

    StubTreeItem(QString text, TreeItem* parent = 0):
            TreeItem(parent),
        mText(text) {
    }

    virtual QString text() const {
        return mText;
    }

    void setText(const QString& text) {
        mText = text;

        emit dataChanged(this);
    }

};

void TreeModelTest::initTestCase() {
    //QModelIndex must be registered in order to be used with QSignalSpy
    mModelIndexType = qRegisterMetaType<QModelIndex>("QModelIndex");
}

void TreeModelTest::init() {
    mEmptyRootItem = new StubTreeItem("root");

    mSingleItem = new StubTreeItem("root");
    TreeItem* parent = mSingleItem;
    mSingleItem->appendChild(new StubTreeItem("root-1", parent));

    mSeveralFlatItems = new StubTreeItem("root");
    parent = mSeveralFlatItems;
    mSeveralFlatItems->appendChild(new StubTreeItem("root-1", parent));
    mSeveralFlatItems->appendChild(new StubTreeItem("root-2", parent));
    mSeveralFlatItems->appendChild(new StubTreeItem("root-3", parent));

    mSingleNestedItem = new StubTreeItem("root");
    parent = mSingleNestedItem;
    mSingleNestedItem->appendChild(new StubTreeItem("root-1", parent));
    parent = mSingleNestedItem->child(0);
    mSingleNestedItem->child(0)->appendChild(new StubTreeItem("root-1-1", parent));

    mSeveralNestedItems = new StubTreeItem("root");
    parent = mSeveralNestedItems;
    mSeveralNestedItems->appendChild(new StubTreeItem("root-1", parent));
    parent = mSeveralNestedItems->child(0);
    mSeveralNestedItems->child(0)->appendChild(new StubTreeItem("root-1-1", parent));
    parent = mSeveralNestedItems;
    mSeveralNestedItems->appendChild(new StubTreeItem("root-2", parent));
    mSeveralNestedItems->appendChild(new StubTreeItem("root-3", parent));
    parent = mSeveralNestedItems->child(2);
    mSeveralNestedItems->child(2)->appendChild(new StubTreeItem("root-3-1", parent));
    mSeveralNestedItems->child(2)->appendChild(new StubTreeItem("root-3-2", parent));
}

void TreeModelTest::cleanup() {
    delete mEmptyRootItem;
    delete mSingleItem;
    delete mSeveralFlatItems;
    delete mSingleNestedItem;
    delete mSeveralNestedItems;
}

void TreeModelTest::testConstructorEmptyRootItem() {
    TreeModel model(mEmptyRootItem);
    mEmptyRootItem = 0;

    QCOMPARE(model.rowCount(), 0);
    QCOMPARE(model.columnCount(), 1);
    QCOMPARE(model.headerData(0, Qt::Horizontal).toString(), QString("root"));
}

void TreeModelTest::testConstructorSingleItem() {
    TreeModel model(mSingleItem);
    mSingleItem = 0;

    QCOMPARE(model.rowCount(), 1);
    QCOMPARE(model.columnCount(), 1);
    QCOMPARE(model.headerData(0, Qt::Horizontal).toString(), QString("root"));

    QModelIndex index = model.index(0, 0);
    assertItem(index, "root-1", 0, QModelIndex());
}

void TreeModelTest::testConstructorSeveralFlatItems() {
    TreeModel model(mSeveralFlatItems);
    mSeveralFlatItems = 0;

    QCOMPARE(model.rowCount(), 3);
    QCOMPARE(model.columnCount(), 1);
    QCOMPARE(model.headerData(0, Qt::Horizontal).toString(), QString("root"));

    QModelIndex index = model.index(0, 0);
    assertItem(index, "root-1", 0, QModelIndex());

    index = model.index(1, 0);
    assertItem(index, "root-2", 0, QModelIndex());

    index = model.index(2, 0);
    assertItem(index, "root-3", 0, QModelIndex());
}

void TreeModelTest::testConstructorSingleNestedItem() {
    TreeModel model(mSingleNestedItem);
    mSingleNestedItem = 0;

    QCOMPARE(model.rowCount(), 1);
    QCOMPARE(model.columnCount(), 1);
    QCOMPARE(model.headerData(0, Qt::Horizontal).toString(), QString("root"));

    QModelIndex index = model.index(0, 0);
    assertItem(index, "root-1", 1, QModelIndex());

    QModelIndex parent = index;
    index = model.index(0, 0, parent);
    assertItem(index, "root-1-1", 0, parent);
}

void TreeModelTest::testConstructorSeveralNestedItems() {
    TreeModel model(mSeveralNestedItems);
    mSeveralNestedItems = 0;

    QCOMPARE(model.rowCount(), 3);
    QCOMPARE(model.columnCount(), 1);
    QCOMPARE(model.headerData(0, Qt::Horizontal).toString(), QString("root"));

    QModelIndex index = model.index(0, 0);
    assertItem(index, "root-1", 1, QModelIndex());

    QModelIndex parent = index;
    index = model.index(0, 0, parent);
    assertItem(index, "root-1-1", 0, parent);

    index = model.index(1, 0);
    assertItem(index, "root-2", 0, QModelIndex());

    index = model.index(2, 0);
    assertItem(index, "root-3", 2, QModelIndex());

    parent = index;
    index = model.index(0, 0, parent);
    assertItem(index, "root-3-1", 0, parent);

    index = model.index(1, 0, parent);
    assertItem(index, "root-3-2", 0, parent);
}

//QModelIndex must be declared as a metatype to be used in qvariant_cast
Q_DECLARE_METATYPE(QModelIndex);

void TreeModelTest::testAppendTopLevelItem() {
    TreeItem* rootItem = mEmptyRootItem;
    mEmptyRootItem = 0;

    TreeModel model(rootItem);

    QSignalSpy aboutToSpy(&model,
                          SIGNAL(rowsAboutToBeInserted(QModelIndex,int,int)));
    QSignalSpy insertedSpy(&model, SIGNAL(rowsInserted(QModelIndex,int,int)));

    rootItem->appendChild(new StubTreeItem("root-1", rootItem));

    QCOMPARE(model.rowCount(), 1);
    QCOMPARE(model.columnCount(), 1);
    QCOMPARE(model.headerData(0, Qt::Horizontal).toString(), QString("root"));

    QModelIndex index = model.index(0, 0);
    assertItem(index, "root-1", 0, QModelIndex());

    //The proper way to check this would be asserting not only that the signals
    //were emitted, but that when they were emitted the item wasn't and was
    //already added (depending on the signal). However, that is too much effort
    //for little gain.
    QCOMPARE(aboutToSpy.count(), 1);
    assertSignal(aboutToSpy, 0, QModelIndex(), 0);
    QCOMPARE(insertedSpy.count(), 1);
    assertSignal(insertedSpy, 0, QModelIndex(), 0);
}

void TreeModelTest::testInsertTopLevelItem() {
    TreeItem* rootItem = mSeveralFlatItems;
    mSeveralFlatItems = 0;

    TreeModel model(rootItem);

    QSignalSpy aboutToSpy(&model,
                          SIGNAL(rowsAboutToBeInserted(QModelIndex,int,int)));
    QSignalSpy insertedSpy(&model, SIGNAL(rowsInserted(QModelIndex,int,int)));

    rootItem->insertChild(new StubTreeItem("root-2new", rootItem), 1);

    QCOMPARE(model.rowCount(), 4);
    QCOMPARE(model.columnCount(), 1);
    QCOMPARE(model.headerData(0, Qt::Horizontal).toString(), QString("root"));

    QModelIndex index = model.index(0, 0);
    assertItem(index, "root-1", 0, QModelIndex());

    index = model.index(1, 0);
    assertItem(index, "root-2new", 0, QModelIndex());

    index = model.index(2, 0);
    assertItem(index, "root-2", 0, QModelIndex());

    index = model.index(3, 0);
    assertItem(index, "root-3", 0, QModelIndex());

    QCOMPARE(aboutToSpy.count(), 1);
    assertSignal(aboutToSpy, 0, QModelIndex(), 1);
    QCOMPARE(insertedSpy.count(), 1);
    assertSignal(insertedSpy, 0, QModelIndex(), 1);
}

void TreeModelTest::testRemoveTopLevelItem() {
    TreeItem* rootItem = mSeveralFlatItems;
    mSeveralFlatItems = 0;

    TreeModel model(rootItem);

    QSignalSpy aboutToSpy(&model,
                          SIGNAL(rowsAboutToBeRemoved(QModelIndex,int,int)));
    QSignalSpy removedSpy(&model, SIGNAL(rowsRemoved(QModelIndex,int,int)));

    TreeItem* itemToBeRemoved = rootItem->child(1);
    rootItem->removeChild(itemToBeRemoved);
    delete itemToBeRemoved;

    QCOMPARE(model.rowCount(), 2);
    QCOMPARE(model.columnCount(), 1);
    QCOMPARE(model.headerData(0, Qt::Horizontal).toString(), QString("root"));

    QModelIndex index = model.index(0, 0);
    assertItem(index, "root-1", 0, QModelIndex());

    index = model.index(1, 0);
    assertItem(index, "root-3", 0, QModelIndex());

    QCOMPARE(aboutToSpy.count(), 1);
    assertSignal(aboutToSpy, 0, QModelIndex(), 1);
    QCOMPARE(removedSpy.count(), 1);
    assertSignal(removedSpy, 0, QModelIndex(), 1);
}


void TreeModelTest::testChangeChildrenOfNestedItemAddedBeforeConstructor() {
    TreeItem* rootItem = mSingleNestedItem;
    mSingleNestedItem = 0;

    TreeModel model(rootItem);
    TreeItem* item = rootItem->child(0)->child(0);

    QSignalSpy aboutToInsertSpy(&model,
                          SIGNAL(rowsAboutToBeInserted(QModelIndex,int,int)));
    QSignalSpy insertedSpy(&model, SIGNAL(rowsInserted(QModelIndex,int,int)));
    QSignalSpy aboutToRemoveSpy(&model,
                          SIGNAL(rowsAboutToBeRemoved(QModelIndex,int,int)));
    QSignalSpy removedSpy(&model, SIGNAL(rowsRemoved(QModelIndex,int,int)));

    StubTreeItem childToBeRemoved("root-1-1-3", item);
    item->appendChild(new StubTreeItem("root-1-1-1", item));
    item->appendChild(&childToBeRemoved);
    item->insertChild(new StubTreeItem("root-1-1-2", item), 1);
    item->removeChild(&childToBeRemoved);

    QCOMPARE(model.rowCount(), 1);
    QCOMPARE(model.columnCount(), 1);
    QCOMPARE(model.headerData(0, Qt::Horizontal).toString(), QString("root"));

    QModelIndex index = model.index(0, 0);
    assertItem(index, "root-1", 1, QModelIndex());

    QModelIndex parent = index;
    index = model.index(0, 0, parent);
    assertItem(index, "root-1-1", 2, parent);

    parent = index;
    index = model.index(0, 0, parent);
    assertItem(index, "root-1-1-1", 0, parent);

    index = model.index(1, 0, parent);
    assertItem(index, "root-1-1-2", 0, parent);

    QCOMPARE(aboutToInsertSpy.count(), 3);
    assertSignal(aboutToInsertSpy, 0, parent, 0);
    assertSignal(aboutToInsertSpy, 1, parent, 1);
    assertSignal(aboutToInsertSpy, 2, parent, 1);
    QCOMPARE(insertedSpy.count(), 3);
    assertSignal(insertedSpy, 0, parent, 0);
    assertSignal(insertedSpy, 1, parent, 1);
    assertSignal(insertedSpy, 2, parent, 1);
    QCOMPARE(aboutToRemoveSpy.count(), 1);
    assertSignal(aboutToRemoveSpy, 0, parent, 2);
    QCOMPARE(removedSpy.count(), 1);
    assertSignal(removedSpy, 0, parent, 2);
}

void TreeModelTest::testChangeChildrenOfNestedItemAddedAfterConstructor() {
    TreeItem* rootItem = mEmptyRootItem;
    mEmptyRootItem = 0;

    TreeModel model(rootItem);
    rootItem->appendChild(new StubTreeItem("root-1", rootItem));

    TreeItem* item = new StubTreeItem("root-1-1", rootItem->child(0));
    rootItem->child(0)->appendChild(item);

    QSignalSpy aboutToInsertSpy(&model,
                          SIGNAL(rowsAboutToBeInserted(QModelIndex,int,int)));
    QSignalSpy insertedSpy(&model, SIGNAL(rowsInserted(QModelIndex,int,int)));
    QSignalSpy aboutToRemoveSpy(&model,
                          SIGNAL(rowsAboutToBeRemoved(QModelIndex,int,int)));
    QSignalSpy removedSpy(&model, SIGNAL(rowsRemoved(QModelIndex,int,int)));

    StubTreeItem childToBeRemoved("root-1-1-3", item);
    item->appendChild(new StubTreeItem("root-1-1-1", item));
    item->appendChild(&childToBeRemoved);
    item->insertChild(new StubTreeItem("root-1-1-2", item), 1);
    item->removeChild(&childToBeRemoved);

    QCOMPARE(model.rowCount(), 1);
    QCOMPARE(model.columnCount(), 1);
    QCOMPARE(model.headerData(0, Qt::Horizontal).toString(), QString("root"));

    QModelIndex index = model.index(0, 0);
    assertItem(index, "root-1", 1, QModelIndex());

    QModelIndex parent = index;
    index = model.index(0, 0, parent);
    assertItem(index, "root-1-1", 2, parent);

    parent = index;
    index = model.index(0, 0, parent);
    assertItem(index, "root-1-1-1", 0, parent);

    index = model.index(1, 0, parent);
    assertItem(index, "root-1-1-2", 0, parent);

    QCOMPARE(aboutToInsertSpy.count(), 3);
    assertSignal(aboutToInsertSpy, 0, parent, 0);
    assertSignal(aboutToInsertSpy, 1, parent, 1);
    assertSignal(aboutToInsertSpy, 2, parent, 1);
    QCOMPARE(insertedSpy.count(), 3);
    assertSignal(insertedSpy, 0, parent, 0);
    assertSignal(insertedSpy, 1, parent, 1);
    assertSignal(insertedSpy, 2, parent, 1);
    QCOMPARE(aboutToRemoveSpy.count(), 1);
    assertSignal(aboutToRemoveSpy, 0, parent, 2);
    QCOMPARE(removedSpy.count(), 1);
    assertSignal(removedSpy, 0, parent, 2);
}

void TreeModelTest::testChangeChildrenOfRemovedItem() {
    TreeItem* rootItem = mEmptyRootItem;
    mEmptyRootItem = 0;

    TreeModel model(rootItem);

    StubTreeItem item("root-1", rootItem);
    rootItem->appendChild(&item);
    rootItem->removeChild(&item);

    QSignalSpy aboutToSpy(&model,
                          SIGNAL(rowsAboutToBeInserted(QModelIndex,int,int)));
    QSignalSpy insertedSpy(&model, SIGNAL(rowsInserted(QModelIndex,int,int)));

    item.appendChild(new StubTreeItem("root-1-1", &item));

    QCOMPARE(model.rowCount(), 0);
    QCOMPARE(model.columnCount(), 1);
    QCOMPARE(model.headerData(0, Qt::Horizontal).toString(), QString("root"));

    QCOMPARE(aboutToSpy.count(), 0);
    QCOMPARE(insertedSpy.count(), 0);
}

void TreeModelTest::testChangeChildrenOfRemovedNestedItem() {
    TreeItem* rootItem = mEmptyRootItem;
    mEmptyRootItem = 0;

    TreeModel model(rootItem);

    StubTreeItem item("root-1", rootItem);
    TreeItem* nestedItem = new StubTreeItem("root-1-1", &item);
    item.appendChild(nestedItem);
    rootItem->appendChild(&item);
    rootItem->removeChild(&item);

    QSignalSpy aboutToSpy(&model,
                          SIGNAL(rowsAboutToBeInserted(QModelIndex,int,int)));
    QSignalSpy insertedSpy(&model, SIGNAL(rowsInserted(QModelIndex,int,int)));

    nestedItem->appendChild(new StubTreeItem("root-1-1-1", nestedItem));

    QCOMPARE(model.rowCount(), 0);
    QCOMPARE(model.columnCount(), 1);
    QCOMPARE(model.headerData(0, Qt::Horizontal).toString(), QString("root"));

    QCOMPARE(aboutToSpy.count(), 0);
    QCOMPARE(insertedSpy.count(), 0);
}

void TreeModelTest::testChangeItemData() {
    TreeItem* rootItem = mEmptyRootItem;
    mEmptyRootItem = 0;

    StubTreeItem* item = new StubTreeItem("root-1", rootItem);
    rootItem->appendChild(item);
    TreeModel model(rootItem);

    QSignalSpy dataChangedSpy(&model,
                              SIGNAL(dataChanged(QModelIndex,QModelIndex)));

    item->setText("root-1modified");

    QCOMPARE(model.rowCount(), 1);
    QCOMPARE(model.columnCount(), 1);
    QCOMPARE(model.headerData(0, Qt::Horizontal).toString(), QString("root"));

    QModelIndex index = model.index(0, 0);
    assertItem(index, "root-1modified", 0, QModelIndex());

    QCOMPARE(dataChangedSpy.count(), 1);
    QVariant argument = dataChangedSpy.at(0).at(0);
    QCOMPARE(argument.userType(), mModelIndexType);
    QCOMPARE(qvariant_cast<QModelIndex>(argument), index);
    argument = dataChangedSpy.at(0).at(1);
    QCOMPARE(argument.userType(), mModelIndexType);
    QCOMPARE(qvariant_cast<QModelIndex>(argument), index);
}

//Qt::Orientation must be declared as a metatype to be used in qvariant_cast
Q_DECLARE_METATYPE(Qt::Orientation);

void TreeModelTest::testChangeRootItemData() {
    StubTreeItem* rootItem = new StubTreeItem("root");
    TreeModel model(rootItem);

    //Qt::Orientation must be registered in order to be used with QSignalSpy
    int orientationIndexType = qRegisterMetaType<Qt::Orientation>("Qt::Orientation");
    QSignalSpy headerDataChangedSpy(&model,
                        SIGNAL(headerDataChanged(Qt::Orientation,int,int)));

    rootItem->setText("root modified");

    QCOMPARE(model.rowCount(), 0);
    QCOMPARE(model.columnCount(), 1);
    QCOMPARE(model.headerData(0, Qt::Horizontal).toString(),
             QString("root modified"));

    QCOMPARE(headerDataChangedSpy.count(), 1);
    QVariant argument = headerDataChangedSpy.at(0).at(0);
    QCOMPARE(argument.userType(), orientationIndexType);
    QCOMPARE(qvariant_cast<Qt::Orientation>(argument), Qt::Horizontal);
    argument = headerDataChangedSpy.at(0).at(1);
    QCOMPARE(argument.type(), QVariant::Int);
    QCOMPARE(argument.toInt(), 0);
    argument = headerDataChangedSpy.at(0).at(2);
    QCOMPARE(argument.type(), QVariant::Int);
    QCOMPARE(argument.toInt(), 0);
}

void TreeModelTest::testDataWithInvalidIndex() {
    TreeModel model(mSingleItem);
    mSingleItem = 0;

    QCOMPARE(model.data(QModelIndex()), QVariant());
}

void TreeModelTest::testDataWithInvalidRole() {
    TreeModel model(mSingleItem);
    mSingleItem = 0;

    QCOMPARE(model.data(model.index(0, 0), Qt::DecorationRole), QVariant());
    QCOMPARE(model.data(model.index(0, 0), Qt::EditRole), QVariant());
    QCOMPARE(model.data(model.index(0, 0), Qt::ToolTipRole), QVariant());
    QCOMPARE(model.data(model.index(0, 0), Qt::StatusTipRole), QVariant());
    QCOMPARE(model.data(model.index(0, 0), Qt::WhatsThisRole), QVariant());
    QCOMPARE(model.data(model.index(0, 0), Qt::SizeHintRole), QVariant());
}

void TreeModelTest::testFlagsWithInvalidIndex() {
    TreeModel model(mSingleItem);
    mSingleItem = 0;

    QCOMPARE(model.flags(QModelIndex()), Qt::NoItemFlags);
}

void TreeModelTest::testHeaderDataWithInvalidSection() {
    TreeModel model(mEmptyRootItem);
    mEmptyRootItem = 0;

    QCOMPARE(model.headerData(1, Qt::Horizontal), QVariant());
}

void TreeModelTest::testHeaderDataWithInvalidOrientation() {
    TreeModel model(mEmptyRootItem);
    mEmptyRootItem = 0;

    QCOMPARE(model.headerData(0, Qt::Vertical), QVariant());
}

void TreeModelTest::testHeaderDataWithInvalidRole() {
    TreeModel model(mEmptyRootItem);
    mEmptyRootItem = 0;

    QCOMPARE(model.headerData(0, Qt::Horizontal, Qt::DecorationRole), QVariant());
    QCOMPARE(model.headerData(0, Qt::Horizontal, Qt::EditRole), QVariant());
    QCOMPARE(model.headerData(0, Qt::Horizontal, Qt::ToolTipRole), QVariant());
    QCOMPARE(model.headerData(0, Qt::Horizontal, Qt::StatusTipRole), QVariant());
    QCOMPARE(model.headerData(0, Qt::Horizontal, Qt::WhatsThisRole), QVariant());
    QCOMPARE(model.headerData(0, Qt::Horizontal, Qt::SizeHintRole), QVariant());
}

void TreeModelTest::testIndexOutOfBounds() {
    TreeModel model(mSingleNestedItem);
    mSingleNestedItem = 0;

    QCOMPARE(model.index(0, 1), QModelIndex());
    QCOMPARE(model.index(1, 0), QModelIndex());

    QModelIndex parent = model.index(0, 0);
    QVERIFY(parent.isValid());
    QCOMPARE(model.index(0, 1, parent), QModelIndex());
    QCOMPARE(model.index(1, 0, parent), QModelIndex());
}

void TreeModelTest::testParentWithInvalidIndex() {
    TreeModel model(mSingleItem);
    mSingleItem = 0;

    QCOMPARE(model.parent(QModelIndex()), QModelIndex());
}

/////////////////////////////////// Helpers ////////////////////////////////////

void TreeModelTest::assertItem(const QModelIndex& index,
                               const QString& displayRoleData,
                               int childrenCount,
                               const QModelIndex& parent) const {
    const QAbstractItemModel* model = index.model();

    QVERIFY(index.isValid());
    QCOMPARE(model->data(index).toString(), displayRoleData);
    QCOMPARE(model->flags(index), Qt::ItemIsEnabled | Qt::ItemIsSelectable);
    QCOMPARE(model->parent(index), parent);
    QCOMPARE(model->rowCount(index), childrenCount);
    QCOMPARE(model->columnCount(index), 1);
}

void TreeModelTest::assertSignal(const QSignalSpy& spy, int signalIndex,
                                const QModelIndex& index, int row) const {
    QCOMPARE(spy.at(signalIndex).count(), 3);

    QVariant argument = spy.at(signalIndex).at(0);
    QCOMPARE(argument.userType(), mModelIndexType);
    QCOMPARE(qvariant_cast<QModelIndex>(argument), index);

    argument = spy.at(signalIndex).at(1);
    QCOMPARE(argument.type(), QVariant::Int);
    QCOMPARE(argument.toInt(), row);

    argument = spy.at(signalIndex).at(2);
    QCOMPARE(argument.type(), QVariant::Int);
    QCOMPARE(argument.toInt(), row);
}

QTEST_MAIN(TreeModelTest)

#include "TreeModelTest.moc"
