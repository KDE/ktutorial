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

#include "WaitForEventTreeItem.h"

#include <QSignalSpy>

#include <KLocalizedString>

#include "../data/WaitForEvent.h"

class WaitForEventTreeItemTest: public QObject {
Q_OBJECT

private slots:

    void initTestCase();

    void testConstructor();
    void testConstructorFull();

    void testWaitForEventSetReceiverName();
    void testWaitForEventSetReceiverNameChange();

    void testWaitForEventSetEventName();
    void testWaitForEventSetEventNameChange();

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

void WaitForEventTreeItemTest::initTestCase() {
    //TreeItem* must be registered in order to be used with QSignalSpy
    mTreeItemStarType = qRegisterMetaType<TreeItem*>("TreeItem*");
}

void WaitForEventTreeItemTest::testConstructor() {
    WaitForEvent waitForEvent;

    StubTreeItem parent;
    WaitForEventTreeItem item(&waitForEvent, &parent);

    QCOMPARE(item.parent(), &parent);
    QCOMPARE(item.waitFor(), &waitForEvent);
    QCOMPARE(item.text(), i18nc("@item", "When the event (event not set) is "
                                         "received by object (object not "
                                         "set)"));
}

void WaitForEventTreeItemTest::testConstructorFull() {
    WaitForEvent waitForEvent;
    waitForEvent.setReceiverName("receiverName");
    waitForEvent.setEventName("eventName");

    StubTreeItem parent;
    WaitForEventTreeItem item(&waitForEvent, &parent);

    QCOMPARE(item.parent(), &parent);
    QCOMPARE(item.waitFor(), &waitForEvent);
    QCOMPARE(item.text(), i18nc("@item", "When the event \"eventName\" is "
                                         "received by object "
                                         "\"receiverName\""));
}

void WaitForEventTreeItemTest::testWaitForEventSetReceiverName() {
    WaitForEvent waitForEvent;
    waitForEvent.setReceiverName("receiverName");

    WaitForEventTreeItem item(&waitForEvent);

    QCOMPARE(item.text(), i18nc("@item", "When the event (event not set) is "
                                         "received by object \"receiverName\""));
}

void WaitForEventTreeItemTest::testWaitForEventSetReceiverNameChange() {
    WaitForEvent waitForEvent;
    WaitForEventTreeItem item(&waitForEvent);

    waitForEvent.setReceiverName("receiverName");

    QSignalSpy dataChangedSpy(&item, SIGNAL(dataChanged(TreeItem*)));

    waitForEvent.setReceiverName("receiverNameChanged");

    QCOMPARE(item.text(), i18nc("@item", "When the event (event not set) is "
                                         "received by object "
                                         "\"receiverNameChanged\""));
    QCOMPARE(dataChangedSpy.count(), 1);
    assertDataChanged(dataChangedSpy, 0, &item);
}

void WaitForEventTreeItemTest::testWaitForEventSetEventName() {
    WaitForEvent waitForEvent;
    waitForEvent.setEventName("eventName");

    WaitForEventTreeItem item(&waitForEvent);

    QCOMPARE(item.text(), i18nc("@item", "When the event \"eventName\" is "
                                         "received by object (object not "
                                         "set)"));
}

void WaitForEventTreeItemTest::testWaitForEventSetEventNameChange() {
    WaitForEvent waitForEvent;
    WaitForEventTreeItem item(&waitForEvent);

    waitForEvent.setEventName("eventName");

    QSignalSpy dataChangedSpy(&item, SIGNAL(dataChanged(TreeItem*)));

    waitForEvent.setEventName("eventNameChanged");

    QCOMPARE(item.text(), i18nc("@item", "When the event "
                                         "\"eventNameChanged\" is received by "
                                         "object (object not set)"));
    QCOMPARE(dataChangedSpy.count(), 1);
    assertDataChanged(dataChangedSpy, 0, &item);
}

/////////////////////////////////// Helpers ////////////////////////////////////

//TreeItem* must be declared as a metatype to be used in qvariant_cast
Q_DECLARE_METATYPE(TreeItem*);

void WaitForEventTreeItemTest::assertDataChanged(const QSignalSpy& spy,
                                                  int index,
                                                  TreeItem* item) const {
    QCOMPARE(spy.at(index).count(), 1);

    QVariant argument = spy.at(index).at(0);
    QCOMPARE(argument.userType(), mTreeItemStarType);
    QCOMPARE(qvariant_cast<TreeItem*>(argument), item);
}

QTEST_MAIN(WaitForEventTreeItemTest)

#include "WaitForEventTreeItemTest.moc"
