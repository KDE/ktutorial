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

#include "TutorialTreeItem.h"

#include <QSignalSpy>

#include <KLocalizedString>

#include "StepTreeItem.h"
#include "../data/Step.h"
#include "../data/Tutorial.h"

class TutorialTreeItemTest: public QObject {
Q_OBJECT

private slots:

    void initTestCase();

    void testConstructor();
    void testConstructorFullTutorial();

    void testTutorialSetName();
    void testTutorialSetNameChange();
    void testTutorialSetNameEmpty();

    void testTutorialSetDescription();
    void testTutorialSetDescriptionChange();
    void testTutorialSetDescriptionEmpty();

    void testTutorialSetLicenseText();
    void testTutorialSetLicenseTextChange();
    void testTutorialSetLicenseTextEmpty();

    void testTutorialSetCustomSetupCode();
    void testTutorialSetCustomSetupCodeChange();
    void testTutorialSetCustomSetupCodeEmpty();

    void testTutorialSetCustomTearDownCode();
    void testTutorialSetCustomTearDownCodeChange();
    void testTutorialSetCustomTearDownCodeEmpty();

    void testTutorialAddStep();
    void testTutorialAddStepAtIndex();

    void testTutorialRemoveStep();

    void testChildOrderWhenSettingDataInTutorial();
    void testChildOrderWhenUnsettingDataInTutorial();

private:

    int mTreeItemStarType;

    void assertEmptyName(TreeItem* nameItem) const;
    void assertName(TreeItem* nameItem, const QString& name) const;
    void assertEmptyDescription(TreeItem* descriptionItem) const;
    void assertDescription(TreeItem* descriptionItem,
                           const QString& description) const;
    void assertLicenseText(TreeItem* licenseItem,
                           const QString& licenseText) const;
    void assertCustomSetupCode(TreeItem* setupItem, const QString& code) const;
    void assertCustomTearDownCode(TreeItem* tearDownItem,
                                  const QString& code) const;
    void assertStep(TreeItem* stepItem, const QString& stepId) const;

    void assertDataChanged(const QSignalSpy& spy, int index,
                           TreeItem* item) const;

};

class StubTreeItem: public TreeItem {
public:
    virtual QString text() const {
        return "";
    }
};

void TutorialTreeItemTest::initTestCase() {
    //TreeItem* must be registered in order to be used with QSignalSpy
    mTreeItemStarType = qRegisterMetaType<TreeItem*>("TreeItem*");
}

void TutorialTreeItemTest::testConstructor() {
    Tutorial tutorial;

    StubTreeItem parent;
    TutorialTreeItem item(&tutorial, &parent);

    QCOMPARE(item.parent(), &parent);
    QCOMPARE(item.text(), i18nc("@item", "Tutorial"));
    QCOMPARE(item.childCount(), 2);
    assertEmptyName(item.child(0));
    assertEmptyDescription(item.child(1));
}

void TutorialTreeItemTest::testConstructorFullTutorial() {
    Tutorial tutorial;
    tutorial.setName("The name");
    tutorial.setDescription("The description");
    tutorial.setLicenseText("The license text");
    tutorial.setCustomSetupCode("The setup code");
    tutorial.setCustomTearDownCode("The tear down code");

    Step* step1 = new Step();
    step1->setId("First step");
    tutorial.addStep(step1);

    Step* step2 = new Step();
    step2->setId("Second step");
    tutorial.addStep(step2);

    StubTreeItem parent;
    TutorialTreeItem item(&tutorial, &parent);

    QCOMPARE(item.parent(), &parent);
    QCOMPARE(item.text(), i18nc("@item", "Tutorial %1", "theName"));
    QCOMPARE(item.childCount(), 7);
    assertName(item.child(0), "The name");
    assertDescription(item.child(1), "The description");
    assertLicenseText(item.child(2), "The license text");
    assertCustomSetupCode(item.child(3), "The setup code");
    assertCustomTearDownCode(item.child(4), "The tear down code");
    assertStep(item.child(5), "First step");
    assertStep(item.child(6), "Second step");
}

//TreeItem* must be declared as a metatype to be used in qvariant_cast
Q_DECLARE_METATYPE(TreeItem*);

