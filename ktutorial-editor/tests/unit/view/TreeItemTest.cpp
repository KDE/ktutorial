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

#include "TreeItem.h"

#include <QSignalSpy>

class TreeItemTest: public QObject {
Q_OBJECT

private slots:

    void initTestCase();

    void testConstructor();

    void testAppendChild();
    void testAppendChildSeveralChildren();

    void testInsertChild();
    void testInsertChildSeveralChildren();

    void testRemoveChild();
    void testRemoveChildSeveralChildren();

private:

    int mTreeItemStarType;

    void assertAboutToBeInsertedSignal(const QSignalSpy& spy, int index,
                                       TreeItem* item, int itemIndex) const;

    void assertSignal(const QSignalSpy& spy, int index, TreeItem* item) const;

};

class StubTreeItem: public TreeItem {
public:

    QString mText;

    StubTreeItem(TreeItem* parent = 0): TreeItem(parent) {
    }

    virtual QString text() const {
        return mText;
    }

};

void TreeItemTest::initTestCase() {
    //TreeItem* must be registered in order to be used with QSignalSpy
    mTreeItemStarType = qRegisterMetaType<TreeItem*>("TreeItem*");
}

void TreeItemTest::testConstructor() {
    StubTreeItem parent;
    StubTreeItem treeItem(&parent);

    QCOMPARE(parent.parent(), (TreeItem*)0);
    QCOMPARE(treeItem.parent(), &parent);
}

//TreeItem* must be declared as a metatype to be used in qvariant_cast
Q_DECLARE_METATYPE(TreeItem*);

void TreeItemTest::testAppendChild() {
    StubTreeItem treeItem;
    TreeItem* child = new StubTreeItem(&treeItem);

    QSignalSpy aboutToBeInsertedSpy(&treeItem,
                                SIGNAL(childAboutToBeInserted(TreeItem*, int)));
    QSignalSpy insertedSpy(&treeItem, SIGNAL(childInserted(TreeItem*)));

    treeItem.appendChild(child);

    QCOMPARE(treeItem.childCount(), 1);
    QCOMPARE(treeItem.child(0), child);
    QCOMPARE(child->childIndex(), 0);
    QCOMPARE(child->parent(), &treeItem);

    //The proper way to check this would be asserting not only that the signals
    //were emitted, but that when they were emitted the item wasn't and was
    //already added (depending on the signal). However, that is too much effort
    //for little gain.
    QCOMPARE(aboutToBeInsertedSpy.count(), 1);
    assertAboutToBeInsertedSignal(aboutToBeInsertedSpy, 0, child, 0);
    QCOMPARE(insertedSpy.count(), 1);
    assertSignal(insertedSpy, 0, child);
}

void TreeItemTest::testAppendChildSeveralChildren() {
    StubTreeItem treeItem;
    TreeItem* child1 = new StubTreeItem(&treeItem);
    TreeItem* child2 = new StubTreeItem(&treeItem);
    TreeItem* child3 = new StubTreeItem(&treeItem);

    QSignalSpy aboutToBeInsertedSpy(&treeItem,
                                SIGNAL(childAboutToBeInserted(TreeItem*, int)));
    QSignalSpy insertedSpy(&treeItem, SIGNAL(childInserted(TreeItem*)));

    treeItem.appendChild(child1);
    treeItem.appendChild(child2);
    treeItem.appendChild(child3);

    QCOMPARE(treeItem.childCount(), 3);
    QCOMPARE(treeItem.child(0), child1);
    QCOMPARE(treeItem.child(1), child2);
    QCOMPARE(treeItem.child(2), child3);
    QCOMPARE(child1->childIndex(), 0);
    QCOMPARE(child1->parent(), &treeItem);
    QCOMPARE(child2->childIndex(), 1);
    QCOMPARE(child2->parent(), &treeItem);
    QCOMPARE(child3->childIndex(), 2);
    QCOMPARE(child3->parent(), &treeItem);

    QCOMPARE(aboutToBeInsertedSpy.count(), 3);
    assertAboutToBeInsertedSignal(aboutToBeInsertedSpy, 0, child1, 0);
    assertAboutToBeInsertedSignal(aboutToBeInsertedSpy, 1, child2, 1);
    assertAboutToBeInsertedSignal(aboutToBeInsertedSpy, 2, child3, 2);
    QCOMPARE(insertedSpy.count(), 3);
    assertSignal(insertedSpy, 0, child1);
    assertSignal(insertedSpy, 1, child2);
    assertSignal(insertedSpy, 2, child3);
}

