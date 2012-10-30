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

#include <QAction>
#include <QTimer>

#include <KXmlGuiWindow>

#include "ObjectFinder.h"

namespace ktutorial {

class ObjectFinderTest: public QObject {
Q_OBJECT

private slots:

    void initTestCase();
    void cleanupTestCase();

    void testFindObjectSingleName();
    void testFindObjectSingleNameUnknown();
    void testFindObjectComplexNameDirectChild();
    void testFindObjectComplexNameNestedChild();
    void testFindObjectComplexNameAncestorNameNotUnique();
    void testFindObjectComplexNameUnknownParent();
    
    void testFindObjectAmbiguousSingleNameDirectChild();
    void testFindObjectAmbiguousSingleNameNestedChildUnnamedAncestors();
    void testFindObjectAmbiguousSingleNameNestedChildUnnamedAncestorsDeeperThanNamed();
    void testFindObjectAmbiguousSingleNameNestedChildNamedAncestors();
    void testFindObjectAmbiguousSingleNameNestedChildNamedAncestorsSameDeepThanMixed();
    void testFindObjectAmbiguousSingleNameNestedChildMixedAncestorsSameDeepThanNamed();
    void testFindObjectAmbiguousComplexName();
    void testFindObjectAmbiguousComplexNameUniqueAncestor();
    void testFindObjectComplexNameDifferentAncestorIfSolvingAmbiguity();
    
    void testFindObjectEmptyName();
    void testFindObjectSingleSlash();
    void testFindObjectSlashEndedName();
    void testFindObjectSeveralSlashes();

private:

    KXmlGuiWindow* mMainWindow;
    QObject* mObject1_1_1;
    QAction* mAction1_1_2;
    QObject* mObject1_2_1;
    QObject* mObject2_1_1;
    QObject* mObject2_1_2;
    QAction* mAction2_1_3;
    QAction* mAction2_1_4;
    QObject* mObject3_1_1;
    QObject* mObject3_2_1_1;
    QAction* mAction3_3_1_1;
    QObject* mAmbiguousObject5;
    QObject* mAmbiguousObject8_1_2;
    QObject* mAmbiguousObject10_1;
    QObject* mAmbiguousObject11_1_2;
    QObject* mAmbiguousObject12_1;
    QObject* mAmbiguousObject12_2_3;
    QObject* mObject14_1_2;
    QObject* mAmbiguousObject15_2_2_1;
    QObject* mAmbiguousObject16_2_1_1_1;

