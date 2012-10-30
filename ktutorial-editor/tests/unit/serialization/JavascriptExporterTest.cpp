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

#include "JavascriptExporter.h"

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

#define TUTORIAL_EMPTY_INFORMATION_CODE \
"t = Kross.module(\"kdetranslation\");\n"\
"\n"\
"//Error: Tutorial without name!\n"\
"//Error: Tutorial without description!\n"\
"\n"

#define STEP_WITH_ID_THE_ID_AND_EMPTY_TEXT_START_CODE \
"//Step The id\n"\
"theIdStep = ktutorial.newStep(\"The id\");\n"\
"//Error: Step without text!\n"\
"\n"

#define CONNECT_STEP_SETUP \
"connect(theIdStep, \"setup(QObject*)\",\n"\
"        this, \"theIdStepSetup(QObject*)\");\n"\
"\n"

#define STEP_WITH_ID_THE_ID_AND_EMPTY_TEXT_END_CODE \
"tutorial.addStep(theIdStep);\n"\
"\n"

class JavascriptExporterTest: public QObject {
Q_OBJECT

private slots:

    void testTutorialLicense();
    void testTutorialInformation();
    void testTutorialInformationWithEscapeSequences();
    void testTutorialSetupCode();
    void testTutorialTearDownCode();
    void testTutorialWithSeveralSteps();

    void testStep();
    void testStepWithEscapeSequences();
    void testStepWithRichText();
    void testStepWithSemanticMarkup();
    void testStepSetupCode();
    void testStepSetupCodeWithReactions();
    void testStepTearDownCode();
    void testStepWithoutId();
    void testStepWithSeveralReactions();

    void testReactionOptionNextStep();
    void testReactionOptionNextStepWithEscapeSequences();
    void testReactionOptionNextStepWithoutOptionNameOrStepId();
    void testReactionOptionCustomCode();
    void testReactionOptionCustomCodeWithEscapeSequences();
    void testReactionOptionCustomCodeWithoutOptionNameOrCustomCode();
    void testReactionConditionNextStep();
    void testReactionConditionNextStepWithAncestorNames();
    void testReactionConditionNextStepWithEscapeSequences();
    void testReactionConditionNextStepWithoutConditionOrStepId();
    void testReactionConditionCustomCode();
    void testReactionConditionCustomCodeWithAncestorNames();
    void testReactionConditionCustomCodeWithEscapeSequences();
    void testReactionConditionCustomCodeWithoutConditionOrCustomCode();

    void testWaitForEvent();
    void testWaitForEventWithAncestorNames();
    void testWaitForEventWithEscapeSequences();
    void testWaitForEventWithoutReceiverNameOrEventName();
    void testWaitForProperty();
    void testWaitForPropertyWithAncestorNames();
    void testWaitForPropertyWithEscapeSequences();
    void testWaitForPropertyWithoutObjectNameOrPropertyNameOrValue();
    void testWaitForSignal();
    void testWaitForSignalWithAncestorNames();
    void testWaitForSignalWithEscapeSequences();
    void testWaitForSignalWithoutEmitterNameOrSignalName();
    void testWaitForStepActivation();
    void testWaitForWindow();
    void testWaitForWindowWithAncestorNames();
    void testWaitForWindowWithEscapeSequences();
    void testWaitForWindowWithoutWindowObjectName();
    void testWaitForComposed();
    void testWaitForComposedWithInvalidChildWaitFor();
    void testWaitForNot();
    void testWaitForNotWithInvalidNegatedWaitFor();
    void testWaitForNotWithoutNegatedWaitFor();

};

void JavascriptExporterTest::testTutorialLicense() {
    Tutorial tutorial;
    tutorial.setLicenseText("The license text, which should be\nwrapped");

    JavascriptExporter exporter;
    QString exportedTutorial = exporter.exportTutorial(&tutorial);

    QString expected =
"/*****************************************\n"
" *   The license text, which should be   *\n"
" *   wrapped                             *\n"
" *****************************************/\n"
"\n"
TUTORIAL_EMPTY_INFORMATION_CODE;

    QCOMPARE(exportedTutorial, expected);
}

void JavascriptExporterTest::testTutorialInformation() {
    Tutorial tutorial;
    tutorial.setName("The name");
    tutorial.setDescription("The description");

    JavascriptExporter exporter;
    QString exportedTutorial = exporter.exportTutorial(&tutorial);

    QString expected =
"t = Kross.module(\"kdetranslation\");\n"
"\n"
"tutorial.tutorialInformationAsObject().setName(t.i18n(\"The name\"));\n"
"tutorial.tutorialInformationAsObject().setDescription("
                                            "t.i18n(\"The description\"));\n"
"\n";

    QCOMPARE(exportedTutorial, expected);
}

void JavascriptExporterTest::testTutorialInformationWithEscapeSequences() {
    Tutorial tutorial;
    tutorial.setName("The \"name\"");
    tutorial.setDescription("The\tdescription\nwith \"escape\" sequences");

    JavascriptExporter exporter;
    QString exportedTutorial = exporter.exportTutorial(&tutorial);

    QString expected =
"t = Kross.module(\"kdetranslation\");\n"
"\n"
"tutorial.tutorialInformationAsObject().setName(t.i18n(\"The \\\"name\\\"\"));\n"
"tutorial.tutorialInformationAsObject().setDescription(\
t.i18n(\"The\\tdescription\\nwith \\\"escape\\\" sequences\"));\n"
"\n";

    QCOMPARE(exportedTutorial, expected);
}

void JavascriptExporterTest::testTutorialSetupCode() {
    Tutorial tutorial;
    tutorial.setCustomSetupCode("The custom setup\ncode");

    JavascriptExporter exporter;
    QString exportedTutorial = exporter.exportTutorial(&tutorial);

    QString expected =
TUTORIAL_EMPTY_INFORMATION_CODE
"function tutorialSetup(tutorial) {\n"
"    The custom setup\n"
"    code\n"
"}\n"
"connect(tutorial, \"setup(QObject*)\",\n"
"        this, \"tutorialSetup(QObject*)\");\n"
"\n";

    QCOMPARE(exportedTutorial, expected);
}

void JavascriptExporterTest::testTutorialTearDownCode() {
    Tutorial tutorial;
    tutorial.setCustomTearDownCode("The custom tear down\ncode");

    JavascriptExporter exporter;
    QString exportedTutorial = exporter.exportTutorial(&tutorial);

    QString expected =
TUTORIAL_EMPTY_INFORMATION_CODE
"function tutorialTearDown(tutorial) {\n"
"    The custom tear down\n"
"    code\n"
"}\n"
"connect(tutorial, \"tearDown(QObject*)\",\n"
"        this, \"tutorialTearDown(QObject*)\");\n"
"\n";

    QCOMPARE(exportedTutorial, expected);
}

void JavascriptExporterTest::testTutorialWithSeveralSteps() {
    Tutorial tutorial;

    Step* step1 = new Step();
    step1->setId("The id1");
    step1->setText("The text1");
    tutorial.addStep(step1);

    WaitForSignal* waitForSignal1 = new WaitForSignal();
    waitForSignal1->setEmitterName("The emitter name");
    waitForSignal1->setSignalName("theSignalName(Argument1Type)");

    Reaction* reaction1 = new Reaction();
    reaction1->setTriggerType(Reaction::ConditionMet);
    reaction1->setWaitFor(waitForSignal1);
    reaction1->setResponseType(Reaction::CustomCode);
    reaction1->setCustomCode("The custom\ncode1");
    step1->addReaction(reaction1);

    Step* step2 = new Step();
    step2->setId("The id2");
    step2->setText("The text2");
    tutorial.addStep(step2);

    WaitForSignal* waitForSignal2 = new WaitForSignal();
    waitForSignal2->setEmitterName("The emitter name");
    waitForSignal2->setSignalName("theSignalName(Argument1Type)");

    Reaction* reaction2 = new Reaction();
    reaction2->setTriggerType(Reaction::ConditionMet);
    reaction2->setWaitFor(waitForSignal2);
    reaction2->setResponseType(Reaction::CustomCode);
    reaction2->setCustomCode("The custom\ncode2");
    step2->addReaction(reaction2);

    JavascriptExporter exporter;
    QString exportedTutorial = exporter.exportTutorial(&tutorial);

    QString expected =
TUTORIAL_EMPTY_INFORMATION_CODE
"//Step The id1\n"
"theId1Step = ktutorial.newStep(\"The id1\");\n"
"theId1Step.setText(t.i18nc(\"@info/plain\", \"The text1\"));\n"
"\n"
"function theId1StepSetup(step) {\n"
"    waitForTheSignalNameByTheEmitterName = \
ktutorial.newWaitFor(\"WaitForSignal\");\n"
"    waitForTheSignalNameByTheEmitterName.setSignal(\
ktutorial.findObject(\"The emitter name\"), \
\"theSignalName(Argument1Type)\");\n"
"    step.addWaitFor(waitForTheSignalNameByTheEmitterName, self, \
\"theId1StepWaitForTheSignalNameByTheEmitterNameConditionMet()\");\n"
"}\n"
"connect(theId1Step, \"setup(QObject*)\",\n"
"        this, \"theId1StepSetup(QObject*)\");\n"
"\n"
"function theId1StepWaitForTheSignalNameByTheEmitterNameConditionMet() {\n"
"    The custom\n"
"    code1\n"
"}\n"
"\n"
"tutorial.addStep(theId1Step);\n"
"\n"
"//Step The id2\n"
"theId2Step = ktutorial.newStep(\"The id2\");\n"
"theId2Step.setText(t.i18nc(\"@info/plain\", \"The text2\"));\n"
"\n"
"function theId2StepSetup(step) {\n"
"    waitForTheSignalNameByTheEmitterName = \
ktutorial.newWaitFor(\"WaitForSignal\");\n"
"    waitForTheSignalNameByTheEmitterName.setSignal(\
ktutorial.findObject(\"The emitter name\"), \
\"theSignalName(Argument1Type)\");\n"
"    step.addWaitFor(waitForTheSignalNameByTheEmitterName, self, \
\"theId2StepWaitForTheSignalNameByTheEmitterNameConditionMet()\");\n"
"}\n"
"connect(theId2Step, \"setup(QObject*)\",\n"
"        this, \"theId2StepSetup(QObject*)\");\n"
"\n"
"function theId2StepWaitForTheSignalNameByTheEmitterNameConditionMet() {\n"
"    The custom\n"
"    code2\n"
"}\n"
"\n"
"tutorial.addStep(theId2Step);\n"
"\n";

    QCOMPARE(exportedTutorial, expected);
}

