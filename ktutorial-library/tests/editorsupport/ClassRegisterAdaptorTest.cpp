/***************************************************************************
 *   Copyright (C) 2010 by Daniel Calviño Sánchez <danxuliu@gmail.com>     *
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

#include "ClassRegisterAdaptor.h"
#include "ObjectRegister.h"

namespace ktutorial {
namespace editorsupport {

class ClassRegisterAdaptorTest: public QObject {
Q_OBJECT

Q_PROPERTY(QString dummyProperty READ dummyProperty)
Q_PROPERTY(QString dummyPropertyWithNotifySignal
           READ dummyPropertyWithNotifySignal NOTIFY dummySignal)

public:

    QString dummyProperty() const {
        return mDummyProperty;
    }

    QString dummyPropertyWithNotifySignal() const {
        return mDummyPropertyWithNotifySignal;
    }

Q_SIGNALS:

    void dummySignal();
    void dummySignal(int argument1, const QString& argument2);

private slots:

    void testConstructor();

    void testSuperClass();
    void testSuperClassWithUnknownClassName();

    void testPropertyList();
    void testPropertyListWithUnknownClassName();

    void testSignalList();
    void testSignalListWithUnknownClassName();

private:

    QString mDummyProperty;
    QString mDummyPropertyWithNotifySignal;

};

void ClassRegisterAdaptorTest::testConstructor() {
    ObjectRegister objectRegister;
    ClassRegisterAdaptor* adaptor = new ClassRegisterAdaptor(&objectRegister);

    QCOMPARE(adaptor->parent(), &objectRegister);
}

void ClassRegisterAdaptorTest::testSuperClass() {
    ObjectRegister objectRegister;
    ClassRegisterAdaptor* adaptor = new ClassRegisterAdaptor(&objectRegister);

    objectRegister.idForObject(this);

    QCOMPARE(adaptor->superClass(
                        "ktutorial::editorsupport::ClassRegisterAdaptorTest"),
             QString("QObject"));
    QCOMPARE(adaptor->superClass("QObject"), QString(""));
}

void ClassRegisterAdaptorTest::testSuperClassWithUnknownClassName() {
    ObjectRegister objectRegister;
    ClassRegisterAdaptor* adaptor = new ClassRegisterAdaptor(&objectRegister);

    QCOMPARE(adaptor->superClass("UnknownClassName"), QString(""));
}

void ClassRegisterAdaptorTest::testPropertyList() {
    ObjectRegister objectRegister;
    ClassRegisterAdaptor* adaptor = new ClassRegisterAdaptor(&objectRegister);

    objectRegister.idForObject(this);

    QStringList propertyList = adaptor->propertyList(
                        "ktutorial::editorsupport::ClassRegisterAdaptorTest");
    QCOMPARE(propertyList.count(), 2);
    QCOMPARE(propertyList[0], QString("dummyProperty"));
    QCOMPARE(propertyList[1], QString("dummyPropertyWithNotifySignal"));
}

void ClassRegisterAdaptorTest::testPropertyListWithUnknownClassName() {
    ObjectRegister objectRegister;
    ClassRegisterAdaptor* adaptor = new ClassRegisterAdaptor(&objectRegister);

    QStringList propertyList = adaptor->propertyList("UnknownClassName");
    QCOMPARE(propertyList.count(), 0);
}

void ClassRegisterAdaptorTest::testSignalList() {
    ObjectRegister objectRegister;
    ClassRegisterAdaptor* adaptor = new ClassRegisterAdaptor(&objectRegister);

    objectRegister.idForObject(this);

    QStringList signalList = adaptor->signalList(
                        "ktutorial::editorsupport::ClassRegisterAdaptorTest");
    QCOMPARE(signalList.count(), 2);
    QCOMPARE(signalList[0], QString("dummySignal()"));
    QCOMPARE(signalList[1], QString("dummySignal(int,QString)"));
}

void ClassRegisterAdaptorTest::testSignalListWithUnknownClassName() {
    ObjectRegister objectRegister;
    ClassRegisterAdaptor* adaptor = new ClassRegisterAdaptor(&objectRegister);

    QStringList signalList = adaptor->signalList("UnknownClassName");
    QCOMPARE(signalList.count(), 0);
}

}
}

QTEST_MAIN(ktutorial::editorsupport::ClassRegisterAdaptorTest)

#include "ClassRegisterAdaptorTest.moc"
