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

#include "TutorialReader.h"

#include <KLocalizedString>

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

class TutorialReaderTest: public QObject {
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

    void testXmlNotWellFormed();
    void testXmlWithoutRootTutorialElement();
    void testXmlWithGarbageElementsAndAttributes();

private:

    void assertWaitForSignal(WaitFor* waitFor, const QString& emitterName,
                             const QString& signalName) const;

    QString addGarbageToXmlData(const QString& data) const;

};

void TutorialReaderTest::testTutorial() {
    QString data =
HEADER_XML
"<tutorial name=\"The &quot;name&quot;\">\n"
"    <description>The description,\nwith &lt; and &gt;\n</description>\n"
"    <license>The license text,\nwith &lt; and &gt;\n</license>\n"
"    <setup>The setup code,\nwith &lt; and &gt;\n</setup>\n"
"    <tearDown>The tear down code,\nwith &lt; and &gt;\n</tearDown>\n"
"</tutorial>\n";

    TutorialReader reader;
    QScopedPointer<Tutorial> tutorial(reader.readTutorial(data));

    QVERIFY(tutorial);
    QCOMPARE(tutorial->name(), QString("The \"name\""));
    QCOMPARE(tutorial->description(),
             QString("The description,\nwith < and >\n"));
    QCOMPARE(tutorial->licenseText(),
             QString("The license text,\nwith < and >\n"));
    QCOMPARE(tutorial->customSetupCode(),
             QString("The setup code,\nwith < and >\n"));
    QCOMPARE(tutorial->customTearDownCode(),
             QString("The tear down code,\nwith < and >\n"));
    QCOMPARE(tutorial->steps().count(), 0);
}

void TutorialReaderTest::testTutorialEmpty() {
    QString data =
HEADER_XML
"<tutorial/>\n";

    TutorialReader reader;
    QScopedPointer<Tutorial> tutorial(reader.readTutorial(data));

    QVERIFY(tutorial);
    QCOMPARE(tutorial->name(), QString(""));
    QCOMPARE(tutorial->description(), QString(""));
    QCOMPARE(tutorial->licenseText(), QString(""));
    QCOMPARE(tutorial->customSetupCode(), QString(""));
    QCOMPARE(tutorial->customTearDownCode(), QString(""));
    QCOMPARE(tutorial->steps().count(), 0);
}

void TutorialReaderTest::testTutorialWithSeveralSteps() {
    QString data =
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

    TutorialReader reader;
    QScopedPointer<Tutorial> tutorial(reader.readTutorial(data));

    QVERIFY(tutorial);
    QCOMPARE(tutorial->name(), QString("The \"name\""));
    QCOMPARE(tutorial->description(),
             QString("The description,\nwith < and >\n"));
    QCOMPARE(tutorial->licenseText(),
             QString("The license text,\nwith < and >\n"));
    QCOMPARE(tutorial->customSetupCode(),
             QString("The setup code,\nwith < and >\n"));
    QCOMPARE(tutorial->customTearDownCode(),
             QString("The tear down code,\nwith < and >\n"));
    QCOMPARE(tutorial->steps().count(), 2);
    Step* step = tutorial->steps()[0];
    QVERIFY(step);
    QCOMPARE(step->id(), QString("The id1"));
    QCOMPARE(step->text(), QString("The text1"));
    step = tutorial->steps()[1];
    QVERIFY(step);
    QCOMPARE(step->id(), QString("The id2"));
    QCOMPARE(step->text(), QString("The text2"));
}

