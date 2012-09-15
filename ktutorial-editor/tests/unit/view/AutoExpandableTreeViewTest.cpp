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

#include "AutoExpandableTreeView.h"

#include "TextTreeItem.h"
#include "TreeModel.h"

class AutoExpandableTreeViewTest: public QObject {
Q_OBJECT

private slots:

    void testConstructor();

    void testInsertRootItemChild();
    void testInsertDeepNestedChildParentExpanded();
    void testInsertDeepNestedChildParentNotExpanded();

    void testChangeDeepNestedChild();

};

void AutoExpandableTreeViewTest::testConstructor() {
    QWidget parent;
    AutoExpandableTreeView* view = new AutoExpandableTreeView(&parent);

    QCOMPARE(view->parentWidget(), &parent);
}

void AutoExpandableTreeViewTest::testInsertRootItemChild() {
    AutoExpandableTreeView view;
    TextTreeItem* rootItem = new TextTreeItem();
    TreeModel model(rootItem);
    view.setModel(&model);

    TextTreeItem* item1 = new TextTreeItem(rootItem);
    TextTreeItem* item1_1 = new TextTreeItem(item1);
    TextTreeItem* item1_1_1 = new TextTreeItem(item1_1);
    item1_1->appendChild(item1_1_1);
    item1->appendChild(item1_1);
    TextTreeItem* item1_2 = new TextTreeItem(item1);
    TextTreeItem* item1_2_1 = new TextTreeItem(item1_2);
    item1_2->appendChild(item1_2_1);
    TextTreeItem* item1_2_2 = new TextTreeItem(item1_2);
    TextTreeItem* item1_2_2_1 = new TextTreeItem(item1_2_2);
    item1_2_2->appendChild(item1_2_2_1);
    TextTreeItem* item1_2_2_2 = new TextTreeItem(item1_2_2);
    item1_2_2->appendChild(item1_2_2_2);
    item1_2->appendChild(item1_2_2);
    item1->appendChild(item1_2);

    rootItem->appendChild(item1);

    QModelIndex index1 = model.index(0, 0);
    QVERIFY(view.isExpanded(index1));
    QModelIndex index1_1 = model.index(0, 0, index1);
    QVERIFY(view.isExpanded(index1_1));
    QModelIndex index1_2 = model.index(1, 0, index1);
    QVERIFY(view.isExpanded(index1_2));
    QModelIndex index1_2_2 = model.index(1, 0, index1_2);
    QVERIFY(view.isExpanded(index1_2_2));
}

void AutoExpandableTreeViewTest::testInsertDeepNestedChildParentExpanded() {
    AutoExpandableTreeView view;
    TextTreeItem* rootItem = new TextTreeItem();
    TreeModel model(rootItem);
    view.setModel(&model);

    TextTreeItem* item1 = new TextTreeItem(rootItem);
    TextTreeItem* item1_1 = new TextTreeItem(item1);
    TextTreeItem* item1_1_1 = new TextTreeItem(item1_1);
    item1_1->appendChild(item1_1_1);
    item1->appendChild(item1_1);
    TextTreeItem* item1_2 = new TextTreeItem(item1);
    TextTreeItem* item1_2_1 = new TextTreeItem(item1_2);
    item1_2->appendChild(item1_2_1);
    item1->appendChild(item1_2);
    rootItem->appendChild(item1);

    QModelIndex index1 = model.index(0, 0);
    view.setExpanded(index1, true);
    //Collapse sibling of parent to check that it is not expanded again
    QModelIndex index1_1 = model.index(0, 0, index1);
    view.setExpanded(index1_1, false);
    QModelIndex index1_2 = model.index(1, 0, index1);
    view.setExpanded(index1_2, true);

    TextTreeItem* item1_2_2 = new TextTreeItem(item1_2);
    TextTreeItem* item1_2_2_1 = new TextTreeItem(item1_2_2);
    TextTreeItem* item1_2_2_1_1 = new TextTreeItem(item1_2_2_1);
    item1_2_2_1->appendChild(item1_2_2_1_1);
    TextTreeItem* item1_2_2_1_2 = new TextTreeItem(item1_2_2_1);
    item1_2_2_1->appendChild(item1_2_2_1_2);
    item1_2_2->appendChild(item1_2_2_1);
    TextTreeItem* item1_2_2_2 = new TextTreeItem(item1_2_2);
    item1_2_2->appendChild(item1_2_2_2);
    item1_2->appendChild(item1_2_2);

    QVERIFY(view.isExpanded(index1));
    QVERIFY(!view.isExpanded(index1_1));
    QVERIFY(view.isExpanded(index1_2));
    QModelIndex index1_2_2 = model.index(1, 0, index1_2);
    QVERIFY(view.isExpanded(index1_2_2));
    QModelIndex index1_2_2_1 = model.index(0, 0, index1_2_2);
    QVERIFY(view.isExpanded(index1_2_2_1));
}

