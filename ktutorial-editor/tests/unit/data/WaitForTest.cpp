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

#include "WaitFor.h"

class WaitForTest: public QObject {
Q_OBJECT

private slots:

    void testOperatorEqual();

    void testOperatorNotEqual();

};

class StubWaitFor: public WaitFor {
public:

    virtual WaitFor* clone() const {
        return 0;
    }

    virtual bool equals(const WaitFor& waitFor) const {
        Q_UNUSED(waitFor);
        return false;
    }

};

void WaitForTest::testOperatorEqual() {
    StubWaitFor waitFor1;
    StubWaitFor waitFor2;

    QCOMPARE(waitFor1 == waitFor2, false);
}

void WaitForTest::testOperatorNotEqual() {
    StubWaitFor waitFor1;
    StubWaitFor waitFor2;

    QCOMPARE(waitFor1 != waitFor2, true);
}

QTEST_MAIN(WaitForTest)

#include "WaitForTest.moc"