void JavascriptExporterTest::testStep() {
    Tutorial tutorial;
    Step* step = new Step();
    step->setId("The id");
    step->setText("The text");
    tutorial.addStep(step);

    JavascriptExporter exporter;
    QString exportedTutorial = exporter.exportTutorial(&tutorial);

    QString expected =
TUTORIAL_EMPTY_INFORMATION_CODE
"//Step The id\n"
"theIdStep = ktutorial.newStep(\"The id\");\n"
"theIdStep.setText(t.i18nc(\"@info/plain\", \"The text\"));\n"
"\n"
"tutorial.addStep(theIdStep);\n"
"\n";

    QCOMPARE(exportedTutorial, expected);
}

void JavascriptExporterTest::testStepWithEscapeSequences() {
    Tutorial tutorial;
    Step* step = new Step();
    step->setId("The \"id\"");
    step->setText("The\ttext\nwith \"escape\" sequences");
    tutorial.addStep(step);

    JavascriptExporter exporter;
    QString exportedTutorial = exporter.exportTutorial(&tutorial);

    QString expected =
TUTORIAL_EMPTY_INFORMATION_CODE
"//Step The \"id\"\n"
"theIdStep = ktutorial.newStep(\"The \\\"id\\\"\");\n"
"theIdStep.setText(t.i18nc(\"@info/plain\", \
\"The\\ttext\\nwith \\\"escape\\\" sequences\"));\n"
"\n"
"tutorial.addStep(theIdStep);\n"
"\n";

    QCOMPARE(exportedTutorial, expected);
}

void JavascriptExporterTest::testStepWithRichText() {
    Tutorial tutorial;
    Step* step = new Step();
    step->setId("The id");
    step->setText("The <i>rich</i> text");
    tutorial.addStep(step);

    JavascriptExporter exporter;
    QString exportedTutorial = exporter.exportTutorial(&tutorial);

    QString expected =
TUTORIAL_EMPTY_INFORMATION_CODE
"//Step The id\n"
"theIdStep = ktutorial.newStep(\"The id\");\n"
"theIdStep.setText(t.i18nc(\"@info\", \"The <i>rich</i> text\"));\n"
"\n"
"tutorial.addStep(theIdStep);\n"
"\n";

    QCOMPARE(exportedTutorial, expected);
}

void JavascriptExporterTest::testStepWithSemanticMarkup() {
    Tutorial tutorial;
    Step* step = new Step();
    step->setId("The id");
    step->setText("The text with <emphasis>semantic markup</emphasis>");
    tutorial.addStep(step);

    JavascriptExporter exporter;
    QString exportedTutorial = exporter.exportTutorial(&tutorial);

    QString expected =
TUTORIAL_EMPTY_INFORMATION_CODE
"//Step The id\n"
"theIdStep = ktutorial.newStep(\"The id\");\n"
"theIdStep.setText(t.i18nc(\"@info\", \
\"The text with <emphasis>semantic markup</emphasis>\"));\n"
"\n"
"tutorial.addStep(theIdStep);\n"
"\n";

    QCOMPARE(exportedTutorial, expected);
}

void JavascriptExporterTest::testStepSetupCode() {
    Tutorial tutorial;
    Step* step = new Step();
    step->setId("The id");
    step->setCustomSetupCode("The custom setup\ncode");
    tutorial.addStep(step);

    JavascriptExporter exporter;
    QString exportedTutorial = exporter.exportTutorial(&tutorial);

    QString expected =
TUTORIAL_EMPTY_INFORMATION_CODE
STEP_WITH_ID_THE_ID_AND_EMPTY_TEXT_START_CODE
"function theIdStepSetup(step) {\n"
"    The custom setup\n"
"    code\n"
"}\n"
"connect(theIdStep, \"setup(QObject*)\",\n"
"        this, \"theIdStepSetup(QObject*)\");\n"
"\n"
STEP_WITH_ID_THE_ID_AND_EMPTY_TEXT_END_CODE;

    QCOMPARE(exportedTutorial, expected);
}

void JavascriptExporterTest::testStepSetupCodeWithReactions() {
    Tutorial tutorial;
    Step* step = new Step();
    step->setId("The id");
    step->setCustomSetupCode("The custom setup\ncode");
    tutorial.addStep(step);

    Reaction* reaction = new Reaction();
    reaction->setTriggerType(Reaction::OptionSelected);
    reaction->setOptionName("The option name");
    reaction->setResponseType(Reaction::NextStep);
    reaction->setNextStepId("Another step");
    step->addReaction(reaction);

    JavascriptExporter exporter;
    QString exportedTutorial = exporter.exportTutorial(&tutorial);

    QString expected =
TUTORIAL_EMPTY_INFORMATION_CODE
STEP_WITH_ID_THE_ID_AND_EMPTY_TEXT_START_CODE
"function theIdStepSetup(step) {\n"
"    theOptionNameOption = ktutorial.newOption(\
t.i18nc(\"@action Tutorial option\", \"The option name\"));\n"
"    step.addOption(theOptionNameOption, \"Another step\");\n"
"\n"
"    The custom setup\n"
"    code\n"
"}\n"
"connect(theIdStep, \"setup(QObject*)\",\n"
"        this, \"theIdStepSetup(QObject*)\");\n"
"\n"
STEP_WITH_ID_THE_ID_AND_EMPTY_TEXT_END_CODE;

    QCOMPARE(exportedTutorial, expected);
}

void JavascriptExporterTest::testStepTearDownCode() {
    Tutorial tutorial;
    Step* step = new Step();
    step->setId("The id");
    step->setCustomTearDownCode("The custom tear down\ncode");
    tutorial.addStep(step);

    JavascriptExporter exporter;
    QString exportedTutorial = exporter.exportTutorial(&tutorial);

    QString expected =
TUTORIAL_EMPTY_INFORMATION_CODE
STEP_WITH_ID_THE_ID_AND_EMPTY_TEXT_START_CODE
"function theIdStepTearDown(step) {\n"
"    The custom tear down\n"
"    code\n"
"}\n"
"connect(theIdStep, \"tearDown(QObject*)\",\n"
"        this, \"theIdStepTearDown(QObject*)\");\n"
"\n"
STEP_WITH_ID_THE_ID_AND_EMPTY_TEXT_END_CODE;

    QCOMPARE(exportedTutorial, expected);
}

void JavascriptExporterTest::testStepWithoutId() {
    Tutorial tutorial;
    Step* step = new Step();
    step->setText("The text");
    step->setCustomSetupCode("The custom setup\ncode");
    step->setCustomTearDownCode("The custom tear down\ncode");
    tutorial.addStep(step);

    Reaction* reaction = new Reaction();
    reaction->setTriggerType(Reaction::OptionSelected);
    reaction->setOptionName("The option name");
    reaction->setResponseType(Reaction::NextStep);
    reaction->setNextStepId("Another step");
    step->addReaction(reaction);

    JavascriptExporter exporter;
    QString exportedTutorial = exporter.exportTutorial(&tutorial);

    QString expected =
TUTORIAL_EMPTY_INFORMATION_CODE
"//Error: Step without id!\n"
"\n";

    QCOMPARE(exportedTutorial, expected);
}

void JavascriptExporterTest::testStepWithSeveralReactions() {
    Tutorial tutorial;

    Step* step = new Step();
    step->setId("The id");
    tutorial.addStep(step);

    WaitForSignal* waitForSignal = new WaitForSignal();
    waitForSignal->setEmitterName("The emitter name");
    waitForSignal->setSignalName("theSignalName(Argument1Type)");

    Reaction* reaction1 = new Reaction();
    reaction1->setTriggerType(Reaction::ConditionMet);
    reaction1->setWaitFor(waitForSignal);
    reaction1->setResponseType(Reaction::CustomCode);
    reaction1->setCustomCode("The custom\ncode1");
    step->addReaction(reaction1);

    Reaction* reaction2 = new Reaction();
    reaction2->setTriggerType(Reaction::OptionSelected);
    reaction2->setOptionName("The option name");
    reaction2->setResponseType(Reaction::CustomCode);
    reaction2->setCustomCode("The custom\ncode2");
    step->addReaction(reaction2);

    JavascriptExporter exporter;
    QString exportedTutorial = exporter.exportTutorial(&tutorial);

    QString expected =
TUTORIAL_EMPTY_INFORMATION_CODE
STEP_WITH_ID_THE_ID_AND_EMPTY_TEXT_START_CODE
"function theIdStepSetup(step) {\n"
"    waitForTheSignalNameByTheEmitterName = \
ktutorial.newWaitFor(\"WaitForSignal\");\n"
"    waitForTheSignalNameByTheEmitterName.setSignal(\
ktutorial.findObject(\"The emitter name\"), \
\"theSignalName(Argument1Type)\");\n"
"    step.addWaitFor(waitForTheSignalNameByTheEmitterName, self, \
\"theIdStepWaitForTheSignalNameByTheEmitterNameConditionMet()\");\n"
"\n"
"    theOptionNameOption = ktutorial.newOption(\
t.i18nc(\"@action Tutorial option\", \"The option name\"));\n"
"    step.addOption(theOptionNameOption, self, \
\"theIdStepTheOptionNameOptionSelected()\");\n"
"}\n"
CONNECT_STEP_SETUP
"function theIdStepWaitForTheSignalNameByTheEmitterNameConditionMet() {\n"
"    The custom\n"
"    code1\n"
"}\n"
"\n"
"function theIdStepTheOptionNameOptionSelected() {\n"
"    The custom\n"
"    code2\n"
"}\n"
"\n"
STEP_WITH_ID_THE_ID_AND_EMPTY_TEXT_END_CODE;

    QCOMPARE(exportedTutorial, expected);
}

