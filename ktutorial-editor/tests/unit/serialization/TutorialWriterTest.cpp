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

#include <QTest>

#include "TutorialWriter.h"

#include "../data/Reaction.h"
#include "../data/Step.h"
#include "../data/Tutorial.h"
#include "../data/WaitForComposed.h"
#include "../data/WaitForEvent.h"
#include "../data/WaitForNot.h"
#include "../data/WaitForProperty.h"
#include "../data/WaitForSignal.h"
#include "../data/WaitForStepActivation.h"
#include "../data/WaitForWindow.h"

#define HEADER_XML \
    "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n"

#define TUTORIAL_ELEMENTS \
"    <description>The description,\nwith &lt; and &gt;\n</description>\n" \
"    <license>The license text,\nwith &lt; and &gt;\n</license>\n" \
"    <setup>The setup code,\nwith &lt; and &gt;\n</setup>\n" \
"    <tearDown>The tear down code,\nwith &lt; and &gt;\n</tearDown>\n"

#define STEP_PARENT_START \
HEADER_XML \
"<tutorial>\n"

#define STEP_PARENT_END \
"</tutorial>\n"

#define STEP_ELEMENTS \
"        <text>The text,\nwith &lt; and &gt;\n</text>\n" \
"        <setup>The setup code,\nwith &lt; and &gt;\n</setup>\n" \
"        <tearDown>The tear down code,\nwith &lt; and &gt;\n</tearDown>\n"

#define REACTION_PARENT_START \
STEP_PARENT_START \
"    <step>\n"

#define REACTION_PARENT_END \
"    </step>\n" \
STEP_PARENT_END

#define WAITFOR_PARENT_START \
REACTION_PARENT_START \
"        <reaction triggerType=\"ConditionMet\" responseType=\"NextStep\">\n"

#define WAITFOR_PARENT_END \
"        </reaction>\n" \
REACTION_PARENT_END

class TutorialWriterTest: public QObject {
Q_OBJECT

private slots:

    void testTutorial();
    void testTutorialEmpty();
    void testTutorialWithSeveralSteps();

    void testStep();
    void testStepEmpty();
    void testStepWithSeveralReactions();

    void testReactionConditionCustomCode();
    void testReactionOptionNextStep();
    void testReactionEmpty();

    void testWaitForEvent();
    void testWaitForEventEmpty();
    void testWaitForProperty();
    void testWaitForPropertyEmpty();
    void testWaitForSignal();
    void testWaitForSignalEmpty();
    void testWaitForStepActivation();
    void testWaitForWindow();
    void testWaitForWindowEmpty();
    void testWaitForComposed();
    void testWaitForComposedEmpty();
    void testWaitForNot();
    void testWaitForNotWithoutNegatedWaitFor();

};

void TutorialWriterTest::testTutorial() {
    Tutorial tutorial;
    tutorial.setName("The \"name\"");
    tutorial.setDescription("The description,\nwith < and >\n");
    tutorial.setLicenseText("The license text,\nwith < and >\n");
    tutorial.setCustomSetupCode("The setup code,\nwith < and >\n");
    tutorial.setCustomTearDownCode("The tear down code,\nwith < and >\n");

    TutorialWriter saver;
    QString savedTutorial = saver.writeTutorial(&tutorial);

    QString expected =
HEADER_XML
"<tutorial name=\"The &quot;name&quot;\">\n"
"    <description>The description,\nwith &lt; and &gt;\n</description>\n"
"    <license>The license text,\nwith &lt; and &gt;\n</license>\n"
"    <setup>The setup code,\nwith &lt; and &gt;\n</setup>\n"
"    <tearDown>The tear down code,\nwith &lt; and &gt;\n</tearDown>\n"
"</tutorial>\n";

    QCOMPARE(savedTutorial, expected);
}

void TutorialWriterTest::testTutorialEmpty() {
    Tutorial tutorial;

    TutorialWriter saver;
    QString savedTutorial = saver.writeTutorial(&tutorial);

    QString expected =
HEADER_XML
"<tutorial/>\n";

    QCOMPARE(savedTutorial, expected);
}