void TreeItemTest::testInsertChild() {
    StubTreeItem treeItem;
    TreeItem* child = new StubTreeItem(&treeItem);

    QSignalSpy aboutToBeInsertedSpy(&treeItem,
                                SIGNAL(childAboutToBeInserted(TreeItem*, int)));
    QSignalSpy insertedSpy(&treeItem, SIGNAL(childInserted(TreeItem*)));

    treeItem.insertChild(child, 0);

    QCOMPARE(treeItem.childCount(), 1);
    QCOMPARE(treeItem.child(0), child);
    QCOMPARE(child->childIndex(), 0);
    QCOMPARE(child->parent(), &treeItem);

    QCOMPARE(aboutToBeInsertedSpy.count(), 1);
    assertAboutToBeInsertedSignal(aboutToBeInsertedSpy, 0, child, 0);
    QCOMPARE(insertedSpy.count(), 1);
    assertSignal(insertedSpy, 0, child);
}

void TreeItemTest::testInsertChildSeveralChildren() {
    StubTreeItem treeItem;
    TreeItem* child1 = new StubTreeItem(&treeItem);
    TreeItem* child2 = new StubTreeItem(&treeItem);
    TreeItem* child3 = new StubTreeItem(&treeItem);
    TreeItem* child4 = new StubTreeItem(&treeItem);

    QSignalSpy aboutToBeInsertedSpy(&treeItem,
                                SIGNAL(childAboutToBeInserted(TreeItem*, int)));
    QSignalSpy insertedSpy(&treeItem, SIGNAL(childInserted(TreeItem*)));

    treeItem.insertChild(child2, 0);
    treeItem.insertChild(child1, 0);
    treeItem.insertChild(child4, 2);
    treeItem.insertChild(child3, 2);

    QCOMPARE(treeItem.childCount(), 4);
    QCOMPARE(treeItem.child(0), child1);
    QCOMPARE(treeItem.child(1), child2);
    QCOMPARE(treeItem.child(2), child3);
    QCOMPARE(treeItem.child(3), child4);
    QCOMPARE(child1->childIndex(), 0);
    QCOMPARE(child1->parent(), &treeItem);
    QCOMPARE(child2->childIndex(), 1);
    QCOMPARE(child2->parent(), &treeItem);
    QCOMPARE(child3->childIndex(), 2);
    QCOMPARE(child3->parent(), &treeItem);
    QCOMPARE(child4->childIndex(), 3);
    QCOMPARE(child4->parent(), &treeItem);

    QCOMPARE(aboutToBeInsertedSpy.count(), 4);
    assertAboutToBeInsertedSignal(aboutToBeInsertedSpy, 0, child2, 0);
    assertAboutToBeInsertedSignal(aboutToBeInsertedSpy, 1, child1, 0);
    assertAboutToBeInsertedSignal(aboutToBeInsertedSpy, 2, child4, 2);
    assertAboutToBeInsertedSignal(aboutToBeInsertedSpy, 3, child3, 2);
    QCOMPARE(insertedSpy.count(), 4);
    assertSignal(insertedSpy, 0, child2);
    assertSignal(insertedSpy, 1, child1);
    assertSignal(insertedSpy, 2, child4);
    assertSignal(insertedSpy, 3, child3);
}

