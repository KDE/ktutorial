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

#include "WaitForComposedTreeItem.h"

#include <QSignalSpy>

#include <KLocalizedString>

#include "WaitForSignalTreeItem.h"
#include "../data/WaitForComposed.h"
#include "../data/WaitForSignal.h"

class WaitForComposedTreeItemTest: public QObject {
Q_OBJECT

private slots:

    void initTestCase();

    void testConstructor();
    void testConstructorFull();

    void testWaitForComposedSetAndType();
    void testWaitForComposedSetOrType();
    void testWaitForComposedSetTypeChange();

    void testWaitForAddWaitFor();

    void testWaitForRemoveWaitFor();

private:

    int mTreeItemStarType;

    void assertSignalItem(TreeItem* item, WaitForSignal* waitForSignal) const;

    void assertDataChanged(const QSignalSpy& spy, int index,
                           TreeItem* item) const;

};

class StubTreeItem: public TreeItem {
public:
    virtual QString text() const {
        return "";
    }
};

void WaitForComposedTreeItemTest::initTestCase() {
    //TreeItem* must be registered in order to be used with QSignalSpy
    mTreeItemStarType = qRegisterMetaType<TreeItem*>("TreeItem*");
}

void WaitForComposedTreeItemTest::testConstructor() {
    WaitForComposed waitForComposed;

    StubTreeItem parent;
    WaitForComposedTreeItem item(&waitForComposed, &parent);

    QCOMPARE(item.parent(), &parent);
    QCOMPARE(item.waitFor(), &waitForComposed);
    QCOMPARE(item.childCount(), 0);
}

void WaitForComposedTreeItemTest::testConstructorFull() {
    WaitForComposed waitForComposed;

    WaitForSignal* waitFor1 = new WaitForSignal();
    waitForComposed.addWaitFor(waitFor1);

    WaitForSignal* waitFor2 = new WaitForSignal();
    waitForComposed.addWaitFor(waitFor2);

    WaitForSignal* waitFor3 = new WaitForSignal();
    waitForComposed.addWaitFor(waitFor3);

    StubTreeItem parent;
    WaitForComposedTreeItem item(&waitForComposed, &parent);

    QCOMPARE(item.parent(), &parent);
    QCOMPARE(item.waitFor(), &waitForComposed);
    QCOMPARE(item.childCount(), 3);
    assertSignalItem(item.child(0), waitFor1);
    assertSignalItem(item.child(1), waitFor2);
    assertSignalItem(item.child(2), waitFor3);
}

void WaitForComposedTreeItemTest::testWaitForComposedSetAndType() {
    WaitForComposed waitForComposed;
    waitForComposed.setCompositionType(WaitForComposed::And);

    WaitForComposedTreeItem item(&waitForComposed);

    QCOMPARE(item.text(), i18nc("@item",
                                "When all the contained conditions match"));
    QCOMPARE(item.childCount(), 0);
}

void WaitForComposedTreeItemTest::testWaitForComposedSetOrType() {
    WaitForComposed waitForComposed;
    waitForComposed.setCompositionType(WaitForComposed::Or);

    WaitForComposedTreeItem item(&waitForComposed);

    QCOMPARE(item.text(), i18nc("@item",
                                "When any of the contained conditions match"));
    QCOMPARE(item.childCount(), 0);
}

void WaitForComposedTreeItemTest::testWaitForComposedSetTypeChange() {
    WaitForComposed waitForComposed;
    waitForComposed.setCompositionType(WaitForComposed::And);

    WaitForComposedTreeItem item(&waitForComposed);

    QSignalSpy dataChangedSpy(&item, SIGNAL(dataChanged(TreeItem*)));

    waitForComposed.setCompositionType(WaitForComposed::Or);

    QCOMPARE(item.text(), i18nc("@item",
                                "When any of the contained conditions match"));
    QCOMPARE(item.childCount(), 0);
    QCOMPARE(dataChangedSpy.count(), 1);
    assertDataChanged(dataChangedSpy, 0, &item);
}

void WaitForComposedTreeItemTest::testWaitForAddWaitFor() {
    WaitForComposed waitForComposed;
    WaitForSignal* waitFor1 = new WaitForSignal();
    WaitForSignal* waitFor2 = new WaitForSignal();
    WaitForSignal* waitFor3 = new WaitForSignal();

    WaitForComposedTreeItem item(&waitForComposed);

    waitForComposed.addWaitFor(waitFor1);
    waitForComposed.addWaitFor(waitFor2);
    waitForComposed.addWaitFor(waitFor3);

    QCOMPARE(item.childCount(), 3);
    assertSignalItem(item.child(0), waitFor1);
    assertSignalItem(item.child(1), waitFor2);
    assertSignalItem(item.child(2), waitFor3);
}

void WaitForComposedTreeItemTest::testWaitForRemoveWaitFor() {
    WaitForComposed waitForComposed;

    //They will be removed and not deleted by the WaitForComposed, so they are
    //created in stack
    WaitForSignal waitFor1;
    WaitForSignal waitFor2;
    WaitForSignal waitFor3;

    WaitForComposedTreeItem item(&waitForComposed);

    waitForComposed.addWaitFor(&waitFor1);
    waitForComposed.addWaitFor(&waitFor2);
    waitForComposed.addWaitFor(&waitFor3);

    waitForComposed.removeWaitFor(&waitFor2);

    QCOMPARE(item.childCount(), 2);
    assertSignalItem(item.child(0), &waitFor1);
    assertSignalItem(item.child(1), &waitFor3);

    waitForComposed.removeWaitFor(&waitFor1);
    waitForComposed.removeWaitFor(&waitFor3);

    QCOMPARE(item.childCount(), 0);
}

/////////////////////////////////// Helpers ////////////////////////////////////

void WaitForComposedTreeItemTest::assertSignalItem(TreeItem* item,
                                        WaitForSignal* waitForSignal) const {
    QVERIFY(qobject_cast<WaitForSignalTreeItem*>(item));
    QCOMPARE(static_cast<WaitForSignalTreeItem*>(item)->waitFor(),
             waitForSignal);
}

//TreeItem* must be declared as a metatype to be used in qvariant_cast
Q_DECLARE_METATYPE(TreeItem*);

void WaitForComposedTreeItemTest::assertDataChanged(const QSignalSpy& spy,
                                                    int index,
                                                    TreeItem* item) const {
    QCOMPARE(spy.at(index).count(), 1);

    QVariant argument = spy.at(index).at(0);
    QCOMPARE(argument.userType(), mTreeItemStarType);
    QCOMPARE(qvariant_cast<TreeItem*>(argument), item);
}

QTEST_MAIN(WaitForComposedTreeItemTest)

#include "WaitForComposedTreeItemTest.moc"