void JavascriptExporterTest::testReactionOptionNextStep() {
    Tutorial tutorial;
    Step* step = new Step();
    step->setId("The id");
    tutorial.addStep(step);

    Reaction* reaction = new Reaction();
    reaction->setTriggerType(Reaction::OptionSelected);
    reaction->setOptionName("The option name");
    reaction->setResponseType(Reaction::NextStep);
    reaction->setNextStepId("Another step");
    step->addReaction(reaction);

    JavascriptExporter exporter;
    QString exportedTutorial = exporter.exportTutorial(&tutorial);

    QString expected =
TUTORIAL_EMPTY_INFORMATION_CODE
STEP_WITH_ID_THE_ID_AND_EMPTY_TEXT_START_CODE
"function theIdStepSetup(step) {\n"
"    theOptionNameOption = ktutorial.newOption(\
t.i18nc(\"@action Tutorial option\", \"The option name\"));\n"
"    step.addOption(theOptionNameOption, \"Another step\");\n"
"}\n"
CONNECT_STEP_SETUP
STEP_WITH_ID_THE_ID_AND_EMPTY_TEXT_END_CODE;

    QCOMPARE(exportedTutorial, expected);
}

void JavascriptExporterTest::testReactionOptionNextStepWithEscapeSequences() {
    Tutorial tutorial;
    Step* step = new Step();
    step->setId("The id");
    tutorial.addStep(step);

    Reaction* reaction = new Reaction();
    reaction->setTriggerType(Reaction::OptionSelected);
    reaction->setOptionName("The \"option\" name");
    reaction->setResponseType(Reaction::NextStep);
    reaction->setNextStepId("Another \"step\"");
    step->addReaction(reaction);

    JavascriptExporter exporter;
    QString exportedTutorial = exporter.exportTutorial(&tutorial);

    QString expected =
TUTORIAL_EMPTY_INFORMATION_CODE
STEP_WITH_ID_THE_ID_AND_EMPTY_TEXT_START_CODE
"function theIdStepSetup(step) {\n"
"    theOptionNameOption = ktutorial.newOption(\
t.i18nc(\"@action Tutorial option\", \"The \\\"option\\\" name\"));\n"
"    step.addOption(theOptionNameOption, \"Another \\\"step\\\"\");\n"
"}\n"
CONNECT_STEP_SETUP
STEP_WITH_ID_THE_ID_AND_EMPTY_TEXT_END_CODE;

    QCOMPARE(exportedTutorial, expected);
}

void JavascriptExporterTest::
                        testReactionOptionNextStepWithoutOptionNameOrStepId() {
    Tutorial tutorial;
    Step* step = new Step();
    step->setId("The id");
    tutorial.addStep(step);

    Reaction* reaction = new Reaction();
    reaction->setTriggerType(Reaction::OptionSelected);
    reaction->setResponseType(Reaction::NextStep);
    step->addReaction(reaction);

    JavascriptExporter exporter;
    QString exportedTutorial = exporter.exportTutorial(&tutorial);

    QString expected =
TUTORIAL_EMPTY_INFORMATION_CODE
STEP_WITH_ID_THE_ID_AND_EMPTY_TEXT_START_CODE
"function theIdStepSetup(step) {\n"
"    //Error: Option without name!\n"
"}\n"
CONNECT_STEP_SETUP
STEP_WITH_ID_THE_ID_AND_EMPTY_TEXT_END_CODE;

    QCOMPARE(exportedTutorial, expected);

    reaction->setOptionName("The option name");

    exportedTutorial = exporter.exportTutorial(&tutorial);

    expected =
TUTORIAL_EMPTY_INFORMATION_CODE
STEP_WITH_ID_THE_ID_AND_EMPTY_TEXT_START_CODE
"function theIdStepSetup(step) {\n"
"    //Error: Next step id not set!\n"
"}\n"
CONNECT_STEP_SETUP
STEP_WITH_ID_THE_ID_AND_EMPTY_TEXT_END_CODE;

    QCOMPARE(exportedTutorial, expected);
}

void JavascriptExporterTest::testReactionOptionCustomCode() {
    Tutorial tutorial;
    Step* step = new Step();
    step->setId("The id");
    tutorial.addStep(step);

    Reaction* reaction = new Reaction();
    reaction->setTriggerType(Reaction::OptionSelected);
    reaction->setOptionName("The option name");
    reaction->setResponseType(Reaction::CustomCode);
    reaction->setCustomCode("The custom\ncode");
    step->addReaction(reaction);

    JavascriptExporter exporter;
    QString exportedTutorial = exporter.exportTutorial(&tutorial);

    QString expected =
TUTORIAL_EMPTY_INFORMATION_CODE
STEP_WITH_ID_THE_ID_AND_EMPTY_TEXT_START_CODE
"function theIdStepSetup(step) {\n"
"    theOptionNameOption = ktutorial.newOption(\
t.i18nc(\"@action Tutorial option\", \"The option name\"));\n"
"    step.addOption(theOptionNameOption, self, \
\"theIdStepTheOptionNameOptionSelected()\");\n"
"}\n"
CONNECT_STEP_SETUP
"function theIdStepTheOptionNameOptionSelected() {\n"
"    The custom\n"
"    code\n"
"}\n"
"\n"
STEP_WITH_ID_THE_ID_AND_EMPTY_TEXT_END_CODE;

    QCOMPARE(exportedTutorial, expected);
}

void JavascriptExporterTest::testReactionOptionCustomCodeWithEscapeSequences() {
    Tutorial tutorial;
    Step* step = new Step();
    step->setId("The id");
    tutorial.addStep(step);

    Reaction* reaction = new Reaction();
    reaction->setTriggerType(Reaction::OptionSelected);
    reaction->setOptionName("The \"option\" name");
    reaction->setResponseType(Reaction::CustomCode);
    reaction->setCustomCode("The custom\ncode");
    step->addReaction(reaction);

    JavascriptExporter exporter;
    QString exportedTutorial = exporter.exportTutorial(&tutorial);

    QString expected =
TUTORIAL_EMPTY_INFORMATION_CODE
STEP_WITH_ID_THE_ID_AND_EMPTY_TEXT_START_CODE
"function theIdStepSetup(step) {\n"
"    theOptionNameOption = ktutorial.newOption(\
t.i18nc(\"@action Tutorial option\", \"The \\\"option\\\" name\"));\n"
"    step.addOption(theOptionNameOption, self, \
\"theIdStepTheOptionNameOptionSelected()\");\n"
"}\n"
CONNECT_STEP_SETUP
"function theIdStepTheOptionNameOptionSelected() {\n"
"    The custom\n"
"    code\n"
"}\n"
"\n"
STEP_WITH_ID_THE_ID_AND_EMPTY_TEXT_END_CODE;

    QCOMPARE(exportedTutorial, expected);
}

void JavascriptExporterTest::
                testReactionOptionCustomCodeWithoutOptionNameOrCustomCode() {
    Tutorial tutorial;
    Step* step = new Step();
    step->setId("The id");
    tutorial.addStep(step);

    Reaction* reaction = new Reaction();
    reaction->setTriggerType(Reaction::OptionSelected);
    reaction->setResponseType(Reaction::CustomCode);
    step->addReaction(reaction);

    JavascriptExporter exporter;
    QString exportedTutorial = exporter.exportTutorial(&tutorial);

    QString expected =
TUTORIAL_EMPTY_INFORMATION_CODE
STEP_WITH_ID_THE_ID_AND_EMPTY_TEXT_START_CODE
"function theIdStepSetup(step) {\n"
"    //Error: Option without name!\n"
"}\n"
CONNECT_STEP_SETUP
STEP_WITH_ID_THE_ID_AND_EMPTY_TEXT_END_CODE;

    QCOMPARE(exportedTutorial, expected);

    reaction->setOptionName("The option name");

    exportedTutorial = exporter.exportTutorial(&tutorial);

    expected =
TUTORIAL_EMPTY_INFORMATION_CODE
STEP_WITH_ID_THE_ID_AND_EMPTY_TEXT_START_CODE
"function theIdStepSetup(step) {\n"
"    theOptionNameOption = ktutorial.newOption(\
t.i18nc(\"@action Tutorial option\", \"The option name\"));\n"
"    step.addOption(theOptionNameOption, self, \
\"theIdStepTheOptionNameOptionSelected()\");\n"
"}\n"
CONNECT_STEP_SETUP
"function theIdStepTheOptionNameOptionSelected() {\n"
"    //Error: No code set!\n"
"}\n"
"\n"
STEP_WITH_ID_THE_ID_AND_EMPTY_TEXT_END_CODE;

    QCOMPARE(exportedTutorial, expected);
}

void JavascriptExporterTest::testReactionConditionNextStep() {
    Tutorial tutorial;
    Step* step = new Step();
    step->setId("The id");
    tutorial.addStep(step);

    WaitForSignal* waitForSignal = new WaitForSignal();
    waitForSignal->setEmitterName("The emitter name");
    waitForSignal->setSignalName("theSignalName(Argument1Type, Argument2Type)");

    Reaction* reaction = new Reaction();
    reaction->setTriggerType(Reaction::ConditionMet);
    reaction->setWaitFor(waitForSignal);
    reaction->setResponseType(Reaction::NextStep);
    reaction->setNextStepId("Another step");
    step->addReaction(reaction);

    JavascriptExporter exporter;
    QString exportedTutorial = exporter.exportTutorial(&tutorial);

    QString expected =
TUTORIAL_EMPTY_INFORMATION_CODE
STEP_WITH_ID_THE_ID_AND_EMPTY_TEXT_START_CODE
"function theIdStepSetup(step) {\n"
"    waitForTheSignalNameByTheEmitterName = \
ktutorial.newWaitFor(\"WaitForSignal\");\n"
"    waitForTheSignalNameByTheEmitterName.setSignal(\
ktutorial.findObject(\"The emitter name\"), \
\"theSignalName(Argument1Type, Argument2Type)\");\n"
"    step.addWaitFor(waitForTheSignalNameByTheEmitterName, \"Another step\");\n"
"}\n"
CONNECT_STEP_SETUP
STEP_WITH_ID_THE_ID_AND_EMPTY_TEXT_END_CODE;

    QCOMPARE(exportedTutorial, expected);
}