void TutorialTreeItemTest::testTutorialSetName() {
    Tutorial tutorial;
    TutorialTreeItem item(&tutorial);

    //Setting the name changes the data returned by text() in the
    //TutorialTreeItem itself, as the id is based on the name
    QSignalSpy dataChangedRootSpy(&item, SIGNAL(dataChanged(TreeItem*)));
    QSignalSpy dataChangedSpy(item.child(0), SIGNAL(dataChanged(TreeItem*)));

    tutorial.setName("The name");

    QCOMPARE(item.text(), i18nc("@item", "Tutorial %1", "theName"));
    QCOMPARE(item.childCount(), 2);
    assertName(item.child(0), "The name");
    assertEmptyDescription(item.child(1));
    QCOMPARE(dataChangedRootSpy.count(), 1);
    assertDataChanged(dataChangedRootSpy, 0, &item);
    QCOMPARE(dataChangedSpy.count(), 1);
    assertDataChanged(dataChangedSpy, 0, item.child(0));
}

void TutorialTreeItemTest::testTutorialSetNameChange() {
    Tutorial tutorial;
    TutorialTreeItem item(&tutorial);

    tutorial.setName("The name");

    QSignalSpy dataChangedRootSpy(&item, SIGNAL(dataChanged(TreeItem*)));
    QSignalSpy dataChangedSpy(item.child(0), SIGNAL(dataChanged(TreeItem*)));

    tutorial.setName("The name changed");

    QCOMPARE(item.text(), i18nc("@item", "Tutorial %1", "theNameChanged"));
    QCOMPARE(item.childCount(), 2);
    assertName(item.child(0), "The name changed");
    assertEmptyDescription(item.child(1));
    QCOMPARE(dataChangedRootSpy.count(), 1);
    assertDataChanged(dataChangedRootSpy, 0, &item);
    QCOMPARE(dataChangedSpy.count(), 1);
    assertDataChanged(dataChangedSpy, 0, item.child(0));
}

void TutorialTreeItemTest::testTutorialSetNameEmpty() {
    Tutorial tutorial;
    TutorialTreeItem item(&tutorial);

    tutorial.setName("The name");

    QSignalSpy dataChangedRootSpy(&item, SIGNAL(dataChanged(TreeItem*)));
    QSignalSpy dataChangedSpy(item.child(0), SIGNAL(dataChanged(TreeItem*)));

    tutorial.setName("");

    QCOMPARE(item.text(), i18nc("@item", "Tutorial"));
    QCOMPARE(item.childCount(), 2);
    assertEmptyName(item.child(0));
    assertEmptyDescription(item.child(1));
    QCOMPARE(dataChangedRootSpy.count(), 1);
    assertDataChanged(dataChangedRootSpy, 0, &item);
    QCOMPARE(dataChangedSpy.count(), 1);
    assertDataChanged(dataChangedSpy, 0, item.child(0));
}

void TutorialTreeItemTest::testTutorialSetDescription() {
    Tutorial tutorial;
    TutorialTreeItem item(&tutorial);

    QSignalSpy dataChangedSpy(item.child(1), SIGNAL(dataChanged(TreeItem*)));

    tutorial.setDescription("The description");

    QCOMPARE(item.childCount(), 2);
    assertEmptyName(item.child(0));
    assertDescription(item.child(1), "The description");
    QCOMPARE(dataChangedSpy.count(), 1);
    assertDataChanged(dataChangedSpy, 0, item.child(1));
}

void TutorialTreeItemTest::testTutorialSetDescriptionChange() {
    Tutorial tutorial;
    TutorialTreeItem item(&tutorial);

    tutorial.setDescription("The description");

    QSignalSpy dataChangedSpy(item.child(1), SIGNAL(dataChanged(TreeItem*)));

    tutorial.setDescription("The description changed");

    QCOMPARE(item.childCount(), 2);
    assertEmptyName(item.child(0));
    assertDescription(item.child(1), "The description changed");
    QCOMPARE(dataChangedSpy.count(), 1);
    assertDataChanged(dataChangedSpy, 0, item.child(1));
}

