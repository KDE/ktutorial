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

#include "ObjectRegister.h"

class DummyClass: public QObject {
Q_OBJECT
};

class DummyChildClass1: public DummyClass {
Q_OBJECT
};

class DummyChildClass2: public DummyClass {
Q_OBJECT
};

namespace ktutorial {
namespace editorsupport {

class ObjectRegisterTest: public QObject {
Q_OBJECT

private slots:

    void testConstructor();

    void testRegisterObject();
    void testRegisterObjectSeveralObjects();
    void testRegisterObjectTwice();
    void testRegisterNullObject();

    void testObjectForIdWithDestroyedObject();

    void testClear();

};

void ObjectRegisterTest::testConstructor() {
    QObject parent;
    ObjectRegister* objectRegister = new ObjectRegister(&parent);

    QCOMPARE(objectRegister->parent(), &parent);
}

void ObjectRegisterTest::testRegisterObject() {
    ObjectRegister objectRegister;
    DummyChildClass1 object;

    int id = objectRegister.idForObject(&object);

    QCOMPARE(objectRegister.objectForId(id), &object);
    QCOMPARE(objectRegister.metaObjectForClassName("DummyChildClass1"),
             &DummyChildClass1::staticMetaObject);
    QCOMPARE(objectRegister.metaObjectForClassName("DummyClass"),
             &DummyClass::staticMetaObject);
    QCOMPARE(objectRegister.metaObjectForClassName("QObject"),
             &QObject::staticMetaObject);
}

void ObjectRegisterTest::testRegisterObjectSeveralObjects() {
    ObjectRegister objectRegister;
    DummyChildClass1 object1;
    DummyChildClass2 object2;
    QObject object3;

    int id1 = objectRegister.idForObject(&object1);
    int id2 = objectRegister.idForObject(&object2);
    int id3 = objectRegister.idForObject(&object3);

    QVERIFY(id1 != id2);
    QVERIFY(id1 != id3);
    QVERIFY(id2 != id3);
    QCOMPARE(objectRegister.objectForId(id1), &object1);
    QCOMPARE(objectRegister.objectForId(id2), &object2);
    QCOMPARE(objectRegister.objectForId(id3), &object3);
    QCOMPARE(objectRegister.metaObjectForClassName("DummyChildClass1"),
             &DummyChildClass1::staticMetaObject);
    QCOMPARE(objectRegister.metaObjectForClassName("DummyChildClass2"),
             &DummyChildClass2::staticMetaObject);
    QCOMPARE(objectRegister.metaObjectForClassName("DummyClass"),
             &DummyClass::staticMetaObject);
    QCOMPARE(objectRegister.metaObjectForClassName("QObject"),
             &QObject::staticMetaObject);
}

void ObjectRegisterTest::testRegisterObjectTwice() {
    ObjectRegister objectRegister;
    QObject object;

    int id1 = objectRegister.idForObject(&object);
    int id2 = objectRegister.idForObject(&object);

    QCOMPARE(id2, id1);
    QCOMPARE(objectRegister.objectForId(id1), &object);
    QCOMPARE(objectRegister.metaObjectForClassName("QObject"),
             &QObject::staticMetaObject);
}

void ObjectRegisterTest::testRegisterNullObject() {
    ObjectRegister objectRegister;

    int id = objectRegister.idForObject(0);

    QCOMPARE(objectRegister.objectForId(id), (QObject*)0);
}

void ObjectRegisterTest::testObjectForIdWithDestroyedObject() {
    ObjectRegister objectRegister;
    QObject* object = new QObject();

    int id = objectRegister.idForObject(object);

    delete object;

    QCOMPARE(objectRegister.objectForId(id), (QObject*)0);
    QCOMPARE(objectRegister.metaObjectForClassName("QObject"),
             &QObject::staticMetaObject);
}

void ObjectRegisterTest::testClear() {
    ObjectRegister objectRegister;
    QObject object;
    object.setObjectName("The name");

    int id = objectRegister.idForObject(&object);

    objectRegister.clear();

    QCOMPARE(objectRegister.objectForId(id), (QObject*)0);
    QCOMPARE(objectRegister.metaObjectForClassName("QObject"), (QMetaObject*)0);
}

}
}

QTEST_MAIN(ktutorial::editorsupport::ObjectRegisterTest)

#include "ObjectRegisterTest.moc"