void JavascriptExporterTest::testReactionConditionNextStepWithAncestorNames() {
    Tutorial tutorial;
    Step* step = new Step();
    step->setId("The id");
    tutorial.addStep(step);

    WaitForSignal* waitForSignal = new WaitForSignal();
    waitForSignal->setEmitterName("Grandparent/parent/the emitter name");
    waitForSignal->setSignalName("theSignalName(Argument1Type, Argument2Type)");

    Reaction* reaction = new Reaction();
    reaction->setTriggerType(Reaction::ConditionMet);
    reaction->setWaitFor(waitForSignal);
    reaction->setResponseType(Reaction::NextStep);
    reaction->setNextStepId("Another step");
    step->addReaction(reaction);

    JavascriptExporter exporter;
    QString exportedTutorial = exporter.exportTutorial(&tutorial);

    QString expected =
TUTORIAL_EMPTY_INFORMATION_CODE
STEP_WITH_ID_THE_ID_AND_EMPTY_TEXT_START_CODE
"function theIdStepSetup(step) {\n"
"    waitForTheSignalNameByGrandparentParentTheEmitterName = \
ktutorial.newWaitFor(\"WaitForSignal\");\n"
"    waitForTheSignalNameByGrandparentParentTheEmitterName.setSignal(\
ktutorial.findObject(\"Grandparent/parent/the emitter name\"), \
\"theSignalName(Argument1Type, Argument2Type)\");\n"
"    step.addWaitFor(waitForTheSignalNameByGrandparentParentTheEmitterName, \
\"Another step\");\n"
"}\n"
CONNECT_STEP_SETUP
STEP_WITH_ID_THE_ID_AND_EMPTY_TEXT_END_CODE;

    QCOMPARE(exportedTutorial, expected);
}

void JavascriptExporterTest::
                            testReactionConditionNextStepWithEscapeSequences() {
    Tutorial tutorial;
    Step* step = new Step();
    step->setId("The id");
    tutorial.addStep(step);

    WaitForSignal* waitForSignal = new WaitForSignal();
    waitForSignal->setEmitterName("The \"emitter\" name");
    waitForSignal->setSignalName("\"theSignalName\"(Argument1Type, "
                                                   "Argument2Type)");

    Reaction* reaction = new Reaction();
    reaction->setTriggerType(Reaction::ConditionMet);
    reaction->setWaitFor(waitForSignal);
    reaction->setResponseType(Reaction::NextStep);
    reaction->setNextStepId("Another \"step\"");
    step->addReaction(reaction);

    JavascriptExporter exporter;
    QString exportedTutorial = exporter.exportTutorial(&tutorial);

    QString expected =
TUTORIAL_EMPTY_INFORMATION_CODE
STEP_WITH_ID_THE_ID_AND_EMPTY_TEXT_START_CODE
"function theIdStepSetup(step) {\n"
"    waitForTheSignalNameByTheEmitterName = \
ktutorial.newWaitFor(\"WaitForSignal\");\n"
"    waitForTheSignalNameByTheEmitterName.setSignal(\
ktutorial.findObject(\"The \\\"emitter\\\" name\"), \
\"\\\"theSignalName\\\"(Argument1Type, Argument2Type)\");\n"
"    step.addWaitFor(waitForTheSignalNameByTheEmitterName, \
\"Another \\\"step\\\"\");\n"
"}\n"
CONNECT_STEP_SETUP
STEP_WITH_ID_THE_ID_AND_EMPTY_TEXT_END_CODE;

    QCOMPARE(exportedTutorial, expected);
}

void JavascriptExporterTest::
                    testReactionConditionNextStepWithoutConditionOrStepId() {
    Tutorial tutorial;
    Step* step = new Step();
    step->setId("The id");
    tutorial.addStep(step);

    Reaction* reaction = new Reaction();
    reaction->setTriggerType(Reaction::ConditionMet);
    reaction->setResponseType(Reaction::NextStep);
    step->addReaction(reaction);

    JavascriptExporter exporter;
    QString exportedTutorial = exporter.exportTutorial(&tutorial);

    QString expected =
TUTORIAL_EMPTY_INFORMATION_CODE
STEP_WITH_ID_THE_ID_AND_EMPTY_TEXT_START_CODE
"function theIdStepSetup(step) {\n"
"    //Error: WaitFor not set!\n"
"}\n"
CONNECT_STEP_SETUP
STEP_WITH_ID_THE_ID_AND_EMPTY_TEXT_END_CODE;

    QCOMPARE(exportedTutorial, expected);

    WaitForSignal* waitForSignal = new WaitForSignal();
    waitForSignal->setEmitterName("The emitter name");
    waitForSignal->setSignalName("theSignalName(Argument1Type, Argument2Type)");
    reaction->setWaitFor(waitForSignal);

    exportedTutorial = exporter.exportTutorial(&tutorial);

    expected =
TUTORIAL_EMPTY_INFORMATION_CODE
STEP_WITH_ID_THE_ID_AND_EMPTY_TEXT_START_CODE
"function theIdStepSetup(step) {\n"
"    //Error: Next step id not set!\n"
"}\n"
CONNECT_STEP_SETUP
STEP_WITH_ID_THE_ID_AND_EMPTY_TEXT_END_CODE;

    QCOMPARE(exportedTutorial, expected);
}

void JavascriptExporterTest::testReactionConditionCustomCode() {
    Tutorial tutorial;
    Step* step = new Step();
    step->setId("The id");
    tutorial.addStep(step);

    WaitForSignal* waitForSignal = new WaitForSignal();
    waitForSignal->setEmitterName("The emitter name");
    waitForSignal->setSignalName("theSignalName(Argument1Type, Argument2Type)");

    Reaction* reaction = new Reaction();
    reaction->setTriggerType(Reaction::ConditionMet);
    reaction->setWaitFor(waitForSignal);
    reaction->setResponseType(Reaction::CustomCode);
    reaction->setCustomCode("The custom\ncode");
    step->addReaction(reaction);

    JavascriptExporter exporter;
    QString exportedTutorial = exporter.exportTutorial(&tutorial);

    QString expected =
TUTORIAL_EMPTY_INFORMATION_CODE
STEP_WITH_ID_THE_ID_AND_EMPTY_TEXT_START_CODE
"function theIdStepSetup(step) {\n"
"    waitForTheSignalNameByTheEmitterName = \
ktutorial.newWaitFor(\"WaitForSignal\");\n"
"    waitForTheSignalNameByTheEmitterName.setSignal(\
ktutorial.findObject(\"The emitter name\"), \
\"theSignalName(Argument1Type, Argument2Type)\");\n"
"    step.addWaitFor(waitForTheSignalNameByTheEmitterName, self, \
\"theIdStepWaitForTheSignalNameByTheEmitterNameConditionMet()\");\n"
"}\n"
CONNECT_STEP_SETUP
"function theIdStepWaitForTheSignalNameByTheEmitterNameConditionMet() {\n"
"    The custom\n"
"    code\n"
"}\n"
"\n"
STEP_WITH_ID_THE_ID_AND_EMPTY_TEXT_END_CODE;

    QCOMPARE(exportedTutorial, expected);
}

void JavascriptExporterTest::
                            testReactionConditionCustomCodeWithAncestorNames() {
    Tutorial tutorial;
    Step* step = new Step();
    step->setId("The id");
    tutorial.addStep(step);

    WaitForSignal* waitForSignal = new WaitForSignal();
    waitForSignal->setEmitterName("Grandparent/parent/the emitter name");
    waitForSignal->setSignalName("theSignalName(Argument1Type, Argument2Type)");

    Reaction* reaction = new Reaction();
    reaction->setTriggerType(Reaction::ConditionMet);
    reaction->setWaitFor(waitForSignal);
    reaction->setResponseType(Reaction::CustomCode);
    reaction->setCustomCode("The custom\ncode");
    step->addReaction(reaction);

    JavascriptExporter exporter;
    QString exportedTutorial = exporter.exportTutorial(&tutorial);

    QString expected =
TUTORIAL_EMPTY_INFORMATION_CODE
STEP_WITH_ID_THE_ID_AND_EMPTY_TEXT_START_CODE
"function theIdStepSetup(step) {\n"
"    waitForTheSignalNameByGrandparentParentTheEmitterName = \
ktutorial.newWaitFor(\"WaitForSignal\");\n"
"    waitForTheSignalNameByGrandparentParentTheEmitterName.setSignal(\
ktutorial.findObject(\"Grandparent/parent/the emitter name\"), \
\"theSignalName(Argument1Type, Argument2Type)\");\n"
"    step.addWaitFor(waitForTheSignalNameByGrandparentParentTheEmitterName, \
self, \"theIdStepWaitForTheSignalNameByGrandparentParentTheEmitterNameConditionMet()\");\n"
"}\n"
CONNECT_STEP_SETUP
"function theIdStepWaitForTheSignalNameByGrandparentParentTheEmitterNameConditionMet() {\n"
"    The custom\n"
"    code\n"
"}\n"
"\n"
STEP_WITH_ID_THE_ID_AND_EMPTY_TEXT_END_CODE;

    QCOMPARE(exportedTutorial, expected);
}

