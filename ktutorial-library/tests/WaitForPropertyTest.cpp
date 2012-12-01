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

#include <QSignalSpy>

#include "WaitForProperty.h"

//WaitFor* must be declared as a metatype to be used in qvariant_cast
Q_DECLARE_METATYPE(ktutorial::WaitFor*);

namespace ktutorial {

class WaitForPropertyTest: public QObject {
Q_OBJECT
Q_PROPERTY(QString stringProperty READ stringProperty
                                  NOTIFY stringPropertyChanged)
Q_PROPERTY(int intProperty READ intProperty)

public:

    QString stringProperty() const {
        return mStringProperty;
    }

    int intProperty() const {
        return mIntProperty;
    }

signals:

    void stringPropertyChanged();

protected:

    void connectNotify(const char* signal) {
        if (QLatin1String(signal) == SIGNAL(stringPropertyChanged())) {
            mStringPropertyChangedConnectionCount++;
        }
    }

private:

    QString mStringProperty;
    int mStringPropertyChangedConnectionCount;

    int mIntProperty;

private slots:

    void init() {
        mStringProperty = "";
        mStringPropertyChangedConnectionCount = 0;

        mIntProperty = 0;
    }

    void testConstructor();
    void testConstructorWithPropertyWithoutNotifySignal();
    void testConstructorWithNullObject();
    void testConstructorWithUnknownPropertyName();
    void testConstructorDefault();
    void testConstructorDefaultWithPropertyWithoutNotifySignal();
    void testConstructorDefaultWithNullObject();
    void testConstructorDefaultWithUnknownPropertyName();

    void testConditionMet();
    void testConditionMetWhenStepWasNotActive();
    void testConditionMetWithPropertyWithoutNotifySignal();
    void testConditionMetWithValueTypeDifferentFromPropertyType();

