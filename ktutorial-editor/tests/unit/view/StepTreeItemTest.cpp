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

#include "StepTreeItem.h"

#include <QSignalSpy>

#include <KLocalizedString>

#include "ReactionTreeItem.h"
#include "../data/Step.h"
#include "../data/Reaction.h"

class StepTreeItemTest: public QObject {
Q_OBJECT

private slots:

    void initTestCase();

    void testConstructor();
    void testConstructorFullStep();

    void testStepSetId();
    void testStepSetIdChange();
    void testStepSetIdEmpty();

    void testStepSetText();
    void testStepSetTextChange();
    void testStepSetTextEmpty();

    void testStepSetCustomSetupCode();
    void testStepSetCustomSetupCodeChange();
    void testStepSetCustomSetupCodeEmpty();

    void testStepSetCustomTearDownCode();
    void testStepSetCustomTearDownCodeChange();
    void testStepSetCustomTearDownCodeEmpty();

    void testStepAddReaction();
    void testStepAddReactionAtIndex();

    void testStepRemoveReaction();

    void testChildOrderWhenSettingDataInStep();
    void testChildOrderWhenUnsettingDataInStep();

private:

    int mTreeItemStarType;

    void assertEmptyText(TreeItem* textItem) const;
    void assertText(TreeItem* textItem, const QString& licenseText) const;
    void assertCustomSetupCode(TreeItem* setupItem, const QString& code) const;
    void assertCustomTearDownCode(TreeItem* tearDownItem,
                                  const QString& code) const;
    void assertReaction(TreeItem* reactionItem, const Reaction* reaction) const;

    void assertDataChanged(const QSignalSpy& spy, int index,
                           TreeItem* item) const;

};

class StubTreeItem: public TreeItem {
public:
    virtual QString text() const {
        return "";
    }
};

void StepTreeItemTest::initTestCase() {
    //TreeItem* must be registered in order to be used with QSignalSpy
    mTreeItemStarType = qRegisterMetaType<TreeItem*>("TreeItem*");
}

void StepTreeItemTest::testConstructor() {
    Step step;

    StubTreeItem parent;
    StepTreeItem item(&step, &parent);

    QCOMPARE(item.parent(), &parent);
    QCOMPARE(item.text(), i18nc("@item", "Step (id not set)"));
    QCOMPARE(item.step(), &step);
    QCOMPARE(item.childCount(), 1);
    assertEmptyText(item.child(0));
}

void StepTreeItemTest::testConstructorFullStep() {
    Step step;
    step.setId("The id");
    step.setText("The text");
    step.setCustomSetupCode("The setup code");
    step.setCustomTearDownCode("The tear down code");

    Reaction* reaction1 = new Reaction();
    step.addReaction(reaction1);

    Reaction* reaction2 = new Reaction();
    step.addReaction(reaction2);

    StubTreeItem parent;
    StepTreeItem item(&step, &parent);

    QCOMPARE(item.parent(), &parent);
    QCOMPARE(item.text(), i18nc("@item", "Step %1", "The id"));
    QCOMPARE(item.step(), &step);
    QCOMPARE(item.childCount(), 5);
    assertText(item.child(0), "The text");
    assertCustomSetupCode(item.child(1), "The setup code");
    assertCustomTearDownCode(item.child(2), "The tear down code");
    assertReaction(item.child(3), reaction1);
    assertReaction(item.child(4), reaction2);
}

//TreeItem* must be declared as a metatype to be used in qvariant_cast
Q_DECLARE_METATYPE(TreeItem*);

void StepTreeItemTest::testStepSetId() {
    Step step;
    StepTreeItem item(&step);

    //Setting the id changes the data returned by text() in the
    //StepTreeItem itself
    QSignalSpy dataChangedSpy(&item, SIGNAL(dataChanged(TreeItem*)));

    step.setId("The id");

    QCOMPARE(item.text(), i18nc("@item", "Step %1", "The id"));
    QCOMPARE(item.childCount(), 1);
    assertEmptyText(item.child(0));
    QCOMPARE(dataChangedSpy.count(), 1);
    assertDataChanged(dataChangedSpy, 0, &item);
}

void StepTreeItemTest::testStepSetIdChange() {
    Step step;
    StepTreeItem item(&step);

    step.setId("The id");

    QSignalSpy dataChangedSpy(&item, SIGNAL(dataChanged(TreeItem*)));

    step.setId("The id changed");

    QCOMPARE(item.text(), i18nc("@item", "Step %1", "The id changed"));
    QCOMPARE(item.childCount(), 1);
    assertEmptyText(item.child(0));
    QCOMPARE(dataChangedSpy.count(), 1);
    assertDataChanged(dataChangedSpy, 0, &item);
}

