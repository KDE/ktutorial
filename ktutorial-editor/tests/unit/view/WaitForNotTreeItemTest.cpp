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

#include "WaitForNotTreeItem.h"

#include <KLocalizedString>

#include "WaitForSignalTreeItem.h"
#include "../data/WaitForNot.h"
#include "../data/WaitForSignal.h"

class WaitForNotTreeItemTest: public QObject {
Q_OBJECT

private slots:

    void testConstructor();
    void testConstructorFull();

    void testWaitForNotSetNegatedWaitFor();
    void testWaitForNotSetNegatedWaitForChange();
    void testWaitForNotSetNegatedWaitForEmpty();

private:

    void assertSignalItem(TreeItem* item, WaitForSignal* waitForSignal) const;

};

class StubTreeItem: public TreeItem {
public:
    virtual QString text() const {
        return "";
    }
};

void WaitForNotTreeItemTest::testConstructor() {
    WaitForNot waitForNot;

    StubTreeItem parent;
    WaitForNotTreeItem item(&waitForNot, &parent);

    QCOMPARE(item.parent(), &parent);
    QCOMPARE(item.waitFor(), &waitForNot);
    QCOMPARE(item.text(),
             i18nc("@item", "The contained condition can not have been met"));
    QCOMPARE(item.childCount(), 0);
}

void WaitForNotTreeItemTest::testConstructorFull() {
    WaitForNot waitForNot;
    WaitForSignal* waitForSignal = new WaitForSignal();
    waitForNot.setNegatedWaitFor(waitForSignal);

    StubTreeItem parent;
    WaitForNotTreeItem item(&waitForNot, &parent);

    QCOMPARE(item.parent(), &parent);
    QCOMPARE(item.waitFor(), &waitForNot);
    QCOMPARE(item.text(),
             i18nc("@item", "The contained condition can not have been met"));
    QCOMPARE(item.childCount(), 1);
    assertSignalItem(item.child(0), waitForSignal);
}

void WaitForNotTreeItemTest::testWaitForNotSetNegatedWaitFor() {
    WaitForNot waitForNot;
    WaitForSignal* waitForSignal = new WaitForSignal();
    WaitForNotTreeItem item(&waitForNot);

    waitForNot.setNegatedWaitFor(waitForSignal);

    QCOMPARE(item.childCount(), 1);
    assertSignalItem(item.child(0), waitForSignal);
}

void WaitForNotTreeItemTest::testWaitForNotSetNegatedWaitForChange() {
    WaitForNot waitForNot;
    //It will be removed and not deleted by the WaitForNot, so it is created in
    //stack
    WaitForSignal waitForSignal1;
    WaitForNotTreeItem item(&waitForNot);

    waitForNot.setNegatedWaitFor(&waitForSignal1);

    WaitForSignal* waitForSignal2 = new WaitForSignal();
    waitForNot.setNegatedWaitFor(waitForSignal2);

    QCOMPARE(item.childCount(), 1);
    assertSignalItem(item.child(0), waitForSignal2);
}

void WaitForNotTreeItemTest::testWaitForNotSetNegatedWaitForEmpty() {
    WaitForNot waitForNot;
    //It will be removed and not deleted by the WaitForNot, so it is created in
    //stack
    WaitForSignal waitForSignal;
    WaitForNotTreeItem item(&waitForNot);

    waitForNot.setNegatedWaitFor(&waitForSignal);
    waitForNot.setNegatedWaitFor(0);

    QCOMPARE(item.childCount(), 0);
}

/////////////////////////////////// Helpers ////////////////////////////////////

void WaitForNotTreeItemTest::assertSignalItem(TreeItem* item,
                                        WaitForSignal* waitForSignal) const {
    QVERIFY(qobject_cast<WaitForSignalTreeItem*>(item));
    QCOMPARE(static_cast<WaitForSignalTreeItem*>(item)->waitFor(),
             waitForSignal);
}

QTEST_MAIN(WaitForNotTreeItemTest)

#include "WaitForNotTreeItemTest.moc"
