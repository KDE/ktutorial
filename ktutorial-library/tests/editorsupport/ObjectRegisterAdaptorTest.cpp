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

#include "ObjectRegisterAdaptor.h"
#include "ObjectRegister.h"

namespace ktutorial {
namespace editorsupport {

class ObjectRegisterAdaptorTest: public QObject {
Q_OBJECT

private slots:

    void testConstructor();

    void testObjectName();
    void testObjectNameWithUnknownId();

    void testClassName();
    void testClassNameWithUnknownId();

    void testChildObjectIds();
    void testChildObjectIdsWithUnknownId();

    void testClear();

};

void ObjectRegisterAdaptorTest::testConstructor() {
    ObjectRegister objectRegister;
    ObjectRegisterAdaptor* adaptor = new ObjectRegisterAdaptor(&objectRegister);

    QCOMPARE(adaptor->parent(), &objectRegister);
}

void ObjectRegisterAdaptorTest::testObjectName() {
    ObjectRegister objectRegister;
    this->setObjectName("The name");
    ObjectRegisterAdaptor* adaptor = new ObjectRegisterAdaptor(&objectRegister);

    int id = objectRegister.idForObject(this);

    QCOMPARE(adaptor->objectName(id), QString("The name"));
}

void ObjectRegisterAdaptorTest::testObjectNameWithUnknownId() {
    ObjectRegister objectRegister;
    ObjectRegisterAdaptor* adaptor = new ObjectRegisterAdaptor(&objectRegister);

    QCOMPARE(adaptor->objectName(42), QString(""));
}

void ObjectRegisterAdaptorTest::testClassName() {
    ObjectRegister objectRegister;
    ObjectRegisterAdaptor* adaptor = new ObjectRegisterAdaptor(&objectRegister);

    int id = objectRegister.idForObject(this);

    QCOMPARE(adaptor->className(id),
             QString("ktutorial::editorsupport::ObjectRegisterAdaptorTest"));
}

void ObjectRegisterAdaptorTest::testClassNameWithUnknownId() {
    ObjectRegister objectRegister;
    ObjectRegisterAdaptor* adaptor = new ObjectRegisterAdaptor(&objectRegister);

    QCOMPARE(adaptor->className(42), QString(""));
}

void ObjectRegisterAdaptorTest::testChildObjectIds() {
    ObjectRegister objectRegister;
    ObjectRegisterAdaptor* adaptor = new ObjectRegisterAdaptor(&objectRegister);

    QObject* object1 = new QObject(this);
    QObject* object2 = new QObject(this);
    QObject* object3 = new QObject(this);

    int id = objectRegister.idForObject(this);

    QList<int> childObjectIds = adaptor->childObjectIds(id);
    QCOMPARE(childObjectIds.count(), 3);
    QCOMPARE(objectRegister.objectForId(childObjectIds[0]), object1);
    QCOMPARE(objectRegister.objectForId(childObjectIds[1]), object2);
    QCOMPARE(objectRegister.objectForId(childObjectIds[2]), object3);
}

void ObjectRegisterAdaptorTest::testChildObjectIdsWithUnknownId() {
    ObjectRegister objectRegister;
    ObjectRegisterAdaptor* adaptor = new ObjectRegisterAdaptor(&objectRegister);

    QCOMPARE(adaptor->childObjectIds(42).count(), 0);
}

void ObjectRegisterAdaptorTest::testClear() {
    ObjectRegister objectRegister;
    ObjectRegisterAdaptor* adaptor = new ObjectRegisterAdaptor(&objectRegister);

    int id = objectRegister.idForObject(this);

    adaptor->clear();

    QCOMPARE(adaptor->className(id), QString());
}

}
}

QTEST_MAIN(ktutorial::editorsupport::ObjectRegisterAdaptorTest)

#include "ObjectRegisterAdaptorTest.moc"