void TutorialTreeItemTest::testTutorialSetDescriptionEmpty() {
    Tutorial tutorial;
    TutorialTreeItem item(&tutorial);

    tutorial.setDescription("The description");

    QSignalSpy dataChangedSpy(item.child(1), SIGNAL(dataChanged(TreeItem*)));

    tutorial.setDescription("");

    QCOMPARE(item.childCount(), 2);
    assertEmptyName(item.child(0));
    assertEmptyDescription(item.child(1));
    QCOMPARE(dataChangedSpy.count(), 1);
    assertDataChanged(dataChangedSpy, 0, item.child(1));
}

void TutorialTreeItemTest::testTutorialSetLicenseText() {
    Tutorial tutorial;
    TutorialTreeItem item(&tutorial);

    tutorial.setLicenseText("The license text");

    QCOMPARE(item.childCount(), 3);
    assertEmptyName(item.child(0));
    assertEmptyDescription(item.child(1));
    assertLicenseText(item.child(2), "The license text");
}

void TutorialTreeItemTest::testTutorialSetLicenseTextChange() {
    Tutorial tutorial;
    TutorialTreeItem item(&tutorial);

    tutorial.setLicenseText("The license text");

    QSignalSpy dataChangedSpy(item.child(2)->child(0),
                              SIGNAL(dataChanged(TreeItem*)));

    tutorial.setLicenseText("The license text changed");

    QCOMPARE(item.childCount(), 3);
    assertEmptyName(item.child(0));
    assertEmptyDescription(item.child(1));
    assertLicenseText(item.child(2), "The license text changed");
    QCOMPARE(dataChangedSpy.count(), 1);
    assertDataChanged(dataChangedSpy, 0, item.child(2)->child(0));
}

void TutorialTreeItemTest::testTutorialSetLicenseTextEmpty() {
    Tutorial tutorial;
    TutorialTreeItem item(&tutorial);

    tutorial.setLicenseText("The license text");
    tutorial.setLicenseText("");

    QCOMPARE(item.childCount(), 2);
    assertEmptyName(item.child(0));
    assertEmptyDescription(item.child(1));
}

void TutorialTreeItemTest::testTutorialSetCustomSetupCode() {
    Tutorial tutorial;
    TutorialTreeItem item(&tutorial);

    tutorial.setCustomSetupCode("The setup code");

    QCOMPARE(item.childCount(), 3);
    assertEmptyName(item.child(0));
    assertEmptyDescription(item.child(1));
    assertCustomSetupCode(item.child(2), "The setup code");
}

void TutorialTreeItemTest::testTutorialSetCustomSetupCodeChange() {
    Tutorial tutorial;
    TutorialTreeItem item(&tutorial);

    tutorial.setCustomSetupCode("The setup code");

    QSignalSpy dataChangedSpy(item.child(2)->child(0),
                              SIGNAL(dataChanged(TreeItem*)));

    tutorial.setCustomSetupCode("The setup code changed");

    QCOMPARE(item.childCount(), 3);
    assertEmptyName(item.child(0));
    assertEmptyDescription(item.child(1));
    assertCustomSetupCode(item.child(2), "The setup code changed");
    QCOMPARE(dataChangedSpy.count(), 1);
    assertDataChanged(dataChangedSpy, 0, item.child(2)->child(0));
}

void TutorialTreeItemTest::testTutorialSetCustomSetupCodeEmpty() {
    Tutorial tutorial;
    TutorialTreeItem item(&tutorial);

    tutorial.setCustomSetupCode("The setup code");
    tutorial.setCustomSetupCode("");

    QCOMPARE(item.childCount(), 2);
    assertEmptyName(item.child(0));
    assertEmptyDescription(item.child(1));
}

void TutorialTreeItemTest::testTutorialSetCustomTearDownCode() {
    Tutorial tutorial;
    TutorialTreeItem item(&tutorial);

    tutorial.setCustomTearDownCode("The tear down code");

    QCOMPARE(item.childCount(), 3);
    assertEmptyName(item.child(0));
    assertEmptyDescription(item.child(1));
    assertCustomTearDownCode(item.child(2), "The tear down code");
}

