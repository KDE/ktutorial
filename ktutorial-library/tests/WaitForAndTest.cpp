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

#include "WaitForAnd.h"

namespace ktutorial {

class WaitForAndTest: public QObject {
Q_OBJECT

private slots:

    void testConditionMet();
    void testConditionMetNoChildren();

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

void WaitForAndTest::testConditionMet() {
    WaitForAnd waitForAnd;

    MockWaitFor* waitFor1 = new MockWaitFor();
    waitForAnd.add(waitFor1);

    MockWaitFor* waitFor2 = new MockWaitFor();
    waitForAnd.add(waitFor2);

    MockWaitFor* waitFor3 = new MockWaitFor();
    waitForAnd.add(waitFor3);

    QVERIFY(!waitForAnd.conditionMet());

    waitFor1->mConditionMet = true;

    QVERIFY(!waitForAnd.conditionMet());

    waitFor2->mConditionMet = true;
    waitFor3->mConditionMet = true;

    QVERIFY(waitForAnd.conditionMet());
}

void WaitForAndTest::testConditionMetNoChildren() {
    WaitForAnd waitForAnd;

    QVERIFY(!waitForAnd.conditionMet());
}

}

QTEST_MAIN(ktutorial::WaitForAndTest)

#include "WaitForAndTest.moc"