void StepTreeItemTest::testStepSetIdEmpty() {
    Step step;
    StepTreeItem item(&step);

    step.setId("The id");

    QSignalSpy dataChangedSpy(&item, SIGNAL(dataChanged(TreeItem*)));

    step.setId("");

    QCOMPARE(item.text(), i18nc("@item", "Step (id not set)"));
    QCOMPARE(item.childCount(), 1);
    assertEmptyText(item.child(0));
    QCOMPARE(dataChangedSpy.count(), 1);
    assertDataChanged(dataChangedSpy, 0, &item);
}

void StepTreeItemTest::testStepSetText() {
    Step step;
    StepTreeItem item(&step);

    QSignalSpy dataChangedSpy(item.child(0), SIGNAL(dataChanged(TreeItem*)));

    step.setText("The text");

    QCOMPARE(item.childCount(), 1);
    assertText(item.child(0), "The text");
    QCOMPARE(dataChangedSpy.count(), 1);
    assertDataChanged(dataChangedSpy, 0, item.child(0));
}

void StepTreeItemTest::testStepSetTextChange() {
    Step step;
    StepTreeItem item(&step);

    step.setText("The text");

    QSignalSpy dataChangedSpy(item.child(0), SIGNAL(dataChanged(TreeItem*)));

    step.setText("The text changed");

    QCOMPARE(item.childCount(), 1);
    assertText(item.child(0), "The text changed");
    QCOMPARE(dataChangedSpy.count(), 1);
    assertDataChanged(dataChangedSpy, 0, item.child(0));
}

void StepTreeItemTest::testStepSetTextEmpty() {
    Step step;
    StepTreeItem item(&step);

    step.setText("The text");

    QSignalSpy dataChangedSpy(item.child(0), SIGNAL(dataChanged(TreeItem*)));

    step.setText("");

    QCOMPARE(item.childCount(), 1);
    assertEmptyText(item.child(0));
    QCOMPARE(dataChangedSpy.count(), 1);
    assertDataChanged(dataChangedSpy, 0, item.child(0));
}

void StepTreeItemTest::testStepSetCustomSetupCode() {
    Step step;
    StepTreeItem item(&step);

    step.setCustomSetupCode("The setup code");

    QCOMPARE(item.childCount(), 2);
    assertEmptyText(item.child(0));
    assertCustomSetupCode(item.child(1), "The setup code");
}

void StepTreeItemTest::testStepSetCustomSetupCodeChange() {
    Step step;
    StepTreeItem item(&step);

    step.setCustomSetupCode("The setup code");

    QSignalSpy dataChangedSpy(item.child(1)->child(0),
                              SIGNAL(dataChanged(TreeItem*)));

    step.setCustomSetupCode("The setup code changed");

    QCOMPARE(item.childCount(), 2);
    assertEmptyText(item.child(0));
    assertCustomSetupCode(item.child(1), "The setup code changed");
    QCOMPARE(dataChangedSpy.count(), 1);
    assertDataChanged(dataChangedSpy, 0, item.child(1)->child(0));
}

void StepTreeItemTest::testStepSetCustomSetupCodeEmpty() {
    Step step;
    StepTreeItem item(&step);

    step.setCustomSetupCode("The setup code");
    step.setCustomSetupCode("");

    QCOMPARE(item.childCount(), 1);
    assertEmptyText(item.child(0));
}

void StepTreeItemTest::testStepSetCustomTearDownCode() {
    Step step;
    StepTreeItem item(&step);

    step.setCustomTearDownCode("The tear down code");

    QCOMPARE(item.childCount(), 2);
    assertEmptyText(item.child(0));
    assertCustomTearDownCode(item.child(1), "The tear down code");
}

void StepTreeItemTest::testStepSetCustomTearDownCodeChange() {
    Step step;
    StepTreeItem item(&step);

    step.setCustomTearDownCode("The tear down code");

    QSignalSpy dataChangedSpy(item.child(1)->child(0),
                              SIGNAL(dataChanged(TreeItem*)));

    step.setCustomTearDownCode("The tear down code changed");

    QCOMPARE(item.childCount(), 2);
    assertEmptyText(item.child(0));
    assertCustomTearDownCode(item.child(1), "The tear down code changed");
    QCOMPARE(dataChangedSpy.count(), 1);
    assertDataChanged(dataChangedSpy, 0, item.child(1)->child(0));
}