void TutorialTreeItemTest::testTutorialSetCustomTearDownCodeChange() {
    Tutorial tutorial;
    TutorialTreeItem item(&tutorial);

    tutorial.setCustomTearDownCode("The tear down code");

    QSignalSpy dataChangedSpy(item.child(2)->child(0),
                              SIGNAL(dataChanged(TreeItem*)));

    tutorial.setCustomTearDownCode("The tear down code changed");

    QCOMPARE(item.childCount(), 3);
    assertEmptyName(item.child(0));
    assertEmptyDescription(item.child(1));
    assertCustomTearDownCode(item.child(2), "The tear down code changed");
    QCOMPARE(dataChangedSpy.count(), 1);
    assertDataChanged(dataChangedSpy, 0, item.child(2)->child(0));
}

void TutorialTreeItemTest::testTutorialSetCustomTearDownCodeEmpty() {
    Tutorial tutorial;
    TutorialTreeItem item(&tutorial);

    tutorial.setCustomTearDownCode("The tear down code");
    tutorial.setCustomTearDownCode("");

    QCOMPARE(item.childCount(), 2);
    assertEmptyName(item.child(0));
    assertEmptyDescription(item.child(1));
}

void TutorialTreeItemTest::testTutorialAddStep() {
    Tutorial tutorial;
    TutorialTreeItem item(&tutorial);

    Step* step = new Step();
    step->setId("Step id");
    tutorial.addStep(step);

    QCOMPARE(item.childCount(), 3);
    assertEmptyName(item.child(0));
    assertEmptyDescription(item.child(1));
    assertStep(item.child(2), "Step id");
}

void TutorialTreeItemTest::testTutorialAddStepAtIndex() {
    Tutorial tutorial;
    TutorialTreeItem item(&tutorial);

    Step* step2 = new Step();
    step2->setId("Step id2");
    tutorial.addStep(step2, 0);

    Step* step1 = new Step();
    step1->setId("Step id1");
    tutorial.addStep(step1, 0);

    Step* step3 = new Step();
    step3->setId("Step id3");
    tutorial.addStep(step3, 2);

    QCOMPARE(item.childCount(), 5);
    assertEmptyName(item.child(0));
    assertEmptyDescription(item.child(1));
    assertStep(item.child(2), "Step id1");
    assertStep(item.child(3), "Step id2");
    assertStep(item.child(4), "Step id3");
}

void TutorialTreeItemTest::testTutorialRemoveStep() {
    Tutorial tutorial;
    TutorialTreeItem item(&tutorial);

    //It will be removed and not deleted by the Tutorial, so it is created in
    //stack
    Step step;
    step.setId("Step id");
    tutorial.addStep(&step);
    tutorial.removeStep(&step);

    QCOMPARE(item.childCount(), 2);
    assertEmptyName(item.child(0));
    assertEmptyDescription(item.child(1));
}

