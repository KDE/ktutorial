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

#include "WaitForComposed.h"

#include <QSignalSpy>

class WaitForComposedTest: public QObject {
Q_OBJECT

private slots:

    void initTestCase();

    void testConstructor();

    void testClone();
    void testCloneEmpty();

    void testEquals();

    void testSetCompositionType();

    void testAddWaitFor();
    void testRemoveWaitFor();

private:

    int mWaitForStarType;

    void assertWaitForSignal(const QSignalSpy& spy, int index,
                             WaitFor* waitFor);

};

class StubWaitFor: public WaitFor {
Q_OBJECT
public:

    int mValue;

    StubWaitFor(int value = 0): mValue(value) {
    }

    virtual WaitFor* clone() const {
        return new StubWaitFor(mValue);
    }

    virtual bool equals(const WaitFor& waitFor) const {
        if (!qobject_cast<const StubWaitFor*>(&waitFor)) {
            return false;
        }

        return mValue == static_cast<const StubWaitFor*>(&waitFor)->mValue;
    }
};

void WaitForComposedTest::initTestCase() {
    //WaitFor* must be registered in order to be used with QSignalSpy
    mWaitForStarType = qRegisterMetaType<WaitFor*>("WaitFor*");
}

void WaitForComposedTest::testConstructor() {
    QObject parent;
    WaitForComposed* waitForComposed = new WaitForComposed(&parent);

    QCOMPARE(waitForComposed->parent(), &parent);
    QCOMPARE(waitForComposed->compositionType(), WaitForComposed::And);
}

void WaitForComposedTest::testClone() {
    WaitForComposed waitForComposed;
    waitForComposed.setCompositionType(WaitForComposed::Or);
    StubWaitFor* waitFor1 = new StubWaitFor(4);
    waitForComposed.addWaitFor(waitFor1);
    StubWaitFor* waitFor2 = new StubWaitFor(8);
    waitForComposed.addWaitFor(waitFor2);
    StubWaitFor* waitFor3 = new StubWaitFor(15);
    waitForComposed.addWaitFor(waitFor3);

    WaitForComposed* cloned =
                        static_cast<WaitForComposed*>(waitForComposed.clone());

    QVERIFY(cloned != &waitForComposed);
    QCOMPARE(cloned->compositionType(), WaitForComposed::Or);
    QCOMPARE(cloned->waitFors().count(), 3);
    QVERIFY(cloned->waitFors()[0] != waitFor1);
    QVERIFY(*cloned->waitFors()[0] == *waitFor1);
    QVERIFY(cloned->waitFors()[1] != waitFor2);
    QVERIFY(*cloned->waitFors()[1] == *waitFor2);
    QVERIFY(cloned->waitFors()[2] != waitFor3);
    QVERIFY(*cloned->waitFors()[2] == *waitFor3);
    delete cloned;
}

void WaitForComposedTest::testCloneEmpty() {
    WaitForComposed waitForComposed;
    waitForComposed.setCompositionType(WaitForComposed::Or);

    WaitForComposed* cloned =
                        static_cast<WaitForComposed*>(waitForComposed.clone());

    QVERIFY(cloned != &waitForComposed);
    QCOMPARE(cloned->compositionType(), WaitForComposed::Or);
    QCOMPARE(cloned->waitFors().count(), 0);
    delete cloned;
}

void WaitForComposedTest::testEquals() {
    WaitForComposed waitForComposed1;
    WaitForComposed waitForComposed2;

    QCOMPARE(waitForComposed1 == waitForComposed2, true);

    waitForComposed1.setCompositionType(WaitForComposed::Or);

    QCOMPARE(waitForComposed1 == waitForComposed2, false);
    QCOMPARE(waitForComposed2 == waitForComposed1, false);

    waitForComposed2.setCompositionType(WaitForComposed::Or);

    QCOMPARE(waitForComposed1 == waitForComposed2, true);

    StubWaitFor* waitFor1_1 = new StubWaitFor(4);
    waitForComposed1.addWaitFor(waitFor1_1);
    StubWaitFor* waitFor1_2 = new StubWaitFor(8);
    waitForComposed1.addWaitFor(waitFor1_2);
    StubWaitFor* waitFor1_3 = new StubWaitFor(15);
    waitForComposed1.addWaitFor(waitFor1_3);

    QCOMPARE(waitForComposed1 == waitForComposed2, false);
    QCOMPARE(waitForComposed2 == waitForComposed1, false);

    StubWaitFor* waitFor2_1 = new StubWaitFor(4);
    waitForComposed2.addWaitFor(waitFor2_1);
    StubWaitFor* waitFor2_2 = new StubWaitFor(8);
    waitForComposed2.addWaitFor(waitFor2_2);
    StubWaitFor* waitFor2_3 = new StubWaitFor(16);
    waitForComposed2.addWaitFor(waitFor2_3);

    QCOMPARE(waitForComposed1 == waitForComposed2, false);
    QCOMPARE(waitForComposed2 == waitForComposed1, false);

    waitFor2_3->mValue = 15;

    QCOMPARE(waitForComposed1 == waitForComposed2, true);
    QCOMPARE(waitForComposed2 == waitForComposed1, true);

    StubWaitFor stubWaitFor;

    QCOMPARE(waitForComposed1 == stubWaitFor, false);
}