void StepTreeItemTest::testStepSetCustomTearDownCodeEmpty() {
    Step step;
    StepTreeItem item(&step);

    step.setCustomTearDownCode("The tear down code");
    step.setCustomTearDownCode("");

    QCOMPARE(item.childCount(), 1);
    assertEmptyText(item.child(0));
}

void StepTreeItemTest::testStepAddReaction() {
    Step step;
    StepTreeItem item(&step);

    Reaction* reaction = new Reaction();
    step.addReaction(reaction);

    QCOMPARE(item.childCount(), 2);
    assertEmptyText(item.child(0));
    assertReaction(item.child(1), reaction);
}

void StepTreeItemTest::testStepAddReactionAtIndex() {
    Step step;
    StepTreeItem item(&step);

    Reaction* reaction2 = new Reaction();
    step.addReaction(reaction2, 0);

    Reaction* reaction1 = new Reaction();
    step.addReaction(reaction1, 0);

    Reaction* reaction3 = new Reaction();
    step.addReaction(reaction3, 2);

    QCOMPARE(item.childCount(), 4);
    assertEmptyText(item.child(0));
    assertReaction(item.child(1), reaction1);
    assertReaction(item.child(2), reaction2);
    assertReaction(item.child(3), reaction3);
}

void StepTreeItemTest::testStepRemoveReaction() {
    Step step;
    StepTreeItem item(&step);

    //It will be removed and not deleted by the Step, so it is created in
    //stack
    Reaction reaction;
    step.addReaction(&reaction);
    step.removeReaction(&reaction);

    QCOMPARE(item.childCount(), 1);
    assertEmptyText(item.child(0));
}

void StepTreeItemTest::testChildOrderWhenSettingDataInStep() {
    Step step;
    StepTreeItem item(&step);

    step.setCustomSetupCode("The setup code");

    QCOMPARE(item.text(), i18nc("@item", "Step (id not set)"));
    QCOMPARE(item.childCount(), 2);
    assertEmptyText(item.child(0));
    assertCustomSetupCode(item.child(1), "The setup code");

    step.setText("The text");

    QCOMPARE(item.text(), i18nc("@item", "Step (id not set)"));
    QCOMPARE(item.childCount(), 2);
    assertText(item.child(0), "The text");
    assertCustomSetupCode(item.child(1), "The setup code");

    Reaction* reaction1 = new Reaction();
    step.addReaction(reaction1);

    QCOMPARE(item.text(), i18nc("@item", "Step (id not set)"));
    QCOMPARE(item.childCount(), 3);
    assertText(item.child(0), "The text");
    assertCustomSetupCode(item.child(1), "The setup code");
    assertReaction(item.child(2), reaction1);

    step.setCustomTearDownCode("The tear down code");

    QCOMPARE(item.text(), i18nc("@item", "Step (id not set)"));
    QCOMPARE(item.childCount(), 4);
    assertText(item.child(0), "The text");
    assertCustomSetupCode(item.child(1), "The setup code");
    assertCustomTearDownCode(item.child(2), "The tear down code");
    assertReaction(item.child(3), reaction1);

    Reaction* reaction2 = new Reaction();
    step.addReaction(reaction2);

    QCOMPARE(item.text(), i18nc("@item", "Step (id not set)"));
    QCOMPARE(item.childCount(), 5);
    assertText(item.child(0), "The text");
    assertCustomSetupCode(item.child(1), "The setup code");
    assertCustomTearDownCode(item.child(2), "The tear down code");
    assertReaction(item.child(3), reaction1);
    assertReaction(item.child(4), reaction2);

    Reaction* reaction3 = new Reaction();
    step.addReaction(reaction3, 1);

    QCOMPARE(item.text(), i18nc("@item", "Step (id not set)"));
    QCOMPARE(item.childCount(), 6);
    assertText(item.child(0), "The text");
    assertCustomSetupCode(item.child(1), "The setup code");
    assertCustomTearDownCode(item.child(2), "The tear down code");
    assertReaction(item.child(3), reaction1);
    assertReaction(item.child(4), reaction3);
    assertReaction(item.child(5), reaction2);

    step.setId("The id");

    QCOMPARE(item.text(), i18nc("@item", "Step %1", "The id"));
    QCOMPARE(item.childCount(), 6);
    assertText(item.child(0), "The text");
    assertCustomSetupCode(item.child(1), "The setup code");
    assertCustomTearDownCode(item.child(2), "The tear down code");
    assertReaction(item.child(3), reaction1);
    assertReaction(item.child(4), reaction3);
    assertReaction(item.child(5), reaction2);
}

