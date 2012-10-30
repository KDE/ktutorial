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

#include "ReactionTreeItem.h"

#include <QSignalSpy>

#include <KLocalizedString>

#include "WaitForTreeItem.h"
#include "../data/Reaction.h"
#include "../data/WaitForSignal.h"

class ReactionTreeItemTest: public QObject {
Q_OBJECT

private slots:

    void initTestCase();

    void testConstructorConditionCustomCode();
    void testConstructorOptionNextStep();
    void testConstructorConditionCustomCodeFull();
    void testConstructorOptionNextStepFull();

    void testReactionSetOptionName();
    void testReactionSetOptionNameChange();
    void testReactionSetOptionNameEmpty();

    void testReactionSetWaitFor();
    void testReactionSetWaitForChange();
    void testReactionSetWaitForNull();

    void testReactionSetNextStepId();
    void testReactionSetNextStepIdChange();
    void testReactionSetNextStepIdEmpty();

    void testReactionSetCustomCode();
    void testReactionSetCustomCodeChange();
    void testReactionSetCustomCodeEmpty();

    void testReactionSetTriggerTypeToOptionSelected();
    void testReactionSetTriggerTypeToConditionMet();

    void testReactionSetResponseTypeToNextStep();
    void testReactionSetResponseTypeToCustomCode();

private:

    int mTreeItemStarType;

    void assertText(TreeItem* textItem, const QString& licenseText) const;

    void assertWaitFor(TreeItem* item, WaitFor* waitFor) const;

    void assertCustomCode(TreeItem* textItem, const QString& code) const;

    void assertDataChanged(const QSignalSpy& spy, int index,
                           TreeItem* item) const;

};

class StubTreeItem: public TreeItem {
public:
    virtual QString text() const {
        return "";
    }
};

void ReactionTreeItemTest::initTestCase() {
    //TreeItem* must be registered in order to be used with QSignalSpy
    mTreeItemStarType = qRegisterMetaType<TreeItem*>("TreeItem*");
}

void ReactionTreeItemTest::testConstructorConditionCustomCode() {
    Reaction reaction;
    reaction.setTriggerType(Reaction::ConditionMet);
    reaction.setResponseType(Reaction::CustomCode);

    StubTreeItem parent;
    ReactionTreeItem item(&reaction, &parent);

    QCOMPARE(item.parent(), &parent);
    QCOMPARE(item.text(), i18nc("@item", "Reaction"));
    QCOMPARE(item.reaction(), &reaction);
    QCOMPARE(item.childCount(), 2);
    assertWaitFor(item.child(0), 0);
    assertCustomCode(item.child(1), i18nc("@item", "(No code set)"));
}

void ReactionTreeItemTest::testConstructorOptionNextStep() {
    Reaction reaction;
    reaction.setTriggerType(Reaction::OptionSelected);
    reaction.setResponseType(Reaction::NextStep);

    StubTreeItem parent;
    ReactionTreeItem item(&reaction, &parent);

    QCOMPARE(item.parent(), &parent);
    QCOMPARE(item.text(), i18nc("@item", "Reaction"));
    QCOMPARE(item.reaction(), &reaction);
    QCOMPARE(item.childCount(), 2);
    assertText(item.child(0), i18nc("@item", "When the option (option name not "
                                             "set) is selected"));
    assertText(item.child(1), i18nc("@item", "Change to step (step id not "
                                             "set)"));
}

void ReactionTreeItemTest::testConstructorConditionCustomCodeFull() {
    WaitFor* waitFor = new WaitForSignal();
    Reaction reaction;
    reaction.setTriggerType(Reaction::ConditionMet);
    reaction.setWaitFor(waitFor);
    reaction.setResponseType(Reaction::CustomCode);
    reaction.setCustomCode("The custom code");

    StubTreeItem parent;
    ReactionTreeItem item(&reaction, &parent);

    QCOMPARE(item.parent(), &parent);
    QCOMPARE(item.text(), i18nc("@item", "Reaction"));
    QCOMPARE(item.reaction(), &reaction);
    QCOMPARE(item.childCount(), 2);
    assertWaitFor(item.child(0), waitFor);
    assertCustomCode(item.child(1), "The custom code");
}