void JavascriptExporterTest::
                        testReactionConditionCustomCodeWithEscapeSequences() {
    Tutorial tutorial;
    Step* step = new Step();
    step->setId("The id");
    tutorial.addStep(step);

    WaitForSignal* waitForSignal = new WaitForSignal();
    waitForSignal->setEmitterName("The \"emitter\" name");
    waitForSignal->setSignalName("\"theSignalName\"(Argument1Type, "
                                                   "Argument2Type)");

    Reaction* reaction = new Reaction();
    reaction->setTriggerType(Reaction::ConditionMet);
    reaction->setWaitFor(waitForSignal);
    reaction->setResponseType(Reaction::CustomCode);
    reaction->setCustomCode("The custom\ncode");
    step->addReaction(reaction);

    JavascriptExporter exporter;
    QString exportedTutorial = exporter.exportTutorial(&tutorial);

    QString expected =
TUTORIAL_EMPTY_INFORMATION_CODE
STEP_WITH_ID_THE_ID_AND_EMPTY_TEXT_START_CODE
"function theIdStepSetup(step) {\n"
"    waitForTheSignalNameByTheEmitterName = \
ktutorial.newWaitFor(\"WaitForSignal\");\n"
"    waitForTheSignalNameByTheEmitterName.setSignal(\
ktutorial.findObject(\"The \\\"emitter\\\" name\"), \
\"\\\"theSignalName\\\"(Argument1Type, Argument2Type)\");\n"
"    step.addWaitFor(waitForTheSignalNameByTheEmitterName, self, \
\"theIdStepWaitForTheSignalNameByTheEmitterNameConditionMet()\");\n"
"}\n"
CONNECT_STEP_SETUP
"function theIdStepWaitForTheSignalNameByTheEmitterNameConditionMet() {\n"
"    The custom\n"
"    code\n"
"}\n"
"\n"
STEP_WITH_ID_THE_ID_AND_EMPTY_TEXT_END_CODE;

    QCOMPARE(exportedTutorial, expected);
}

void JavascriptExporterTest::
                testReactionConditionCustomCodeWithoutConditionOrCustomCode() {
    Tutorial tutorial;
    Step* step = new Step();
    step->setId("The id");
    tutorial.addStep(step);

    Reaction* reaction = new Reaction();
    reaction->setTriggerType(Reaction::ConditionMet);
    reaction->setResponseType(Reaction::CustomCode);
    step->addReaction(reaction);

    JavascriptExporter exporter;
    QString exportedTutorial = exporter.exportTutorial(&tutorial);

    QString expected =
TUTORIAL_EMPTY_INFORMATION_CODE
STEP_WITH_ID_THE_ID_AND_EMPTY_TEXT_START_CODE
"function theIdStepSetup(step) {\n"
"    //Error: WaitFor not set!\n"
"}\n"
CONNECT_STEP_SETUP
STEP_WITH_ID_THE_ID_AND_EMPTY_TEXT_END_CODE;

    QCOMPARE(exportedTutorial, expected);

    WaitForSignal* waitForSignal = new WaitForSignal();
    waitForSignal->setEmitterName("The emitter name");
    waitForSignal->setSignalName("theSignalName(Argument1Type, Argument2Type)");
    reaction->setWaitFor(waitForSignal);

    exportedTutorial = exporter.exportTutorial(&tutorial);

    expected =
TUTORIAL_EMPTY_INFORMATION_CODE
STEP_WITH_ID_THE_ID_AND_EMPTY_TEXT_START_CODE
"function theIdStepSetup(step) {\n"
"    waitForTheSignalNameByTheEmitterName = \
ktutorial.newWaitFor(\"WaitForSignal\");\n"
"    waitForTheSignalNameByTheEmitterName.setSignal(\
ktutorial.findObject(\"The emitter name\"), \
\"theSignalName(Argument1Type, Argument2Type)\");\n"
"    step.addWaitFor(waitForTheSignalNameByTheEmitterName, self, \
\"theIdStepWaitForTheSignalNameByTheEmitterNameConditionMet()\");\n"
"}\n"
CONNECT_STEP_SETUP
"function theIdStepWaitForTheSignalNameByTheEmitterNameConditionMet() {\n"
"    //Error: No code set!\n"
"}\n"
"\n"
STEP_WITH_ID_THE_ID_AND_EMPTY_TEXT_END_CODE;

    QCOMPARE(exportedTutorial, expected);
}

void JavascriptExporterTest::testWaitForEvent() {
    Tutorial tutorial;
    Step* step = new Step();
    step->setId("The id");
    tutorial.addStep(step);

    WaitForEvent* waitForEvent = new WaitForEvent();
    waitForEvent->setReceiverName("The receiver name");
    waitForEvent->setEventName("TheEventName");

    Reaction* reaction = new Reaction();
    reaction->setTriggerType(Reaction::ConditionMet);
    reaction->setWaitFor(waitForEvent);
    reaction->setResponseType(Reaction::NextStep);
    reaction->setNextStepId("Another step");
    step->addReaction(reaction);

    JavascriptExporter exporter;
    QString exportedTutorial = exporter.exportTutorial(&tutorial);

    QString expected =
TUTORIAL_EMPTY_INFORMATION_CODE
STEP_WITH_ID_THE_ID_AND_EMPTY_TEXT_START_CODE
"function theIdStepSetup(step) {\n"
"    waitForTheEventNameInTheReceiverName = \
ktutorial.newWaitFor(\"WaitForEvent\");\n"
"    waitForTheEventNameInTheReceiverName.setEvent(\
ktutorial.findObject(\"The receiver name\"), \
\"TheEventName\");\n"
"    step.addWaitFor(waitForTheEventNameInTheReceiverName, \"Another step\");\n"
"}\n"
CONNECT_STEP_SETUP
STEP_WITH_ID_THE_ID_AND_EMPTY_TEXT_END_CODE;

    QCOMPARE(exportedTutorial, expected);
}

void JavascriptExporterTest::testWaitForEventWithAncestorNames() {
    Tutorial tutorial;
    Step* step = new Step();
    step->setId("The id");
    tutorial.addStep(step);

    WaitForEvent* waitForEvent = new WaitForEvent();
    waitForEvent->setReceiverName("Grandparent/parent/the receiver name");
    waitForEvent->setEventName("TheEventName");

    Reaction* reaction = new Reaction();
    reaction->setTriggerType(Reaction::ConditionMet);
    reaction->setWaitFor(waitForEvent);
    reaction->setResponseType(Reaction::NextStep);
    reaction->setNextStepId("Another step");
    step->addReaction(reaction);

    JavascriptExporter exporter;
    QString exportedTutorial = exporter.exportTutorial(&tutorial);

    QString expected =
TUTORIAL_EMPTY_INFORMATION_CODE
STEP_WITH_ID_THE_ID_AND_EMPTY_TEXT_START_CODE
"function theIdStepSetup(step) {\n"
"    waitForTheEventNameInGrandparentParentTheReceiverName = \
ktutorial.newWaitFor(\"WaitForEvent\");\n"
"    waitForTheEventNameInGrandparentParentTheReceiverName.setEvent(\
ktutorial.findObject(\"Grandparent/parent/the receiver name\"), \
\"TheEventName\");\n"
"    step.addWaitFor(waitForTheEventNameInGrandparentParentTheReceiverName, \
\"Another step\");\n"
"}\n"
CONNECT_STEP_SETUP
STEP_WITH_ID_THE_ID_AND_EMPTY_TEXT_END_CODE;

    QCOMPARE(exportedTutorial, expected);
}

void JavascriptExporterTest::testWaitForEventWithEscapeSequences() {
    Tutorial tutorial;
    Step* step = new Step();
    step->setId("The id");
    tutorial.addStep(step);

    WaitForEvent* waitForEvent = new WaitForEvent();
    waitForEvent->setReceiverName("The \"receiver\" name");
    waitForEvent->setEventName("\"TheEventName\"");

    Reaction* reaction = new Reaction();
    reaction->setTriggerType(Reaction::ConditionMet);
    reaction->setWaitFor(waitForEvent);
    reaction->setResponseType(Reaction::NextStep);
    reaction->setNextStepId("Another step");
    step->addReaction(reaction);

    JavascriptExporter exporter;
    QString exportedTutorial = exporter.exportTutorial(&tutorial);

    QString expected =
TUTORIAL_EMPTY_INFORMATION_CODE
STEP_WITH_ID_THE_ID_AND_EMPTY_TEXT_START_CODE
"function theIdStepSetup(step) {\n"
"    waitForTheEventNameInTheReceiverName = \
ktutorial.newWaitFor(\"WaitForEvent\");\n"
"    waitForTheEventNameInTheReceiverName.setEvent(\
ktutorial.findObject(\"The \\\"receiver\\\" name\"), \
\"\\\"TheEventName\\\"\");\n"
"    step.addWaitFor(waitForTheEventNameInTheReceiverName, \"Another step\");\n"
"}\n"
CONNECT_STEP_SETUP
STEP_WITH_ID_THE_ID_AND_EMPTY_TEXT_END_CODE;

    QCOMPARE(exportedTutorial, expected);
}

void JavascriptExporterTest::testWaitForEventWithoutReceiverNameOrEventName() {
    Tutorial tutorial;
    Step* step = new Step();
    step->setId("The id");
    tutorial.addStep(step);

    WaitForEvent* waitForEvent = new WaitForEvent();

    Reaction* reaction = new Reaction();
    reaction->setTriggerType(Reaction::ConditionMet);
    reaction->setWaitFor(waitForEvent);
    reaction->setResponseType(Reaction::NextStep);
    reaction->setNextStepId("Another step");
    step->addReaction(reaction);

    JavascriptExporter exporter;
    QString exportedTutorial = exporter.exportTutorial(&tutorial);

    QString expected =
TUTORIAL_EMPTY_INFORMATION_CODE
STEP_WITH_ID_THE_ID_AND_EMPTY_TEXT_START_CODE
"function theIdStepSetup(step) {\n"
"    //Error: WaitForEvent without receiver name!\n"
"}\n"
CONNECT_STEP_SETUP
STEP_WITH_ID_THE_ID_AND_EMPTY_TEXT_END_CODE;

    QCOMPARE(exportedTutorial, expected);

    waitForEvent->setReceiverName("The receiver name");

    exportedTutorial = exporter.exportTutorial(&tutorial);

    expected =
TUTORIAL_EMPTY_INFORMATION_CODE
STEP_WITH_ID_THE_ID_AND_EMPTY_TEXT_START_CODE
"function theIdStepSetup(step) {\n"
"    //Error: WaitForEvent without event name!\n"
"}\n"
CONNECT_STEP_SETUP
STEP_WITH_ID_THE_ID_AND_EMPTY_TEXT_END_CODE;

    QCOMPARE(exportedTutorial, expected);
}

