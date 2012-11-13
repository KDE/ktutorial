/****************************************************************************
 *   Copyright (C) 2010 by Daniel Calviño Sánchez <danxuliu@gmail.com>      *
 *   Copyright (C) 2012 by Daniel Calviño Sánchez <danxuliu@gmail.com>      *
 *                                                                          *
 *   This program is free software; you can redistribute it and/or modify   *
 *   it under the terms of the GNU General Public License as published by   *
 *   the Free Software Foundation; either version 2 of the License, or      *
 *   (at your option) any later version.                                    *
 *                                                                          *
 *   This program is distributed in the hope that it will be useful,        *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of         *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the          *
 *   GNU General Public License for more details.                           *
 *                                                                          *
 *   You should have received a copy of the GNU General Public License      *
 *   along with this program; If not, see <http://www.gnu.org/licenses/>.   *
 ****************************************************************************/

t = Kross.module("kdetranslation");

tutorial.tutorialInformationAsObject().setName(t.i18n("Basic concepts"));
tutorial.tutorialInformationAsObject().setDescription(t.i18n("This tutorial shows how to design a basic tutorial for the KTutorial test application."));

//Step start
startStep = ktutorial.newStep("start");
startStep.setText(t.i18nc("@info", "<para>Welcome to the tutorial to learn the basic concepts in <application>KTutorial editor</application>.</para>\n\n<para>When you end this tutorial you will have written a tutorial for the KTutorial test application that teaches the user how to clear the text area. First it will ask the user to write something in the text area, and then clear it using a menu item or a toolbar button.</para>\n\n<para>We will start setting the tutorial information. This information is shown to the user in the tutorial selection dialog as a way to know the purpose of a tutorial before starting it.</para>\n\n<para>Click the <link url=\"widget:setTutorialInformationToolButton\"><interface>Set information</interface></link> direct access in the dock panels, or in <interface>Edit|Tutorial|Set information...</interface>.</para>"));

function startStepSetup(step) {
    waitForTutorialInformationDialogToBeShown = ktutorial.newWaitFor("WaitForWindow");
    waitForTutorialInformationDialogToBeShown.setWindowObjectName("TutorialInformationDialog");
    step.addWaitFor(waitForTutorialInformationDialogToBeShown, "tutorialInformationDialog");
}
connect(startStep, "setup(QObject*)",
        this, "startStepSetup(QObject*)");

tutorial.addStep(startStep);

//Step tutorialInformationDialog
tutorialInformationDialogStep = ktutorial.newStep("tutorialInformationDialog");
tutorialInformationDialogStep.setText(t.i18nc("@info", "Now, set the name of the tutorial to \"Example\" and its description to \"Example tutorial\" and accept the dialog clicking in <link url=\"widget:okButton\"><interface>Ok</interface> button</link>."));

function tutorialInformationDialogStepSetup(step) {
    waitForClickedByOkButton = ktutorial.newWaitFor("WaitForSignal");
    waitForClickedByOkButton.setSignal(ktutorial.findObject("okButton"), "clicked()");
    step.addWaitFor(waitForClickedByOkButton, "addStartStep");
}
connect(tutorialInformationDialogStep, "setup(QObject*)",
        this, "tutorialInformationDialogStepSetup(QObject*)");

tutorial.addStep(tutorialInformationDialogStep);

//Step addStartStep
addStartStepStep = ktutorial.newStep("addStartStep");
addStartStepStep.setText(t.i18nc("@info", "<para>Now it is time to add the first step of the tutorial. Every tutorial must have a step called \"start\", so KTutorial can know where the tutorial starts.</para>\n\n<para>Click the <link url=\"widget:addStepToolButton\"><interface>Add step</interface></link> direct access in the dock panels, or in <interface>Edit|Step|Add step...</interface>.</para>"));

function addStartStepStepSetup(step) {
    waitForStepDataDialogToBeShown = ktutorial.newWaitFor("WaitForWindow");
    waitForStepDataDialogToBeShown.setWindowObjectName("StepDataDialog");
    step.addWaitFor(waitForStepDataDialogToBeShown, "setStartStepData");
}
connect(addStartStepStep, "setup(QObject*)",
        this, "addStartStepStepSetup(QObject*)");

tutorial.addStep(addStartStepStep);