void ReactionTreeItemTest::testConstructorOptionNextStepFull() {
    Reaction reaction;
    reaction.setTriggerType(Reaction::OptionSelected);
    reaction.setOptionName("The option name");
    reaction.setResponseType(Reaction::NextStep);
    reaction.setNextStepId("The step id");

    StubTreeItem parent;
    ReactionTreeItem item(&reaction, &parent);

    QCOMPARE(item.parent(), &parent);
    QCOMPARE(item.text(), i18nc("@item", "Reaction"));
    QCOMPARE(item.reaction(), &reaction);
    QCOMPARE(item.childCount(), 2);
    assertText(item.child(0), i18nc("@item", "When the option \"The option "
                                             "name\" is selected"));
    assertText(item.child(1), i18nc("@item", "Change to step \"The step id\""));
}

void ReactionTreeItemTest::testReactionSetOptionName() {
    Reaction reaction;
    reaction.setTriggerType(Reaction::OptionSelected);

    ReactionTreeItem item(&reaction);

    QSignalSpy dataChangedSpy(item.child(0), SIGNAL(dataChanged(TreeItem*)));

    reaction.setOptionName("The option name");

    QCOMPARE(item.childCount(), 2);
    assertText(item.child(0), i18nc("@item", "When the option \"The option "
                                             "name\" is selected"));
    QCOMPARE(dataChangedSpy.count(), 1);
    assertDataChanged(dataChangedSpy, 0, item.child(0));
}

void ReactionTreeItemTest::testReactionSetOptionNameChange() {
    Reaction reaction;
    reaction.setTriggerType(Reaction::OptionSelected);

    ReactionTreeItem item(&reaction);

    reaction.setOptionName("The option name");

    QSignalSpy dataChangedSpy(item.child(0), SIGNAL(dataChanged(TreeItem*)));

    reaction.setOptionName("The new option name");

    QCOMPARE(item.childCount(), 2);
    assertText(item.child(0), i18nc("@item", "When the option \"The new option "
                                             "name\" is selected"));
    QCOMPARE(dataChangedSpy.count(), 1);
    assertDataChanged(dataChangedSpy, 0, item.child(0));
}

void ReactionTreeItemTest::testReactionSetOptionNameEmpty() {
    Reaction reaction;
    reaction.setTriggerType(Reaction::OptionSelected);
    reaction.setOptionName("The option name");

    ReactionTreeItem item(&reaction);

    QSignalSpy dataChangedSpy(item.child(0), SIGNAL(dataChanged(TreeItem*)));

    reaction.setOptionName("");

    QCOMPARE(item.childCount(), 2);
    assertText(item.child(0), i18nc("@item", "When the option (option name not "
                                    "set) is selected"));
    QCOMPARE(dataChangedSpy.count(), 1);
    assertDataChanged(dataChangedSpy, 0, item.child(0));
}

void ReactionTreeItemTest::testReactionSetWaitFor() {
    WaitForSignal* waitFor = new WaitForSignal();
    Reaction reaction;
    reaction.setTriggerType(Reaction::ConditionMet);

    ReactionTreeItem item(&reaction);

    reaction.setWaitFor(waitFor);

    QCOMPARE(item.childCount(), 2);
    assertWaitFor(item.child(0), waitFor);
}

void ReactionTreeItemTest::testReactionSetWaitForChange() {
    //It will be removed and not deleted by the Reaction, so it is created in
    //stack
    WaitForSignal waitFor;

    WaitForSignal* waitFor2 = new WaitForSignal();
    Reaction reaction;
    reaction.setTriggerType(Reaction::ConditionMet);

    ReactionTreeItem item(&reaction);

    reaction.setWaitFor(&waitFor);
    reaction.setWaitFor(waitFor2);

    QCOMPARE(item.childCount(), 2);
    assertWaitFor(item.child(0), waitFor2);
}