void WaitForComposedTest::testSetCompositionType() {
    WaitForComposed waitForComposed;

    QSignalSpy dataChangedSpy(&waitForComposed, SIGNAL(dataChanged(WaitFor*)));

    waitForComposed.setCompositionType(WaitForComposed::Or);

    QCOMPARE(waitForComposed.compositionType(), WaitForComposed::Or);
    QCOMPARE(dataChangedSpy.count(), 1);
    assertWaitForSignal(dataChangedSpy, 0, &waitForComposed);
}

void WaitForComposedTest::testAddWaitFor() {
    WaitForComposed waitForComposed;
    WaitFor* waitFor1 = new StubWaitFor();
    WaitFor* waitFor2 = new StubWaitFor();
    WaitFor* waitFor3 = new StubWaitFor();

    QSignalSpy waitForAddedSpy(&waitForComposed,
                               SIGNAL(waitForAdded(WaitFor*)));

    waitForComposed.addWaitFor(waitFor1);
    waitForComposed.addWaitFor(waitFor2);
    waitForComposed.addWaitFor(waitFor3);

    QCOMPARE(waitForComposed.waitFors().count(), 3);
    QCOMPARE(waitForComposed.waitFors()[0], waitFor1);
    QCOMPARE(waitForComposed.waitFors()[1], waitFor2);
    QCOMPARE(waitForComposed.waitFors()[2], waitFor3);
    QCOMPARE(waitForAddedSpy.count(), 3);
    assertWaitForSignal(waitForAddedSpy, 0, waitFor1);
    assertWaitForSignal(waitForAddedSpy, 1, waitFor2);
    assertWaitForSignal(waitForAddedSpy, 2, waitFor3);
}

void WaitForComposedTest::testRemoveWaitFor() {
    WaitForComposed waitForComposed;

    //They will be removed and not deleted by the WaitForComposed, so they are
    //created in stack
    StubWaitFor waitFor1;
    StubWaitFor waitFor2;
    StubWaitFor waitFor3;

    waitForComposed.addWaitFor(&waitFor1);
    waitForComposed.addWaitFor(&waitFor2);
    waitForComposed.addWaitFor(&waitFor3);

    QSignalSpy waitForRemovedSpy(&waitForComposed,
                                 SIGNAL(waitForRemoved(WaitFor*)));

    waitForComposed.removeWaitFor(&waitFor2);

    QCOMPARE(waitForComposed.waitFors().count(), 2);
    QCOMPARE(waitForComposed.waitFors()[0], &waitFor1);
    QCOMPARE(waitForComposed.waitFors()[1], &waitFor3);
    QCOMPARE(waitForRemovedSpy.count(), 1);
    assertWaitForSignal(waitForRemovedSpy, 0, &waitFor2);

    waitForComposed.removeWaitFor(&waitFor1);
    waitForComposed.removeWaitFor(&waitFor3);

    QCOMPARE(waitForComposed.waitFors().count(), 0);
    QCOMPARE(waitForRemovedSpy.count(), 3);
    assertWaitForSignal(waitForRemovedSpy, 1, &waitFor1);
    assertWaitForSignal(waitForRemovedSpy, 2, &waitFor3);
}

//WaitFor* must be declared as a metatype to be used in qvariant_cast
Q_DECLARE_METATYPE(WaitFor*);

/////////////////////////////////// Helpers ////////////////////////////////////

void WaitForComposedTest::assertWaitForSignal(const QSignalSpy& spy, int index,
                                              WaitFor* waitFor) {
    QCOMPARE(spy.at(index).count(), 1);

    QVariant argument = spy.at(index).at(0);
    QCOMPARE(argument.userType(), mWaitForStarType);
    QCOMPARE(qvariant_cast<WaitFor*>(argument), waitFor);
}

QTEST_MAIN(WaitForComposedTest)

#include "WaitForComposedTest.moc"