//Step setStartStepData
setStartStepDataStep = ktutorial.newStep("setStartStepData");
setStartStepDataStep.setText(t.i18nc("@info", "<para>Set the id of the step to <emphasis>start</emphasis> and the text to <emphasis>Write &lt;emphasis&gt;Hello world!&lt;/emphasis&gt; in the text area.</emphasis>, then accept the dialog.</para>\n\n<para><emphasis>&lt;emphasis&gt;</emphasis> is a special markup that is not shown in the tutorial. Instead, the text between <emphasis>&lt;emphasis&gt;</emphasis> and <emphasis>&lt;/emphasis&gt;</emphasis> will be written in italics.</para>"));

function setStartStepDataStepSetup(step) {
    waitForClickedByOkButton = ktutorial.newWaitFor("WaitForSignal");
    waitForClickedByOkButton.setSignal(ktutorial.findObject("okButton"), "clicked()");
    step.addWaitFor(waitForClickedByOkButton, "addReactionToStartStep");
}
connect(setStartStepDataStep, "setup(QObject*)",
        this, "setStartStepDataStepSetup(QObject*)");

tutorial.addStep(setStartStepDataStep);

//Step addReactionToStartStep
addReactionToStartStepStep = ktutorial.newStep("addReactionToStartStep");
addReactionToStartStepStep.setText(t.i18nc("@info", "<para>The start step will wait until the user writes <emphasis>Hello world!</emphasis> in the text area of the KTutorial test application.</para>\n\n<para>For the step to do that we have to add a reaction to it. To add a reaction, in the <link url=\"widget:centralTreeView\">tutorial tree view</link> select the step to add the reaction to and click the <link url=\"widget:addReactionToolButton\"><interface>Add reaction</interface></link> direct access in the dock panels, or in <interface>Edit|Reaction|Add reaction...</interface>.</para>"));

function addReactionToStartStepStepSetup(step) {
    waitForReactionDialogToBeShown = ktutorial.newWaitFor("WaitForWindow");
    waitForReactionDialogToBeShown.setWindowObjectName("ReactionDialog");
    step.addWaitFor(waitForReactionDialogToBeShown, "setTriggerInStartStepReaction");
}
connect(addReactionToStartStepStep, "setup(QObject*)",
        this, "addReactionToStartStepStepSetup(QObject*)");

tutorial.addStep(addReactionToStartStepStep);

//Step setTriggerInStartStepReaction
setTriggerInStartStepReactionStep = ktutorial.newStep("setTriggerInStartStepReaction");
setTriggerInStartStepReactionStep.setText(t.i18nc("@info", "<para>A reaction can be triggered when the user selects some option or when a condition is met, but not both in the same reaction (although several reactions can be added to the same step).</para>\n\n<para>We want the step to react when something happens, that is, when a condition is met, so select the <link url=\"widget:triggerConditionRadioButton\"><interface>condition met</interface> radio button</link>.</para>"));

function setTriggerInStartStepReactionStepSetup(step) {
    waitForClickedByTriggerConditionRadioButton = ktutorial.newWaitFor("WaitForSignal");
    waitForClickedByTriggerConditionRadioButton.setSignal(ktutorial.findObject("triggerConditionRadioButton"), "clicked(bool)");
    step.addWaitFor(waitForClickedByTriggerConditionRadioButton, "addNewWaitForInStartStepReaction");
}
connect(setTriggerInStartStepReactionStep, "setup(QObject*)",
        this, "setTriggerInStartStepReactionStepSetup(QObject*)");

tutorial.addStep(setTriggerInStartStepReactionStep);

//Step addNewWaitForInStartStepReaction
addNewWaitForInStartStepReactionStep = ktutorial.newStep("addNewWaitForInStartStepReaction");
addNewWaitForInStartStepReactionStep.setText(t.i18nc("@info", "<para>Once the <interface>condition met</interface> button is selected we can add a new condition to the reaction. Only one condition can be added to each reaction, although the condition can be composed of other conditions.</para>\n\n<para>Click <link url=\"widget:addButton\"><interface>Add...</interface> button</link> to show the dialog to add new conditions.</para>"));

function addNewWaitForInStartStepReactionStepSetup(step) {
    waitForAddWaitForDialogToBeShown = ktutorial.newWaitFor("WaitForWindow");
    waitForAddWaitForDialogToBeShown.setWindowObjectName("addWaitForDialog");
    step.addWaitFor(waitForAddWaitForDialogToBeShown, "newWaitForSignalForStartStepReaction");
}
connect(addNewWaitForInStartStepReactionStep, "setup(QObject*)",
        this, "addNewWaitForInStartStepReactionStepSetup(QObject*)");