void ReactionTreeItemTest::testReactionSetWaitForNull() {
    //It will be removed and not deleted by the Reaction, so it is created in
    //stack
    WaitForSignal waitFor;

    Reaction reaction;
    reaction.setTriggerType(Reaction::ConditionMet);
    reaction.setWaitFor(&waitFor);

    ReactionTreeItem item(&reaction);

    reaction.setWaitFor(0);

    QCOMPARE(item.childCount(), 2);
    assertWaitFor(item.child(0), 0);
}

void ReactionTreeItemTest::testReactionSetNextStepId() {
    Reaction reaction;
    reaction.setResponseType(Reaction::NextStep);

    ReactionTreeItem item(&reaction);

    QSignalSpy dataChangedSpy(item.child(1), SIGNAL(dataChanged(TreeItem*)));

    reaction.setNextStepId("The step id");

    QCOMPARE(item.childCount(), 2);
    assertText(item.child(1), i18nc("@item", "Change to step \"The step id\""));
    QCOMPARE(dataChangedSpy.count(), 1);
    assertDataChanged(dataChangedSpy, 0, item.child(1));
}

void ReactionTreeItemTest::testReactionSetNextStepIdChange() {
    Reaction reaction;
    reaction.setResponseType(Reaction::NextStep);

    ReactionTreeItem item(&reaction);

    reaction.setNextStepId("The step id");

    QSignalSpy dataChangedSpy(item.child(1), SIGNAL(dataChanged(TreeItem*)));

    reaction.setNextStepId("The new step id");

    QCOMPARE(item.childCount(), 2);
    assertText(item.child(1), i18nc("@item", "Change to step \"The new step "
                                             "id\""));
    QCOMPARE(dataChangedSpy.count(), 1);
    assertDataChanged(dataChangedSpy, 0, item.child(1));
}

void ReactionTreeItemTest::testReactionSetNextStepIdEmpty() {
    Reaction reaction;
    reaction.setResponseType(Reaction::NextStep);
    reaction.setNextStepId("The step id");

    ReactionTreeItem item(&reaction);

    QSignalSpy dataChangedSpy(item.child(1), SIGNAL(dataChanged(TreeItem*)));

    reaction.setNextStepId("");

    QCOMPARE(item.childCount(), 2);
    assertText(item.child(1), i18nc("@item", "Change to step (step id not "
                                             "set)"));
    QCOMPARE(dataChangedSpy.count(), 1);
    assertDataChanged(dataChangedSpy, 0, item.child(1));
}

void ReactionTreeItemTest::testReactionSetCustomCode() {
    Reaction reaction;
    reaction.setResponseType(Reaction::CustomCode);

    ReactionTreeItem item(&reaction);

    QSignalSpy dataChangedSpy(item.child(1)->child(0),
                              SIGNAL(dataChanged(TreeItem*)));

    reaction.setCustomCode("The custom code");

    QCOMPARE(item.childCount(), 2);
    assertCustomCode(item.child(1), "The custom code");
    QCOMPARE(dataChangedSpy.count(), 1);
    assertDataChanged(dataChangedSpy, 0, item.child(1)->child(0));
}

void ReactionTreeItemTest::testReactionSetCustomCodeChange() {
    Reaction reaction;
    reaction.setResponseType(Reaction::CustomCode);

    ReactionTreeItem item(&reaction);

    reaction.setCustomCode("The custom code");

    QSignalSpy dataChangedSpy(item.child(1)->child(0),
                              SIGNAL(dataChanged(TreeItem*)));

    reaction.setCustomCode("The new custom code");

    QCOMPARE(item.childCount(), 2);
    assertCustomCode(item.child(1), "The new custom code");
    QCOMPARE(dataChangedSpy.count(), 1);
    assertDataChanged(dataChangedSpy, 0, item.child(1)->child(0));
}

