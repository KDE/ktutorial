/***************************************************************************
 *   Copyright (C) 2011 by Daniel Calviño Sánchez <danxuliu@gmail.com>     *
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

#include "WaitForStepActivation.h"

class WaitForStepActivationTest: public QObject {
Q_OBJECT

private slots:

    void testConstructor();

    void testClone();

    void testEquals();

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

void WaitForStepActivationTest::testConstructor() {
    QObject parent;
    WaitForStepActivation* waitForStepActivation =
                                            new WaitForStepActivation(&parent);

    QCOMPARE(waitForStepActivation->parent(), &parent);
}

void WaitForStepActivationTest::testClone() {
    WaitForStepActivation waitForStepActivation;

    WaitForStepActivation* cloned = static_cast<WaitForStepActivation*>
                                                (waitForStepActivation.clone());

    QVERIFY(cloned);
    QVERIFY(cloned != &waitForStepActivation);
    delete cloned;
}

void WaitForStepActivationTest::testEquals() {
    WaitForStepActivation waitForStepActivation1;
    WaitForStepActivation waitForStepActivation2;

    QCOMPARE(waitForStepActivation1 == waitForStepActivation2, true);
    QCOMPARE(waitForStepActivation2 == waitForStepActivation1, true);

    StubWaitFor stubWaitFor;

    QCOMPARE(waitForStepActivation1 == stubWaitFor, false);
}

QTEST_MAIN(WaitForStepActivationTest)

#include "WaitForStepActivationTest.moc"
