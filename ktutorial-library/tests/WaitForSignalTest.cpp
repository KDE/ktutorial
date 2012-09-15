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
#include "WaitForSignal.h"
#undef private
#undef protected

//WaitFor* must be declared as a metatype to be used in qvariant_cast
Q_DECLARE_METATYPE(ktutorial::WaitFor*);

namespace ktutorial {

class WaitForSignalTest: public QObject {
Q_OBJECT

signals:

    void dummySignal();

protected:

    void connectNotify(const char* signal) {
        if (QLatin1String(signal) == SIGNAL(dummySignal())) {
            mDummySignalConnectionCount++;
        }
    }

private:

    int mDummySignalConnectionCount;

private slots:

    void init() {
        mDummySignalConnectionCount = 0;
    }

    void testConstructor();
    void testConstructorWithoutSignalMacro();
    void testConstructorWithNullObject();
    void testConstructorDefault();
    void testConstructorDefaultWithoutSignalMacro();
    void testConstructorDefaultWithNullObject();

    void testSetActive();

    void testSignalWaitEnd();
    void testSignalWaitEndNotActive();

};

void WaitForSignalTest::testConstructor() {
    WaitForSignal waitForSignal(this, SIGNAL(dummySignal()));

    QVERIFY(!waitForSignal.isActive());
    QVERIFY(!waitForSignal.conditionMet());
    QCOMPARE(mDummySignalConnectionCount, 1);
}

void WaitForSignalTest::testConstructorWithoutSignalMacro() {
    WaitForSignal waitForSignal(this, "dummySignal()");

    QVERIFY(!waitForSignal.isActive());
    QVERIFY(!waitForSignal.conditionMet());
    QCOMPARE(mDummySignalConnectionCount, 1);
}

void WaitForSignalTest::testConstructorWithNullObject() {
    WaitForSignal waitForSignal(0, SIGNAL(dummySignal()));

    QVERIFY(!waitForSignal.isActive());
    QVERIFY(!waitForSignal.conditionMet());
}

void WaitForSignalTest::testConstructorDefault() {
    WaitForSignal waitForSignal;
    waitForSignal.setSignal(this, SIGNAL(dummySignal()));

    QVERIFY(!waitForSignal.isActive());
    QVERIFY(!waitForSignal.conditionMet());
    QCOMPARE(mDummySignalConnectionCount, 1);
}

void WaitForSignalTest::testConstructorDefaultWithoutSignalMacro() {
    WaitForSignal waitForSignal;
    waitForSignal.setSignal(this, "dummySignal()");

    QVERIFY(!waitForSignal.isActive());
    QVERIFY(!waitForSignal.conditionMet());
    QCOMPARE(mDummySignalConnectionCount, 1);
}

void WaitForSignalTest::testConstructorDefaultWithNullObject() {
    WaitForSignal waitForSignal;
    waitForSignal.setSignal(0, SIGNAL(dummySignal()));

    QVERIFY(!waitForSignal.isActive());
    QVERIFY(!waitForSignal.conditionMet());
}

void WaitForSignalTest::testSetActive() {
    WaitForSignal waitForSignal(this, SIGNAL(dummySignal()));
    waitForSignal.mConditionMet = true;

    waitForSignal.setActive(true);

    QVERIFY(waitForSignal.isActive());
    QVERIFY(!waitForSignal.conditionMet());
}

void WaitForSignalTest::testSignalWaitEnd() {
    WaitForSignal waitForSignal(this, SIGNAL(dummySignal()));
    waitForSignal.setActive(true);

    //WaitFor* must be registered in order to be used with QSignalSpy
    int waitForStarType = qRegisterMetaType<WaitFor*>("WaitFor*");
    QSignalSpy waitEndedSpy(&waitForSignal, SIGNAL(waitEnded(WaitFor*)));

    //The signal is connected to signalWaitEnd() in the constructor
    emit dummySignal();

    QVERIFY(waitForSignal.conditionMet());
    QCOMPARE(waitEndedSpy.count(), 1);
    QVariant argument = waitEndedSpy.at(0).at(0);
    QCOMPARE(argument.userType(), waitForStarType);
    QCOMPARE(qvariant_cast<WaitFor*>(argument), &waitForSignal);
}

void WaitForSignalTest::testSignalWaitEndNotActive() {
    WaitForSignal waitForSignal(this, SIGNAL(dummySignal()));

    qRegisterMetaType<WaitFor*>("WaitFor*");
    QSignalSpy waitEndedSpy(&waitForSignal, SIGNAL(waitEnded(WaitFor*)));

    //The signal is connected to signalWaitEnd() in the constructor
    emit dummySignal();

    QVERIFY(!waitForSignal.conditionMet());
    QCOMPARE(waitEndedSpy.count(), 0);
}

}

QTEST_MAIN(ktutorial::WaitForSignalTest)

#include "WaitForSignalTest.moc"