void JavascriptExporterTest::testWaitForProperty() {
    Tutorial tutorial;
    Step* step = new Step();
    step->setId("The id");
    tutorial.addStep(step);

    WaitForProperty* waitForProperty = new WaitForProperty();
    waitForProperty->setObjectName("The object name");
    waitForProperty->setPropertyName("thePropertyName");
    waitForProperty->setValue("TheValue");

    Reaction* reaction = new Reaction();
    reaction->setTriggerType(Reaction::ConditionMet);
    reaction->setWaitFor(waitForProperty);
    reaction->setResponseType(Reaction::NextStep);
    reaction->setNextStepId("Another step");
    step->addReaction(reaction);

    JavascriptExporter exporter;
    QString exportedTutorial = exporter.exportTutorial(&tutorial);

    QString expected =
TUTORIAL_EMPTY_INFORMATION_CODE
STEP_WITH_ID_THE_ID_AND_EMPTY_TEXT_START_CODE
"function theIdStepSetup(step) {\n"
"    waitForThePropertyNameInTheObjectName = \
ktutorial.newWaitFor(\"WaitForProperty\");\n"
"    waitForThePropertyNameInTheObjectName.setProperty(\
ktutorial.findObject(\"The object name\"), \"thePropertyName\", TheValue);\n"
"    step.addWaitFor(waitForThePropertyNameInTheObjectName, \
\"Another step\");\n"
"}\n"
CONNECT_STEP_SETUP
STEP_WITH_ID_THE_ID_AND_EMPTY_TEXT_END_CODE;

    QCOMPARE(exportedTutorial, expected);
}

void JavascriptExporterTest::testWaitForPropertyWithAncestorNames() {
    Tutorial tutorial;
    Step* step = new Step();
    step->setId("The id");
    tutorial.addStep(step);

    WaitForProperty* waitForProperty = new WaitForProperty();
    waitForProperty->setObjectName("Grandparent/parent/the object name");
    waitForProperty->setPropertyName("thePropertyName");
    waitForProperty->setValue("TheValue");

    Reaction* reaction = new Reaction();
    reaction->setTriggerType(Reaction::ConditionMet);
    reaction->setWaitFor(waitForProperty);
    reaction->setResponseType(Reaction::NextStep);
    reaction->setNextStepId("Another step");
    step->addReaction(reaction);

    JavascriptExporter exporter;
    QString exportedTutorial = exporter.exportTutorial(&tutorial);

    QString expected =
TUTORIAL_EMPTY_INFORMATION_CODE
STEP_WITH_ID_THE_ID_AND_EMPTY_TEXT_START_CODE
"function theIdStepSetup(step) {\n"
"    waitForThePropertyNameInGrandparentParentTheObjectName = \
ktutorial.newWaitFor(\"WaitForProperty\");\n"
"    waitForThePropertyNameInGrandparentParentTheObjectName.setProperty(\
ktutorial.findObject(\"Grandparent/parent/the object name\"), \
\"thePropertyName\", TheValue);\n"
"    step.addWaitFor(waitForThePropertyNameInGrandparentParentTheObjectName, \
\"Another step\");\n"
"}\n"
CONNECT_STEP_SETUP
STEP_WITH_ID_THE_ID_AND_EMPTY_TEXT_END_CODE;

    QCOMPARE(exportedTutorial, expected);
}

void JavascriptExporterTest::testWaitForPropertyWithEscapeSequences() {
    Tutorial tutorial;
    Step* step = new Step();
    step->setId("The id");
    tutorial.addStep(step);

    WaitForProperty* waitForProperty = new WaitForProperty();
    waitForProperty->setObjectName("The \"object\" name");
    waitForProperty->setPropertyName("\"thePropertyName\"");
    waitForProperty->setValue("\"The \\\"value\\\"\"");

    Reaction* reaction = new Reaction();
    reaction->setTriggerType(Reaction::ConditionMet);
    reaction->setWaitFor(waitForProperty);
    reaction->setResponseType(Reaction::NextStep);
    reaction->setNextStepId("Another step");
    step->addReaction(reaction);

    JavascriptExporter exporter;
    QString exportedTutorial = exporter.exportTutorial(&tutorial);

    QString expected =
TUTORIAL_EMPTY_INFORMATION_CODE
STEP_WITH_ID_THE_ID_AND_EMPTY_TEXT_START_CODE
"function theIdStepSetup(step) {\n"
"    waitForThePropertyNameInTheObjectName = \
ktutorial.newWaitFor(\"WaitForProperty\");\n"
"    waitForThePropertyNameInTheObjectName.setProperty(\
ktutorial.findObject(\"The \\\"object\\\" name\"), \
\"\\\"thePropertyName\\\"\", \"The \\\"value\\\"\");\n"
"    step.addWaitFor(waitForThePropertyNameInTheObjectName, \
\"Another step\");\n"
"}\n"
CONNECT_STEP_SETUP
STEP_WITH_ID_THE_ID_AND_EMPTY_TEXT_END_CODE;

    QCOMPARE(exportedTutorial, expected);
}

void JavascriptExporterTest::
                testWaitForPropertyWithoutObjectNameOrPropertyNameOrValue() {
    Tutorial tutorial;
    Step* step = new Step();
    step->setId("The id");
    tutorial.addStep(step);

    WaitForProperty* waitForProperty = new WaitForProperty();

    Reaction* reaction = new Reaction();
    reaction->setTriggerType(Reaction::ConditionMet);
    reaction->setWaitFor(waitForProperty);
    reaction->setResponseType(Reaction::NextStep);
    reaction->setNextStepId("Another step");
    step->addReaction(reaction);

    JavascriptExporter exporter;
    QString exportedTutorial = exporter.exportTutorial(&tutorial);

    QString expected =
TUTORIAL_EMPTY_INFORMATION_CODE
STEP_WITH_ID_THE_ID_AND_EMPTY_TEXT_START_CODE
"function theIdStepSetup(step) {\n"
"    //Error: WaitForProperty without object name!\n"
"}\n"
CONNECT_STEP_SETUP
STEP_WITH_ID_THE_ID_AND_EMPTY_TEXT_END_CODE;

    QCOMPARE(exportedTutorial, expected);

    waitForProperty->setObjectName("The object name");

    exportedTutorial = exporter.exportTutorial(&tutorial);

    expected =
TUTORIAL_EMPTY_INFORMATION_CODE
STEP_WITH_ID_THE_ID_AND_EMPTY_TEXT_START_CODE
"function theIdStepSetup(step) {\n"
"    //Error: WaitForProperty without property name!\n"
"}\n"
CONNECT_STEP_SETUP
STEP_WITH_ID_THE_ID_AND_EMPTY_TEXT_END_CODE;

    QCOMPARE(exportedTutorial, expected);

    waitForProperty->setPropertyName("The property name");

    exportedTutorial = exporter.exportTutorial(&tutorial);

    expected =
TUTORIAL_EMPTY_INFORMATION_CODE
STEP_WITH_ID_THE_ID_AND_EMPTY_TEXT_START_CODE
"function theIdStepSetup(step) {\n"
"    //Error: WaitForProperty without value!\n"
"}\n"
CONNECT_STEP_SETUP
STEP_WITH_ID_THE_ID_AND_EMPTY_TEXT_END_CODE;

    QCOMPARE(exportedTutorial, expected);
}

void JavascriptExporterTest::testWaitForSignal() {
    Tutorial tutorial;
    Step* step = new Step();
    step->setId("The id");
    tutorial.addStep(step);

    WaitForSignal* waitForSignal = new WaitForSignal();
    waitForSignal->setEmitterName("The emitter name");
    waitForSignal->setSignalName("theSignalName(Argument1Type, Argument2Type)");

    Reaction* reaction = new Reaction();
    reaction->setTriggerType(Reaction::ConditionMet);
    reaction->setWaitFor(waitForSignal);
    reaction->setResponseType(Reaction::NextStep);
    reaction->setNextStepId("Another step");
    step->addReaction(reaction);

    JavascriptExporter exporter;
    QString exportedTutorial = exporter.exportTutorial(&tutorial);

    QString expected =
TUTORIAL_EMPTY_INFORMATION_CODE
STEP_WITH_ID_THE_ID_AND_EMPTY_TEXT_START_CODE
"function theIdStepSetup(step) {\n"
"    waitForTheSignalNameByTheEmitterName = \
ktutorial.newWaitFor(\"WaitForSignal\");\n"
"    waitForTheSignalNameByTheEmitterName.setSignal(\
ktutorial.findObject(\"The emitter name\"), \
\"theSignalName(Argument1Type, Argument2Type)\");\n"
"    step.addWaitFor(waitForTheSignalNameByTheEmitterName, \"Another step\");\n"
"}\n"
CONNECT_STEP_SETUP
STEP_WITH_ID_THE_ID_AND_EMPTY_TEXT_END_CODE;

    QCOMPARE(exportedTutorial, expected);
}

void JavascriptExporterTest::testWaitForSignalWithAncestorNames() {
    Tutorial tutorial;
    Step* step = new Step();
    step->setId("The id");
    tutorial.addStep(step);

    WaitForSignal* waitForSignal = new WaitForSignal();
    waitForSignal->setEmitterName("Grandparent/parent/the emitter name");
    waitForSignal->setSignalName("theSignalName(Argument1Type, Argument2Type)");

    Reaction* reaction = new Reaction();
    reaction->setTriggerType(Reaction::ConditionMet);
    reaction->setWaitFor(waitForSignal);
    reaction->setResponseType(Reaction::NextStep);
    reaction->setNextStepId("Another step");
    step->addReaction(reaction);

    JavascriptExporter exporter;
    QString exportedTutorial = exporter.exportTutorial(&tutorial);

    QString expected =
TUTORIAL_EMPTY_INFORMATION_CODE
STEP_WITH_ID_THE_ID_AND_EMPTY_TEXT_START_CODE
"function theIdStepSetup(step) {\n"
"    waitForTheSignalNameByGrandparentParentTheEmitterName = \
ktutorial.newWaitFor(\"WaitForSignal\");\n"
"    waitForTheSignalNameByGrandparentParentTheEmitterName.setSignal(\
ktutorial.findObject(\"Grandparent/parent/the emitter name\"), \
\"theSignalName(Argument1Type, Argument2Type)\");\n"
"    step.addWaitFor(waitForTheSignalNameByGrandparentParentTheEmitterName, \
\"Another step\");\n"
"}\n"
CONNECT_STEP_SETUP
STEP_WITH_ID_THE_ID_AND_EMPTY_TEXT_END_CODE;

    QCOMPARE(exportedTutorial, expected);
}