void TutorialTreeItemTest::testChildOrderWhenSettingDataInTutorial() {
    Tutorial tutorial;
    TutorialTreeItem item(&tutorial);

    tutorial.setCustomSetupCode("The setup code");

    QCOMPARE(item.text(), i18nc("@item", "Tutorial"));
    QCOMPARE(item.childCount(), 3);
    assertEmptyName(item.child(0));
    assertEmptyDescription(item.child(1));
    assertCustomSetupCode(item.child(2), "The setup code");

    tutorial.setDescription("The description");

    QCOMPARE(item.text(), i18nc("@item", "Tutorial"));
    QCOMPARE(item.childCount(), 3);
    assertEmptyName(item.child(0));
    assertDescription(item.child(1), "The description");
    assertCustomSetupCode(item.child(2), "The setup code");

    tutorial.setCustomTearDownCode("The tear down code");

    QCOMPARE(item.text(), i18nc("@item", "Tutorial"));
    QCOMPARE(item.childCount(), 4);
    assertEmptyName(item.child(0));
    assertDescription(item.child(1), "The description");
    assertCustomSetupCode(item.child(2), "The setup code");
    assertCustomTearDownCode(item.child(3), "The tear down code");

    Step* step1 = new Step();
    step1->setId("First step");
    tutorial.addStep(step1);

    QCOMPARE(item.text(), i18nc("@item", "Tutorial"));
    QCOMPARE(item.childCount(), 5);
    assertEmptyName(item.child(0));
    assertDescription(item.child(1), "The description");
    assertCustomSetupCode(item.child(2), "The setup code");
    assertCustomTearDownCode(item.child(3), "The tear down code");
    assertStep(item.child(4), "First step");

    tutorial.setName("The name");

    QCOMPARE(item.text(), i18nc("@item", "Tutorial %1", "theName"));
    QCOMPARE(item.childCount(), 5);
    assertName(item.child(0), "The name");
    assertDescription(item.child(1), "The description");
    assertCustomSetupCode(item.child(2), "The setup code");
    assertCustomTearDownCode(item.child(3), "The tear down code");
    assertStep(item.child(4), "First step");

    Step* step2 = new Step();
    step2->setId("Second step");
    tutorial.addStep(step2);

    QCOMPARE(item.text(), i18nc("@item", "Tutorial %1", "theName"));
    QCOMPARE(item.childCount(), 6);
    assertName(item.child(0), "The name");
    assertDescription(item.child(1), "The description");
    assertCustomSetupCode(item.child(2), "The setup code");
    assertCustomTearDownCode(item.child(3), "The tear down code");
    assertStep(item.child(4), "First step");
    assertStep(item.child(5), "Second step");

    Step* step3 = new Step();
    step3->setId("Third step");
    tutorial.addStep(step3, 1);

    QCOMPARE(item.text(), i18nc("@item", "Tutorial %1", "theName"));
    QCOMPARE(item.childCount(), 7);
    assertName(item.child(0), "The name");
    assertDescription(item.child(1), "The description");
    assertCustomSetupCode(item.child(2), "The setup code");
    assertCustomTearDownCode(item.child(3), "The tear down code");
    assertStep(item.child(4), "First step");
    assertStep(item.child(5), "Third step");
    assertStep(item.child(6), "Second step");

    tutorial.setLicenseText("The license text");

    QCOMPARE(item.text(), i18nc("@item", "Tutorial %1", "theName"));
    QCOMPARE(item.childCount(), 8);
    assertName(item.child(0), "The name");
    assertDescription(item.child(1), "The description");
    assertLicenseText(item.child(2), "The license text");
    assertCustomSetupCode(item.child(3), "The setup code");
    assertCustomTearDownCode(item.child(4), "The tear down code");
    assertStep(item.child(5), "First step");
    assertStep(item.child(6), "Third step");
    assertStep(item.child(7), "Second step");
}

void TutorialTreeItemTest::testChildOrderWhenUnsettingDataInTutorial() {
    Tutorial tutorial;
    tutorial.setName("The name");
    tutorial.setDescription("The description");
    tutorial.setLicenseText("The license text");
    tutorial.setCustomSetupCode("The setup code");
    tutorial.setCustomTearDownCode("The tear down code");

    //They will be removed and not deleted by the Tutorial, so they are created
    //in stack
    Step step1;
    step1.setId("First step");
    tutorial.addStep(&step1);

    Step step2;
    step2.setId("Second step");
    tutorial.addStep(&step2);

    TutorialTreeItem item(&tutorial);

    tutorial.setLicenseText("");

    QCOMPARE(item.text(), i18nc("@item", "Tutorial %1", "theName"));
    QCOMPARE(item.childCount(), 6);
    assertName(item.child(0), "The name");
    assertDescription(item.child(1), "The description");
    assertCustomSetupCode(item.child(2), "The setup code");
    assertCustomTearDownCode(item.child(3), "The tear down code");
    assertStep(item.child(4), "First step");
    assertStep(item.child(5), "Second step");

    tutorial.setName("");

    QCOMPARE(item.text(), i18nc("@item", "Tutorial"));
    QCOMPARE(item.childCount(), 6);
    assertEmptyName(item.child(0));
    assertDescription(item.child(1), "The description");
    assertCustomSetupCode(item.child(2), "The setup code");
    assertCustomTearDownCode(item.child(3), "The tear down code");
    assertStep(item.child(4), "First step");
    assertStep(item.child(5), "Second step");

    tutorial.removeStep(&step1);

    QCOMPARE(item.text(), i18nc("@item", "Tutorial"));
    QCOMPARE(item.childCount(), 5);
    assertEmptyName(item.child(0));
    assertDescription(item.child(1), "The description");
    assertCustomSetupCode(item.child(2), "The setup code");
    assertCustomTearDownCode(item.child(3), "The tear down code");
    assertStep(item.child(4), "Second step");

    tutorial.setCustomTearDownCode("");

    QCOMPARE(item.text(), i18nc("@item", "Tutorial"));
    QCOMPARE(item.childCount(), 4);
    assertEmptyName(item.child(0));
    assertDescription(item.child(1), "The description");
    assertCustomSetupCode(item.child(2), "The setup code");
    assertStep(item.child(3), "Second step");

    tutorial.setDescription("");

    QCOMPARE(item.text(), i18nc("@item", "Tutorial"));
    QCOMPARE(item.childCount(), 4);
    assertEmptyName(item.child(0));
    assertEmptyDescription(item.child(1));
    assertCustomSetupCode(item.child(2), "The setup code");
    assertStep(item.child(3), "Second step");

    tutorial.setCustomSetupCode("");

    QCOMPARE(item.text(), i18nc("@item", "Tutorial"));
    QCOMPARE(item.childCount(), 3);
    assertEmptyName(item.child(0));
    assertEmptyDescription(item.child(1));
    assertStep(item.child(2), "Second step");

    tutorial.removeStep(&step2);

    QCOMPARE(item.text(), i18nc("@item", "Tutorial"));
    QCOMPARE(item.childCount(), 2);
    assertEmptyName(item.child(0));
    assertEmptyDescription(item.child(1));
}