void TutorialReaderTest::testStep() {
    QString data =
STEP_PARENT_START
"    <step id=\"The &quot;id&quot;\">\n"
"        <text>The text,\nwith &lt; and &gt;\n</text>\n"
"        <setup>The setup code,\nwith &lt; and &gt;\n</setup>\n"
"        <tearDown>The tear down code,\nwith &lt; and &gt;\n</tearDown>\n"
"    </step>\n"
STEP_PARENT_END;

    TutorialReader reader;
    QScopedPointer<Tutorial> tutorial(reader.readTutorial(data));
    Step* step = tutorial->steps()[0];

    QVERIFY(step);
    QCOMPARE(step->id(), QString("The \"id\""));
    QCOMPARE(step->text(), QString("The text,\nwith < and >\n"));
    QCOMPARE(step->customSetupCode(),
             QString("The setup code,\nwith < and >\n"));
    QCOMPARE(step->customTearDownCode(),
             QString("The tear down code,\nwith < and >\n"));
    QCOMPARE(step->reactions().count(), 0);
}

void TutorialReaderTest::testStepEmpty() {
    QString data =
STEP_PARENT_START
"    <step/>\n"
STEP_PARENT_END;

    TutorialReader reader;
    QScopedPointer<Tutorial> tutorial(reader.readTutorial(data));
    Step* step = tutorial->steps()[0];

    QVERIFY(step);
    QCOMPARE(step->id(), QString(""));
    QCOMPARE(step->text(), QString(""));
    QCOMPARE(step->customSetupCode(), QString(""));
    QCOMPARE(step->customTearDownCode(), QString(""));
    QCOMPARE(step->reactions().count(), 0);
}

void TutorialReaderTest::testStepWithSeveralReactions() {
    QString data =
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

    TutorialReader reader;
    QScopedPointer<Tutorial> tutorial(reader.readTutorial(data));
    Step* step = tutorial->steps()[0];

    QVERIFY(step);
    QCOMPARE(step->id(), QString("The \"id\""));
    QCOMPARE(step->text(), QString("The text,\nwith < and >\n"));
    QCOMPARE(step->customSetupCode(),
             QString("The setup code,\nwith < and >\n"));
    QCOMPARE(step->customTearDownCode(),
             QString("The tear down code,\nwith < and >\n"));
    QCOMPARE(step->reactions().count(), 2);
    Reaction* reaction = step->reactions()[0];
    QVERIFY(reaction);
    QCOMPARE(reaction->triggerType(), Reaction::ConditionMet);
    QCOMPARE(reaction->responseType(), Reaction::CustomCode);
    reaction = step->reactions()[1];
    QVERIFY(reaction);
    QCOMPARE(reaction->triggerType(), Reaction::OptionSelected);
    QCOMPARE(reaction->optionName(), QString("The option name"));
    QCOMPARE(reaction->responseType(), Reaction::NextStep);
    QCOMPARE(reaction->nextStepId(), QString("Another id"));
}

void TutorialReaderTest::testReactionConditionCustomCode() {
    QString data =
REACTION_PARENT_START
"        <reaction triggerType=\"ConditionMet\" responseType=\"CustomCode\">\n"
"            <option name=\"The &quot;option&quot; name\"/>\n"
"            <waitForSignal emitterName=\"The emitter name\" \
signalName=\"theSignalName(Argument1Type, Argument2Type)\"/>\n"
"            <customCode>The custom code,\nwith &lt; and &gt;\n</customCode>\n"
"            <nextStep id=\"Another &quot;id&quot;\"/>\n"
"        </reaction>\n"
REACTION_PARENT_END;

    TutorialReader reader;
    QScopedPointer<Tutorial> tutorial(reader.readTutorial(data));
    Reaction* reaction = tutorial->steps()[0]->reactions()[0];

    QVERIFY(reaction);
    QCOMPARE(reaction->triggerType(), Reaction::ConditionMet);
    QCOMPARE(reaction->optionName(), QString("The \"option\" name"));
    assertWaitForSignal(reaction->waitFor(), "The emitter name",
                        "theSignalName(Argument1Type, Argument2Type)");
    QCOMPARE(reaction->responseType(), Reaction::CustomCode);
    QCOMPARE(reaction->customCode(),
             QString("The custom code,\nwith < and >\n"));
    QCOMPARE(reaction->nextStepId(), QString("Another \"id\""));
}