void JavascriptExporterTest::testWaitForSignalWithEscapeSequences() {
    Tutorial tutorial;
    Step* step = new Step();
    step->setId("The id");
    tutorial.addStep(step);

    WaitForSignal* waitForSignal = new WaitForSignal();
    waitForSignal->setEmitterName("The \"emitter\" name");
    waitForSignal->setSignalName("\"theSignalName\"(Argument1Type, "
                                                   "Argument2Type)");

    Reaction* reaction = new Reaction();
    reaction->setTriggerType(Reaction::ConditionMet);
    reaction->setWaitFor(waitForSignal);
    reaction->setResponseType(Reaction::NextStep);
    reaction->setNextStepId("Another step");
    step->addReaction(reaction);

    JavascriptExporter exporter;
    QString exportedTutorial = exporter.exportTutorial(&tutorial);

    QString expected =
TUTORIAL_EMPTY_INFORMATION_CODE
STEP_WITH_ID_THE_ID_AND_EMPTY_TEXT_START_CODE
"function theIdStepSetup(step) {\n"
"    waitForTheSignalNameByTheEmitterName = \
ktutorial.newWaitFor(\"WaitForSignal\");\n"
"    waitForTheSignalNameByTheEmitterName.setSignal(\
ktutorial.findObject(\"The \\\"emitter\\\" name\"), \
\"\\\"theSignalName\\\"(Argument1Type, Argument2Type)\");\n"
"    step.addWaitFor(waitForTheSignalNameByTheEmitterName, \"Another step\");\n"
"}\n"
CONNECT_STEP_SETUP
STEP_WITH_ID_THE_ID_AND_EMPTY_TEXT_END_CODE;

    QCOMPARE(exportedTutorial, expected);
}

void JavascriptExporterTest::testWaitForSignalWithoutEmitterNameOrSignalName() {
    Tutorial tutorial;
    Step* step = new Step();
    step->setId("The id");
    tutorial.addStep(step);

    WaitForSignal* waitForSignal = new WaitForSignal();

    Reaction* reaction = new Reaction();
    reaction->setTriggerType(Reaction::ConditionMet);
    reaction->setWaitFor(waitForSignal);
    reaction->setResponseType(Reaction::NextStep);
    reaction->setNextStepId("Another step");
    step->addReaction(reaction);

    JavascriptExporter exporter;
    QString exportedTutorial = exporter.exportTutorial(&tutorial);

    QString expected =
TUTORIAL_EMPTY_INFORMATION_CODE
STEP_WITH_ID_THE_ID_AND_EMPTY_TEXT_START_CODE
"function theIdStepSetup(step) {\n"
"    //Error: WaitForSignal without emitter name!\n"
"}\n"
CONNECT_STEP_SETUP
STEP_WITH_ID_THE_ID_AND_EMPTY_TEXT_END_CODE;

    QCOMPARE(exportedTutorial, expected);

    waitForSignal->setEmitterName("The emitter name");

    exportedTutorial = exporter.exportTutorial(&tutorial);

    expected =
TUTORIAL_EMPTY_INFORMATION_CODE
STEP_WITH_ID_THE_ID_AND_EMPTY_TEXT_START_CODE
"function theIdStepSetup(step) {\n"
"    //Error: WaitForSignal without signal name!\n"
"}\n"
CONNECT_STEP_SETUP
STEP_WITH_ID_THE_ID_AND_EMPTY_TEXT_END_CODE;

    QCOMPARE(exportedTutorial, expected);
}

void JavascriptExporterTest::testWaitForStepActivation() {
    Tutorial tutorial;
    Step* step = new Step();
    step->setId("The id");
    tutorial.addStep(step);

    WaitForStepActivation* waitForStepActivation = new WaitForStepActivation();

    Reaction* reaction = new Reaction();
    reaction->setTriggerType(Reaction::ConditionMet);
    reaction->setWaitFor(waitForStepActivation);
    reaction->setResponseType(Reaction::NextStep);
    reaction->setNextStepId("Another step");
    step->addReaction(reaction);

    JavascriptExporter exporter;
    QString exportedTutorial = exporter.exportTutorial(&tutorial);

    QString expected =
TUTORIAL_EMPTY_INFORMATION_CODE
STEP_WITH_ID_THE_ID_AND_EMPTY_TEXT_START_CODE
"function theIdStepSetup(step) {\n"
"    waitForStepActivation = ktutorial.newWaitFor(\"WaitForStepActivation\");\n"
"    waitForStepActivation.setStep(tutorial, step);\n"
"    step.addWaitFor(waitForStepActivation, \"Another step\");\n"
"}\n"
CONNECT_STEP_SETUP
STEP_WITH_ID_THE_ID_AND_EMPTY_TEXT_END_CODE;

    QCOMPARE(exportedTutorial, expected);
}

void JavascriptExporterTest::testWaitForWindow() {
    Tutorial tutorial;
    Step* step = new Step();
    step->setId("The id");
    tutorial.addStep(step);

    WaitForWindow* waitForWindow = new WaitForWindow();
    waitForWindow->setWindowObjectName("The window object name");

    Reaction* reaction = new Reaction();
    reaction->setTriggerType(Reaction::ConditionMet);
    reaction->setWaitFor(waitForWindow);
    reaction->setResponseType(Reaction::NextStep);
    reaction->setNextStepId("Another step");
    step->addReaction(reaction);

    JavascriptExporter exporter;
    QString exportedTutorial = exporter.exportTutorial(&tutorial);

    QString expected =
TUTORIAL_EMPTY_INFORMATION_CODE
STEP_WITH_ID_THE_ID_AND_EMPTY_TEXT_START_CODE
"function theIdStepSetup(step) {\n"
"    waitForTheWindowObjectNameToBeShown = \
ktutorial.newWaitFor(\"WaitForWindow\");\n"
"    waitForTheWindowObjectNameToBeShown.setWindowObjectName(\
\"The window object name\");\n"
"    step.addWaitFor(waitForTheWindowObjectNameToBeShown, \"Another step\");\n"
"}\n"
CONNECT_STEP_SETUP
STEP_WITH_ID_THE_ID_AND_EMPTY_TEXT_END_CODE;

    QCOMPARE(exportedTutorial, expected);
}

void JavascriptExporterTest::testWaitForWindowWithAncestorNames() {
    Tutorial tutorial;
    Step* step = new Step();
    step->setId("The id");
    tutorial.addStep(step);

    WaitForWindow* waitForWindow = new WaitForWindow();
    waitForWindow->setWindowObjectName("Grandparent/parent/"
                                       "the window object name");

    Reaction* reaction = new Reaction();
    reaction->setTriggerType(Reaction::ConditionMet);
    reaction->setWaitFor(waitForWindow);
    reaction->setResponseType(Reaction::NextStep);
    reaction->setNextStepId("Another step");
    step->addReaction(reaction);

    JavascriptExporter exporter;
    QString exportedTutorial = exporter.exportTutorial(&tutorial);

    QString expected =
TUTORIAL_EMPTY_INFORMATION_CODE
STEP_WITH_ID_THE_ID_AND_EMPTY_TEXT_START_CODE
"function theIdStepSetup(step) {\n"
"    waitForGrandparentParentTheWindowObjectNameToBeShown = \
ktutorial.newWaitFor(\"WaitForWindow\");\n"
"    waitForGrandparentParentTheWindowObjectNameToBeShown.setWindowObjectName(\
\"Grandparent/parent/the window object name\");\n"
"    step.addWaitFor(waitForGrandparentParentTheWindowObjectNameToBeShown, \
\"Another step\");\n"
"}\n"
CONNECT_STEP_SETUP
STEP_WITH_ID_THE_ID_AND_EMPTY_TEXT_END_CODE;

    QCOMPARE(exportedTutorial, expected);
}

void JavascriptExporterTest::testWaitForWindowWithEscapeSequences() {
    Tutorial tutorial;
    Step* step = new Step();
    step->setId("The id");
    tutorial.addStep(step);

    WaitForWindow* waitForWindow = new WaitForWindow();
    waitForWindow->setWindowObjectName("The \"window\" object name");

    Reaction* reaction = new Reaction();
    reaction->setTriggerType(Reaction::ConditionMet);
    reaction->setWaitFor(waitForWindow);
    reaction->setResponseType(Reaction::NextStep);
    reaction->setNextStepId("Another step");
    step->addReaction(reaction);

    JavascriptExporter exporter;
    QString exportedTutorial = exporter.exportTutorial(&tutorial);

    QString expected =
TUTORIAL_EMPTY_INFORMATION_CODE
STEP_WITH_ID_THE_ID_AND_EMPTY_TEXT_START_CODE
"function theIdStepSetup(step) {\n"
"    waitForTheWindowObjectNameToBeShown = \
ktutorial.newWaitFor(\"WaitForWindow\");\n"
"    waitForTheWindowObjectNameToBeShown.setWindowObjectName(\
\"The \\\"window\\\" object name\");\n"
"    step.addWaitFor(waitForTheWindowObjectNameToBeShown, \"Another step\");\n"
"}\n"
CONNECT_STEP_SETUP
STEP_WITH_ID_THE_ID_AND_EMPTY_TEXT_END_CODE;

    QCOMPARE(exportedTutorial, expected);
}