void TutorialWriterTest::testTutorialWithSeveralSteps() {
    Tutorial tutorial;
    tutorial.setName("The \"name\"");
    tutorial.setDescription("The description,\nwith < and >\n");
    tutorial.setLicenseText("The license text,\nwith < and >\n");
    tutorial.setCustomSetupCode("The setup code,\nwith < and >\n");
    tutorial.setCustomTearDownCode("The tear down code,\nwith < and >\n");

    Step* step1 = new Step();
    step1->setId("The id1");
    step1->setText("The text1");
    tutorial.addStep(step1);

    Step* step2 = new Step();
    step2->setId("The id2");
    step2->setText("The text2");
    tutorial.addStep(step2);

    TutorialWriter saver;
    QString savedTutorial = saver.writeTutorial(&tutorial);

    QString expected =
HEADER_XML
"<tutorial name=\"The &quot;name&quot;\">\n"
TUTORIAL_ELEMENTS
"    <step id=\"The id1\">\n"
"        <text>The text1</text>\n"
"    </step>\n"
"    <step id=\"The id2\">\n"
"        <text>The text2</text>\n"
"    </step>\n"
"</tutorial>\n";

    QCOMPARE(savedTutorial, expected);
}

void TutorialWriterTest::testStep() {
    Tutorial tutorial;

    Step* step = new Step();
    step->setId("The \"id\"");
    step->setText("The text,\nwith < and >\n");
    step->setCustomSetupCode("The setup code,\nwith < and >\n");
    step->setCustomTearDownCode("The tear down code,\nwith < and >\n");
    tutorial.addStep(step);

    TutorialWriter saver;
    QString savedTutorial = saver.writeTutorial(&tutorial);

    QString expected =
STEP_PARENT_START
"    <step id=\"The &quot;id&quot;\">\n"
"        <text>The text,\nwith &lt; and &gt;\n</text>\n"
"        <setup>The setup code,\nwith &lt; and &gt;\n</setup>\n"
"        <tearDown>The tear down code,\nwith &lt; and &gt;\n</tearDown>\n"
"    </step>\n"
STEP_PARENT_END;

    QCOMPARE(savedTutorial, expected);
}

void TutorialWriterTest::testStepEmpty() {
    Tutorial tutorial;

    Step* step = new Step();
    tutorial.addStep(step);

    TutorialWriter saver;
    QString savedTutorial = saver.writeTutorial(&tutorial);

    QString expected =
STEP_PARENT_START
"    <step/>\n"
STEP_PARENT_END;

    QCOMPARE(savedTutorial, expected);
}

void TutorialWriterTest::testStepWithSeveralReactions() {
    Tutorial tutorial;

    Step* step = new Step();
    step->setId("The \"id\"");
    step->setText("The text,\nwith < and >\n");
    step->setCustomSetupCode("The setup code,\nwith < and >\n");
    step->setCustomTearDownCode("The tear down code,\nwith < and >\n");
    tutorial.addStep(step);

    Reaction* reaction1 = new Reaction();
    reaction1->setTriggerType(Reaction::ConditionMet);
    reaction1->setResponseType(Reaction::CustomCode);
    step->addReaction(reaction1);

    Reaction* reaction2 = new Reaction();
    reaction2->setTriggerType(Reaction::OptionSelected);
    reaction2->setOptionName("The option name");
    reaction2->setResponseType(Reaction::NextStep);
    reaction2->setNextStepId("Another id");
    step->addReaction(reaction2);

    TutorialWriter saver;
    QString savedTutorial = saver.writeTutorial(&tutorial);

    QString expected =
STEP_PARENT_START
"    <step id=\"The &quot;id&quot;\">\n"
STEP_ELEMENTS
"        <reaction triggerType=\"ConditionMet\" responseType=\"CustomCode\"/>\n"
"        <reaction triggerType=\"OptionSelected\" responseType=\"NextStep\">\n"
"            <option name=\"The option name\"/>\n"
"            <nextStep id=\"Another id\"/>\n"
"        </reaction>\n"
"    </step>\n"
STEP_PARENT_END;

    QCOMPARE(savedTutorial, expected);
}