void TutorialReaderTest::testReactionOptionNextStep() {
    QString data =
REACTION_PARENT_START
"        <reaction triggerType=\"OptionSelected\" responseType=\"NextStep\">\n"
"            <option name=\"The &quot;option&quot; name\"/>\n"
"            <waitForSignal emitterName=\"The emitter name\" \
signalName=\"theSignalName(Argument1Type, Argument2Type)\"/>\n"
"            <customCode>The custom code,\nwith &lt; and &gt;\n</customCode>\n"
"            <nextStep id=\"Another &quot;id&quot;\"/>\n"
"        </reaction>\n"
REACTION_PARENT_END;

    TutorialReader reader;
    QScopedPointer<Tutorial> tutorial(reader.readTutorial(data));
    Reaction* reaction = tutorial->steps()[0]->reactions()[0];

    QVERIFY(reaction);
    QCOMPARE(reaction->triggerType(), Reaction::OptionSelected);
    QCOMPARE(reaction->optionName(), QString("The \"option\" name"));
    assertWaitForSignal(reaction->waitFor(), "The emitter name",
                        "theSignalName(Argument1Type, Argument2Type)");
    QCOMPARE(reaction->responseType(), Reaction::NextStep);
    QCOMPARE(reaction->customCode(),
             QString("The custom code,\nwith < and >\n"));
    QCOMPARE(reaction->nextStepId(), QString("Another \"id\""));
}

void TutorialReaderTest::testReactionEmpty() {
    QString data =
REACTION_PARENT_START
"        <reaction triggerType=\"ConditionMet\" responseType=\"CustomCode\"/>\n"
REACTION_PARENT_END;

    TutorialReader reader;
    QScopedPointer<Tutorial> tutorial(reader.readTutorial(data));
    Reaction* reaction = tutorial->steps()[0]->reactions()[0];

    QVERIFY(reaction);
    QCOMPARE(reaction->triggerType(), Reaction::ConditionMet);
    QCOMPARE(reaction->optionName(), QString(""));
    QCOMPARE(reaction->waitFor(), (WaitFor*)0);
    QCOMPARE(reaction->responseType(), Reaction::CustomCode);
    QCOMPARE(reaction->customCode(), QString(""));
    QCOMPARE(reaction->nextStepId(), QString(""));
}

void TutorialReaderTest::testWaitForEvent() {
    QString data =
WAITFOR_PARENT_START
"            <waitForEvent receiverName=\"The &quot;receiver&quot; name\" \
eventName=\"The&quot;Event&quot;Name\"/>\n"
WAITFOR_PARENT_END;

    TutorialReader reader;
    QScopedPointer<Tutorial> tutorial(reader.readTutorial(data));
    WaitFor* waitFor = tutorial->steps()[0]->reactions()[0]->waitFor();

    WaitForEvent* waitForEvent = qobject_cast<WaitForEvent*>(waitFor);
    QVERIFY(waitForEvent);
    QCOMPARE(waitForEvent->receiverName(), QString("The \"receiver\" name"));
    QCOMPARE(waitForEvent->eventName(), QString("The\"Event\"Name"));
}

void TutorialReaderTest::testWaitForEventEmpty() {
    QString data =
WAITFOR_PARENT_START
"            <waitForEvent/>\n"
WAITFOR_PARENT_END;

    TutorialReader reader;
    QScopedPointer<Tutorial> tutorial(reader.readTutorial(data));
    WaitFor* waitFor = tutorial->steps()[0]->reactions()[0]->waitFor();

    WaitForEvent* waitForEvent = qobject_cast<WaitForEvent*>(waitFor);
    QVERIFY(waitForEvent);
    QCOMPARE(waitForEvent->receiverName(), QString(""));
    QCOMPARE(waitForEvent->eventName(), QString(""));
}

