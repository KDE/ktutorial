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

#include "WaitForNot.h"

#include <QSignalSpy>

class WaitForNotTest: public QObject {
Q_OBJECT

private slots:

    void testConstructor();

    void testClone();
    void testCloneEmpty();

    void testEquals();

    void testSetNegatedWaitFor();

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

void WaitForNotTest::testConstructor() {
    QObject parent;
    WaitForNot* waitForNot = new WaitForNot(&parent);

    QCOMPARE(waitForNot->parent(), &parent);
    QCOMPARE(waitForNot->negatedWaitFor(), (WaitFor*)0);
}

void WaitForNotTest::testClone() {
    WaitForNot waitForNot;
    StubWaitFor* negatedWaitFor = new StubWaitFor(16);
    waitForNot.setNegatedWaitFor(negatedWaitFor);

    WaitForNot* cloned = static_cast<WaitForNot*>(waitForNot.clone());

    QVERIFY(cloned != &waitForNot);
    QVERIFY(cloned->negatedWaitFor() != negatedWaitFor);
    QVERIFY(*cloned->negatedWaitFor() == *negatedWaitFor);
    delete cloned;
}

void WaitForNotTest::testCloneEmpty() {
    WaitForNot waitForNot;

    WaitForNot* cloned = static_cast<WaitForNot*>(waitForNot.clone());

    QVERIFY(cloned != &waitForNot);
    QCOMPARE(cloned->negatedWaitFor(), (WaitFor*)0);
    delete cloned;
}

void WaitForNotTest::testEquals() {
    WaitForNot waitForNot1;
    WaitForNot waitForNot2;

    QCOMPARE(waitForNot1 == waitForNot2, true);

    StubWaitFor* negatedWaitFor1 = new StubWaitFor(4);
    waitForNot1.setNegatedWaitFor(negatedWaitFor1);

    QCOMPARE(waitForNot1 == waitForNot2, false);
    QCOMPARE(waitForNot2 == waitForNot1, false);

    StubWaitFor* negatedWaitFor2 = new StubWaitFor();
    waitForNot2.setNegatedWaitFor(negatedWaitFor2);

    QCOMPARE(waitForNot1 == waitForNot2, false);
    QCOMPARE(waitForNot2 == waitForNot1, false);

    negatedWaitFor2->mValue = 4;

    QCOMPARE(waitForNot1 == waitForNot2, true);
    QCOMPARE(waitForNot2 == waitForNot1, true);

    StubWaitFor stubWaitFor;

    QCOMPARE(waitForNot1 == stubWaitFor, false);
}

//WaitFor* must be declared as a metatype to be used in qvariant_cast
Q_DECLARE_METATYPE(WaitFor*);

void WaitForNotTest::testSetNegatedWaitFor() {
    WaitForNot waitForNot;
    WaitFor* negatedWaitFor = new StubWaitFor();

    //WaitFor* must be registered in order to be used with QSignalSpy
    int waitForStarType = qRegisterMetaType<WaitFor*>("WaitFor*");
    QSignalSpy dataChangedSpy(&waitForNot, SIGNAL(dataChanged(WaitFor*)));

    waitForNot.setNegatedWaitFor(negatedWaitFor);

    QCOMPARE(waitForNot.negatedWaitFor(), negatedWaitFor);
    QCOMPARE(dataChangedSpy.count(), 1);
    QVariant argument = dataChangedSpy.at(0).at(0);
    QCOMPARE(argument.userType(), waitForStarType);
    QCOMPARE(qvariant_cast<WaitFor*>(argument), &waitForNot);
}

QTEST_MAIN(WaitForNotTest)

#include "WaitForNotTest.moc"
