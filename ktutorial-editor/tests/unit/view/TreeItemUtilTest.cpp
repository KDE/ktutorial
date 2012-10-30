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

#include "TreeItemUtil.h"

#include "TextTreeItem.h"

class TreeItemUtilTest: public QObject {
Q_OBJECT

private slots:

    void testAddFlatItem();
    void testAddFlatItemAlreadyAdded();

    void testRemoveFlatItem();
    void testRemoveFlatItemAlreadyRemoved();

    void testAddNestedItem();
    void testAddNestedItemAlreadyAdded();

    void testRemoveNestedItem();
    void testRemoveNestedItemAlreadyRemoved();

};

class StubTreeItem: public TreeItem {
public:
    virtual QString text() const {
        return "";
    }
};

void TreeItemUtilTest::testAddFlatItem() {
    StubTreeItem parent;
    TreeItem* lastItem = new TextTreeItem(&parent);
    parent.appendChild(lastItem);

    TextTreeItem* item = 0;

    TreeItemUtil::addFlatItemIfNeeded(&parent, item, 0);

    QVERIFY(item);
    QCOMPARE(item->parent(), &parent);
    QCOMPARE(parent.childCount(), 2);
    QCOMPARE(parent.child(0), item);
    QCOMPARE(parent.child(1), lastItem);
}

void TreeItemUtilTest::testAddFlatItemAlreadyAdded() {
    StubTreeItem parent;
    TreeItem* lastItem = new TextTreeItem(&parent);
    parent.appendChild(lastItem);

    TextTreeItem* item = 0;

    TreeItemUtil::addFlatItemIfNeeded(&parent, item, 0);
    TreeItemUtil::addFlatItemIfNeeded(&parent, item, 0);

    QVERIFY(item);
    QCOMPARE(item->parent(), &parent);
    QCOMPARE(parent.childCount(), 2);
    QCOMPARE(parent.child(0), item);
    QCOMPARE(parent.child(1), lastItem);
}

void TreeItemUtilTest::testRemoveFlatItem() {
    StubTreeItem parent;
    TreeItem* lastItem = new TextTreeItem(&parent);
    parent.appendChild(lastItem);

    TextTreeItem* item = new TextTreeItem(&parent);
    parent.insertChild(item, 0);

    TreeItemUtil::removeFlatItemIfNeeded(item);

    QVERIFY(!item);
    QCOMPARE(parent.childCount(), 1);
    QCOMPARE(parent.child(0), lastItem);
}

void TreeItemUtilTest::testRemoveFlatItemAlreadyRemoved() {
    StubTreeItem parent;
    TreeItem* lastItem = new TextTreeItem(&parent);
    parent.appendChild(lastItem);

    TextTreeItem* item = new TextTreeItem(&parent);
    parent.insertChild(item, 0);

    TreeItemUtil::removeFlatItemIfNeeded(item);
    TreeItemUtil::removeFlatItemIfNeeded(item);

    QVERIFY(!item);
    QCOMPARE(parent.childCount(), 1);
    QCOMPARE(parent.child(0), lastItem);
}

void TreeItemUtilTest::testAddNestedItem() {
    StubTreeItem parent;
    TreeItem* lastItem = new TextTreeItem(&parent);
    parent.appendChild(lastItem);

    TextTreeItem* item = 0;

    TreeItemUtil::addNestedItemIfNeeded(&parent, item, 0, "Parent text");

    QVERIFY(item);
    QCOMPARE(parent.childCount(), 2);
    QCOMPARE(item->parent(), parent.child(0));
    QCOMPARE(parent.child(0)->parent(), &parent);
    QCOMPARE(parent.child(0)->text(), QString("Parent text"));
    QCOMPARE(parent.child(0)->childCount(), 1);
    QCOMPARE(parent.child(0)->child(0), item);
    QCOMPARE(parent.child(1), lastItem);
}

void TreeItemUtilTest::testAddNestedItemAlreadyAdded() {
    StubTreeItem parent;
    TreeItem* lastItem = new TextTreeItem(&parent);
    parent.appendChild(lastItem);

    TextTreeItem* item = 0;

    TreeItemUtil::addNestedItemIfNeeded(&parent, item, 0, "Parent text");
    TreeItemUtil::addNestedItemIfNeeded(&parent, item, 0, "Parent text2");

    QVERIFY(item);
    QCOMPARE(parent.childCount(), 2);
    QCOMPARE(item->parent(), parent.child(0));
    QCOMPARE(parent.child(0)->parent(), &parent);
    QCOMPARE(parent.child(0)->text(), QString("Parent text"));
    QCOMPARE(parent.child(0)->childCount(), 1);
    QCOMPARE(parent.child(0)->child(0), item);
    QCOMPARE(parent.child(1), lastItem);
}

void TreeItemUtilTest::testRemoveNestedItem() {
    StubTreeItem parent;
    TreeItem* lastItem = new TextTreeItem(&parent);
    parent.appendChild(lastItem);

    TextTreeItem* item = new TextTreeItem(&parent);
    parent.insertChild(item, 0);
    TextTreeItem* childItem = new TextTreeItem(item);
    item->appendChild(childItem);

    TreeItemUtil::removeNestedItemIfNeeded(childItem);

    QVERIFY(!childItem);
    QCOMPARE(parent.childCount(), 1);
    QCOMPARE(parent.child(0), lastItem);
}

void TreeItemUtilTest::testRemoveNestedItemAlreadyRemoved() {
    StubTreeItem parent;
    TreeItem* lastItem = new TextTreeItem(&parent);
    parent.appendChild(lastItem);

    TextTreeItem* item = new TextTreeItem(&parent);
    parent.insertChild(item, 0);
    TextTreeItem* childItem = new TextTreeItem(item);
    item->appendChild(childItem);

    TreeItemUtil::removeNestedItemIfNeeded(childItem);
    TreeItemUtil::removeNestedItemIfNeeded(childItem);

    QVERIFY(!childItem);
    QCOMPARE(parent.childCount(), 1);
    QCOMPARE(parent.child(0), lastItem);
}

QTEST_MAIN(TreeItemUtilTest)

#include "TreeItemUtilTest.moc"