/////////////////////////////////// Helpers ////////////////////////////////////

void TutorialTreeItemTest::assertEmptyName(TreeItem* nameItem) const {
    QCOMPARE(nameItem->text(), i18nc("@item", "Name: (name not set)"));
}

void TutorialTreeItemTest::assertName(TreeItem* nameItem,
                                      const QString& name) const {
    QCOMPARE(nameItem->text(), i18nc("@item Noun", "Name: %1", name));
}

void TutorialTreeItemTest::assertEmptyDescription(
                                            TreeItem* descriptionItem) const {
    QCOMPARE(descriptionItem->text(),
             i18nc("@item", "Description: (description not set)"));
}

void TutorialTreeItemTest::assertDescription(TreeItem* descriptionItem,
                                             const QString& description) const {
    QCOMPARE(descriptionItem->text(),
             i18nc("@item", "Description: %1", description));
}

void TutorialTreeItemTest::assertLicenseText(TreeItem* licenseItem,
                                             const QString& licenseText) const {
    QCOMPARE(licenseItem->text(), i18nc("@item", "License:"));
    QCOMPARE(licenseItem->childCount(), 1);
    QCOMPARE(licenseItem->child(0)->text(),
             i18nc("@item", licenseText.toAscii()));
}

void TutorialTreeItemTest::assertCustomSetupCode(TreeItem* setupItem,
                                                 const QString& code) const {
    QCOMPARE(setupItem->text(), i18nc("@item", "Setup:"));
    QCOMPARE(setupItem->childCount(), 1);
    QCOMPARE(setupItem->child(0)->text(), i18nc("@item", code.toAscii()));
}

void TutorialTreeItemTest::assertCustomTearDownCode(TreeItem* tearDownItem,
                                                    const QString& code) const {
    QCOMPARE(tearDownItem->text(), i18nc("@item", "Tear down:"));
    QCOMPARE(tearDownItem->childCount(), 1);
    QCOMPARE(tearDownItem->child(0)->text(), i18nc("@item", code.toAscii()));
}

void TutorialTreeItemTest::assertStep(TreeItem* stepItem,
                                      const QString& stepId) const {
    QVERIFY(qobject_cast<StepTreeItem*>(stepItem));
    QCOMPARE(stepItem->text(), i18nc("@item Noun", "Step %1", stepId));
}

void TutorialTreeItemTest::assertDataChanged(const QSignalSpy& spy, int index,
                                             TreeItem* item) const {
    QCOMPARE(spy.at(index).count(), 1);

    QVariant argument = spy.at(index).at(0);
    QCOMPARE(argument.userType(), mTreeItemStarType);
    QCOMPARE(qvariant_cast<TreeItem*>(argument), item);
}

QTEST_MAIN(TutorialTreeItemTest)

#include "TutorialTreeItemTest.moc"