void TutorialWriterTest::testReactionConditionCustomCode() {
    Tutorial tutorial;
    Step* step = new Step();
    tutorial.addStep(step);

    WaitForSignal* waitForSignal = new WaitForSignal();
    waitForSignal->setEmitterName("The emitter name");
    waitForSignal->setSignalName("theSignalName(Argument1Type, Argument2Type)");

    Reaction* reaction = new Reaction();
    reaction->setTriggerType(Reaction::ConditionMet);
    reaction->setWaitFor(waitForSignal);
    reaction->setOptionName("The \"option\" name");
    reaction->setResponseType(Reaction::CustomCode);
    reaction->setCustomCode("The custom code,\nwith < and >\n");
    reaction->setNextStepId("Another \"id\"");
    step->addReaction(reaction);

    TutorialWriter saver;
    QString savedTutorial = saver.writeTutorial(&tutorial);

    QString expected =
REACTION_PARENT_START
"        <reaction triggerType=\"ConditionMet\" responseType=\"CustomCode\">\n"
"            <option name=\"The &quot;option&quot; name\"/>\n"
"            <waitForSignal emitterName=\"The emitter name\" \
signalName=\"theSignalName(Argument1Type, Argument2Type)\"/>\n"
"            <customCode>The custom code,\nwith &lt; and &gt;\n</customCode>\n"
"            <nextStep id=\"Another &quot;id&quot;\"/>\n"
"        </reaction>\n"
REACTION_PARENT_END;

    QCOMPARE(savedTutorial, expected);
}

void TutorialWriterTest::testReactionOptionNextStep() {
    Tutorial tutorial;
    Step* step = new Step();
    tutorial.addStep(step);

    WaitForSignal* waitForSignal = new WaitForSignal();
    waitForSignal->setEmitterName("The emitter name");
    waitForSignal->setSignalName("theSignalName(Argument1Type, Argument2Type)");

    Reaction* reaction = new Reaction();
    reaction->setTriggerType(Reaction::OptionSelected);
    reaction->setOptionName("The \"option\" name");
    reaction->setWaitFor(waitForSignal);
    reaction->setResponseType(Reaction::NextStep);
    reaction->setNextStepId("Another \"id\"");
    reaction->setCustomCode("The custom code,\nwith < and >\n");
    step->addReaction(reaction);

    TutorialWriter saver;
    QString savedTutorial = saver.writeTutorial(&tutorial);

    QString expected =
REACTION_PARENT_START
"        <reaction triggerType=\"OptionSelected\" responseType=\"NextStep\">\n"
"            <option name=\"The &quot;option&quot; name\"/>\n"
"            <waitForSignal emitterName=\"The emitter name\" \
signalName=\"theSignalName(Argument1Type, Argument2Type)\"/>\n"
"            <customCode>The custom code,\nwith &lt; and &gt;\n</customCode>\n"
"            <nextStep id=\"Another &quot;id&quot;\"/>\n"
"        </reaction>\n"
REACTION_PARENT_END;

    QCOMPARE(savedTutorial, expected);
}

void TutorialWriterTest::testReactionEmpty() {
    Tutorial tutorial;
    Step* step = new Step();
    tutorial.addStep(step);

    Reaction* reaction = new Reaction();
    reaction->setTriggerType(Reaction::ConditionMet);
    reaction->setResponseType(Reaction::CustomCode);
    step->addReaction(reaction);

    TutorialWriter saver;
    QString savedTutorial = saver.writeTutorial(&tutorial);

    QString expected =
REACTION_PARENT_START
"        <reaction triggerType=\"ConditionMet\" responseType=\"CustomCode\"/>\n"
REACTION_PARENT_END;

    QCOMPARE(savedTutorial, expected);
}

void TutorialWriterTest::testWaitForEvent() {
    Tutorial tutorial;
    Step* step = new Step();
    tutorial.addStep(step);

    WaitForEvent* waitForEvent = new WaitForEvent();
    waitForEvent->setReceiverName("The \"receiver\" name");
    waitForEvent->setEventName("The\"Event\"Name");

    Reaction* reaction = new Reaction();
    reaction->setTriggerType(Reaction::ConditionMet);
    reaction->setWaitFor(waitForEvent);
    reaction->setResponseType(Reaction::NextStep);
    step->addReaction(reaction);

    TutorialWriter saver;
    QString savedTutorial = saver.writeTutorial(&tutorial);

    QString expected =
WAITFOR_PARENT_START
"            <waitForEvent receiverName=\"The &quot;receiver&quot; name\" \
eventName=\"The&quot;Event&quot;Name\"/>\n"
WAITFOR_PARENT_END;

    QCOMPARE(savedTutorial, expected);
}