void TutorialReaderTest::testWaitForProperty() {
    QString data =
WAITFOR_PARENT_START
"            <waitForProperty objectName=\"The &quot;object&quot; name\" \
propertyName=\"thePropertyName\" \
value=\"&quot;The \\&quot;value\\&quot;&quot;\"/>\n"
WAITFOR_PARENT_END;

    TutorialReader reader;
    QScopedPointer<Tutorial> tutorial(reader.readTutorial(data));
    WaitFor* waitFor = tutorial->steps()[0]->reactions()[0]->waitFor();

    WaitForProperty* waitForProperty = qobject_cast<WaitForProperty*>(waitFor);
    QVERIFY(waitForProperty);
    QCOMPARE(waitForProperty->objectName(), QString("The \"object\" name"));
    QCOMPARE(waitForProperty->propertyName(), QString("thePropertyName"));
    QCOMPARE(waitForProperty->value(), QString("\"The \\\"value\\\"\""));
}

void TutorialReaderTest::testWaitForPropertyEmpty() {
    QString data =
WAITFOR_PARENT_START
"            <waitForProperty/>\n"
WAITFOR_PARENT_END;

    TutorialReader reader;
    QScopedPointer<Tutorial> tutorial(reader.readTutorial(data));
    WaitFor* waitFor = tutorial->steps()[0]->reactions()[0]->waitFor();

    WaitForProperty* waitForProperty = qobject_cast<WaitForProperty*>(waitFor);
    QVERIFY(waitForProperty);
    QCOMPARE(waitForProperty->objectName(), QString(""));
    QCOMPARE(waitForProperty->propertyName(), QString(""));
    QCOMPARE(waitForProperty->value(), QString(""));
}

void TutorialReaderTest::testWaitForSignal() {
    QString data =
WAITFOR_PARENT_START
"            <waitForSignal emitterName=\"The &quot;emitter&quot; name\" \
signalName=\"theSignalName(&quot;Argument1Type&quot;)\"/>\n"
WAITFOR_PARENT_END;

    TutorialReader reader;
    QScopedPointer<Tutorial> tutorial(reader.readTutorial(data));
    WaitFor* waitFor = tutorial->steps()[0]->reactions()[0]->waitFor();

    assertWaitForSignal(waitFor, "The \"emitter\" name",
                        "theSignalName(\"Argument1Type\")");
}

void TutorialReaderTest::testWaitForSignalEmpty() {
    QString data =
WAITFOR_PARENT_START
"            <waitForSignal/>\n"
WAITFOR_PARENT_END;

    TutorialReader reader;
    QScopedPointer<Tutorial> tutorial(reader.readTutorial(data));
    WaitFor* waitFor = tutorial->steps()[0]->reactions()[0]->waitFor();

    assertWaitForSignal(waitFor, "", "");
}

void TutorialReaderTest::testWaitForStepActivation() {
    QString data =
WAITFOR_PARENT_START
"            <waitForStepActivation/>\n"
WAITFOR_PARENT_END;

    TutorialReader reader;
    QScopedPointer<Tutorial> tutorial(reader.readTutorial(data));
    WaitFor* waitFor = tutorial->steps()[0]->reactions()[0]->waitFor();

    WaitForStepActivation* waitForStepActivation =
                                qobject_cast<WaitForStepActivation*>(waitFor);
    QVERIFY(waitForStepActivation);
}

void TutorialReaderTest::testWaitForWindow() {
    QString data =
WAITFOR_PARENT_START
"            <waitForWindow windowObjectName=\"The &quot;window&quot; object \
name\"/>\n"
WAITFOR_PARENT_END;

    TutorialReader reader;
    QScopedPointer<Tutorial> tutorial(reader.readTutorial(data));
    WaitFor* waitFor = tutorial->steps()[0]->reactions()[0]->waitFor();

    WaitForWindow* waitForWindow = qobject_cast<WaitForWindow*>(waitFor);
    QVERIFY(waitForWindow);
    QCOMPARE(waitForWindow->windowObjectName(),
             QString("The \"window\" object name"));
}