void TreeItemTest::testRemoveChild() {
    StubTreeItem treeItem;
    //It will be removed and not deleted by parent TreeItem, so it is created in
    //stack
    StubTreeItem child(&treeItem);

    QSignalSpy aboutToBeRemovedSpy(&treeItem,
                                    SIGNAL(childAboutToBeRemoved(TreeItem*)));
    QSignalSpy removedSpy(&treeItem, SIGNAL(childRemoved(TreeItem*)));

    treeItem.appendChild(&child);
    treeItem.removeChild(&child);

    QCOMPARE(treeItem.childCount(), 0);
    QCOMPARE(child.childIndex(), -1);
    QCOMPARE(child.parent(), &treeItem);

    QCOMPARE(aboutToBeRemovedSpy.count(), 1);
    assertSignal(aboutToBeRemovedSpy, 0, &child);
    QCOMPARE(removedSpy.count(), 1);
    assertSignal(removedSpy, 0, &child);
}

void TreeItemTest::testRemoveChildSeveralChildren() {
    StubTreeItem treeItem;
    //They will be removed and not deleted by parent TreeItem, so they are
    //created in stack
    StubTreeItem child1(&treeItem);
    StubTreeItem child2(&treeItem);
    StubTreeItem child3(&treeItem);

    QSignalSpy aboutToBeRemovedSpy(&treeItem,
                                    SIGNAL(childAboutToBeRemoved(TreeItem*)));
    QSignalSpy removedSpy(&treeItem, SIGNAL(childRemoved(TreeItem*)));

    treeItem.appendChild(&child1);
    treeItem.appendChild(&child2);
    treeItem.appendChild(&child3);

    treeItem.removeChild(&child2);

    QCOMPARE(treeItem.childCount(), 2);
    QCOMPARE(treeItem.child(0), &child1);
    QCOMPARE(treeItem.child(1), &child3);
    QCOMPARE(child1.childIndex(), 0);
    QCOMPARE(child1.parent(), &treeItem);
    QCOMPARE(child2.childIndex(), -1);
    QCOMPARE(child2.parent(), &treeItem);
    QCOMPARE(child3.childIndex(), 1);
    QCOMPARE(child3.parent(), &treeItem);

    QCOMPARE(aboutToBeRemovedSpy.count(), 1);
    assertSignal(aboutToBeRemovedSpy, 0, &child2);
    QCOMPARE(removedSpy.count(), 1);
    assertSignal(removedSpy, 0, &child2);

    treeItem.removeChild(&child1);
    treeItem.removeChild(&child3);

    QCOMPARE(treeItem.childCount(), 0);
    QCOMPARE(child1.childIndex(), -1);
    QCOMPARE(child1.parent(), &treeItem);
    QCOMPARE(child2.childIndex(), -1);
    QCOMPARE(child2.parent(), &treeItem);
    QCOMPARE(child3.childIndex(), -1);
    QCOMPARE(child3.parent(), &treeItem);

    QCOMPARE(aboutToBeRemovedSpy.count(), 3);
    assertSignal(aboutToBeRemovedSpy, 1, &child1);
    assertSignal(aboutToBeRemovedSpy, 2, &child3);
    QCOMPARE(removedSpy.count(), 3);
    assertSignal(removedSpy, 1, &child1);
    assertSignal(removedSpy, 2, &child3);
}

/////////////////////////////////// Helpers ////////////////////////////////////

void TreeItemTest::assertAboutToBeInsertedSignal(const QSignalSpy& spy,
                                                 int index, TreeItem* item,
                                                 int itemIndex) const {
    QCOMPARE(spy.at(index).count(), 2);

    QVariant argument = spy.at(index).at(0);
    QCOMPARE(argument.userType(), mTreeItemStarType);
    QCOMPARE(qvariant_cast<TreeItem*>(argument), item);

    argument = spy.at(index).at(1);
    QCOMPARE(argument.type(), QVariant::Int);
    QCOMPARE(argument.toInt(), itemIndex);
}

void TreeItemTest::assertSignal(const QSignalSpy& spy, int index,
                                TreeItem* item) const {
    QCOMPARE(spy.at(index).count(), 1);

    QVariant argument = spy.at(index).at(0);
    QCOMPARE(argument.userType(), mTreeItemStarType);
    QCOMPARE(qvariant_cast<TreeItem*>(argument), item);
}

QTEST_MAIN(TreeItemTest)

#include "TreeItemTest.moc"
