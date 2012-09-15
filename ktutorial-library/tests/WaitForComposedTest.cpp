/***************************************************************************
 *   Copyright (C) 2009 by Daniel Calviño Sánchez <danxuliu@gmail.com>     *
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

#define protected public
#define private public
#include "WaitForComposed.h"
#undef private
#undef protected

//WaitFor* must be declared as a metatype to be used in qvariant_cast
Q_DECLARE_METATYPE(ktutorial::WaitFor*);

namespace ktutorial {

class WaitForComposedTest: public QObject {
Q_OBJECT

private slots:

    void testSetActiveTrue();
    void testSetActiveFalse();

    void testAdd();
    void testAddTwice();

    void testChildWaitEnd();
    void testChildWaitEndConditionNotMet();
    void testChildWaitEndNotActive();

};

class MockWaitFor: public WaitFor {
public:
    virtual bool conditionMet() const {
        return false;
    }

    void emitWaitEnded() {
        emit waitEnded(this);
    }
};

class MockWaitForComposed: public WaitForComposed {
public:

    bool mConditionMet;

    MockWaitForComposed(): WaitForComposed() {
        mConditionMet = false;
    }

    virtual bool conditionMet() const {
        return mConditionMet;
    }
};

void WaitForComposedTest::testSetActiveTrue() {
    MockWaitForComposed waitForComposed;
    waitForComposed.setActive(false);

    WaitFor* waitFor1 = new MockWaitFor();
    waitForComposed.add(waitFor1);
    waitFor1->setActive(false);

    WaitFor* waitFor2 = new MockWaitFor();
    waitForComposed.add(waitFor2);
    waitFor2->setActive(false);

    WaitFor* waitFor3 = new MockWaitFor();
    waitForComposed.add(waitFor3);
    waitFor3->setActive(false);

    waitForComposed.setActive(true);

    QVERIFY(waitForComposed.isActive());
    QVERIFY(waitFor1->isActive());
    QVERIFY(waitFor2->isActive());
    QVERIFY(waitFor3->isActive());
}

void WaitForComposedTest::testSetActiveFalse() {
    MockWaitForComposed waitForComposed;
    waitForComposed.setActive(true);

    WaitFor* waitFor1 = new MockWaitFor();
    waitForComposed.add(waitFor1);
    waitFor1->setActive(true);

    WaitFor* waitFor2 = new MockWaitFor();
    waitForComposed.add(waitFor2);
    waitFor2->setActive(true);

    WaitFor* waitFor3 = new MockWaitFor();
    waitForComposed.add(waitFor3);
    waitFor3->setActive(true);

    waitForComposed.setActive(false);

    QVERIFY(!waitForComposed.isActive());
    QVERIFY(!waitFor1->isActive());
    QVERIFY(!waitFor2->isActive());
    QVERIFY(!waitFor3->isActive());
}

void WaitForComposedTest::testAdd() {
    MockWaitForComposed waitForComposed;

    WaitFor* waitFor1 = new MockWaitFor();
    waitForComposed.add(waitFor1);

    QCOMPARE(waitFor1->parent(), &waitForComposed);
    QCOMPARE(waitForComposed.mWaitFors.size(), 1);
    QCOMPARE(waitForComposed.mWaitFors[0], waitFor1);

    WaitFor* waitFor2 = new MockWaitFor();
    waitForComposed.add(waitFor2);

    QCOMPARE(waitFor1->parent(), &waitForComposed);
    QCOMPARE(waitFor2->parent(), &waitForComposed);
    QCOMPARE(waitForComposed.mWaitFors.size(), 2);
    QCOMPARE(waitForComposed.mWaitFors[0], waitFor1);
    QCOMPARE(waitForComposed.mWaitFors[1], waitFor2);
}

void WaitForComposedTest::testAddTwice() {
    MockWaitForComposed waitForComposed;

    WaitFor* waitFor1 = new MockWaitFor();
    waitForComposed.add(waitFor1);

    WaitFor* waitFor2 = new MockWaitFor();
    waitForComposed.add(waitFor2);

    waitForComposed.add(waitFor1);
    waitForComposed.add(waitFor2);

    QCOMPARE(waitFor1->parent(), &waitForComposed);
    QCOMPARE(waitFor2->parent(), &waitForComposed);
    QCOMPARE(waitForComposed.mWaitFors.size(), 2);
    QCOMPARE(waitForComposed.mWaitFors[0], waitFor1);
    QCOMPARE(waitForComposed.mWaitFors[1], waitFor2);
}

void WaitForComposedTest::testChildWaitEnd() {
    MockWaitForComposed waitForComposed;

    MockWaitFor* waitFor1 = new MockWaitFor();
    waitForComposed.add(waitFor1);

    MockWaitFor* waitFor2 = new MockWaitFor();
    waitForComposed.add(waitFor2);

    //WaitFor* must be registered in order to be used with QSignalSpy
    int waitForStarType = qRegisterMetaType<WaitFor*>("WaitFor*");
    QSignalSpy waitEndedSpy(&waitForComposed, SIGNAL(waitEnded(WaitFor*)));

    waitForComposed.setActive(true);
    waitForComposed.mConditionMet = true;

    emit waitFor1->emitWaitEnded();

    QCOMPARE(waitEndedSpy.count(), 1);
    QVariant argument = waitEndedSpy.at(0).at(0);
    QCOMPARE(argument.userType(), waitForStarType);
    QCOMPARE(qvariant_cast<WaitFor*>(argument), &waitForComposed);
}

void WaitForComposedTest::testChildWaitEndConditionNotMet() {
    MockWaitForComposed waitForComposed;

    MockWaitFor* waitFor1 = new MockWaitFor();
    waitForComposed.add(waitFor1);

    MockWaitFor* waitFor2 = new MockWaitFor();
    waitForComposed.add(waitFor2);

    //WaitFor* must be registered in order to be used with QSignalSpy
    qRegisterMetaType<WaitFor*>("WaitFor*");
    QSignalSpy waitEndedSpy(&waitForComposed, SIGNAL(waitEnded(WaitFor*)));

    waitForComposed.setActive(true);
    waitForComposed.mConditionMet = false;

    emit waitFor1->emitWaitEnded();

    QCOMPARE(waitEndedSpy.count(), 0);
}

void WaitForComposedTest::testChildWaitEndNotActive() {
    MockWaitForComposed waitForComposed;

    MockWaitFor* waitFor1 = new MockWaitFor();
    waitForComposed.add(waitFor1);

    MockWaitFor* waitFor2 = new MockWaitFor();
    waitForComposed.add(waitFor2);

    //WaitFor* must be registered in order to be used with QSignalSpy
    qRegisterMetaType<WaitFor*>("WaitFor*");
    QSignalSpy waitEndedSpy(&waitForComposed, SIGNAL(waitEnded(WaitFor*)));

    waitForComposed.setActive(false);
    waitForComposed.mConditionMet = true;

    emit waitFor1->emitWaitEnded();

    QCOMPARE(waitEndedSpy.count(), 0);
}

}

QTEST_MAIN(ktutorial::WaitForComposedTest)

#include "WaitForComposedTest.moc"