tutorial.addStep(addNewWaitForInStartStepReactionStep);

//Step newWaitForSignalForStartStepReaction
newWaitForSignalForStartStepReactionStep = ktutorial.newStep("newWaitForSignalForStartStepReaction");
newWaitForSignalForStartStepReactionStep.setText(t.i18nc("@info", "<para>In the dialog you can select the type of condition to wait for.</para>\n\n<para>There are simple and composed conditions. Simple conditions wait for something specific. Composed conditions wait for other conditions to be met.</para>\n\n<para>In this case, as it will be explained later, we want to wait for a signal, so select \"The specified signal is emitted\" in the <link url=\"widget:waitForTypeComboBox\">combobox</link> and accept the dialog.</para>"));

function newWaitForSignalForStartStepReactionStepSetup(step) {
    waitForClickedByAddWaitForDialogOkButton = ktutorial.newWaitFor("WaitForSignal");
    waitForClickedByAddWaitForDialogOkButton.setSignal(ktutorial.findObject("addWaitForDialog/okButton"), "clicked()");
    step.addWaitFor(waitForClickedByAddWaitForDialogOkButton, "editWaitForSignalInStartStepReaction");
}
connect(newWaitForSignalForStartStepReactionStep, "setup(QObject*)",
        this, "newWaitForSignalForStartStepReactionStepSetup(QObject*)");

tutorial.addStep(newWaitForSignalForStartStepReactionStep);

//Step editWaitForSignalInStartStepReaction
editWaitForSignalInStartStepReactionStep = ktutorial.newStep("editWaitForSignalInStartStepReaction");
editWaitForSignalInStartStepReactionStep.setText(t.i18nc("@info", "<para>Just adding the condition is not enough to define it. Which signal will it wait for? Emitted by which object? Simple conditions have to be edited after being added to set its parameters.</para>\n\n<para>Composed conditions, on the other hand, have no parameters to be set. They are defined by its own type and its child conditions.</para>\n\n<para>To edit a condition, select it in the <link url=\"widget:waitForTreeView\">condition tree</link> and then click the <link url=\"widget:editButton\"><interface>Edit...</interface> button</link>.</para>"));

function editWaitForSignalInStartStepReactionStepSetup(step) {
    waitForEditWaitForSignalDialogToBeShown = ktutorial.newWaitFor("WaitForWindow");
    waitForEditWaitForSignalDialogToBeShown.setWindowObjectName("editWaitForSignalDialog");
    step.addWaitFor(waitForEditWaitForSignalDialogToBeShown, "setWaitForSignalTextChangedData");
}
connect(editWaitForSignalInStartStepReactionStep, "setup(QObject*)",
        this, "editWaitForSignalInStartStepReactionStepSetup(QObject*)");

tutorial.addStep(editWaitForSignalInStartStepReactionStep);

//Step setWaitForSignalTextChangedData
setWaitForSignalTextChangedDataStep = ktutorial.newStep("setWaitForSignalTextChangedData");
setWaitForSignalTextChangedDataStep.setText(t.i18nc("@info", "<para>The parameters that have to be set depend on the type of the condition. In this case, the name of the object that emits the signal and the name of the signal are needed.</para>\n\n<para><application>KTutorial editor</application> is able to <emphasis>talk</emphasis> to the application that the tutorial is being designed for and show you the name of the objects and the signals they emit. However, this will be seen in another tutorial. In this one you will be given all the values you need.</para>\n\n<para>Ok, back to the condition. We have asked the user to write <emphasis>Hello world!</emphasis> in the text area, so we want to react when the text in the text area changes.</para>\n\n<para>The text area is appropriately named <emphasis>textArea</emphasis>, and it emits the signal <emphasis>textChanged()</emphasis>. Set those values in the <link url=\"widget:objectNameLineEdit\"><interface>emitter name</interface> field</link> and in the <link url=\"widget:signalNameLineEdit\"><interface>signal name</interface> field</link>, respectively, and accept the dialog.</para>"));