    void testPropertyChangeToExpectedValue();
    void testPropertyChangeToExpectedValueWhenNotActive();

};

void WaitForPropertyTest::testConstructor() {
    WaitForProperty waitForProperty(this, "stringProperty", "Expected value");

    QVERIFY(!waitForProperty.isActive());
    QVERIFY(!waitForProperty.conditionMet());
    QCOMPARE(mStringPropertyChangedConnectionCount, 1);
}

void WaitForPropertyTest::testConstructorWithPropertyWithoutNotifySignal() {
    WaitForProperty waitForProperty(this, "intProperty", 42);

    QVERIFY(!waitForProperty.isActive());
    QVERIFY(!waitForProperty.conditionMet());
}

void WaitForPropertyTest::testConstructorWithNullObject() {
    WaitForProperty waitForProperty(0, "stringProperty", "Expected value");

    QVERIFY(!waitForProperty.isActive());
    QVERIFY(!waitForProperty.conditionMet());
}

void WaitForPropertyTest::testConstructorWithUnknownPropertyName() {
    WaitForProperty waitForProperty(this, "unknownProperty", "Expected value");

    QVERIFY(!waitForProperty.isActive());
    QVERIFY(!waitForProperty.conditionMet());
}

void WaitForPropertyTest::testConstructorDefault() {
    WaitForProperty waitForProperty;
    waitForProperty.setProperty(this, "stringProperty", "Expected value");

    QVERIFY(!waitForProperty.isActive());
    QVERIFY(!waitForProperty.conditionMet());
    QCOMPARE(mStringPropertyChangedConnectionCount, 1);
}

void WaitForPropertyTest::
                    testConstructorDefaultWithPropertyWithoutNotifySignal() {
    WaitForProperty waitForProperty;
    waitForProperty.setProperty(this, "intProperty", 42);

    QVERIFY(!waitForProperty.isActive());
    QVERIFY(!waitForProperty.conditionMet());
}

void WaitForPropertyTest::testConstructorDefaultWithNullObject() {
    WaitForProperty waitForProperty;
    waitForProperty.setProperty(0, "stringProperty", "Expected value");

    QVERIFY(!waitForProperty.isActive());
    QVERIFY(!waitForProperty.conditionMet());
}

void WaitForPropertyTest::testConstructorDefaultWithUnknownPropertyName() {
    WaitForProperty waitForProperty;
    waitForProperty.setProperty(this, "unknownProperty", "Expected value");

    QVERIFY(!waitForProperty.isActive());
    QVERIFY(!waitForProperty.conditionMet());
}

void WaitForPropertyTest::testConditionMet() {
    WaitForProperty waitForProperty(this, "stringProperty", "Expected value");

    waitForProperty.setActive(true);

    QVERIFY(!waitForProperty.conditionMet());

    mStringProperty = "Another value";
    emit stringPropertyChanged();

    QVERIFY(!waitForProperty.conditionMet());

    mStringProperty = "Expected value";
    emit stringPropertyChanged();

    QVERIFY(waitForProperty.conditionMet());
}

void WaitForPropertyTest::testConditionMetWhenStepWasNotActive() {
    WaitForProperty waitForProperty(this, "stringProperty", "Expected value");

    mStringProperty = "Another value";
    emit stringPropertyChanged();

    waitForProperty.setActive(true);

    QVERIFY(!waitForProperty.conditionMet());

    waitForProperty.setActive(false);

    mStringProperty = "Expected value";
    emit stringPropertyChanged();

    waitForProperty.setActive(true);

    QVERIFY(waitForProperty.conditionMet());
}

void WaitForPropertyTest::testConditionMetWithPropertyWithoutNotifySignal() {
    WaitForProperty waitForProperty(this, "intProperty", 42);

    waitForProperty.setActive(true);

    QVERIFY(!waitForProperty.conditionMet());

    mIntProperty = 4;

    QVERIFY(!waitForProperty.conditionMet());

    mIntProperty = 42;

    QVERIFY(waitForProperty.conditionMet());
}

void WaitForPropertyTest::
                    testConditionMetWithValueTypeDifferentFromPropertyType() {
    WaitForProperty waitForProperty(this, "intProperty", "42");

    waitForProperty.setActive(true);

    QVERIFY(!waitForProperty.conditionMet());

    mIntProperty = 4;

    QVERIFY(!waitForProperty.conditionMet());

    mIntProperty = 42;

    QVERIFY(waitForProperty.conditionMet());
}

void WaitForPropertyTest::testPropertyChangeToExpectedValue() {
    WaitForProperty waitForProperty(this, "stringProperty", "Expected value");
    waitForProperty.setActive(true);

    //WaitFor* must be registered in order to be used with QSignalSpy
    int waitForStarType = qRegisterMetaType<WaitFor*>("WaitFor*");
    QSignalSpy waitEndedSpy(&waitForProperty, SIGNAL(waitEnded(WaitFor*)));

    mStringProperty = "Another value";
    emit stringPropertyChanged();

    QVERIFY(!waitForProperty.conditionMet());
    QCOMPARE(waitEndedSpy.count(), 0);

    mStringProperty = "Expected value";
    emit stringPropertyChanged();

    QVERIFY(waitForProperty.conditionMet());
    QCOMPARE(waitEndedSpy.count(), 1);
    QVariant argument = waitEndedSpy.at(0).at(0);
    QCOMPARE(argument.userType(), waitForStarType);
    QCOMPARE(qvariant_cast<WaitFor*>(argument), &waitForProperty);
}

void WaitForPropertyTest::testPropertyChangeToExpectedValueWhenNotActive() {
    WaitForProperty waitForProperty(this, "stringProperty", "Expected value");
    waitForProperty.setActive(false);

    qRegisterMetaType<WaitFor*>("WaitFor*");
    QSignalSpy waitEndedSpy(&waitForProperty, SIGNAL(waitEnded(WaitFor*)));

    mStringProperty = "Another value";
    emit stringPropertyChanged();

    QVERIFY(!waitForProperty.conditionMet());
    QCOMPARE(waitEndedSpy.count(), 0);

    mStringProperty = "Expected value";
    emit stringPropertyChanged();

    QVERIFY(waitForProperty.conditionMet());
    QCOMPARE(waitEndedSpy.count(), 0);
}

}

QTEST_MAIN(ktutorial::WaitForPropertyTest)

#include "WaitForPropertyTest.moc"
