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

#include <QTest>

#define protected public
#define private public
#include "WaitForNot.h"
#undef private
#undef protected

namespace ktutorial {

class WaitForNotTest: public QObject {
Q_OBJECT

private slots:

    void testConstructor();
    void testConstructorDefault();

    void testConditionMetTrue();
    void testConditionMetFalse();

    void testSetActive();

};

class MockWaitFor: public WaitFor {
public:

    bool mConditionMet;

    MockWaitFor(): WaitFor() {
        mConditionMet = false;
    }

    virtual bool conditionMet() const {
        return mConditionMet;
    }
};

void WaitForNotTest::testConstructor() {
    MockWaitFor* waitFor = new MockWaitFor();

    WaitForNot waitForNot(waitFor);

    QCOMPARE(waitFor->parent(), &waitForNot);
    QCOMPARE(waitForNot.mWaitFor, waitFor);
}

void WaitForNotTest::testConstructorDefault() {
    MockWaitFor* waitFor = new MockWaitFor();

    WaitForNot waitForNot;
    waitForNot.setNegatedWaitFor(waitFor);

    QCOMPARE(waitFor->parent(), &waitForNot);
    QCOMPARE(waitForNot.mWaitFor, waitFor);
}

void WaitForNotTest::testConditionMetTrue() {
    MockWaitFor* waitFor = new MockWaitFor();
    waitFor->mConditionMet = false;

    WaitForNot waitForNot(waitFor);

    QVERIFY(waitForNot.conditionMet());
}

void WaitForNotTest::testConditionMetFalse() {
    MockWaitFor* waitFor = new MockWaitFor();
    waitFor->mConditionMet = true;

    WaitForNot waitForNot(waitFor);

    QVERIFY(!waitForNot.conditionMet());
}

void WaitForNotTest::testSetActive() {
    MockWaitFor* waitFor = new MockWaitFor();
    waitFor->setActive(false);

    WaitForNot waitForNot(waitFor);

    waitForNot.setActive(true);

    QVERIFY(waitForNot.isActive());
    QVERIFY(waitFor->isActive());
}

}

QTEST_MAIN(ktutorial::WaitForNotTest)

#include "WaitForNotTest.moc"
