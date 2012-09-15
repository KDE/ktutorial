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

#include "WaitForSignalTreeItem.h"

#include <KLocalizedString>

#include "../data/WaitForSignal.h"

class WaitForSignalTreeItemTest: public QObject {
Q_OBJECT

private slots:

    void initTestCase();

    void testConstructor();
    void testConstructorFull();

    void testWaitForSignalSetEmitterName();
    void testWaitForSignalSetEmitterNameChange();

    void testWaitForSignalSetSignalName();
    void testWaitForSignalSetSignalNameChange();

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

void WaitForSignalTreeItemTest::initTestCase() {
    //TreeItem* must be registered in order to be used with QSignalSpy
    mTreeItemStarType = qRegisterMetaType<TreeItem*>("TreeItem*");
}

void WaitForSignalTreeItemTest::testConstructor() {
    WaitForSignal waitForSignal;

    StubTreeItem parent;
    WaitForSignalTreeItem item(&waitForSignal, &parent);

    QCOMPARE(item.parent(), &parent);
    QCOMPARE(item.waitFor(), &waitForSignal);
    QCOMPARE(item.text(), i18nc("@item", "When the signal (signal not set) is "
                                         "emitted by object (object not set)"));
}

void WaitForSignalTreeItemTest::testConstructorFull() {
    WaitForSignal waitForSignal;
    waitForSignal.setEmitterName("emitterName");
    waitForSignal.setSignalName("signalName");

    StubTreeItem parent;
    WaitForSignalTreeItem item(&waitForSignal, &parent);

    QCOMPARE(item.parent(), &parent);
    QCOMPARE(item.waitFor(), &waitForSignal);
    QCOMPARE(item.text(), i18nc("@item", "When the signal \"signalName\" is "
                                         "emitted by object \"emitterName\""));
}

void WaitForSignalTreeItemTest::testWaitForSignalSetEmitterName() {
    WaitForSignal waitForSignal;
    waitForSignal.setEmitterName("emitterName");

    WaitForSignalTreeItem item(&waitForSignal);

    QCOMPARE(item.text(), i18nc("@item", "When the signal (signal not set) is "
                                         "emitted by object \"emitterName\""));
}

void WaitForSignalTreeItemTest::testWaitForSignalSetEmitterNameChange() {
    WaitForSignal waitForSignal;
    WaitForSignalTreeItem item(&waitForSignal);

    waitForSignal.setEmitterName("emitterName");

    QSignalSpy dataChangedSpy(&item, SIGNAL(dataChanged(TreeItem*)));

    waitForSignal.setEmitterName("emitterNameChanged");

    QCOMPARE(item.text(), i18nc("@item", "When the signal (signal not set) is "
                                         "emitted by object "
                                         "\"emitterNameChanged\""));
    QCOMPARE(dataChangedSpy.count(), 1);
    assertDataChanged(dataChangedSpy, 0, &item);
}

void WaitForSignalTreeItemTest::testWaitForSignalSetSignalName() {
    WaitForSignal waitForSignal;
    waitForSignal.setSignalName("signalName");

    WaitForSignalTreeItem item(&waitForSignal);

    QCOMPARE(item.text(), i18nc("@item", "When the signal \"signalName\" is "
                                         "emitted by object (object not set)"));
}

void WaitForSignalTreeItemTest::testWaitForSignalSetSignalNameChange() {
    WaitForSignal waitForSignal;
    WaitForSignalTreeItem item(&waitForSignal);

    waitForSignal.setSignalName("signalName");

    QSignalSpy dataChangedSpy(&item, SIGNAL(dataChanged(TreeItem*)));

    waitForSignal.setSignalName("signalNameChanged");

    QCOMPARE(item.text(), i18nc("@item", "When the signal "
                                         "\"signalNameChanged\" is emitted by "
                                         "object (object not set)"));
    QCOMPARE(dataChangedSpy.count(), 1);
    assertDataChanged(dataChangedSpy, 0, &item);
}

/////////////////////////////////// Helpers ////////////////////////////////////

//TreeItem* must be declared as a metatype to be used in qvariant_cast
Q_DECLARE_METATYPE(TreeItem*);

void WaitForSignalTreeItemTest::assertDataChanged(const QSignalSpy& spy,
                                                  int index,
                                                  TreeItem* item) const {
    QCOMPARE(spy.at(index).count(), 1);

    QVariant argument = spy.at(index).at(0);
    QCOMPARE(argument.userType(), mTreeItemStarType);
    QCOMPARE(qvariant_cast<TreeItem*>(argument), item);
}

QTEST_MAIN(WaitForSignalTreeItemTest)

#include "WaitForSignalTreeItemTest.moc"