void ReactionTreeItemTest::testReactionSetCustomCodeEmpty() {
    Reaction reaction;
    reaction.setResponseType(Reaction::CustomCode);
    reaction.setCustomCode("The custom code");

    ReactionTreeItem item(&reaction);

    QSignalSpy dataChangedSpy(item.child(1)->child(0),
                              SIGNAL(dataChanged(TreeItem*)));

    reaction.setCustomCode("");

    QCOMPARE(item.childCount(), 2);
    assertCustomCode(item.child(1), i18nc("@item", "(No code set)"));
    QCOMPARE(dataChangedSpy.count(), 1);
    assertDataChanged(dataChangedSpy, 0, item.child(1)->child(0));
}

void ReactionTreeItemTest::testReactionSetTriggerTypeToOptionSelected() {
    Reaction reaction;
    reaction.setTriggerType(Reaction::ConditionMet);
    reaction.setOptionName("The option name");

    ReactionTreeItem item(&reaction);

    reaction.setTriggerType(Reaction::OptionSelected);

    QCOMPARE(item.childCount(), 2);
    assertText(item.child(0), i18nc("@item", "When the option \"The option "
                                             "name\" is selected"));
}

void ReactionTreeItemTest::testReactionSetTriggerTypeToConditionMet() {
    WaitForSignal* waitFor = new WaitForSignal();
    Reaction reaction;
    reaction.setTriggerType(Reaction::OptionSelected);
    reaction.setWaitFor(waitFor);

    ReactionTreeItem item(&reaction);

    reaction.setTriggerType(Reaction::ConditionMet);

    QCOMPARE(item.childCount(), 2);
    assertWaitFor(item.child(0), waitFor);
}

void ReactionTreeItemTest::testReactionSetResponseTypeToNextStep() {
    Reaction reaction;
    reaction.setResponseType(Reaction::CustomCode);
    reaction.setNextStepId("The step id");

    ReactionTreeItem item(&reaction);

    reaction.setResponseType(Reaction::NextStep);

    QCOMPARE(item.childCount(), 2);
    assertText(item.child(1), i18nc("@item", "Change to step \"The step id\""));
}

void ReactionTreeItemTest::testReactionSetResponseTypeToCustomCode() {
    Reaction reaction;
    reaction.setResponseType(Reaction::NextStep);
    reaction.setCustomCode("The custom code");

    ReactionTreeItem item(&reaction);

    reaction.setResponseType(Reaction::CustomCode);

    QCOMPARE(item.childCount(), 2);
    assertCustomCode(item.child(1), "The custom code");
}

/////////////////////////////////// Helpers ////////////////////////////////////

void ReactionTreeItemTest::assertText(TreeItem* textItem,
                                      const QString& name) const {
    QCOMPARE(textItem->text(), i18nc("@item", "%1", name));
}

void ReactionTreeItemTest::assertWaitFor(TreeItem* item,
                                         WaitFor* waitFor) const {
    if (!waitFor) {
        QCOMPARE(item->text(), i18nc("@item", "(No condition set)"));
        return;
    }

    WaitForTreeItem* waitForItem = qobject_cast<WaitForTreeItem*>(item);
    QVERIFY(waitForItem);
    QCOMPARE(waitForItem->waitFor(), waitFor);
}

void ReactionTreeItemTest::assertCustomCode(TreeItem* textItem,
                                            const QString& code) const {
    QCOMPARE(textItem->text(), i18nc("@item", "Execute the following code:"));
    QCOMPARE(textItem->childCount(), 1);
    QCOMPARE(textItem->child(0)->text(), i18nc("@item", code.toAscii()));
}

//TreeItem* must be declared as a metatype to be used in qvariant_cast
Q_DECLARE_METATYPE(TreeItem*);

void ReactionTreeItemTest::assertDataChanged(const QSignalSpy& spy, int index,
                                             TreeItem* item) const {
    QCOMPARE(spy.at(index).count(), 1);

    QVariant argument = spy.at(index).at(0);
    QCOMPARE(argument.userType(), mTreeItemStarType);
    QCOMPARE(qvariant_cast<TreeItem*>(argument), item);
}

QTEST_MAIN(ReactionTreeItemTest)

#include "ReactionTreeItemTest.moc"