void TutorialWriterTest::testWaitForEventEmpty() {
    Tutorial tutorial;
    Step* step = new Step();
    tutorial.addStep(step);

    WaitForEvent* waitForEvent = new WaitForEvent();

    Reaction* reaction = new Reaction();
    reaction->setTriggerType(Reaction::ConditionMet);
    reaction->setWaitFor(waitForEvent);
    reaction->setResponseType(Reaction::NextStep);
    step->addReaction(reaction);

    TutorialWriter saver;
    QString savedTutorial = saver.writeTutorial(&tutorial);

    QString expected =
WAITFOR_PARENT_START
"            <waitForEvent/>\n"
WAITFOR_PARENT_END;

    QCOMPARE(savedTutorial, expected);
}

void TutorialWriterTest::testWaitForProperty() {
    Tutorial tutorial;
    Step* step = new Step();
    tutorial.addStep(step);

    WaitForProperty* waitForProperty = new WaitForProperty();
    waitForProperty->setObjectName("The \"object\" name");
    waitForProperty->setPropertyName("thePropertyName");
    waitForProperty->setValue("\"The \\\"value\\\"\"");

    Reaction* reaction = new Reaction();
    reaction->setTriggerType(Reaction::ConditionMet);
    reaction->setWaitFor(waitForProperty);
    reaction->setResponseType(Reaction::NextStep);
    step->addReaction(reaction);

    TutorialWriter saver;
    QString savedTutorial = saver.writeTutorial(&tutorial);

    QString expected =
WAITFOR_PARENT_START
"            <waitForProperty objectName=\"The &quot;object&quot; name\" \
propertyName=\"thePropertyName\" \
value=\"&quot;The \\&quot;value\\&quot;&quot;\"/>\n"
WAITFOR_PARENT_END;

    QCOMPARE(savedTutorial, expected);
}

void TutorialWriterTest::testWaitForPropertyEmpty() {
    Tutorial tutorial;
    Step* step = new Step();
    tutorial.addStep(step);

    WaitForProperty* waitForProperty = new WaitForProperty();

    Reaction* reaction = new Reaction();
    reaction->setTriggerType(Reaction::ConditionMet);
    reaction->setWaitFor(waitForProperty);
    reaction->setResponseType(Reaction::NextStep);
    step->addReaction(reaction);

    TutorialWriter saver;
    QString savedTutorial = saver.writeTutorial(&tutorial);

    QString expected =
WAITFOR_PARENT_START
"            <waitForProperty/>\n"
WAITFOR_PARENT_END;

    QCOMPARE(savedTutorial, expected);
}

void TutorialWriterTest::testWaitForSignal() {
    Tutorial tutorial;
    Step* step = new Step();
    tutorial.addStep(step);

    WaitForSignal* waitForSignal = new WaitForSignal();
    waitForSignal->setEmitterName("The \"emitter\" name");
    waitForSignal->setSignalName("theSignalName(\"Argument1Type\")");

    Reaction* reaction = new Reaction();
    reaction->setTriggerType(Reaction::ConditionMet);
    reaction->setWaitFor(waitForSignal);
    reaction->setResponseType(Reaction::NextStep);
    step->addReaction(reaction);

    TutorialWriter saver;
    QString savedTutorial = saver.writeTutorial(&tutorial);

    QString expected =
WAITFOR_PARENT_START
"            <waitForSignal emitterName=\"The &quot;emitter&quot; name\" \
signalName=\"theSignalName(&quot;Argument1Type&quot;)\"/>\n"
WAITFOR_PARENT_END;

    QCOMPARE(savedTutorial, expected);
}

void TutorialWriterTest::testWaitForSignalEmpty() {
    Tutorial tutorial;
    Step* step = new Step();
    tutorial.addStep(step);

    WaitForSignal* waitForSignal = new WaitForSignal();

    Reaction* reaction = new Reaction();
    reaction->setTriggerType(Reaction::ConditionMet);
    reaction->setWaitFor(waitForSignal);
    reaction->setResponseType(Reaction::NextStep);
    step->addReaction(reaction);

    TutorialWriter saver;
    QString savedTutorial = saver.writeTutorial(&tutorial);

    QString expected =
WAITFOR_PARENT_START
"            <waitForSignal/>\n"
WAITFOR_PARENT_END;

    QCOMPARE(savedTutorial, expected);
}