function setWaitForSignalTextChangedDataStepSetup(step) {
    waitForClickedByEditWaitForSignalDialogOkButton = ktutorial.newWaitFor("WaitForSignal");
    waitForClickedByEditWaitForSignalDialogOkButton.setSignal(ktutorial.findObject("editWaitForSignalDialog/okButton"), "clicked()");
    step.addWaitFor(waitForClickedByEditWaitForSignalDialogOkButton, "setResponseInStartStepReaction");
}
connect(setWaitForSignalTextChangedDataStep, "setup(QObject*)",
        this, "setWaitForSignalTextChangedDataStepSetup(QObject*)");

tutorial.addStep(setWaitForSignalTextChangedDataStep);

//Step setResponseInStartStepReaction
setResponseInStartStepReactionStep = ktutorial.newStep("setResponseInStartStepReaction");
setResponseInStartStepReactionStep.setText(t.i18nc("@info", "<para>Now that the parameters of the condition are set, the step will react whenever the user writes something in the text area. But, how should it react?</para>\n\n<para>We want to advance to the next step when the user writes <emphasis>Hello world!</emphasis>, but textChanged() is emitted whenever the text was changed no matter what was written. So, before changing to the next step, we must check that the text area contains the expected text.</para>\n\n<para>The checks will be made using some custom JavaScript code, so select the <link url=\"widget:responseCodeRadioButton\"><interface>custom response code</interface> radio button.</link>.</para>"));

function setResponseInStartStepReactionStepSetup(step) {
    waitForClickedByResponseCodeRadioButton = ktutorial.newWaitFor("WaitForSignal");
    waitForClickedByResponseCodeRadioButton.setSignal(ktutorial.findObject("responseCodeRadioButton"), "clicked()");
    step.addWaitFor(waitForClickedByResponseCodeRadioButton, "setCodeToCheckTheText");
}
connect(setResponseInStartStepReactionStep, "setup(QObject*)",
        this, "setResponseInStartStepReactionStepSetup(QObject*)");

tutorial.addStep(setResponseInStartStepReactionStep);

//Step setCodeToCheckTheText
setCodeToCheckTheTextStep = ktutorial.newStep("setCodeToCheckTheText");
setCodeToCheckTheTextStep.setText(t.i18nc("@info", "<para>The following code snippet gets the text area object, checks if it contains the text <emphasis>Hello world!</emphasis> (without taking into account upper and lower cases) and, in that case, changes to the step <emphasis>clearText</emphasis>:</para>\n\n<pre>textArea = ktutorial.findObject(\"textArea\");\nif (textArea.plainText.toLowerCase() == t.i18n(\"Hello world!\").toLowerCase()) {\n    tutorial.nextStep(\"clearText\");\n}</pre>\n\n<para>You can copy the code selecting it with the mouse, clicking with the right mouse button on the selection and then clicking on <interface>Copy</interface>. To paste it, click with the right mouse button on the <link url=\"widget:responseCodeTextEdit\">custom response code area</link> and then on <interface>Paste</interface>. Once done, accept the dialog.</para>"));

function setCodeToCheckTheTextStepSetup(step) {
    waitForClickedByOkButton = ktutorial.newWaitFor("WaitForSignal");
    waitForClickedByOkButton.setSignal(ktutorial.findObject("okButton"), "clicked()");
    step.addWaitFor(waitForClickedByOkButton, "addClearTextStep");
}
connect(setCodeToCheckTheTextStep, "setup(QObject*)",
        this, "setCodeToCheckTheTextStepSetup(QObject*)");

tutorial.addStep(setCodeToCheckTheTextStep);

//Step addClearTextStep
addClearTextStepStep = ktutorial.newStep("addClearTextStep");
addClearTextStepStep.setText(t.i18nc("@info", "<para>Well, we have just created the start step of the tutorial. It waits for the user to write <emphasis>Hello world!</emphasis> in the text area and then changes to the clearText step.</para>\n\n<para>Now we will take care of the clearText step. Let's begin by adding a new step.</para>"));

function addClearTextStepStepSetup(step) {
    waitForStepDataDialogToBeShown = ktutorial.newWaitFor("WaitForWindow");
    waitForStepDataDialogToBeShown.setWindowObjectName("StepDataDialog");
    step.addWaitFor(waitForStepDataDialogToBeShown, "setClearTextStepData");
}
connect(addClearTextStepStep, "setup(QObject*)",
        this, "addClearTextStepStepSetup(QObject*)");

