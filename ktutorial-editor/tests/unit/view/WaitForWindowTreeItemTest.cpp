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

#include "WaitForWindowTreeItem.h"

#include <QSignalSpy>

#include <KLocalizedString>

#include "../data/WaitForWindow.h"

class WaitForWindowTreeItemTest: public QObject {
Q_OBJECT

private slots:

    void initTestCase();

    void testConstructor();
    void testConstructorFull();

    void testWaitForWindowSetWindowObjectName();
    void testWaitForWindowSetWindowObjectNameChange();

private:

    int mTreeItemStarType;

    void assertDataChanged(const QSignalSpy& spy, int index,
                           TreeItem* item) const;

};

class StubTreeItem: public TreeItem {
public:
    virtual QString text() const {
        return "";
    }
};

void WaitForWindowTreeItemTest::initTestCase() {
    //TreeItem* must be registered in order to be used with QSignalSpy
    mTreeItemStarType = qRegisterMetaType<TreeItem*>("TreeItem*");
}

void WaitForWindowTreeItemTest::testConstructor() {
    WaitForWindow waitForWindow;

    StubTreeItem parent;
    WaitForWindowTreeItem item(&waitForWindow, &parent);

    QCOMPARE(item.parent(), &parent);
    QCOMPARE(item.waitFor(), &waitForWindow);
    QCOMPARE(item.text(), i18nc("@item", "When the window (object name not "
                                         "set) is shown"));
}

void WaitForWindowTreeItemTest::testConstructorFull() {
    WaitForWindow waitForWindow;
    waitForWindow.setWindowObjectName("windowObjectName");

    StubTreeItem parent;
    WaitForWindowTreeItem item(&waitForWindow, &parent);

    QCOMPARE(item.parent(), &parent);
    QCOMPARE(item.waitFor(), &waitForWindow);
    QCOMPARE(item.text(), i18nc("@item", "When the window \"windowObjectName\" "
                                         "is shown"));
}

void WaitForWindowTreeItemTest::testWaitForWindowSetWindowObjectName() {
    WaitForWindow waitForWindow;
    waitForWindow.setWindowObjectName("windowObjectName");

    WaitForWindowTreeItem item(&waitForWindow);

    QCOMPARE(item.text(), i18nc("@item", "When the window \"windowObjectName\" "
                                         "is shown"));
}

void WaitForWindowTreeItemTest::testWaitForWindowSetWindowObjectNameChange() {
    WaitForWindow waitForWindow;
    WaitForWindowTreeItem item(&waitForWindow);

    waitForWindow.setWindowObjectName("windowObjectName");

    QSignalSpy dataChangedSpy(&item, SIGNAL(dataChanged(TreeItem*)));

    waitForWindow.setWindowObjectName("windowObjectNameChanged");

    QCOMPARE(item.text(), i18nc("@item", "When the window "
                                         "\"windowObjectNameChanged\" is "
                                         "shown"));
    QCOMPARE(dataChangedSpy.count(), 1);
    assertDataChanged(dataChangedSpy, 0, &item);
}

/////////////////////////////////// Helpers ////////////////////////////////////

//TreeItem* must be declared as a metatype to be used in qvariant_cast
Q_DECLARE_METATYPE(TreeItem*);

void WaitForWindowTreeItemTest::assertDataChanged(const QSignalSpy& spy,
                                                  int index,
                                                  TreeItem* item) const {
    QCOMPARE(spy.at(index).count(), 1);

    QVariant argument = spy.at(index).at(0);
    QCOMPARE(argument.userType(), mTreeItemStarType);
    QCOMPARE(qvariant_cast<TreeItem*>(argument), item);
}

QTEST_MAIN(WaitForWindowTreeItemTest)

#include "WaitForWindowTreeItemTest.moc"