    void assertFindObject(const QString& objectName, QObject* object) const;
    void assertFindAction(const QString& objectName, QAction* action) const;

};

void ObjectFinderTest::initTestCase() {
    mMainWindow = new KXmlGuiWindow();

    //-Grand parent1
    //  |-Parent1
    //  |  |-The object
    //  |  |-The action
    //  |-Parent2
    //     |-The object
    //-Grand parent2
    //  |-Parent1
    //     |-The object
    //     |-Another object
    //     |-The action
    //     |-Another action
    //-Grand parent3
    //  |-Parent1
    //  |  |-The object
    //  |-Nested parent
    //  |  |-Another object
    //  |-Nested timer
    //     |-Another action
    QObject* grandParent1 = new QObject(mMainWindow);
    grandParent1->setObjectName("Grand parent1");
    QObject* parent1_1 = new QObject(grandParent1);
    parent1_1->setObjectName("Parent1");
    mObject1_1_1 = new QObject(parent1_1);
    mObject1_1_1->setObjectName("The object");
    mAction1_1_2 = new QAction(parent1_1);
    mAction1_1_2->setObjectName("The action");

    QObject* parent1_2 = new QObject(grandParent1);
    parent1_2->setObjectName("Parent2");
    mObject1_2_1 = new QObject(parent1_2);
    mObject1_2_1->setObjectName("The object");

    QObject* grandParent2 = new QObject(mMainWindow);
    grandParent2->setObjectName("Grand parent2");
    QObject* parent2_1 = new QObject(grandParent2);
    parent2_1->setObjectName("Parent1");
    mObject2_1_1 = new QObject(parent2_1);
    mObject2_1_1->setObjectName("The object");
    mObject2_1_2 = new QObject(parent2_1);
    mObject2_1_2->setObjectName("Another object");
    mAction2_1_3 = new QAction(parent2_1);
    mAction2_1_3->setObjectName("The action");
    mAction2_1_4 = new QAction(parent2_1);
    mAction2_1_4->setObjectName("Another action");

    QObject* grandParent3 = new QObject(mMainWindow);
    grandParent3->setObjectName("Grand parent3");
    QObject* parent3_1 = new QObject(grandParent3);
    parent3_1->setObjectName("Parent1");
    mObject3_1_1 = new QObject(parent3_1);
    mObject3_1_1->setObjectName("The object");

    QObject* parent3_2 = new QObject(grandParent3);
    parent3_2->setObjectName("Nested parent");
    QObject* parent3_2_1 = new QObject(parent3_2);
    mObject3_2_1_1 = new QObject(parent3_2_1);
    mObject3_2_1_1->setObjectName("Another object");

    QTimer* parent3_3 = new QTimer(grandParent3);
    parent3_3->setObjectName("Nested timer");
    QTimer* parent3_3_1 = new QTimer(parent3_3);
    mAction3_3_1_1 = new QAction(parent3_3_1);
    mAction3_3_1_1->setObjectName("Another action");

    //-???
    //  |-???
    //  |  |-Ambiguous object
    //  |-Ambiguous object
    //-Ambiguous object
    //-Object 6
    //  |-Ambiguous object
    //  |-Object 6_2
    //     |-Ambiguous object
    //-Ambiguous object
    QObject* unnamedObject4 = new QObject(mMainWindow);
    QObject* unnamedObject4_1 = new QObject(unnamedObject4);
    QObject* ambiguousObject4_1_1 = new QObject(unnamedObject4_1);
    ambiguousObject4_1_1->setObjectName("Ambiguous object");
    QObject* ambiguousObject4_2 = new QObject(unnamedObject4);
    ambiguousObject4_2->setObjectName("Ambiguous object");

    mAmbiguousObject5 = new QObject(mMainWindow);
    mAmbiguousObject5->setObjectName("Ambiguous object");

    QObject* namedObject6 = new QObject(mMainWindow);
    namedObject6->setObjectName("Object 6");
    QObject* ambiguousObject6_1 = new QObject(namedObject6);
    ambiguousObject6_1->setObjectName("Ambiguous object");
    QObject* namedObject6_2 = new QObject(namedObject6);
    namedObject6_2->setObjectName("Object 6_2");
    QObject* ambiguousObject6_2_1 = new QObject(namedObject6_2);
    ambiguousObject6_2_1->setObjectName("Ambiguous object");

    QObject* ambiguousObject7 = new QObject(mMainWindow);
    ambiguousObject7->setObjectName("Ambiguous object");

    //-???
    //  |-???
    //  |  |-Ambiguous object2
    //  |  |-Ambiguous object3
    //-Object 9
    //  |-Ambiguous object2
    //  |-Object 9_2
    //  |  |-Ambiguous object2
    //  |-Ambiguous object3
    //-???
    //  |-Ambiguous object2
    //  |-Ambiguous object2
    QObject* unnamedObject8 = new QObject(mMainWindow);
    QObject* unnamedObject8_1 = new QObject(unnamedObject8);
    QObject* ambiguousObject8_1_1 = new QObject(unnamedObject8_1);
    ambiguousObject8_1_1->setObjectName("Ambiguous object2");
    mAmbiguousObject8_1_2 = new QObject(unnamedObject8_1);
    mAmbiguousObject8_1_2->setObjectName("Ambiguous object3");

    QObject* namedObject9 = new QObject(mMainWindow);
    namedObject9->setObjectName("Object 9");
    QObject* ambiguousObject9_1 = new QObject(namedObject9);
    ambiguousObject9_1->setObjectName("Ambiguous object2");
    QObject* namedObject9_2 = new QObject(namedObject9);
    namedObject9_2->setObjectName("Object 9_2");
    QObject* ambiguousObject9_2_1 = new QObject(namedObject9_2);
    ambiguousObject9_2_1->setObjectName("Ambiguous object2");
    QObject* ambiguousObject9_3 = new QObject(namedObject9);
    ambiguousObject9_3->setObjectName("Ambiguous object3");

    QObject* unnamedObject10 = new QObject(mMainWindow);
    mAmbiguousObject10_1 = new QObject(unnamedObject10);
    mAmbiguousObject10_1->setObjectName("Ambiguous object2");
    QObject* ambiguousObject10_2 = new QObject(unnamedObject10);
    ambiguousObject10_2->setObjectName("Ambiguous object2");

    //-Object 11
    //  |-Object 11_1
    //     |-Ambiguous object4
    //     |-Ambiguous object5
    //-Object 12
    //  |-Ambiguous object4
    //  |-???
    //     |-Ambiguous object4
    //     |-Ambiguous object5
    //     |-Ambiguous object6
    //-Object 13
    //  |-Ambiguous object4
    //  |-Object 13_2
    //     |-Ambiguous object6
    QObject* namedObject11 = new QObject(mMainWindow);
    namedObject11->setObjectName("Object 11");
    QObject* namedObject11_1 = new QObject(namedObject11);
    namedObject11_1->setObjectName("Object 11_1");
    QObject* ambiguousObject11_1_1 = new QObject(namedObject11_1);
    ambiguousObject11_1_1->setObjectName("Ambiguous object4");
    mAmbiguousObject11_1_2 = new QObject(namedObject11_1);
    mAmbiguousObject11_1_2->setObjectName("Ambiguous object5");

    QObject* namedObject12 = new QObject(mMainWindow);
    namedObject12->setObjectName("Object 12");
    mAmbiguousObject12_1 = new QObject(namedObject12);
    mAmbiguousObject12_1->setObjectName("Ambiguous object4");
    QObject* unnamedObject12_2 = new QObject(namedObject12);
    QObject* ambiguousObject12_2_1 = new QObject(unnamedObject12_2);
    ambiguousObject12_2_1->setObjectName("Ambiguous object4");
    QObject* ambiguousObject12_2_2 = new QObject(unnamedObject12_2);
    ambiguousObject12_2_2->setObjectName("Ambiguous object5");
    mAmbiguousObject12_2_3 = new QObject(unnamedObject12_2);
    mAmbiguousObject12_2_3->setObjectName("Ambiguous object6");
    
    QObject* namedObject13 = new QObject(mMainWindow);
    namedObject13->setObjectName("Object 13");
    QObject* ambiguousObject13_1 = new QObject(namedObject13);
    ambiguousObject13_1->setObjectName("Ambiguous object4");
    QObject* namedObject13_2 = new QObject(namedObject13);
    namedObject13_2->setObjectName("Object 13_2");
    QObject* ambiguousObject13_2_1 = new QObject(namedObject13_2);
    ambiguousObject13_2_1->setObjectName("Ambiguous object6");

    //-Object 14
    //  |-Ambiguous ancestor
    //     |-Ambiguous object7
    //     |-The object
    //-???
    //  |-Ambiguous ancestor
    //  |  |-Object 15_1_1
    //  |     |-Ambiguous object7
    //  |-Unique ancestor
    //     |-Object 15_2_1
    //     |  |-Ambiguous object7
    //     |-???
    //        |-Ambiguous object7
    //-???
    //  |-???
    //  |  |-Ambiguous ancestor
    //  |     |-Ambiguous object7
    //  |-Ambiguous ancestor
    //     |-???
    //     |  |-???
    //     |     |-Ambiguous object7
    //     |-Object 16_2_2
    //        |-Ambiguous object7
    QObject* namedObject14 = new QObject(mMainWindow);
    namedObject14->setObjectName("Object 14");
    QObject* ambiguousAncestor14_1 = new QObject(namedObject14);
    ambiguousAncestor14_1->setObjectName("Ambiguous ancestor");
    QObject* ambiguousObject14_1_1 = new QObject(ambiguousAncestor14_1);
    ambiguousObject14_1_1->setObjectName("Ambiguous object7");
    mObject14_1_2 = new QObject(ambiguousAncestor14_1);
    mObject14_1_2->setObjectName("The object");

    QObject* unnamedObject15 = new QObject(mMainWindow);
    QObject* ambiguousAncestor15_1 = new QObject(unnamedObject15);
    ambiguousAncestor15_1->setObjectName("Ambiguous ancestor");
    QObject* namedObject15_1_1 = new QObject(ambiguousAncestor15_1);
    namedObject15_1_1->setObjectName("Object 15_1_1");
    QObject* ambiguousObject15_1_1_1 = new QObject(namedObject15_1_1);
    ambiguousObject15_1_1_1->setObjectName("Ambiguous object7");
    QObject* uniqueAncestor15_2 = new QObject(unnamedObject15);
    uniqueAncestor15_2->setObjectName("Unique ancestor");
    QObject* namedObject15_2_1 = new QObject(uniqueAncestor15_2);
    namedObject15_2_1->setObjectName("Object 15_2_1");
    QObject* ambiguousObject15_2_1_1 = new QObject(namedObject15_2_1);
    ambiguousObject15_2_1_1->setObjectName("Ambiguous object7");
    QObject* unnamedObject15_2_2 = new QObject(uniqueAncestor15_2);
    mAmbiguousObject15_2_2_1 = new QObject(unnamedObject15_2_2);
    mAmbiguousObject15_2_2_1->setObjectName("Ambiguous object7");

    QObject* unnamedObject16 = new QObject(mMainWindow);
    QObject* unnamedObject16_1 = new QObject(unnamedObject16);
    QObject* ambiguousAncestor16_1_1 = new QObject(unnamedObject16_1);
    ambiguousAncestor16_1_1->setObjectName("Ambiguous ancestor");
    QObject* ambiguousObject16_1_1_1 = new QObject(ambiguousAncestor16_1_1);
    ambiguousObject16_1_1_1->setObjectName("Ambiguous object7");
    QObject* ambiguousAncestor16_2 = new QObject(unnamedObject16);
    ambiguousAncestor16_2->setObjectName("Ambiguous ancestor");
    QObject* unnamedObject16_2_1 = new QObject(ambiguousAncestor16_2);
    QObject* unnamedObject16_2_1_1 = new QObject(unnamedObject16_2_1);
    mAmbiguousObject16_2_1_1_1 = new QObject(unnamedObject16_2_1_1);
    mAmbiguousObject16_2_1_1_1->setObjectName("Ambiguous object7");
    QObject* namedObject16_2_2 = new QObject(ambiguousAncestor16_2);
    namedObject16_2_2->setObjectName("Object 16_2_2");
    QObject* ambiguousObject16_2_2_1 = new QObject(namedObject16_2_2);
    ambiguousObject16_2_2_1->setObjectName("Ambiguous object7");
}

void ObjectFinderTest::cleanupTestCase() {
    delete mMainWindow;
}

void ObjectFinderTest::testFindObjectSingleName() {
    assertFindObject("The object", mObject1_1_1);
}

void ObjectFinderTest::testFindObjectSingleNameUnknown() {
    assertFindObject("Unknown object", 0);
}

void ObjectFinderTest::testFindObjectComplexNameDirectChild() {
    assertFindObject("Grand parent1/Parent1/The object", mObject1_1_1);
    assertFindObject("Grand parent1/Parent2/The object", mObject1_2_1);
    assertFindObject("Grand parent2/Parent1/The object", mObject2_1_1);

    assertFindAction("Grand parent1/Parent1/The action", mAction1_1_2);
    assertFindAction("Grand parent2/Parent1/The action", mAction2_1_3);
}

void ObjectFinderTest::testFindObjectComplexNameNestedChild() {
    assertFindObject("Grand parent2/The object", mObject2_1_1);
    assertFindObject("Nested parent/Another object", mObject3_2_1_1);

    assertFindAction("Grand parent2/The action", mAction2_1_3);
    assertFindAction("Nested timer/Another action", mAction3_3_1_1);
}

void ObjectFinderTest::testFindObjectComplexNameAncestorNameNotUnique() {
    //The ancestor name is not unique, but the full path is
    assertFindObject("Parent1/Another object", mObject2_1_2);

    assertFindAction("Parent1/Another action", mAction2_1_4);
}

void ObjectFinderTest::testFindObjectComplexNameUnknownParent() {
    assertFindObject("Unknown grand parent/The object", 0);
    assertFindObject("Grand parent1/Unknown parent/The object", 0);
}

void ObjectFinderTest::testFindObjectAmbiguousSingleNameDirectChild() {
    assertFindObject("Ambiguous object", mAmbiguousObject5);
}

void ObjectFinderTest::
testFindObjectAmbiguousSingleNameNestedChildUnnamedAncestors() {
    assertFindObject("Ambiguous object2", mAmbiguousObject10_1);
}

void ObjectFinderTest::
testFindObjectAmbiguousSingleNameNestedChildUnnamedAncestorsDeeperThanNamed() {
    assertFindObject("Ambiguous object3", mAmbiguousObject8_1_2);
}

void ObjectFinderTest::
testFindObjectAmbiguousSingleNameNestedChildNamedAncestors() {
    assertFindObject("Ambiguous object4", mAmbiguousObject12_1);
}

void ObjectFinderTest::
testFindObjectAmbiguousSingleNameNestedChildNamedAncestorsSameDeepThanMixed() {
    assertFindObject("Ambiguous object5", mAmbiguousObject11_1_2);
}

void ObjectFinderTest::
testFindObjectAmbiguousSingleNameNestedChildMixedAncestorsSameDeepThanNamed() {
    assertFindObject("Ambiguous object6", mAmbiguousObject12_2_3);
}

void ObjectFinderTest::testFindObjectAmbiguousComplexName() {
    //The ancestor is selected by the rule of shallower unnamed ancestors. The
    //object is selected by the rule of unnamed ancestor even if there are
    //shallower objects, but with named ancestors.
    assertFindObject("Ambiguous ancestor/Ambiguous object7",
                     mAmbiguousObject16_2_1_1_1);
}

void ObjectFinderTest::testFindObjectAmbiguousComplexNameUniqueAncestor() {
    //The ancestor is unique, although the object itself is ambiguous (even
    //among the descendants of that unique ancestor).
    assertFindObject("Unique ancestor/Ambiguous object7",
                     mAmbiguousObject15_2_2_1);
}

void ObjectFinderTest::
testFindObjectComplexNameDifferentAncestorIfSolvingAmbiguity() {
    //The full name is unique, but if the rules to resolve ambiguity were
    //applied no object would be found, as the "Ambiguous ancestor" found using
    //the ambiguity resolving rules have no "The object" descendants.
    assertFindObject("Ambiguous ancestor/The object", mObject14_1_2);
}

void ObjectFinderTest::testFindObjectEmptyName() {
    assertFindObject("", 0);
}

void ObjectFinderTest::testFindObjectSingleSlash() {
    assertFindObject("/", 0);
}

void ObjectFinderTest::testFindObjectSlashEndedName() {
    assertFindObject("Parent/", 0);
}

void ObjectFinderTest::testFindObjectSeveralSlashes() {
    assertFindObject("Parent1///The object", mObject1_1_1);
}

/////////////////////////////////Helpers////////////////////////////////////////

void ObjectFinderTest::assertFindObject(const QString& objectName,
                                     QObject* object) const {
    QCOMPARE(ObjectFinder(0).findObject<QObject*>(objectName, mMainWindow),
             object);
}

void ObjectFinderTest::assertFindAction(const QString& objectName,
                                     QAction* action) const {
    QCOMPARE(ObjectFinder(0).findObject<QAction*>(objectName, mMainWindow),
             action);
}

}

QTEST_MAIN(ktutorial::ObjectFinderTest)

#include "ObjectFinderTest.moc"