tutorial.addStep(addClearTextStepStep);

//Step setClearTextStepData
setClearTextStepDataStep = ktutorial.newStep("setClearTextStepData");
setClearTextStepDataStep.setText(t.i18nc("@info", "<para>Set the id of the step to <emphasis>clearText</emphasis> and the text to <emphasis>Now you have to trigger the clear action. To do this you can click in &lt;interface&gt;File|Clear&lt;/interface&gt;, or in the toolbar button called &lt;interface&gt;Clear&lt;/interface&gt;.</emphasis>, then accept the dialog.</para>\n\n<para>Like <emphasis>&lt;emphasis&gt;</emphasis> in the text of the start step, <emphasis>&lt;interface&gt;</emphasis> is another special markup used when referring to something in the graphical user interface.</para>"));

function setClearTextStepDataStepSetup(step) {
    waitForClickedByOkButton = ktutorial.newWaitFor("WaitForSignal");
    waitForClickedByOkButton.setSignal(ktutorial.findObject("okButton"), "clicked()");
    step.addWaitFor(waitForClickedByOkButton, "addReactionToClearTextStep");
}
connect(setClearTextStepDataStep, "setup(QObject*)",
        this, "setClearTextStepDataStepSetup(QObject*)");

tutorial.addStep(setClearTextStepDataStep);

//Step addReactionToClearTextStep
addReactionToClearTextStepStep = ktutorial.newStep("addReactionToClearTextStep");
addReactionToClearTextStepStep.setText(t.i18nc("@info", "<para>The clearText step will wait for the user to trigger the clear action (that is, clicking <interface>Clear</interface> button in the tool bar or <interface>File|Clear</interface> in the menu).</para>\n\n<para>So we have to add a reaction to the clearText step. Like we did with the start step, select the clearText step in the tutorial tree view and click on <interface>Add reaction</interface>.</para>"));

function addReactionToClearTextStepStepSetup(step) {
    waitForReactionDialogToBeShown = ktutorial.newWaitFor("WaitForWindow");
    waitForReactionDialogToBeShown.setWindowObjectName("ReactionDialog");
    step.addWaitFor(waitForReactionDialogToBeShown, "addWaitForSignalToClearTextStepReaction");
}
connect(addReactionToClearTextStepStep, "setup(QObject*)",
        this, "addReactionToClearTextStepStepSetup(QObject*)");

tutorial.addStep(addReactionToClearTextStepStep);

//Step addWaitForSignalToClearTextStepReaction
addWaitForSignalToClearTextStepReactionStep = ktutorial.newStep("addWaitForSignalToClearTextStepReaction");
addWaitForSignalToClearTextStepReactionStep.setText(t.i18nc("@info", "<para>Again, to wait for something to happen we have to add a condition. And again, that condition is a signal being emitted.</para>\n\n<para>Add a new condition to wait for a signal and edit it to set its parameters.</para>"));

function addWaitForSignalToClearTextStepReactionStepSetup(step) {
    waitForEditWaitForSignalDialogToBeShown = ktutorial.newWaitFor("WaitForWindow");
    waitForEditWaitForSignalDialogToBeShown.setWindowObjectName("editWaitForSignalDialog");
    step.addWaitFor(waitForEditWaitForSignalDialogToBeShown, "editWaitForSignalInClearTextStep");
}
connect(addWaitForSignalToClearTextStepReactionStep, "setup(QObject*)",
        this, "addWaitForSignalToClearTextStepReactionStepSetup(QObject*)");

tutorial.addStep(addWaitForSignalToClearTextStepReactionStep);

//Step editWaitForSignalInClearTextStep
editWaitForSignalInClearTextStepStep = ktutorial.newStep("editWaitForSignalInClearTextStep");
editWaitForSignalInClearTextStepStep.setText(t.i18nc("@info", "<para>Usually, buttons in tool bars and menu entries are graphical representations of the so called <emphasis>actions</emphasis>. Actions are commands given to the application, for example, <emphasis>add step</emphasis> or <emphasis>save file</emphasis>, and they behave the same no matter if they were triggered from a tool bar or from the menu.</para>\n\n<para>So, as a rule of thumb, when you expect the user to click on a menu item or in a tool bar button, wait for the <emphasis>triggered()</emphasis> signal of the action they represent, instead of the <emphasis>clicked()</emphasis> signal of the button.</para>\n\n<para>In this case, the object name of the action is just <emphasis>clear</emphasis>. Set it in the emitter name, set the signal name to <emphasis>triggered()</emphasis> and accept the dialog.</para>"));

