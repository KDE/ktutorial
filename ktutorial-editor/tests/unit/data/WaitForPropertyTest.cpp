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

#include <QtTest>

#include "WaitForProperty.h"

class WaitForPropertyTest: public QObject {
Q_OBJECT

private slots:

    void initTestCase();

    void testConstructor();

    void testClone();

    void testEquals();

    void testSetObjectName();

    void testSetPropertyName();

    void testSetValue();

private:

    int mWaitForStarType;

    void assertWaitForProperty(const QSignalSpy& spy, int index,
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

void WaitForPropertyTest::initTestCase() {
    //WaitFor* must be registered in order to be used with QSignalSpy
    mWaitForStarType = qRegisterMetaType<WaitFor*>("WaitFor*");
}

void WaitForPropertyTest::testConstructor() {
    QObject parent;
    WaitForProperty* waitForProperty = new WaitForProperty(&parent);

    QCOMPARE(waitForProperty->parent(), &parent);
}

void WaitForPropertyTest::testClone() {
    WaitForProperty waitForProperty;
    waitForProperty.setObjectName("The object name");
    waitForProperty.setPropertyName("The property name");
    waitForProperty.setValue("The value");

    WaitForProperty* cloned =
                        static_cast<WaitForProperty*>(waitForProperty.clone());

    QVERIFY(cloned != &waitForProperty);
    QCOMPARE(cloned->objectName(), waitForProperty.objectName());
    QCOMPARE(cloned->propertyName(), waitForProperty.propertyName());
    QCOMPARE(cloned->value(), waitForProperty.value());
    delete cloned;
}

void WaitForPropertyTest::testEquals() {
    WaitForProperty waitForProperty1;
    waitForProperty1.setObjectName("The object name");
    waitForProperty1.setPropertyName("The property name");
    waitForProperty1.setValue("The value");
    WaitForProperty waitForProperty2;

    QCOMPARE(waitForProperty1 == waitForProperty2, false);
    QCOMPARE(waitForProperty2 == waitForProperty1, false);

    waitForProperty2.setObjectName("The object name");
    waitForProperty2.setPropertyName("The property name");
    waitForProperty2.setValue("The value");

    QCOMPARE(waitForProperty1 == waitForProperty2, true);
    QCOMPARE(waitForProperty2 == waitForProperty1, true);

    StubWaitFor stubWaitFor;

    QCOMPARE(waitForProperty1 == stubWaitFor, false);
}

void WaitForPropertyTest::testSetObjectName() {
    WaitForProperty waitForProperty;

    QSignalSpy dataChangedSpy(&waitForProperty, SIGNAL(dataChanged(WaitFor*)));

    waitForProperty.setObjectName("The object name");

    QCOMPARE(waitForProperty.objectName(), QString("The object name"));
    QCOMPARE(dataChangedSpy.count(), 1);
    assertWaitForProperty(dataChangedSpy, 0, &waitForProperty);
}

void WaitForPropertyTest::testSetPropertyName() {
    WaitForProperty waitForProperty;

    QSignalSpy dataChangedSpy(&waitForProperty, SIGNAL(dataChanged(WaitFor*)));

    waitForProperty.setPropertyName("The property name");

    QCOMPARE(waitForProperty.propertyName(), QString("The property name"));
    QCOMPARE(dataChangedSpy.count(), 1);
    assertWaitForProperty(dataChangedSpy, 0, &waitForProperty);
}

void WaitForPropertyTest::testSetValue() {
    WaitForProperty waitForProperty;

    QSignalSpy dataChangedSpy(&waitForProperty, SIGNAL(dataChanged(WaitFor*)));

    waitForProperty.setValue("The value");

    QCOMPARE(waitForProperty.value(), QString("The value"));
    QCOMPARE(dataChangedSpy.count(), 1);
    assertWaitForProperty(dataChangedSpy, 0, &waitForProperty);
}

//WaitFor* must be declared as a metatype to be used in qvariant_cast
Q_DECLARE_METATYPE(WaitFor*);

/////////////////////////////////// Helpers ////////////////////////////////////

void WaitForPropertyTest::assertWaitForProperty(const QSignalSpy& spy,
                                                int index, WaitFor* waitFor) {
    QCOMPARE(spy.at(index).count(), 1);

    QVariant argument = spy.at(index).at(0);
    QCOMPARE(argument.userType(), mWaitForStarType);
    QCOMPARE(qvariant_cast<WaitFor*>(argument), waitFor);
}

QTEST_MAIN(WaitForPropertyTest)

#include "WaitForPropertyTest.moc"
