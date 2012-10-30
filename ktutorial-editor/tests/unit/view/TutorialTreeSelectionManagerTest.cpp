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

#include "TutorialTreeSelectionManager.h"

#include <QSignalSpy>

#include "TreeModel.h"
#include "TutorialTreeItem.h"
#include "../data/Reaction.h"
#include "../data/Step.h"
#include "../data/Tutorial.h"
#include "../data/WaitForComposed.h"

class TutorialTreeSelectionManagerTest: public QObject {
Q_OBJECT

private slots:

    void initTestCase();
    void init();
    void cleanup();
    void cleanupTestCase();

    void testConstructor();

    void testSelectTutorialOrChildren();

    void testSelectStepOrChildrenChangingToOtherItems();
    void testSelectStepOrChildrenWithoutChangingStep();
    void testSelectStepOrChildrenChangingToAnotherStep();

    void testSelectReactionOrChildrenChangingToOtherItems();
    void testSelectReactionOrChildrenWithoutChangingReaction();
    void testSelectReactionOrChildrenChangingToAnotherReaction();

private:

    int mStepStarType;
    int mReactionStarType;

    Tutorial* mTutorial;
    Step* mStep1;
    Reaction* mReaction1;
    WaitForComposed* mWaitFor1;
    WaitForComposed* mWaitFor1_1;
    Step* mStep2;
    Reaction* mReaction2;
    TreeModel* mTreeModel;

    QItemSelectionModel* mSelectionModel;
    TutorialTreeSelectionManager* mSelectionManager;
    QSignalSpy* mStepSelectedSpy;
    QSignalSpy* mReactionSelectedSpy;

    void select(const QModelIndex& index);

    void assertStepSignal(const QSignalSpy* spy, int index, Step* step) const;
    void assertReactionSignal(const QSignalSpy* spy, int index,
                              Reaction* reaction) const;

};

void TutorialTreeSelectionManagerTest::initTestCase() {
    //Step* must be registered in order to be used with QSignalSpy
    mStepStarType = qRegisterMetaType<Step*>("Step*");

    //Reaction* must be registered in order to be used with QSignalSpy
    mReactionStarType = qRegisterMetaType<Reaction*>("Reaction*");

    mTutorial = new Tutorial();
    mTutorial->setName("The name");
    mTutorial->setDescription("The description");
    mTutorial->setLicenseText("The license text");
    mTutorial->setCustomSetupCode("The setup code");
    mTutorial->setCustomTearDownCode("The tear down code");

    mStep1 = new Step();
    mStep1->setId("First step");
    mStep1->setText("Text 1");
    mStep1->setCustomSetupCode("Setup 1");
    mStep1->setCustomTearDownCode("Tear down 1");
    mTutorial->addStep(mStep1);

    mReaction1 = new Reaction();
    mReaction1->setTriggerType(Reaction::ConditionMet);
    mReaction1->setResponseType(Reaction::CustomCode);
    mStep1->addReaction(mReaction1);

    mWaitFor1 = new WaitForComposed();
    mWaitFor1_1 = new WaitForComposed();
    mWaitFor1->addWaitFor(mWaitFor1_1);
    mReaction1->setWaitFor(mWaitFor1);

    mStep2 = new Step();
    mStep2->setId("Second step");
    mStep2->setText("Text 2");
    mStep2->setCustomSetupCode("Setup 2");
    mStep2->setCustomTearDownCode("Tear down 2");
    mTutorial->addStep(mStep2);

    mReaction2 = new Reaction();
    mStep2->addReaction(mReaction2);

    mTreeModel = new TreeModel(new TutorialTreeItem(mTutorial));
}

void TutorialTreeSelectionManagerTest::init() {
    mSelectionModel = new QItemSelectionModel(mTreeModel);
    mSelectionManager = new TutorialTreeSelectionManager(mSelectionModel);

    mStepSelectedSpy = new QSignalSpy(mSelectionManager,
                                      SIGNAL(stepSelected(Step*)));

    mReactionSelectedSpy = new QSignalSpy(mSelectionManager,
                                          SIGNAL(reactionSelected(Reaction*)));
}