void JavascriptExporterTest::testWaitForWindowWithoutWindowObjectName() {
    Tutorial tutorial;
    Step* step = new Step();
    step->setId("The id");
    tutorial.addStep(step);

    WaitForWindow* waitForWindow = new WaitForWindow();

    Reaction* reaction = new Reaction();
    reaction->setTriggerType(Reaction::ConditionMet);
    reaction->setWaitFor(waitForWindow);
    reaction->setResponseType(Reaction::NextStep);
    reaction->setNextStepId("Another step");
    step->addReaction(reaction);

    JavascriptExporter exporter;
    QString exportedTutorial = exporter.exportTutorial(&tutorial);

    QString expected =
TUTORIAL_EMPTY_INFORMATION_CODE
STEP_WITH_ID_THE_ID_AND_EMPTY_TEXT_START_CODE
"function theIdStepSetup(step) {\n"
"    //Error: WaitForWindow without window object name!\n"
"}\n"
CONNECT_STEP_SETUP
STEP_WITH_ID_THE_ID_AND_EMPTY_TEXT_END_CODE;

    QCOMPARE(exportedTutorial, expected);
}

void JavascriptExporterTest::testWaitForComposed() {
    Tutorial tutorial;
    Step* step = new Step();
    step->setId("The id");
    tutorial.addStep(step);

    WaitForComposed* waitForAndGrandChild = new WaitForComposed();
    waitForAndGrandChild->setCompositionType(WaitForComposed::And);

    WaitForSignal* waitForSignal = new WaitForSignal();
    waitForSignal->setEmitterName("The emitter name");
    waitForSignal->setSignalName("theSignalName(Argument1Type, Argument2Type)");

    WaitForComposed* waitForOrChild = new WaitForComposed();
    waitForOrChild->setCompositionType(WaitForComposed::Or);
    waitForOrChild->addWaitFor(waitForAndGrandChild);
    waitForOrChild->addWaitFor(waitForSignal);

    WaitForComposed* waitForAnd = new WaitForComposed();
    waitForAnd->setCompositionType(WaitForComposed::And);
    waitForAnd->addWaitFor(waitForOrChild);

    Reaction* reaction = new Reaction();
    reaction->setTriggerType(Reaction::ConditionMet);
    reaction->setWaitFor(waitForAnd);
    reaction->setResponseType(Reaction::NextStep);
    reaction->setNextStepId("Another step");
    step->addReaction(reaction);

    JavascriptExporter exporter;
    QString exportedTutorial = exporter.exportTutorial(&tutorial);

    QString expected =
TUTORIAL_EMPTY_INFORMATION_CODE
STEP_WITH_ID_THE_ID_AND_EMPTY_TEXT_START_CODE
"function theIdStepSetup(step) {\n"
"    waitForAnd = ktutorial.newWaitFor(\"WaitForAnd\");\n"
"\n"
"    waitForTheSignalNameByTheEmitterName = \
ktutorial.newWaitFor(\"WaitForSignal\");\n"
"    waitForTheSignalNameByTheEmitterName.setSignal(\
ktutorial.findObject(\"The emitter name\"), \
\"theSignalName(Argument1Type, Argument2Type)\");\n"
"\n"
"    waitForOr = ktutorial.newWaitFor(\"WaitForOr\");\n"
"    waitForOr.add(waitForAnd);\n"
"    waitForOr.add(waitForTheSignalNameByTheEmitterName);\n"
"\n"
"    waitForAnd2 = ktutorial.newWaitFor(\"WaitForAnd\");\n"
"    waitForAnd2.add(waitForOr);\n"
"    step.addWaitFor(waitForAnd2, \"Another step\");\n"
"}\n"
CONNECT_STEP_SETUP
STEP_WITH_ID_THE_ID_AND_EMPTY_TEXT_END_CODE;

    QCOMPARE(exportedTutorial, expected);
}

void JavascriptExporterTest::testWaitForComposedWithInvalidChildWaitFor() {
    Tutorial tutorial;
    Step* step = new Step();
    step->setId("The id");
    tutorial.addStep(step);

    WaitForComposed* waitForAndGrandChild = new WaitForComposed();
    waitForAndGrandChild->setCompositionType(WaitForComposed::And);

    WaitForSignal* waitForSignal = new WaitForSignal();

    WaitForComposed* waitForOrChild = new WaitForComposed();
    waitForOrChild->setCompositionType(WaitForComposed::Or);
    waitForOrChild->addWaitFor(waitForAndGrandChild);
    waitForOrChild->addWaitFor(waitForSignal);

    WaitForComposed* waitForAnd = new WaitForComposed();
    waitForAnd->setCompositionType(WaitForComposed::And);
    waitForAnd->addWaitFor(waitForOrChild);

    Reaction* reaction = new Reaction();
    reaction->setTriggerType(Reaction::ConditionMet);
    reaction->setWaitFor(waitForAnd);
    reaction->setResponseType(Reaction::NextStep);
    reaction->setNextStepId("Another step");
    step->addReaction(reaction);

    JavascriptExporter exporter;
    QString exportedTutorial = exporter.exportTutorial(&tutorial);

    QString expected =
TUTORIAL_EMPTY_INFORMATION_CODE
STEP_WITH_ID_THE_ID_AND_EMPTY_TEXT_START_CODE
"function theIdStepSetup(step) {\n"
"    waitForAnd = ktutorial.newWaitFor(\"WaitForAnd\");\n"
"\n"
"    //Error: WaitForSignal without emitter name!\n"
"\n"
"    waitForOr = ktutorial.newWaitFor(\"WaitForOr\");\n"
"    waitForOr.add(waitForAnd);\n"
"\n"
"    waitForAnd2 = ktutorial.newWaitFor(\"WaitForAnd\");\n"
"    waitForAnd2.add(waitForOr);\n"
"    step.addWaitFor(waitForAnd2, \"Another step\");\n"
"}\n"
CONNECT_STEP_SETUP
STEP_WITH_ID_THE_ID_AND_EMPTY_TEXT_END_CODE;

    QCOMPARE(exportedTutorial, expected);
}

void JavascriptExporterTest::testWaitForNot() {
    Tutorial tutorial;
    Step* step = new Step();
    step->setId("The id");
    tutorial.addStep(step);

    WaitForSignal* waitForSignal = new WaitForSignal();
    waitForSignal->setEmitterName("The emitter name");
    waitForSignal->setSignalName("theSignalName(Argument1Type, Argument2Type)");

    WaitForNot* waitForNot = new WaitForNot();
    waitForNot->setNegatedWaitFor(waitForSignal);

    Reaction* reaction = new Reaction();
    reaction->setTriggerType(Reaction::ConditionMet);
    reaction->setWaitFor(waitForNot);
    reaction->setResponseType(Reaction::NextStep);
    reaction->setNextStepId("Another step");
    step->addReaction(reaction);

    JavascriptExporter exporter;
    QString exportedTutorial = exporter.exportTutorial(&tutorial);

    QString expected =
TUTORIAL_EMPTY_INFORMATION_CODE
STEP_WITH_ID_THE_ID_AND_EMPTY_TEXT_START_CODE
"function theIdStepSetup(step) {\n"
"    waitForTheSignalNameByTheEmitterName = \
ktutorial.newWaitFor(\"WaitForSignal\");\n"
"    waitForTheSignalNameByTheEmitterName.setSignal(\
ktutorial.findObject(\"The emitter name\"), \
\"theSignalName(Argument1Type, Argument2Type)\");\n"
"\n"
"    waitForNot = ktutorial.newWaitFor(\"WaitForNot\");\n"
"    waitForNot.setNegatedWaitFor(waitForTheSignalNameByTheEmitterName);\n"
"    step.addWaitFor(waitForNot, \"Another step\");\n"
"}\n"
CONNECT_STEP_SETUP
STEP_WITH_ID_THE_ID_AND_EMPTY_TEXT_END_CODE;

    QCOMPARE(exportedTutorial, expected);
}

void JavascriptExporterTest::testWaitForNotWithInvalidNegatedWaitFor() {
    Tutorial tutorial;
    Step* step = new Step();
    step->setId("The id");
    tutorial.addStep(step);

    WaitForSignal* waitForSignal = new WaitForSignal();

    WaitForNot* waitForNot = new WaitForNot();
    waitForNot->setNegatedWaitFor(waitForSignal);

    Reaction* reaction = new Reaction();
    reaction->setTriggerType(Reaction::ConditionMet);
    reaction->setWaitFor(waitForNot);
    reaction->setResponseType(Reaction::NextStep);
    reaction->setNextStepId("Another step");
    step->addReaction(reaction);

    JavascriptExporter exporter;
    QString exportedTutorial = exporter.exportTutorial(&tutorial);

    QString expected =
TUTORIAL_EMPTY_INFORMATION_CODE
STEP_WITH_ID_THE_ID_AND_EMPTY_TEXT_START_CODE
"function theIdStepSetup(step) {\n"
"    //Error: WaitForSignal without emitter name!\n"
"\n"
"    waitForNot = ktutorial.newWaitFor(\"WaitForNot\");\n"
"    step.addWaitFor(waitForNot, \"Another step\");\n"
"}\n"
CONNECT_STEP_SETUP
STEP_WITH_ID_THE_ID_AND_EMPTY_TEXT_END_CODE;

    QCOMPARE(exportedTutorial, expected);
}

void JavascriptExporterTest::testWaitForNotWithoutNegatedWaitFor() {
    Tutorial tutorial;
    Step* step = new Step();
    step->setId("The id");
    tutorial.addStep(step);

    WaitForNot* waitForNot = new WaitForNot();

    Reaction* reaction = new Reaction();
    reaction->setTriggerType(Reaction::ConditionMet);
    reaction->setWaitFor(waitForNot);
    reaction->setResponseType(Reaction::NextStep);
    reaction->setNextStepId("Another step");
    step->addReaction(reaction);

    JavascriptExporter exporter;
    QString exportedTutorial = exporter.exportTutorial(&tutorial);

    QString expected =
TUTORIAL_EMPTY_INFORMATION_CODE
STEP_WITH_ID_THE_ID_AND_EMPTY_TEXT_START_CODE
"function theIdStepSetup(step) {\n"
"    //Error: WaitForNot without negated WaitFor!\n"
"}\n"
CONNECT_STEP_SETUP
STEP_WITH_ID_THE_ID_AND_EMPTY_TEXT_END_CODE;

    QCOMPARE(exportedTutorial, expected);
}

QTEST_MAIN(JavascriptExporterTest)

#include "JavascriptExporterTest.moc"