void TutorialWriterTest::testWaitForStepActivation() {
    Tutorial tutorial;
    Step* step = new Step();
    tutorial.addStep(step);

    WaitForStepActivation* waitForStepActivation = new WaitForStepActivation();

    Reaction* reaction = new Reaction();
    reaction->setTriggerType(Reaction::ConditionMet);
    reaction->setWaitFor(waitForStepActivation);
    reaction->setResponseType(Reaction::NextStep);
    step->addReaction(reaction);

    TutorialWriter saver;
    QString savedTutorial = saver.writeTutorial(&tutorial);

    QString expected =
WAITFOR_PARENT_START
"            <waitForStepActivation/>\n"
WAITFOR_PARENT_END;

    QCOMPARE(savedTutorial, expected);
}

void TutorialWriterTest::testWaitForWindow() {
    Tutorial tutorial;
    Step* step = new Step();
    tutorial.addStep(step);

    WaitForWindow* waitForWindow = new WaitForWindow();
    waitForWindow->setWindowObjectName("The \"window\" object name");

    Reaction* reaction = new Reaction();
    reaction->setTriggerType(Reaction::ConditionMet);
    reaction->setWaitFor(waitForWindow);
    reaction->setResponseType(Reaction::NextStep);
    step->addReaction(reaction);

    TutorialWriter saver;
    QString savedTutorial = saver.writeTutorial(&tutorial);

    QString expected =
WAITFOR_PARENT_START
"            <waitForWindow windowObjectName=\"The &quot;window&quot; object \
name\"/>\n"
WAITFOR_PARENT_END;

    QCOMPARE(savedTutorial, expected);
}

void TutorialWriterTest::testWaitForWindowEmpty() {
    Tutorial tutorial;
    Step* step = new Step();
    tutorial.addStep(step);

    WaitForWindow* waitForWindow = new WaitForWindow();

    Reaction* reaction = new Reaction();
    reaction->setTriggerType(Reaction::ConditionMet);
    reaction->setWaitFor(waitForWindow);
    reaction->setResponseType(Reaction::NextStep);
    step->addReaction(reaction);

    TutorialWriter saver;
    QString savedTutorial = saver.writeTutorial(&tutorial);

    QString expected =
WAITFOR_PARENT_START
"            <waitForWindow/>\n"
WAITFOR_PARENT_END;

    QCOMPARE(savedTutorial, expected);
}

void TutorialWriterTest::testWaitForComposed() {
    Tutorial tutorial;
    Step* step = new Step();
    tutorial.addStep(step);

    WaitForSignal* waitForSignalChild = new WaitForSignal();
    waitForSignalChild->setEmitterName("The emitter name1");
    waitForSignalChild->setSignalName("theSignalName1()");

    WaitForSignal* waitForSignalGrandChild1 = new WaitForSignal();
    waitForSignalGrandChild1->setEmitterName("The emitter name2");
    waitForSignalGrandChild1->setSignalName("theSignalName2()");

    WaitForSignal* waitForSignalGrandChild2 = new WaitForSignal();
    waitForSignalGrandChild2->setEmitterName("The emitter name3");
    waitForSignalGrandChild2->setSignalName("theSignalName3()");

    WaitForComposed* waitForOrChild = new WaitForComposed();
    waitForOrChild->setCompositionType(WaitForComposed::Or);
    waitForOrChild->addWaitFor(waitForSignalGrandChild1);
    waitForOrChild->addWaitFor(waitForSignalGrandChild2);

    WaitForComposed* waitForAnd = new WaitForComposed();
    waitForAnd->setCompositionType(WaitForComposed::And);
    waitForAnd->addWaitFor(waitForSignalChild);
    waitForAnd->addWaitFor(waitForOrChild);

    Reaction* reaction = new Reaction();
    reaction->setTriggerType(Reaction::ConditionMet);
    reaction->setWaitFor(waitForAnd);
    reaction->setResponseType(Reaction::NextStep);
    step->addReaction(reaction);

    TutorialWriter saver;
    QString savedTutorial = saver.writeTutorial(&tutorial);

    QString expected =
WAITFOR_PARENT_START
"            <waitForComposed compositionType=\"And\">\n"
"                <waitForSignal emitterName=\"The emitter name1\" \
signalName=\"theSignalName1()\"/>\n"
"                <waitForComposed compositionType=\"Or\">\n"
"                    <waitForSignal emitterName=\"The emitter name2\" \
signalName=\"theSignalName2()\"/>\n"
"                    <waitForSignal emitterName=\"The emitter name3\" \
signalName=\"theSignalName3()\"/>\n"
"                </waitForComposed>\n"
"            </waitForComposed>\n"
WAITFOR_PARENT_END;

    QCOMPARE(savedTutorial, expected);
}