void AutoExpandableTreeViewTest::testInsertDeepNestedChildParentNotExpanded() {
    AutoExpandableTreeView view;
    TextTreeItem* rootItem = new TextTreeItem();
    TreeModel model(rootItem);
    view.setModel(&model);

    TextTreeItem* item1 = new TextTreeItem(rootItem);
    TextTreeItem* item1_1 = new TextTreeItem(item1);
    TextTreeItem* item1_1_1 = new TextTreeItem(item1_1);
    item1_1->appendChild(item1_1_1);
    item1->appendChild(item1_1);
    TextTreeItem* item1_2 = new TextTreeItem(item1);
    TextTreeItem* item1_2_1 = new TextTreeItem(item1_2);
    item1_2->appendChild(item1_2_1);
    item1->appendChild(item1_2);
    rootItem->appendChild(item1);

    QModelIndex index1 = model.index(0, 0);
    view.setExpanded(index1, false);
    //Collapse sibling of parent to check that it is not expanded again
    QModelIndex index1_1 = model.index(0, 0, index1);
    view.setExpanded(index1_1, false);
    QModelIndex index1_2 = model.index(1, 0, index1);
    view.setExpanded(index1_2, false);

    TextTreeItem* item1_2_2 = new TextTreeItem(item1_2);
    TextTreeItem* item1_2_2_1 = new TextTreeItem(item1_2_2);
    TextTreeItem* item1_2_2_1_1 = new TextTreeItem(item1_2_2_1);
    item1_2_2_1->appendChild(item1_2_2_1_1);
    TextTreeItem* item1_2_2_1_2 = new TextTreeItem(item1_2_2_1);
    item1_2_2_1->appendChild(item1_2_2_1_2);
    item1_2_2->appendChild(item1_2_2_1);
    TextTreeItem* item1_2_2_2 = new TextTreeItem(item1_2_2);
    item1_2_2->appendChild(item1_2_2_2);

    item1_2->appendChild(item1_2_2);

    QVERIFY(view.isExpanded(index1));
    QVERIFY(!view.isExpanded(index1_1));
    QVERIFY(view.isExpanded(index1_2));
    QModelIndex index1_2_2 = model.index(1, 0, index1_2);
    QVERIFY(view.isExpanded(index1_2_2));
    QModelIndex index1_2_2_1 = model.index(0, 0, index1_2_2);
    QVERIFY(view.isExpanded(index1_2_2_1));
}

void AutoExpandableTreeViewTest::testChangeDeepNestedChild() {
    AutoExpandableTreeView view;
    TextTreeItem* rootItem = new TextTreeItem();
    TreeModel model(rootItem);
    view.setModel(&model);

    TextTreeItem* item1 = new TextTreeItem(rootItem);
    TextTreeItem* item1_1 = new TextTreeItem(item1);
    TextTreeItem* item1_1_1 = new TextTreeItem(item1_1);
    item1_1->appendChild(item1_1_1);
    item1->appendChild(item1_1);
    TextTreeItem* item1_2 = new TextTreeItem(item1);
    TextTreeItem* item1_2_1 = new TextTreeItem(item1_2);
    item1_2->appendChild(item1_2_1);
    TextTreeItem* item1_2_2 = new TextTreeItem(item1_2);
    TextTreeItem* item1_2_2_1 = new TextTreeItem(item1_2_2);
    TextTreeItem* item1_2_2_1_1 = new TextTreeItem(item1_2_2_1);
    item1_2_2_1->appendChild(item1_2_2_1_1);
    TextTreeItem* item1_2_2_1_2 = new TextTreeItem(item1_2_2_1);
    item1_2_2_1->appendChild(item1_2_2_1_2);
    item1_2_2->appendChild(item1_2_2_1);
    TextTreeItem* item1_2_2_2 = new TextTreeItem(item1_2_2);
    item1_2_2->appendChild(item1_2_2_2);
    item1_2->appendChild(item1_2_2);
    item1->appendChild(item1_2);
    rootItem->appendChild(item1);

    QModelIndex index1 = model.index(0, 0);
    view.setExpanded(index1, false);
    //Collapse sibling of parent to check that it is not expanded again
    QModelIndex index1_1 = model.index(0, 0, index1);
    view.setExpanded(index1_1, false);
    QModelIndex index1_2 = model.index(1, 0, index1);
    view.setExpanded(index1_2, false);
    QModelIndex index1_2_2 = model.index(1, 0, index1_2);
    view.setExpanded(index1_2_2, false);
    QModelIndex index1_2_2_1 = model.index(0, 0, index1_2_2);
    view.setExpanded(index1_2_2_1, false);

    item1_2_2_1->setText("New text");

    QVERIFY(view.isExpanded(index1));
    QVERIFY(!view.isExpanded(index1_1));
    QVERIFY(view.isExpanded(index1_2));
    QVERIFY(view.isExpanded(index1_2_2));
    QVERIFY(!view.isExpanded(index1_2_2_1));
}

QTEST_MAIN(AutoExpandableTreeViewTest)

#include "AutoExpandableTreeViewTest.moc"
