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

#include "WaitForWindow.h"

class WaitForWindowTest: public QObject {
Q_OBJECT

private slots:

    void initTestCase();

    void testConstructor();

    void testClone();

    void testEquals();

    void testSetWindowObjectName();

private:

    int mWaitForStarType;

    void assertWaitForWindow(const QSignalSpy& spy, int index,
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

void WaitForWindowTest::initTestCase() {
    //WaitFor* must be registered in order to be used with QSignalSpy
    mWaitForStarType = qRegisterMetaType<WaitFor*>("WaitFor*");
}

void WaitForWindowTest::testConstructor() {
    QObject parent;
    WaitForWindow* waitForWindow = new WaitForWindow(&parent);

    QCOMPARE(waitForWindow->parent(), &parent);
}

void WaitForWindowTest::testClone() {
    WaitForWindow waitForWindow;
    waitForWindow.setWindowObjectName("The window object name");

    WaitForWindow* cloned = static_cast<WaitForWindow*>(waitForWindow.clone());

    QVERIFY(cloned != &waitForWindow);
    QCOMPARE(cloned->windowObjectName(), waitForWindow.windowObjectName());
    delete cloned;
}

void WaitForWindowTest::testEquals() {
    WaitForWindow waitForWindow1;
    waitForWindow1.setWindowObjectName("The window object name");
    WaitForWindow waitForWindow2;

    QCOMPARE(waitForWindow1 == waitForWindow2, false);
    QCOMPARE(waitForWindow2 == waitForWindow1, false);

    waitForWindow2.setWindowObjectName("The window object name");

    QCOMPARE(waitForWindow1 == waitForWindow2, true);
    QCOMPARE(waitForWindow2 == waitForWindow1, true);

    StubWaitFor stubWaitFor;

    QCOMPARE(waitForWindow1 == stubWaitFor, false);
}

void WaitForWindowTest::testSetWindowObjectName() {
    WaitForWindow waitForWindow;

    QSignalSpy dataChangedSpy(&waitForWindow, SIGNAL(dataChanged(WaitFor*)));

    waitForWindow.setWindowObjectName("The window object name");

    QCOMPARE(waitForWindow.windowObjectName(),
             QString("The window object name"));
    QCOMPARE(dataChangedSpy.count(), 1);
    assertWaitForWindow(dataChangedSpy, 0, &waitForWindow);
}

//WaitFor* must be declared as a metatype to be used in qvariant_cast
Q_DECLARE_METATYPE(WaitFor*);

/////////////////////////////////// Helpers ////////////////////////////////////

void WaitForWindowTest::assertWaitForWindow(const QSignalSpy& spy, int index,
                                            WaitFor* waitFor) {
    QCOMPARE(spy.at(index).count(), 1);

    QVariant argument = spy.at(index).at(0);
    QCOMPARE(argument.userType(), mWaitForStarType);
    QCOMPARE(qvariant_cast<WaitFor*>(argument), waitFor);
}

QTEST_MAIN(WaitForWindowTest)

#include "WaitForWindowTest.moc"