void TutorialReaderTest::testWaitForWindowEmpty() {
    QString data =
WAITFOR_PARENT_START
"            <waitForWindow/>\n"
WAITFOR_PARENT_END;

    TutorialReader reader;
    QScopedPointer<Tutorial> tutorial(reader.readTutorial(data));
    WaitFor* waitFor = tutorial->steps()[0]->reactions()[0]->waitFor();

    WaitForWindow* waitForWindow = qobject_cast<WaitForWindow*>(waitFor);
    QVERIFY(waitForWindow);
    QCOMPARE(waitForWindow->windowObjectName(), QString(""));
}

void TutorialReaderTest::testWaitForComposed() {
    QString data =
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

    TutorialReader reader;
    QScopedPointer<Tutorial> tutorial(reader.readTutorial(data));
    WaitFor* waitFor = tutorial->steps()[0]->reactions()[0]->waitFor();

    WaitForComposed* waitForAnd = qobject_cast<WaitForComposed*>(waitFor);
    QVERIFY(waitForAnd);
    QCOMPARE(waitForAnd->compositionType(), WaitForComposed::And);
    QCOMPARE(waitForAnd->waitFors().count(), 2);
    assertWaitForSignal(waitForAnd->waitFors()[0], "The emitter name1",
                        "theSignalName1()");

    WaitForComposed* waitForOr =
                    qobject_cast<WaitForComposed*>(waitForAnd->waitFors()[1]);
    QVERIFY(waitForOr);
    QCOMPARE(waitForOr->compositionType(), WaitForComposed::Or);
    QCOMPARE(waitForOr->waitFors().count(), 2);
    assertWaitForSignal(waitForOr->waitFors()[0], "The emitter name2",
                        "theSignalName2()");
    assertWaitForSignal(waitForOr->waitFors()[1], "The emitter name3",
                        "theSignalName3()");
}

void TutorialReaderTest::testWaitForComposedEmpty() {
    QString data =
WAITFOR_PARENT_START
"            <waitForComposed compositionType=\"And\">\n"
"                <waitForComposed compositionType=\"And\"/>\n"
"                <waitForComposed compositionType=\"Or\"/>\n"
"            </waitForComposed>\n"
WAITFOR_PARENT_END;

    TutorialReader reader;
    QScopedPointer<Tutorial> tutorial(reader.readTutorial(data));
    WaitFor* waitFor = tutorial->steps()[0]->reactions()[0]->waitFor();

    WaitForComposed* waitForAnd = qobject_cast<WaitForComposed*>(waitFor);
    QVERIFY(waitForAnd);
    QCOMPARE(waitForAnd->compositionType(), WaitForComposed::And);
    QCOMPARE(waitForAnd->waitFors().count(), 2);

    WaitForComposed* waitForAndChild =
                    qobject_cast<WaitForComposed*>(waitForAnd->waitFors()[0]);
    QVERIFY(waitForAndChild);
    QCOMPARE(waitForAndChild->compositionType(), WaitForComposed::And);
    QCOMPARE(waitForAndChild->waitFors().count(), 0);

    WaitForComposed* waitForOrChild =
                    qobject_cast<WaitForComposed*>(waitForAnd->waitFors()[1]);
    QVERIFY(waitForOrChild);
    QCOMPARE(waitForOrChild->compositionType(), WaitForComposed::Or);
    QCOMPARE(waitForOrChild->waitFors().count(), 0);
}