void TutorialTreeSelectionManagerTest::cleanup() {
    delete mReactionSelectedSpy;
    delete mStepSelectedSpy;
    delete mSelectionManager;
    delete mSelectionModel;
}

void TutorialTreeSelectionManagerTest::cleanupTestCase() {
    delete mTreeModel;
    delete mTutorial;
}

void TutorialTreeSelectionManagerTest::testConstructor() {
    QObject parent;
    TutorialTreeSelectionManager* selectionManager = 
                    new TutorialTreeSelectionManager(mSelectionModel, &parent);

    QCOMPARE(selectionManager->parent(), &parent);
}

void TutorialTreeSelectionManagerTest::testSelectTutorialOrChildren() {
    //Name
    select(mTreeModel->index(0, 0));
    //Description
    select(mTreeModel->index(1, 0));
    //License
    select(mTreeModel->index(2, 0));
    select(mTreeModel->index(0, 0, mTreeModel->index(2, 0)));
    //Setup
    select(mTreeModel->index(3, 0));
    select(mTreeModel->index(0, 0, mTreeModel->index(3, 0)));
    //Tear down
    select(mTreeModel->index(4, 0));
    select(mTreeModel->index(0, 0, mTreeModel->index(4, 0)));

    QCOMPARE(mStepSelectedSpy->count(), 0);
    QCOMPARE(mReactionSelectedSpy->count(), 0);
}

void TutorialTreeSelectionManagerTest::
                                testSelectStepOrChildrenChangingToOtherItems() {
    QModelIndex step1Index = mTreeModel->index(5, 0);

    select(step1Index);

    QCOMPARE(mStepSelectedSpy->count(), 1);
    assertStepSignal(mStepSelectedSpy, 0, mStep1);

    //Tutorial name
    select(mTreeModel->index(0, 0));

    QCOMPARE(mStepSelectedSpy->count(), 2);
    assertStepSignal(mStepSelectedSpy, 1, 0);

    //Text
    select(mTreeModel->index(0, 0, step1Index));

    QCOMPARE(mStepSelectedSpy->count(), 3);
    assertStepSignal(mStepSelectedSpy, 2, mStep1);

    select(mTreeModel->index(0, 0));

    QCOMPARE(mStepSelectedSpy->count(), 4);
    assertStepSignal(mStepSelectedSpy, 3, 0);

    //Setup
    select(mTreeModel->index(1, 0, step1Index));

    QCOMPARE(mStepSelectedSpy->count(), 5);
    assertStepSignal(mStepSelectedSpy, 4, mStep1);

    select(mTreeModel->index(0, 0));

    QCOMPARE(mStepSelectedSpy->count(), 6);
    assertStepSignal(mStepSelectedSpy, 5, 0);

    //Setup code
    select(mTreeModel->index(0, 0, mTreeModel->index(1, 0, step1Index)));

    QCOMPARE(mStepSelectedSpy->count(), 7);
    assertStepSignal(mStepSelectedSpy, 6, mStep1);

    select(mTreeModel->index(0, 0));

    QCOMPARE(mStepSelectedSpy->count(), 8);
    assertStepSignal(mStepSelectedSpy, 7, 0);

    //Tear down
    select(mTreeModel->index(2, 0, step1Index));

    QCOMPARE(mStepSelectedSpy->count(), 9);
    assertStepSignal(mStepSelectedSpy, 8, mStep1);

    select(mTreeModel->index(0, 0));

    QCOMPARE(mStepSelectedSpy->count(), 10);
    assertStepSignal(mStepSelectedSpy, 9, 0);

    //Tear down code
    select(mTreeModel->index(0, 0, mTreeModel->index(2, 0, step1Index)));

    QCOMPARE(mStepSelectedSpy->count(), 11);
    assertStepSignal(mStepSelectedSpy, 10, mStep1);

    select(mTreeModel->index(0, 0));

    QCOMPARE(mStepSelectedSpy->count(), 12);
    assertStepSignal(mStepSelectedSpy, 11, 0);

    //Reaction
    select(mTreeModel->index(3, 0, step1Index));

    QCOMPARE(mStepSelectedSpy->count(), 13);
    assertStepSignal(mStepSelectedSpy, 12, mStep1);

    select(mTreeModel->index(0, 0));

    QCOMPARE(mStepSelectedSpy->count(), 14);
    assertStepSignal(mStepSelectedSpy, 13, 0);
}