void StepTreeItemTest::testChildOrderWhenUnsettingDataInStep() {
    Step step;
    step.setId("The id");
    step.setText("The text");
    step.setCustomSetupCode("The setup code");
    step.setCustomTearDownCode("The tear down code");

    Reaction reaction1;
    step.addReaction(&reaction1);

    Reaction reaction2;
    step.addReaction(&reaction2);

    StepTreeItem item(&step);

    step.setText("");

    QCOMPARE(item.text(), i18nc("@item", "Step %1", "The id"));
    QCOMPARE(item.childCount(), 5);
    assertEmptyText(item.child(0));
    assertCustomSetupCode(item.child(1), "The setup code");
    assertCustomTearDownCode(item.child(2), "The tear down code");
    assertReaction(item.child(3), &reaction1);
    assertReaction(item.child(4), &reaction2);

    step.removeReaction(&reaction1);

    QCOMPARE(item.text(), i18nc("@item", "Step %1", "The id"));
    QCOMPARE(item.childCount(), 4);
    assertEmptyText(item.child(0));
    assertCustomSetupCode(item.child(1), "The setup code");
    assertCustomTearDownCode(item.child(2), "The tear down code");
    assertReaction(item.child(3), &reaction2);

    step.setCustomTearDownCode("");

    QCOMPARE(item.text(), i18nc("@item", "Step %1", "The id"));
    QCOMPARE(item.childCount(), 3);
    assertEmptyText(item.child(0));
    assertCustomSetupCode(item.child(1), "The setup code");
    assertReaction(item.child(2), &reaction2);

    step.setId("");

    QCOMPARE(item.text(), i18nc("@item", "Step (id not set)"));
    QCOMPARE(item.childCount(), 3);
    assertEmptyText(item.child(0));
    assertCustomSetupCode(item.child(1), "The setup code");
    assertReaction(item.child(2), &reaction2);

    step.setCustomSetupCode("");

    QCOMPARE(item.text(), i18nc("@item", "Step (id not set)"));
    QCOMPARE(item.childCount(), 2);
    assertEmptyText(item.child(0));
    assertReaction(item.child(1), &reaction2);

    step.removeReaction(&reaction2);

    QCOMPARE(item.text(), i18nc("@item", "Step (id not set)"));
    QCOMPARE(item.childCount(), 1);
    assertEmptyText(item.child(0));
}

/////////////////////////////////// Helpers ////////////////////////////////////

void StepTreeItemTest::assertEmptyText(TreeItem* textItem) const {
    QCOMPARE(textItem->text(), i18nc("@item", "Text: (text not set)"));
}

void StepTreeItemTest::assertText(TreeItem* textItem,
                                  const QString& name) const {
    QCOMPARE(textItem->text(), i18nc("@item", "Text: %1", name));
}

void StepTreeItemTest::assertCustomSetupCode(TreeItem* setupItem,
                                                 const QString& code) const {
    QCOMPARE(setupItem->text(), i18nc("@item", "Setup:"));
    QCOMPARE(setupItem->childCount(), 1);
    QCOMPARE(setupItem->child(0)->text(), i18nc("@item", code.toAscii()));
}

void StepTreeItemTest::assertCustomTearDownCode(TreeItem* tearDownItem,
                                                    const QString& code) const {
    QCOMPARE(tearDownItem->text(), i18nc("@item", "Tear down:"));
    QCOMPARE(tearDownItem->childCount(), 1);
    QCOMPARE(tearDownItem->child(0)->text(), i18nc("@item", code.toAscii()));
}

void StepTreeItemTest::assertReaction(TreeItem* reactionItem,
                                      const Reaction* reaction) const {
    QVERIFY(qobject_cast<ReactionTreeItem*>(reactionItem));
    QCOMPARE(static_cast<ReactionTreeItem*>(reactionItem)->reaction(),
             reaction);
}

void StepTreeItemTest::assertDataChanged(const QSignalSpy& spy, int index,
                                             TreeItem* item) const {
    QCOMPARE(spy.at(index).count(), 1);

    QVariant argument = spy.at(index).at(0);
    QCOMPARE(argument.userType(), mTreeItemStarType);
    QCOMPARE(qvariant_cast<TreeItem*>(argument), item);
}

QTEST_MAIN(StepTreeItemTest)

#include "StepTreeItemTest.moc"