void TutorialReaderTest::testWaitForNot() {
    QString data =
WAITFOR_PARENT_START
"            <waitForNot>\n"
"                <waitForSignal emitterName=\"The emitter name\" \
signalName=\"theSignalName()\"/>\n"
"            </waitForNot>\n"
WAITFOR_PARENT_END;

    TutorialReader reader;
    QScopedPointer<Tutorial> tutorial(reader.readTutorial(data));
    WaitFor* waitFor = tutorial->steps()[0]->reactions()[0]->waitFor();

    WaitForNot* waitForNot = qobject_cast<WaitForNot*>(waitFor);
    QVERIFY(waitForNot);
    WaitForSignal* waitForSignal =
                    qobject_cast<WaitForSignal*>(waitForNot->negatedWaitFor());
    QVERIFY(waitForSignal);
    QCOMPARE(waitForSignal->emitterName(), QString("The emitter name"));
    QCOMPARE(waitForSignal->signalName(), QString("theSignalName()"));
}

void TutorialReaderTest::testWaitForNotWithoutNegatedWaitFor() {
    QString data =
WAITFOR_PARENT_START
"            <waitForNot/>\n"
WAITFOR_PARENT_END;

    TutorialReader reader;
    QScopedPointer<Tutorial> tutorial(reader.readTutorial(data));
    WaitFor* waitFor = tutorial->steps()[0]->reactions()[0]->waitFor();

    WaitForNot* waitForNot = qobject_cast<WaitForNot*>(waitFor);
    QVERIFY(waitForNot);
    QCOMPARE(waitForNot->negatedWaitFor(), (WaitFor*)0);
}

void TutorialReaderTest::testXmlNotWellFormed() {
    QString data =
HEADER_XML
"<tutorial>\n"
"    <step>\n"
"    </invalidEndElement>\n"
"</tutorial>\n";

    TutorialReader reader;
    try {
        QScopedPointer<Tutorial> tutorial(reader.readTutorial(data));
        QFAIL("Expected DeserializationException not thrown");
    } catch (DeserializationException e) {
        QVERIFY(e.message().contains(i18n("XML document is not well formed")));
    }
}

void TutorialReaderTest::testXmlWithoutRootTutorialElement() {
    QString data =
HEADER_XML
"<unknownRootElement>\n"
"</unknownRootElement>\n";

    TutorialReader reader;
    try {
        QScopedPointer<Tutorial> tutorial(reader.readTutorial(data));
        QFAIL("Expected DeserializationException not thrown");
    } catch (DeserializationException e) {
        QVERIFY(e.message().contains(i18n("Unknown root element")));
    }
}