void TutorialTreeSelectionManagerTest::
                                testSelectStepOrChildrenWithoutChangingStep() {
    QModelIndex step1Index = mTreeModel->index(5, 0);

    select(step1Index);

    QCOMPARE(mStepSelectedSpy->count(), 1);
    assertStepSignal(mStepSelectedSpy, 0, mStep1);

    //Text
    select(mTreeModel->index(0, 0, step1Index));

    QCOMPARE(mStepSelectedSpy->count(), 1);

    //Setup
    select(mTreeModel->index(1, 0, step1Index));

    QCOMPARE(mStepSelectedSpy->count(), 1);

    //Setup code
    select(mTreeModel->index(0, 0, mTreeModel->index(1, 0, step1Index)));

    QCOMPARE(mStepSelectedSpy->count(), 1);

    //Tear down
    select(mTreeModel->index(2, 0, step1Index));

    QCOMPARE(mStepSelectedSpy->count(), 1);

    //Tear down code
    select(mTreeModel->index(0, 0, mTreeModel->index(2, 0, step1Index)));

    QCOMPARE(mStepSelectedSpy->count(), 1);

    //Reaction
    select(mTreeModel->index(3, 0, step1Index));

    QCOMPARE(mStepSelectedSpy->count(), 1);
}

void TutorialTreeSelectionManagerTest::
                            testSelectStepOrChildrenChangingToAnotherStep() {
    QModelIndex step1Index = mTreeModel->index(5, 0);
    QModelIndex step2Index = mTreeModel->index(6, 0);

    select(step1Index);

    QCOMPARE(mStepSelectedSpy->count(), 1);
    assertStepSignal(mStepSelectedSpy, 0, mStep1);

    select(step2Index);

    QCOMPARE(mStepSelectedSpy->count(), 2);
    assertStepSignal(mStepSelectedSpy, 1, mStep2);

    //Text
    select(mTreeModel->index(0, 0, step1Index));

    QCOMPARE(mStepSelectedSpy->count(), 3);
    assertStepSignal(mStepSelectedSpy, 2, mStep1);

    select(mTreeModel->index(0, 0, step2Index));

    QCOMPARE(mStepSelectedSpy->count(), 4);
    assertStepSignal(mStepSelectedSpy, 3, mStep2);

    //Setup
    select(mTreeModel->index(1, 0, step1Index));

    QCOMPARE(mStepSelectedSpy->count(), 5);
    assertStepSignal(mStepSelectedSpy, 4, mStep1);

    select(mTreeModel->index(1, 0, step2Index));

    QCOMPARE(mStepSelectedSpy->count(), 6);
    assertStepSignal(mStepSelectedSpy, 5, mStep2);

    //Setup code
    select(mTreeModel->index(0, 0, mTreeModel->index(1, 0, step1Index)));

    QCOMPARE(mStepSelectedSpy->count(), 7);
    assertStepSignal(mStepSelectedSpy, 6, mStep1);

    select(mTreeModel->index(0, 0, mTreeModel->index(1, 0, step2Index)));

    QCOMPARE(mStepSelectedSpy->count(), 8);
    assertStepSignal(mStepSelectedSpy, 7, mStep2);

    //Tear down
    select(mTreeModel->index(2, 0, step1Index));

    QCOMPARE(mStepSelectedSpy->count(), 9);
    assertStepSignal(mStepSelectedSpy, 8, mStep1);

    select(mTreeModel->index(2, 0, step2Index));

    QCOMPARE(mStepSelectedSpy->count(), 10);
    assertStepSignal(mStepSelectedSpy, 9, mStep2);

    //Tear down code
    select(mTreeModel->index(0, 0, mTreeModel->index(2, 0, step1Index)));

    QCOMPARE(mStepSelectedSpy->count(), 11);
    assertStepSignal(mStepSelectedSpy, 10, mStep1);

    select(mTreeModel->index(0, 0, mTreeModel->index(2, 0, step2Index)));

    QCOMPARE(mStepSelectedSpy->count(), 12);
    assertStepSignal(mStepSelectedSpy, 11, mStep2);

    //Reaction
    select(mTreeModel->index(3, 0, step1Index));

    QCOMPARE(mStepSelectedSpy->count(), 13);
    assertStepSignal(mStepSelectedSpy, 12, mStep1);

    select(mTreeModel->index(3, 0, step2Index));

    QCOMPARE(mStepSelectedSpy->count(), 14);
    assertStepSignal(mStepSelectedSpy, 13, mStep2);
}