void TutorialWriterTest::testWaitForComposedEmpty() {
    Tutorial tutorial;
    Step* step = new Step();
    tutorial.addStep(step);

    WaitForComposed* waitForAndChild = new WaitForComposed();
    waitForAndChild->setCompositionType(WaitForComposed::And);

    WaitForComposed* waitForOrChild = new WaitForComposed();
    waitForOrChild->setCompositionType(WaitForComposed::Or);

    WaitForComposed* waitForAnd = new WaitForComposed();
    waitForAnd->setCompositionType(WaitForComposed::And);
    waitForAnd->addWaitFor(waitForAndChild);
    waitForAnd->addWaitFor(waitForOrChild);

    Reaction* reaction = new Reaction();
    reaction->setTriggerType(Reaction::ConditionMet);
    reaction->setWaitFor(waitForAnd);
    reaction->setResponseType(Reaction::NextStep);
    step->addReaction(reaction);

    TutorialWriter saver;
    QString savedTutorial = saver.writeTutorial(&tutorial);

    QString expected =
WAITFOR_PARENT_START
"            <waitForComposed compositionType=\"And\">\n"
"                <waitForComposed compositionType=\"And\"/>\n"
"                <waitForComposed compositionType=\"Or\"/>\n"
"            </waitForComposed>\n"
WAITFOR_PARENT_END;

    QCOMPARE(savedTutorial, expected);
}

void TutorialWriterTest::testWaitForNot() {
    Tutorial tutorial;
    Step* step = new Step();
    tutorial.addStep(step);

    WaitForSignal* waitForSignal = new WaitForSignal();
    waitForSignal->setEmitterName("The emitter name");
    waitForSignal->setSignalName("theSignalName()");

    WaitForNot* waitForNot = new WaitForNot();
    waitForNot->setNegatedWaitFor(waitForSignal);

    Reaction* reaction = new Reaction();
    reaction->setTriggerType(Reaction::ConditionMet);
    reaction->setWaitFor(waitForNot);
    reaction->setResponseType(Reaction::NextStep);
    step->addReaction(reaction);

    TutorialWriter saver;
    QString savedTutorial = saver.writeTutorial(&tutorial);

    QString expected =
WAITFOR_PARENT_START
"            <waitForNot>\n"
"                <waitForSignal emitterName=\"The emitter name\" \
signalName=\"theSignalName()\"/>\n"
"            </waitForNot>\n"
WAITFOR_PARENT_END;

    QCOMPARE(savedTutorial, expected);
}

void TutorialWriterTest::testWaitForNotWithoutNegatedWaitFor() {
    Tutorial tutorial;
    Step* step = new Step();
    tutorial.addStep(step);

    WaitForNot* waitForNot = new WaitForNot();

    Reaction* reaction = new Reaction();
    reaction->setTriggerType(Reaction::ConditionMet);
    reaction->setWaitFor(waitForNot);
    reaction->setResponseType(Reaction::NextStep);
    step->addReaction(reaction);

    TutorialWriter saver;
    QString savedTutorial = saver.writeTutorial(&tutorial);

    QString expected =
WAITFOR_PARENT_START
"            <waitForNot/>\n"
WAITFOR_PARENT_END;

    QCOMPARE(savedTutorial, expected);
}

QTEST_MAIN(TutorialWriterTest)

#include "TutorialWriterTest.moc"
