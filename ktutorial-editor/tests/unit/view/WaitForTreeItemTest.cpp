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

#include "WaitForTreeItem.h"

#include "WaitForComposedTreeItem.h"
#include "WaitForEventTreeItem.h"
#include "WaitForNotTreeItem.h"
#include "WaitForPropertyTreeItem.h"
#include "WaitForSignalTreeItem.h"
#include "WaitForStepActivationTreeItem.h"
#include "WaitForWindowTreeItem.h"
#include "../data/WaitFor.h"
#include "../data/WaitForComposed.h"
#include "../data/WaitForEvent.h"
#include "../data/WaitForNot.h"
#include "../data/WaitForProperty.h"
#include "../data/WaitForSignal.h"
#include "../data/WaitForStepActivation.h"
#include "../data/WaitForWindow.h"

class WaitForTreeItemTest: public QObject {
Q_OBJECT

private slots:

    void testConstructor();

    void testTreeItemForWaitForComposed();
    void testTreeItemForWaitForEvent();
    void testTreeItemForWaitForNot();
    void testTreeItemForWaitForProperty();
    void testTreeItemForWaitForSignal();
    void testTreeItemForWaitForStepActivation();
    void testTreeItemForWaitForWindow();

};

class StubTreeItem: public TreeItem {
public:
    virtual QString text() const {
        return "";
    }
};

class StubWaitForTreeItem: public WaitForTreeItem {
public:
    StubWaitForTreeItem(WaitFor* waitFor, TreeItem* parent = 0):
            WaitForTreeItem(waitFor, parent) {
    }

    virtual QString text() const {
        return "";
    }
};

class StubWaitFor: public WaitFor {
public:
    StubWaitFor(QObject* parent = 0): WaitFor(parent) {
    }

    virtual WaitFor* clone() const {
        return 0;
    }

    virtual bool equals(const WaitFor& waitFor) const {
        Q_UNUSED(waitFor);
        return false;
    }
};

void WaitForTreeItemTest::testConstructor() {
    StubWaitFor waitFor;

    StubTreeItem parent;
    StubWaitForTreeItem item(&waitFor, &parent);

    QCOMPARE(item.parent(), &parent);
    QCOMPARE(item.waitFor(), &waitFor);
    QCOMPARE(item.childCount(), 0);
}

void WaitForTreeItemTest::testTreeItemForWaitForComposed() {
    WaitForComposed waitFor;
    StubTreeItem parent;

    WaitForTreeItem* item = WaitForTreeItem::treeItemForWaitFor(&waitFor,
                                                                &parent);

    QVERIFY(qobject_cast<WaitForComposedTreeItem*>(item));
    QCOMPARE(item->parent(), &parent);
    QCOMPARE(item->waitFor(), &waitFor);

    delete item;
}

void WaitForTreeItemTest::testTreeItemForWaitForEvent() {
    WaitForEvent waitFor;
    StubTreeItem parent;

    WaitForTreeItem* item = WaitForTreeItem::treeItemForWaitFor(&waitFor,
                                                                &parent);

    QVERIFY(qobject_cast<WaitForEventTreeItem*>(item));
    QCOMPARE(item->parent(), &parent);
    QCOMPARE(item->waitFor(), &waitFor);

    delete item;
}

void WaitForTreeItemTest::testTreeItemForWaitForNot() {
    WaitForNot waitFor;
    StubTreeItem parent;

    WaitForTreeItem* item = WaitForTreeItem::treeItemForWaitFor(&waitFor,
                                                                &parent);

    QVERIFY(qobject_cast<WaitForNotTreeItem*>(item));
    QCOMPARE(item->parent(), &parent);
    QCOMPARE(item->waitFor(), &waitFor);

    delete item;
}

void WaitForTreeItemTest::testTreeItemForWaitForProperty() {
    WaitForProperty waitFor;
    StubTreeItem parent;

    WaitForTreeItem* item = WaitForTreeItem::treeItemForWaitFor(&waitFor,
                                                                &parent);

    QVERIFY(qobject_cast<WaitForPropertyTreeItem*>(item));
    QCOMPARE(item->parent(), &parent);
    QCOMPARE(item->waitFor(), &waitFor);

    delete item;
}

void WaitForTreeItemTest::testTreeItemForWaitForSignal() {
    WaitForSignal waitFor;
    StubTreeItem parent;

    WaitForTreeItem* item = WaitForTreeItem::treeItemForWaitFor(&waitFor,
                                                                &parent);

    QVERIFY(qobject_cast<WaitForSignalTreeItem*>(item));
    QCOMPARE(item->parent(), &parent);
    QCOMPARE(item->waitFor(), &waitFor);

    delete item;
}

void WaitForTreeItemTest::testTreeItemForWaitForStepActivation() {
    WaitForStepActivation waitFor;
    StubTreeItem parent;

    WaitForTreeItem* item = WaitForTreeItem::treeItemForWaitFor(&waitFor,
                                                                &parent);

    QVERIFY(qobject_cast<WaitForStepActivationTreeItem*>(item));
    QCOMPARE(item->parent(), &parent);
    QCOMPARE(item->waitFor(), &waitFor);

    delete item;
}

void WaitForTreeItemTest::testTreeItemForWaitForWindow() {
    WaitForWindow waitFor;
    StubTreeItem parent;

    WaitForTreeItem* item = WaitForTreeItem::treeItemForWaitFor(&waitFor,
                                                                &parent);

    QVERIFY(qobject_cast<WaitForWindowTreeItem*>(item));
    QCOMPARE(item->parent(), &parent);
    QCOMPARE(item->waitFor(), &waitFor);

    delete item;
}

QTEST_MAIN(WaitForTreeItemTest)

#include "WaitForTreeItemTest.moc"