void TutorialTreeSelectionManagerTest::
                            testSelectReactionOrChildrenChangingToOtherItems() {
    QModelIndex step1Index = mTreeModel->index(5, 0);
    QModelIndex reaction1_1Index = mTreeModel->index(3, 0, step1Index);

    select(reaction1_1Index);

    QCOMPARE(mReactionSelectedSpy->count(), 1);
    assertReactionSignal(mReactionSelectedSpy, 0, mReaction1);

    //Tutorial name
    select(mTreeModel->index(0, 0));

    QCOMPARE(mReactionSelectedSpy->count(), 2);
    assertReactionSignal(mReactionSelectedSpy, 1, 0);

    //Reaction trigger
    select(mTreeModel->index(0, 0, reaction1_1Index));

    QCOMPARE(mReactionSelectedSpy->count(), 3);
    assertReactionSignal(mReactionSelectedSpy, 2, mReaction1);

    select(mTreeModel->index(0, 0));

    QCOMPARE(mReactionSelectedSpy->count(), 4);
    assertReactionSignal(mReactionSelectedSpy, 3, 0);

    //Reaction child condition trigger
    select(mTreeModel->index(0, 0, mTreeModel->index(0, 0, reaction1_1Index)));

    QCOMPARE(mReactionSelectedSpy->count(), 5);
    assertReactionSignal(mReactionSelectedSpy, 4, mReaction1);

    select(mTreeModel->index(0, 0));

    QCOMPARE(mReactionSelectedSpy->count(), 6);
    assertReactionSignal(mReactionSelectedSpy, 5, 0);

    //Reaction response
    select(mTreeModel->index(1, 0, reaction1_1Index));

    QCOMPARE(mReactionSelectedSpy->count(), 7);
    assertReactionSignal(mReactionSelectedSpy, 6, mReaction1);

    select(mTreeModel->index(0, 0));

    QCOMPARE(mReactionSelectedSpy->count(), 8);
    assertReactionSignal(mReactionSelectedSpy, 7, 0);

    //Reaction custom code response
    select(mTreeModel->index(0, 0, mTreeModel->index(1, 0, reaction1_1Index)));

    QCOMPARE(mReactionSelectedSpy->count(), 9);
    assertReactionSignal(mReactionSelectedSpy, 8, mReaction1);

    select(mTreeModel->index(0, 0));

    QCOMPARE(mReactionSelectedSpy->count(), 10);
    assertReactionSignal(mReactionSelectedSpy, 9, 0);
}

void TutorialTreeSelectionManagerTest::
                        testSelectReactionOrChildrenWithoutChangingReaction() {
    QModelIndex step1Index = mTreeModel->index(5, 0);
    QModelIndex reaction1_1Index = mTreeModel->index(3, 0, step1Index);

    select(reaction1_1Index);

    QCOMPARE(mReactionSelectedSpy->count(), 1);
    assertReactionSignal(mReactionSelectedSpy, 0, mReaction1);

    //Reaction trigger
    select(mTreeModel->index(0, 0, reaction1_1Index));

    QCOMPARE(mReactionSelectedSpy->count(), 1);

    //Reaction child condition trigger
    select(mTreeModel->index(0, 0, mTreeModel->index(0, 0, reaction1_1Index)));

    QCOMPARE(mReactionSelectedSpy->count(), 1);

    //Reaction response
    select(mTreeModel->index(1, 0, reaction1_1Index));

    QCOMPARE(mReactionSelectedSpy->count(), 1);

    //Reaction custom code response
    select(mTreeModel->index(0, 0, mTreeModel->index(1, 0, reaction1_1Index)));

    QCOMPARE(mReactionSelectedSpy->count(), 1);
}