void TutorialReaderTest::testXmlWithGarbageElementsAndAttributes() {
    QString data =
HEADER_XML
"<tutorial name=\"The &quot;name&quot;\">\n"
TUTORIAL_ELEMENTS
"    <step id=\"The id1\">\n"
STEP_ELEMENTS
"    </step>\n"
"    <step id=\"The id2\">\n"
"        <text>The text2</text>\n"
"        <reaction triggerType=\"OptionSelected\" responseType=\"NextStep\">\n"
"            <option name=\"The &quot;option&quot; name\"/>\n"
"            <customCode>The custom code,\nwith &lt; and &gt;\n</customCode>\n"
"            <nextStep id=\"Another &quot;id&quot;\"/>\n"
"        </reaction>\n"
"        <reaction triggerType=\"ConditionMet\" responseType=\"CustomCode\">\n"
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
"        </reaction>\n"
"    </step>\n"
"</tutorial>\n";

    data = addGarbageToXmlData(data);

    TutorialReader reader;
    QScopedPointer<Tutorial> tutorial(reader.readTutorial(data));

    QVERIFY(tutorial);
    QCOMPARE(tutorial->name(), QString("The \"name\""));
    QCOMPARE(tutorial->description(),
             QString("The description,\nwith < and >\n"));
    QCOMPARE(tutorial->licenseText(),
             QString("The license text,\nwith < and >\n"));
    QCOMPARE(tutorial->customSetupCode(),
             QString("The setup code,\nwith < and >\n"));
    QCOMPARE(tutorial->customTearDownCode(),
             QString("The tear down code,\nwith < and >\n"));
    QCOMPARE(tutorial->steps().count(), 2);

    Step* step = tutorial->steps()[0];
    QVERIFY(step);
    QCOMPARE(step->id(), QString("The id1"));
    QCOMPARE(step->text(), QString("The text,\nwith < and >\n"));
    QCOMPARE(step->customSetupCode(),
             QString("The setup code,\nwith < and >\n"));
    QCOMPARE(step->customTearDownCode(),
             QString("The tear down code,\nwith < and >\n"));
    QCOMPARE(step->reactions().count(), 0);

    step = tutorial->steps()[1];
    QVERIFY(step);
    QCOMPARE(step->id(), QString("The id2"));
    QCOMPARE(step->text(), QString("The text2"));
    QCOMPARE(step->reactions().count(), 2);

    Reaction* reaction = step->reactions()[0];
    QCOMPARE(reaction->triggerType(), Reaction::OptionSelected);
    QCOMPARE(reaction->optionName(), QString("The \"option\" name"));
    QCOMPARE(reaction->waitFor(), (WaitFor*)0);
    QCOMPARE(reaction->responseType(), Reaction::NextStep);
    QCOMPARE(reaction->customCode(),
             QString("The custom code,\nwith < and >\n"));
    QCOMPARE(reaction->nextStepId(), QString("Another \"id\""));

    reaction = step->reactions()[1];
    QCOMPARE(reaction->triggerType(), Reaction::ConditionMet);
    QCOMPARE(reaction->optionName(), QString(""));
    QCOMPARE(reaction->responseType(), Reaction::CustomCode);
    QCOMPARE(reaction->customCode(), QString(""));
    QCOMPARE(reaction->nextStepId(), QString(""));
    WaitFor* waitFor = reaction->waitFor();

    WaitForComposed* waitForAnd = qobject_cast<WaitForComposed*>(waitFor);
    QVERIFY(waitForAnd);
    QCOMPARE(waitForAnd->compositionType(), WaitForComposed::And);
    QCOMPARE(waitForAnd->waitFors().count(), 2);
    assertWaitForSignal(waitForAnd->waitFors()[0], "The emitter name1",
                        "theSignalName1()");

    WaitForComposed* waitForOr =
                    qobject_cast<WaitForComposed*>(waitForAnd->waitFors()[1]);
    QVERIFY(waitForOr);
    QCOMPARE(waitForOr->compositionType(), WaitForComposed::Or);
    QCOMPARE(waitForOr->waitFors().count(), 2);
    assertWaitForSignal(waitForOr->waitFors()[0], "The emitter name2",
                        "theSignalName2()");
    assertWaitForSignal(waitForOr->waitFors()[1], "The emitter name3",
                        "theSignalName3()");
}

/////////////////////////////////// Helpers ////////////////////////////////////

void TutorialReaderTest::assertWaitForSignal(WaitFor* waitFor,
                                             const QString& emitterName,
                                             const QString& signalName) const {
    WaitForSignal* waitForSignal = qobject_cast<WaitForSignal*>(waitFor);
    QVERIFY(waitForSignal);
    QCOMPARE(waitForSignal->emitterName(), emitterName);
    QCOMPARE(waitForSignal->signalName(), signalName);
}

QString TutorialReaderTest::addGarbageToXmlData(const QString& data) const {
    QString garbagedData = data;
    //Add a " garbage=\"trash\"" attribute to every element
    garbagedData.replace(QRegExp("(<\\w+( \\w+=\"[ &;\"\\w]*\")*)(>|/>)"),
                         "\\1 garbage=\"trash\"\\3");
    //Add a "<garbage/>" element after each start element (even for text
    //elements)
    garbagedData.replace(QRegExp("(<\\w+( \\w+=\"[ &;\"\\w]*\")*>)"),
                         "\\1<garbage/>");
    return garbagedData;
}

QTEST_MAIN(TutorialReaderTest)

#include "TutorialReaderTest.moc"