function editWaitForSignalInClearTextStepStepSetup(step) {
    waitForClickedByEditWaitForSignalDialogOkButton = ktutorial.newWaitFor("WaitForSignal");
    waitForClickedByEditWaitForSignalDialogOkButton.setSignal(ktutorial.findObject("editWaitForSignalDialog/okButton"), "clicked()");
    step.addWaitFor(waitForClickedByEditWaitForSignalDialogOkButton, "setResponseInClearTextStepReaction");
}
connect(editWaitForSignalInClearTextStepStep, "setup(QObject*)",
        this, "editWaitForSignalInClearTextStepStepSetup(QObject*)");

tutorial.addStep(editWaitForSignalInClearTextStepStep);

//Step setResponseInClearTextStepReaction
setResponseInClearTextStepReactionStep = ktutorial.newStep("setResponseInClearTextStepReaction");
setResponseInClearTextStepReactionStep.setText(t.i18nc("@info", "<para>This time there is nothing else to be checked once the condition is met. When the clear action is triggered the text area is cleared, so the signal being emitted is enough to change to the next step in the tutorial.</para>\n\n<para>Just set the name of the step to change to, which we will call <emphasis>end</emphasis>, in the <link url=\"widget:responseStepLineEdit\"><interface>change to step</interface> field</link> and accept the dialog.</para>"));

function setResponseInClearTextStepReactionStepSetup(step) {
    waitForClickedByOkButton = ktutorial.newWaitFor("WaitForSignal");
    waitForClickedByOkButton.setSignal(ktutorial.findObject("okButton"), "clicked()");
    step.addWaitFor(waitForClickedByOkButton, "addEndStep");
}
connect(setResponseInClearTextStepReactionStep, "setup(QObject*)",
        this, "setResponseInClearTextStepReactionStepSetup(QObject*)");

tutorial.addStep(setResponseInClearTextStepReactionStep);

//Step addEndStep
addEndStepStep = ktutorial.newStep("addEndStep");
addEndStepStep.setText(t.i18nc("@info", "<para>We are almost done. Add another step, the last one.</para>"));

function addEndStepStepSetup(step) {
    waitForStepDataDialogToBeShown = ktutorial.newWaitFor("WaitForWindow");
    waitForStepDataDialogToBeShown.setWindowObjectName("StepDataDialog");
    step.addWaitFor(waitForStepDataDialogToBeShown, "setEndStepData");
}
connect(addEndStepStep, "setup(QObject*)",
        this, "addEndStepStepSetup(QObject*)");

tutorial.addStep(addEndStepStep);

//Step setEndStepData
setEndStepDataStep = ktutorial.newStep("setEndStepData");
setEndStepDataStep.setText(t.i18nc("@info", "<para>Set the id of the step to <emphasis>end</emphasis> and its text to <emphasis>And that's all. Now you can close the tutorial.</emphasis>, then accept the dialog.</para>"));

function setEndStepDataStepSetup(step) {
    waitForClickedByOkButton = ktutorial.newWaitFor("WaitForSignal");
    waitForClickedByOkButton.setSignal(ktutorial.findObject("okButton"), "clicked()");
    step.addWaitFor(waitForClickedByOkButton, "end");
}
connect(setEndStepDataStep, "setup(QObject*)",
        this, "setEndStepDataStepSetup(QObject*)");

tutorial.addStep(setEndStepDataStep);

//Step end
endStep = ktutorial.newStep("end");
endStep.setText(t.i18nc("@info", "<para>The last step does not need a reaction, as it just informs the user that the tutorial has ended and that it can be closed.</para>\n\n<para>Finally, to install the tutorial you have just made you would have to export it (<interface>File|Export...</interface> or <interface>Export</interface> button in the tool bar) to a JavaScript file and save it in the tutorials subdirectory of the data directory of KTutorial test application.</para>\n\n<para>Of course, this is too cumbersome to test a tutorial while it is being designed. <application>KTutorial editor</application> is able to launch an application and start the tutorial being designed on it, but this will be shown in another tutorial.</para>\n\n<para>That's all, this basic <application>KTutorial editor</application> walkthrough ends here.</para>"));

tutorial.addStep(endStep);