void TutorialTreeSelectionManagerTest::
                    testSelectReactionOrChildrenChangingToAnotherReaction() {
    QModelIndex step1Index = mTreeModel->index(5, 0);
    QModelIndex reaction1_1Index = mTreeModel->index(3, 0, step1Index);
    QModelIndex step2Index = mTreeModel->index(6, 0);
    QModelIndex reaction2_1Index = mTreeModel->index(3, 0, step2Index);

    select(reaction1_1Index);

    QCOMPARE(mReactionSelectedSpy->count(), 1);
    assertReactionSignal(mReactionSelectedSpy, 0, mReaction1);

    select(reaction2_1Index);

    QCOMPARE(mReactionSelectedSpy->count(), 2);
    assertReactionSignal(mReactionSelectedSpy, 1, mReaction2);

    //Reaction trigger
    select(mTreeModel->index(0, 0, reaction1_1Index));

    QCOMPARE(mReactionSelectedSpy->count(), 3);
    assertReactionSignal(mReactionSelectedSpy, 2, mReaction1);

    select(mTreeModel->index(0, 0, reaction2_1Index));

    QCOMPARE(mReactionSelectedSpy->count(), 4);
    assertReactionSignal(mReactionSelectedSpy, 3, mReaction2);

    //Reaction nested condition trigger
    select(mTreeModel->index(0, 0, mTreeModel->index(0, 0, reaction1_1Index)));

    QCOMPARE(mReactionSelectedSpy->count(), 5);
    assertReactionSignal(mReactionSelectedSpy, 4, mReaction1);

    select(mTreeModel->index(0, 0, reaction2_1Index));

    QCOMPARE(mReactionSelectedSpy->count(), 6);
    assertReactionSignal(mReactionSelectedSpy, 5, mReaction2);

    //Reaction response
    select(mTreeModel->index(1, 0, reaction1_1Index));

    QCOMPARE(mReactionSelectedSpy->count(), 7);
    assertReactionSignal(mReactionSelectedSpy, 6, mReaction1);

    select(mTreeModel->index(1, 0, reaction2_1Index));

    QCOMPARE(mReactionSelectedSpy->count(), 8);
    assertReactionSignal(mReactionSelectedSpy, 7, mReaction2);

    //Reaction custom code response
    select(mTreeModel->index(0, 0, mTreeModel->index(1, 0, reaction1_1Index)));

    QCOMPARE(mReactionSelectedSpy->count(), 9);
    assertReactionSignal(mReactionSelectedSpy, 8, mReaction1);

    select(reaction2_1Index);

    QCOMPARE(mReactionSelectedSpy->count(), 10);
    assertReactionSignal(mReactionSelectedSpy, 9, mReaction2);
}

/////////////////////////////////// Helpers ////////////////////////////////////

void TutorialTreeSelectionManagerTest::select(const QModelIndex& index) {
    mSelectionModel->select(index, QItemSelectionModel::SelectCurrent);
}

//Step* must be declared as a metatype to be used in qvariant_cast
Q_DECLARE_METATYPE(Step*);

void TutorialTreeSelectionManagerTest::assertStepSignal(const QSignalSpy* spy,
                                                        int index,
                                                        Step* step) const {
    QCOMPARE(spy->at(index).count(), 1);

    QVariant argument = spy->at(index).at(0);
    QCOMPARE(argument.userType(), mStepStarType);
    QCOMPARE(qvariant_cast<Step*>(argument), step);
}

//Reaction* must be declared as a metatype to be used in qvariant_cast
Q_DECLARE_METATYPE(Reaction*);

void TutorialTreeSelectionManagerTest::assertReactionSignal(
                                                    const QSignalSpy* spy,
                                                    int index,
                                                    Reaction* reaction) const {
    QCOMPARE(spy->at(index).count(), 1);

    QVariant argument = spy->at(index).at(0);
    QCOMPARE(argument.userType(), mReactionStarType);
    QCOMPARE(qvariant_cast<Reaction*>(argument), reaction);
}

QTEST_MAIN(TutorialTreeSelectionManagerTest)

#include "TutorialTreeSelectionManagerTest.moc"
