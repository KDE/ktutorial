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

tutorial.tutorialInformationAsObject().setName(t.i18n("Interacting with the target application"));
tutorial.tutorialInformationAsObject().setDescription(t.i18n("This tutorial shows how KTutorial editor can interact with the target application of a tutorial, for example, to show the object names or test the tutorial without installing it."));

//Step start
startStep = ktutorial.newStep("start");
startStep.setText(t.i18nc("@info", "<para>Welcome to the tutorial to learn how <application>KTutorial editor</application> can interact with the target application.</para>\n\n<para>The target application is the one that the tutorial is being designed for. <application>KTutorial editor</application> can \"talk\" with target applications to provide some useful features when designing a tutorial.</para>\n\n<para>To see these features we will design a tutorial for <application>KTutorial editor</application> itself that asks the user to add a new step and accept the step dialog.</para>\n\n<para>Let's begin by <link url=\"widget:addStepToolButton\">adding a new step</link>.</para>"));

function startStepSetup(step) {
    waitForStepDataDialogToBeShown = ktutorial.newWaitFor("WaitForWindow");
    waitForStepDataDialogToBeShown.setWindowObjectName("StepDataDialog");
    step.addWaitFor(waitForStepDataDialogToBeShown, "setStartStepId");
}
connect(startStep, "setup(QObject*)",
        this, "startStepSetup(QObject*)");

tutorial.addStep(startStep);

//Step setStartStepId
setStartStepIdStep = ktutorial.newStep("setStartStepId");
setStartStepIdStep.setText(t.i18nc("@info", "<para>Set the <link url=\"widget:idLineEdit\">id of the step</link> to <emphasis>start</emphasis>.</para>"));

function setStartStepIdStepSetup(step) {
    waitForTextChangedByIdLineEdit = ktutorial.newWaitFor("WaitForSignal");
    waitForTextChangedByIdLineEdit.setSignal(ktutorial.findObject("idLineEdit"), "textChanged(QString)");
    step.addWaitFor(waitForTextChangedByIdLineEdit, self, "setStartStepIdStepWaitForTextChangedByIdLineEditConditionMet()");
}
connect(setStartStepIdStep, "setup(QObject*)",
        this, "setStartStepIdStepSetup(QObject*)");

function setStartStepIdStepWaitForTextChangedByIdLineEditConditionMet() {
    idLineEdit = ktutorial.findObject("idLineEdit");
    if (idLineEdit.text == "start") {
        tutorial.nextStep("setStartStepText");
    }
}

tutorial.addStep(setStartStepIdStep);

//Step setStartStepText
setStartStepTextStep = ktutorial.newStep("setStartStepText");
setStartStepTextStep.setText(t.i18nc("@info", "<para>Set the <link url=\"widget:textTextEdit\">text of the step</link> to:</para>\n\n<para><emphasis>Add a new step.</emphasis></para>"));

function setStartStepTextStepSetup(step) {
    waitForTextChangedByTextTextEdit = ktutorial.newWaitFor("WaitForSignal");
    waitForTextChangedByTextTextEdit.setSignal(ktutorial.findObject("textTextEdit"), "textChanged()");
    step.addWaitFor(waitForTextChangedByTextTextEdit, self, "setStartStepTextStepWaitForTextChangedByTextTextEditConditionMet()");
}
connect(setStartStepTextStep, "setup(QObject*)",
        this, "setStartStepTextStepSetup(QObject*)");

function setStartStepTextStepWaitForTextChangedByTextTextEditConditionMet() {
    textTextEdit = ktutorial.findObject("textTextEdit");
    if (textTextEdit.plainText.toLowerCase() == t.i18n("Add a new step.").toLowerCase()) {
        tutorial.nextStep("acceptAddStartStepDialog");
    }
}

tutorial.addStep(setStartStepTextStep);

//Step acceptAddStartStepDialog
acceptAddStartStepDialogStep = ktutorial.newStep("acceptAddStartStepDialog");
acceptAddStartStepDialogStep.setText(t.i18nc("@info", "<para>And finally accept the dialog clicking in the <link url=\"widget:okButton\"><interface>Ok</interface> button</link>.</para>"));

function acceptAddStartStepDialogStepSetup(step) {
    waitForClickedByOkButton = ktutorial.newWaitFor("WaitForSignal");
    waitForClickedByOkButton.setSignal(ktutorial.findObject("okButton"), "clicked()");
    step.addWaitFor(waitForClickedByOkButton, "addReactionToStartStep");
}
connect(acceptAddStartStepDialogStep, "setup(QObject*)",
        this, "acceptAddStartStepDialogStepSetup(QObject*)");

tutorial.addStep(acceptAddStartStepDialogStep);

//Step addReactionToStartStep
addReactionToStartStepStep = ktutorial.newStep("addReactionToStartStep");
addReactionToStartStepStep.setText(t.i18nc("@info", "<para>Now, select the <emphasis>start</emphasis> step and add a new reaction to it.</para>"));

function addReactionToStartStepStepSetup(step) {
    waitForReactionDialogToBeShown = ktutorial.newWaitFor("WaitForWindow");
    waitForReactionDialogToBeShown.setWindowObjectName("ReactionDialog");
    step.addWaitFor(waitForReactionDialogToBeShown, "setStartStepReactionTriggerToConditionMet");
}
connect(addReactionToStartStepStep, "setup(QObject*)",
        this, "addReactionToStartStepStepSetup(QObject*)");

tutorial.addStep(addReactionToStartStepStep);

//Step setStartStepReactionTriggerToConditionMet
setStartStepReactionTriggerToConditionMetStep = ktutorial.newStep("setStartStepReactionTriggerToConditionMet");
setStartStepReactionTriggerToConditionMetStep.setText(t.i18nc("@info", "<para>Set the trigger type to <link url=\"widget:triggerConditionRadioButton\"><interface>condition met</interface></link>.</para>"));

function setStartStepReactionTriggerToConditionMetStepSetup(step) {
    waitForClickedByTriggerConditionRadioButton = ktutorial.newWaitFor("WaitForSignal");
    waitForClickedByTriggerConditionRadioButton.setSignal(ktutorial.findObject("triggerConditionRadioButton"), "clicked()");
    step.addWaitFor(waitForClickedByTriggerConditionRadioButton, "addWaitForToStartStepReaction");
}
connect(setStartStepReactionTriggerToConditionMetStep, "setup(QObject*)",
        this, "setStartStepReactionTriggerToConditionMetStepSetup(QObject*)");

tutorial.addStep(setStartStepReactionTriggerToConditionMetStep);

//Step addWaitForToStartStepReaction
addWaitForToStartStepReactionStep = ktutorial.newStep("addWaitForToStartStepReaction");
addWaitForToStartStepReactionStep.setText(t.i18nc("@info", "<para><link url=\"widget:addButton\"><interface>Add</interface></link> a new condition.</para>"));

function addWaitForToStartStepReactionStepSetup(step) {
    waitForAddWaitForDialogToBeShown = ktutorial.newWaitFor("WaitForWindow");
    waitForAddWaitForDialogToBeShown.setWindowObjectName("addWaitForDialog");
    step.addWaitFor(waitForAddWaitForDialogToBeShown, "selectWaitForWindowInStartStepReaction");
}
connect(addWaitForToStartStepReactionStep, "setup(QObject*)",
        this, "addWaitForToStartStepReactionStepSetup(QObject*)");

tutorial.addStep(addWaitForToStartStepReactionStep);

//Step selectWaitForWindowInStartStepReaction
selectWaitForWindowInStartStepReactionStep = ktutorial.newStep("selectWaitForWindowInStartStepReaction");
selectWaitForWindowInStartStepReactionStep.setText(t.i18nc("@info", "<para>We want the step to react when the dialog to add a new step is shown, so select <emphasis>The specified window is shown</emphasis> in the <link url=\"widget:waitForTypeComboBox\">combo box</link>.</para>"));

function selectWaitForWindowInStartStepReactionStepSetup(step) {
    waitForCurrentIndexChangedByWaitForTypeComboBox = ktutorial.newWaitFor("WaitForSignal");
    waitForCurrentIndexChangedByWaitForTypeComboBox.setSignal(ktutorial.findObject("waitForTypeComboBox"), "currentIndexChanged(int)");
    step.addWaitFor(waitForCurrentIndexChangedByWaitForTypeComboBox, self, "selectWaitForWindowInStartStepReactionStepWaitForCurrentIndexChangedByWaitForTypeComboBoxConditionMet()");
}
connect(selectWaitForWindowInStartStepReactionStep, "setup(QObject*)",
        this, "selectWaitForWindowInStartStepReactionStepSetup(QObject*)");

function selectWaitForWindowInStartStepReactionStepWaitForCurrentIndexChangedByWaitForTypeComboBoxConditionMet() {
    comboBox = ktutorial.findObject("waitForTypeComboBox");
    if (comboBox.currentText == t.i18nc("@item:inlistbox", "The specified window is shown")) {
        tutorial.nextStep("acceptNewWaitForDialogForStartStepReaction");
    }
}

tutorial.addStep(selectWaitForWindowInStartStepReactionStep);

//Step acceptNewWaitForDialogForStartStepReaction
acceptNewWaitForDialogForStartStepReactionStep = ktutorial.newStep("acceptNewWaitForDialogForStartStepReaction");
acceptNewWaitForDialogForStartStepReactionStep.setText(t.i18nc("@info", "<para><link url=\"widget:addWaitForDialog/okButton\">Accept the dialog</link> to add the condition to wait for.</para>"));

function acceptNewWaitForDialogForStartStepReactionStepSetup(step) {
    waitForClickedByAddWaitForDialogOkButton = ktutorial.newWaitFor("WaitForSignal");
    waitForClickedByAddWaitForDialogOkButton.setSignal(ktutorial.findObject("addWaitForDialog/okButton"), "clicked()");
    step.addWaitFor(waitForClickedByAddWaitForDialogOkButton, "editWaitForWindowInStartStepReaction");
}
connect(acceptNewWaitForDialogForStartStepReactionStep, "setup(QObject*)",
        this, "acceptNewWaitForDialogForStartStepReactionStepSetup(QObject*)");

tutorial.addStep(acceptNewWaitForDialogForStartStepReactionStep);

//Step editWaitForWindowInStartStepReaction
editWaitForWindowInStartStepReactionStep = ktutorial.newStep("editWaitForWindowInStartStepReaction");
editWaitForWindowInStartStepReactionStep.setText(t.i18nc("@info", "<para>Select the condition just added and <link url=\"widget:editButton\">edit</link> it.</para>"));

function editWaitForWindowInStartStepReactionStepSetup(step) {
    waitForEditWaitForWindowDialogToBeShown = ktutorial.newWaitFor("WaitForWindow");
    waitForEditWaitForWindowDialogToBeShown.setWindowObjectName("editWaitForWindowDialog");
    step.addWaitFor(waitForEditWaitForWindowDialogToBeShown, "chooseWindowObjectName");
}
connect(editWaitForWindowInStartStepReactionStep, "setup(QObject*)",
        this, "editWaitForWindowInStartStepReactionStepSetup(QObject*)");

tutorial.addStep(editWaitForWindowInStartStepReactionStep);

//Step chooseWindowObjectName
chooseWindowObjectNameStep = ktutorial.newStep("chooseWindowObjectName");
chooseWindowObjectNameStep.setText(t.i18nc("@info", "<para>Now it is time to set the name of the window/dialog that we want to wait for. We asked the user to add a new step, so we want to wait for the dialog that appears when <interface>Add step</interface> is clicked. But... what is the object name of that dialog (note that the object name is not the title)?</para>\n\n<para>Usually the name of the objects (windows, widgets...) will be quite descriptive, so we can suppose how it will look like. However, a name that <emphasis>is something like...</emphasis> is not enough, we need the exact name.</para>\n\n<para><application>KTutorial editor</application> is able to show you a list with all the objects in the target application that KTutorial can access to.</para>\n\n<para>Click on <link url=\"widget:objectNamePushButton\">Choose...</link> to select the object from the list.</para>"));

function chooseWindowObjectNameStepSetup(step) {
    waitForOpenwithToBeShown = ktutorial.newWaitFor("WaitForWindow");
    waitForOpenwithToBeShown.setWindowObjectName("openwith");
    step.addWaitFor(waitForOpenwithToBeShown, "setTargetApplicationToUse");

    waitForRemoteObjectChooserToBeShown = ktutorial.newWaitFor("WaitForWindow");
    waitForRemoteObjectChooserToBeShown.setWindowObjectName("RemoteObjectChooser");
    step.addWaitFor(waitForRemoteObjectChooserToBeShown, "targetApplicationAlreadyRunning");
}
connect(chooseWindowObjectNameStep, "setup(QObject*)",
        this, "chooseWindowObjectNameStepSetup(QObject*)");

tutorial.addStep(chooseWindowObjectNameStep);

//Step setTargetApplicationToUse
setTargetApplicationToUseStep = ktutorial.newStep("setTargetApplicationToUse");
setTargetApplicationToUseStep.setText(t.i18nc("@info", "<para>To show you the list with the objects in the target application <application>KTutorial editor</application> needs to know which one is the target application. You can select it from the list of known applications or set its executable name.</para>\n\n<para>Once the target application is set <application>KTutorial editor</application> will start a new instance of that application to \"talk\" with it.</para>\n\n<para>The target application of this tutorial is <application>KTutorial editor</application> itself, so select it in the list or write <emphasis>ktutorial-editor</emphasis> (which is the name of its executable) in the text line, then accept the dialog.</para>"));

function setTargetApplicationToUseStepSetup(step) {
    waitForHideInOpenwith = ktutorial.newWaitFor("WaitForEvent");
    waitForHideInOpenwith.setEvent(ktutorial.findObject("openwith"), "Hide");
    step.addWaitFor(waitForHideInOpenwith, "highlightWidgetInRemoteObjectChooser");
}
connect(setTargetApplicationToUseStep, "setup(QObject*)",
        this, "setTargetApplicationToUseStepSetup(QObject*)");

tutorial.addStep(setTargetApplicationToUseStep);

//Step targetApplicationAlreadyRunning
targetApplicationAlreadyRunningStep = ktutorial.newStep("targetApplicationAlreadyRunning");
targetApplicationAlreadyRunningStep.setText(t.i18nc("@info", "<para><application>KTutorial editor</application> needs to know which application is the target application of the tutorial to \"talk\" with it.</para>\n\n<para>It seems that you already had selected the target application before starting this tutorial. Unfortunately, if the target application already selected is not <application>KTutorial editor</application>, you will have to close this tutorial and that target application, create a new tutorial (using <interface>File|New</interface>, or <interface>New</interface> in the tool bar) and start this tutorial again.</para>\n\n<para>Yes, I guess I should have warned you about this when you started this tutorial :P</para>"));

function targetApplicationAlreadyRunningStepSetup(step) {
    continueOption = ktutorial.newOption(t.i18nc("@action Tutorial option", "Continue"));
    step.addOption(continueOption, "highlightWidgetInRemoteObjectChooser");
}
connect(targetApplicationAlreadyRunningStep, "setup(QObject*)",
        this, "targetApplicationAlreadyRunningStepSetup(QObject*)");

tutorial.addStep(targetApplicationAlreadyRunningStep);

//Step highlightWidgetInRemoteObjectChooser
highlightWidgetInRemoteObjectChooserStep = ktutorial.newStep("highlightWidgetInRemoteObjectChooser");
highlightWidgetInRemoteObjectChooserStep.setText(t.i18nc("@info", "<para>Now you have the list with the objects that KTutorial can access in the target application.</para>\n\n<para>Note that when you select a widget in the list the widget is highlighted in the target application. For example, select the object named <emphasis>centralTreeView</emphasis>. You will see that, in the target application, the tree view that shows the tutorial being designed is highlighted.</para>\n\n<para>The list also shows the parent/child relationship between objects. For example, select <emphasis>editTutorialDock</emphasis>. The whole dock is highlighted.</para>\n\n<para>Now, select the unnamed object from the ActionListWidget class that appears in the list as a child element of the <emphasis>editTutorialDock</emphasis>. Now only the buttons in the dock are highlighted, as they are part of that widget.</para>\n\n<para>And finally, if you select the <emphasis>setTutorialInformationToolButton</emphasis> element, only the button to set the information of the tutorial will be highlighted.</para>"));

function highlightWidgetInRemoteObjectChooserStepSetup(step) {
    continueOption = ktutorial.newOption(t.i18nc("@action Tutorial option", "Continue"));
    step.addOption(continueOption, "selectStepDataDialogInRemoteObjectChooser");
}
connect(highlightWidgetInRemoteObjectChooserStep, "setup(QObject*)",
        this, "highlightWidgetInRemoteObjectChooserStepSetup(QObject*)");

tutorial.addStep(highlightWidgetInRemoteObjectChooserStep);

//Step selectStepDataDialogInRemoteObjectChooser
selectStepDataDialogInRemoteObjectChooserStep = ktutorial.newStep("selectStepDataDialogInRemoteObjectChooser");
selectStepDataDialogInRemoteObjectChooserStep.setText(t.i18nc("@info", "<para>You may have noticed that the list does not contain the object we are looking for, that is, the dialog that is shown to add a new step. The reason is that the list does not contain all the objects that may exist in the target application at any time. Instead, it is dynamic. When a new object is added or an existing one removed from the target application the list reflects that.</para>\n\n<para>So, in the target application, show the dialog to add a new step. Once you do this, a new object (and all its children) will be added to the end of the list. The object is called <emphasis>StepDataDialog</emphasis> and, yes, it is the dialog that has just been shown.</para>\n\n<para>Select the <emphasis>StepDataDialog</emphasis> element in the list and <link url=\"widget:RemoteObjectChooser/okButton\">accept the dialog</link>.</para>"));

function selectStepDataDialogInRemoteObjectChooserStepSetup(step) {
    waitForClickedByRemoteObjectChooserOkButton = ktutorial.newWaitFor("WaitForSignal");
    waitForClickedByRemoteObjectChooserOkButton.setSignal(ktutorial.findObject("RemoteObjectChooser/okButton"), "clicked()");
    step.addWaitFor(waitForClickedByRemoteObjectChooserOkButton, "acceptEditWaitForWindowDialogForStartStepReaction");
}
connect(selectStepDataDialogInRemoteObjectChooserStep, "setup(QObject*)",
        this, "selectStepDataDialogInRemoteObjectChooserStepSetup(QObject*)");

tutorial.addStep(selectStepDataDialogInRemoteObjectChooserStep);

//Step acceptEditWaitForWindowDialogForStartStepReaction
acceptEditWaitForWindowDialogForStartStepReactionStep = ktutorial.newStep("acceptEditWaitForWindowDialogForStartStepReaction");
acceptEditWaitForWindowDialogForStartStepReactionStep.setText(t.i18nc("@info", "<para>Now that the name of the window was set <link url=\"widget:editWaitForWindowDialog/okButton\">accept the dialog</link>.</para>"));

function acceptEditWaitForWindowDialogForStartStepReactionStepSetup(step) {
    waitForClickedByEditWaitForWindowDialogOkButton = ktutorial.newWaitFor("WaitForSignal");
    waitForClickedByEditWaitForWindowDialogOkButton.setSignal(ktutorial.findObject("editWaitForWindowDialog/okButton"), "clicked()");
    step.addWaitFor(waitForClickedByEditWaitForWindowDialogOkButton, "setNextStepResponseInStartStepReaction");
}
connect(acceptEditWaitForWindowDialogForStartStepReactionStep, "setup(QObject*)",
        this, "acceptEditWaitForWindowDialogForStartStepReactionStepSetup(QObject*)");

tutorial.addStep(acceptEditWaitForWindowDialogForStartStepReactionStep);

//Step setNextStepResponseInStartStepReaction
setNextStepResponseInStartStepReactionStep = ktutorial.newStep("setNextStepResponseInStartStepReaction");
setNextStepResponseInStartStepReactionStep.setText(t.i18nc("@info", "<para>We want to change to another step when the window is shown, so now it is time to set the name of that other step.</para>\n\n<para>Set the <link url=\"widget:responseStepLineEdit\">step to change to</link> to <emphasis>acceptDialog</emphasis>.</para>"));

function setNextStepResponseInStartStepReactionStepSetup(step) {
    waitForTextChangedByResponseStepLineEdit = ktutorial.newWaitFor("WaitForSignal");
    waitForTextChangedByResponseStepLineEdit.setSignal(ktutorial.findObject("responseStepLineEdit"), "textChanged(QString)");
    step.addWaitFor(waitForTextChangedByResponseStepLineEdit, self, "setNextStepResponseInStartStepReactionStepWaitForTextChangedByResponseStepLineEditConditionMet()");
}
connect(setNextStepResponseInStartStepReactionStep, "setup(QObject*)",
        this, "setNextStepResponseInStartStepReactionStepSetup(QObject*)");

function setNextStepResponseInStartStepReactionStepWaitForTextChangedByResponseStepLineEditConditionMet() {
    responseStepLineEdit = ktutorial.findObject("responseStepLineEdit");
    if (responseStepLineEdit.text == "acceptDialog") {
        tutorial.nextStep("acceptStartStepReactionDialog");
    }
}

tutorial.addStep(setNextStepResponseInStartStepReactionStep);

//Step acceptStartStepReactionDialog
acceptStartStepReactionDialogStep = ktutorial.newStep("acceptStartStepReactionDialog");
acceptStartStepReactionDialogStep.setText(t.i18nc("@info", "<para>And finally <link url=\"widget:okButton\">accept the dialog</link>.</para>"));

function acceptStartStepReactionDialogStepSetup(step) {
    waitForClickedByOkButton = ktutorial.newWaitFor("WaitForSignal");
    waitForClickedByOkButton.setSignal(ktutorial.findObject("okButton"), "clicked()");
    step.addWaitFor(waitForClickedByOkButton, "addAcceptDialogStep");
}
connect(acceptStartStepReactionDialogStep, "setup(QObject*)",
        this, "acceptStartStepReactionDialogStepSetup(QObject*)");

tutorial.addStep(acceptStartStepReactionDialogStep);

//Step addAcceptDialogStep
addAcceptDialogStepStep = ktutorial.newStep("addAcceptDialogStep");
addAcceptDialogStepStep.setText(t.i18nc("@info", "<para>We are done with the first step. Now we will take care of the second one, that will wait until the user accepts the dialog.</para>\n\n<para>Of course, the first thing to do is to <link url=\"widget:addStepToolButton\">add a new step</link>.</para>"));

function addAcceptDialogStepStepSetup(step) {
    waitForStepDataDialogToBeShown = ktutorial.newWaitFor("WaitForWindow");
    waitForStepDataDialogToBeShown.setWindowObjectName("StepDataDialog");
    step.addWaitFor(waitForStepDataDialogToBeShown, "setAcceptDialogStepId");
}
connect(addAcceptDialogStepStep, "setup(QObject*)",
        this, "addAcceptDialogStepStepSetup(QObject*)");

tutorial.addStep(addAcceptDialogStepStep);

//Step setAcceptDialogStepId
setAcceptDialogStepIdStep = ktutorial.newStep("setAcceptDialogStepId");
setAcceptDialogStepIdStep.setText(t.i18nc("@info", "<para>Set the <link url=\"widget:idLineEdit\">id of the step</link> to <emphasis>acceptDialog</emphasis>.</para>"));

function setAcceptDialogStepIdStepSetup(step) {
    waitForTextChangedByIdLineEdit = ktutorial.newWaitFor("WaitForSignal");
    waitForTextChangedByIdLineEdit.setSignal(ktutorial.findObject("idLineEdit"), "textChanged(QString)");
    step.addWaitFor(waitForTextChangedByIdLineEdit, self, "setAcceptDialogStepIdStepWaitForTextChangedByIdLineEditConditionMet()");
}
connect(setAcceptDialogStepIdStep, "setup(QObject*)",
        this, "setAcceptDialogStepIdStepSetup(QObject*)");

function setAcceptDialogStepIdStepWaitForTextChangedByIdLineEditConditionMet() {
    idLineEdit = ktutorial.findObject("idLineEdit");
    if (idLineEdit.text == "acceptDialog") {
        tutorial.nextStep("setAcceptDialogStepText");
    }
}

tutorial.addStep(setAcceptDialogStepIdStep);

//Step setAcceptDialogStepText
setAcceptDialogStepTextStep = ktutorial.newStep("setAcceptDialogStepText");
setAcceptDialogStepTextStep.setText(t.i18nc("@info", "<para>Set the <link url=\"widget:textTextEdit\">text of the step</link> to:</para>\n\n<para><emphasis>Set the id to &lt;emphasis&gt;start&lt;/emphasis&gt; and the text to &lt;emphasis&gt;First step&lt;/emphasis&gt;, then accept the dialog.</emphasis></para>\n\n<para>Remember that you can copy the text above and paste it in the text area, you don't need to write it yourself.</para>"));

function setAcceptDialogStepTextStepSetup(step) {
    waitForTextChangedByTextTextEdit = ktutorial.newWaitFor("WaitForSignal");
    waitForTextChangedByTextTextEdit.setSignal(ktutorial.findObject("textTextEdit"), "textChanged()");
    step.addWaitFor(waitForTextChangedByTextTextEdit, self, "setAcceptDialogStepTextStepWaitForTextChangedByTextTextEditConditionMet()");
}
connect(setAcceptDialogStepTextStep, "setup(QObject*)",
        this, "setAcceptDialogStepTextStepSetup(QObject*)");

function setAcceptDialogStepTextStepWaitForTextChangedByTextTextEditConditionMet() {
    textTextEdit = ktutorial.findObject("textTextEdit");
    trimmedText = textTextEdit.plainText.replace(/^\s+|\s+$/g, '');
    if (trimmedText.toLowerCase() == t.i18n("Set the id to &lt;emphasis&gt;start&lt;/emphasis&gt; and the text to &lt;emphasis&gt;First step&lt;/emphasis&gt;, then accept the dialog.").toLowerCase()) {
        tutorial.nextStep("acceptAddAcceptDialogStepDialog");
    }
}

tutorial.addStep(setAcceptDialogStepTextStep);

//Step acceptAddAcceptDialogStepDialog
acceptAddAcceptDialogStepDialogStep = ktutorial.newStep("acceptAddAcceptDialogStepDialog");
acceptAddAcceptDialogStepDialogStep.setText(t.i18nc("@info", "<para>And finally <link url=\"widget:okButton\">accept the dialog</link>.</para>"));

function acceptAddAcceptDialogStepDialogStepSetup(step) {
    waitForClickedByOkButton = ktutorial.newWaitFor("WaitForSignal");
    waitForClickedByOkButton.setSignal(ktutorial.findObject("okButton"), "clicked()");
    step.addWaitFor(waitForClickedByOkButton, "addReactionToAcceptDialogStep");
}
connect(acceptAddAcceptDialogStepDialogStep, "setup(QObject*)",
        this, "acceptAddAcceptDialogStepDialogStepSetup(QObject*)");

tutorial.addStep(acceptAddAcceptDialogStepDialogStep);

//Step addReactionToAcceptDialogStep
addReactionToAcceptDialogStepStep = ktutorial.newStep("addReactionToAcceptDialogStep");
addReactionToAcceptDialogStepStep.setText(t.i18nc("@info", "<para>Now, select the <emphasis>acceptDialog</emphasis> step and add a new reaction to it.</para>"));

function addReactionToAcceptDialogStepStepSetup(step) {
    waitForReactionDialogToBeShown = ktutorial.newWaitFor("WaitForWindow");
    waitForReactionDialogToBeShown.setWindowObjectName("ReactionDialog");
    step.addWaitFor(waitForReactionDialogToBeShown, "setAcceptDialogStepReactionTriggerToConditionMet");
}
connect(addReactionToAcceptDialogStepStep, "setup(QObject*)",
        this, "addReactionToAcceptDialogStepStepSetup(QObject*)");

tutorial.addStep(addReactionToAcceptDialogStepStep);

//Step setAcceptDialogStepReactionTriggerToConditionMet
setAcceptDialogStepReactionTriggerToConditionMetStep = ktutorial.newStep("setAcceptDialogStepReactionTriggerToConditionMet");
setAcceptDialogStepReactionTriggerToConditionMetStep.setText(t.i18nc("@info", "<para>Set the trigger type to <link url=\"widget:triggerConditionRadioButton\"><interface>condition met</interface></link>.</para>"));

function setAcceptDialogStepReactionTriggerToConditionMetStepSetup(step) {
    waitForClickedByTriggerConditionRadioButton = ktutorial.newWaitFor("WaitForSignal");
    waitForClickedByTriggerConditionRadioButton.setSignal(ktutorial.findObject("triggerConditionRadioButton"), "clicked()");
    step.addWaitFor(waitForClickedByTriggerConditionRadioButton, "addWaitForToAcceptDialogStepReaction");
}
connect(setAcceptDialogStepReactionTriggerToConditionMetStep, "setup(QObject*)",
        this, "setAcceptDialogStepReactionTriggerToConditionMetStepSetup(QObject*)");

tutorial.addStep(setAcceptDialogStepReactionTriggerToConditionMetStep);

//Step addWaitForToAcceptDialogStepReaction
addWaitForToAcceptDialogStepReactionStep = ktutorial.newStep("addWaitForToAcceptDialogStepReaction");
addWaitForToAcceptDialogStepReactionStep.setText(t.i18nc("@info", "<para><link url=\"widget:addButton\"><interface>Add</interface></link> a new condition.</para>"));

function addWaitForToAcceptDialogStepReactionStepSetup(step) {
    waitForAddWaitForDialogToBeShown = ktutorial.newWaitFor("WaitForWindow");
    waitForAddWaitForDialogToBeShown.setWindowObjectName("addWaitForDialog");
    step.addWaitFor(waitForAddWaitForDialogToBeShown, "selectWaitForSignalInAcceptDialogStepReaction");
}
connect(addWaitForToAcceptDialogStepReactionStep, "setup(QObject*)",
        this, "addWaitForToAcceptDialogStepReactionStepSetup(QObject*)");

tutorial.addStep(addWaitForToAcceptDialogStepReactionStep);

//Step selectWaitForSignalInAcceptDialogStepReaction
selectWaitForSignalInAcceptDialogStepReactionStep = ktutorial.newStep("selectWaitForSignalInAcceptDialogStepReaction");
selectWaitForSignalInAcceptDialogStepReactionStep.setText(t.i18nc("@info", "<para>To wait for the user to accept the dialog we will wait for the user to click on the <interface>Ok</interface> button.</para>\n\n<para>Buttons emit a signal whenever they are clicked, so select <emphasis>The specified signal is emitted</emphasis> in the <link url=\"widget:waitForTypeComboBox\">combo box</link>.</para>"));

function selectWaitForSignalInAcceptDialogStepReactionStepSetup(step) {
    waitForCurrentIndexChangedByWaitForTypeComboBox = ktutorial.newWaitFor("WaitForSignal");
    waitForCurrentIndexChangedByWaitForTypeComboBox.setSignal(ktutorial.findObject("waitForTypeComboBox"), "currentIndexChanged(int)");
    step.addWaitFor(waitForCurrentIndexChangedByWaitForTypeComboBox, self, "selectWaitForSignalInAcceptDialogStepReactionStepWaitForCurrentIndexChangedByWaitForTypeComboBoxConditionMet()");
}
connect(selectWaitForSignalInAcceptDialogStepReactionStep, "setup(QObject*)",
        this, "selectWaitForSignalInAcceptDialogStepReactionStepSetup(QObject*)");

function selectWaitForSignalInAcceptDialogStepReactionStepWaitForCurrentIndexChangedByWaitForTypeComboBoxConditionMet() {
    comboBox = ktutorial.findObject("waitForTypeComboBox");
    if (comboBox.currentText == t.i18nc("@item:inlistbox", "The specified signal is emitted")) {
        tutorial.nextStep("acceptNewWaitForDialogForAcceptDialogStepReaction");
    }
}

tutorial.addStep(selectWaitForSignalInAcceptDialogStepReactionStep);

//Step acceptNewWaitForDialogForAcceptDialogStepReaction
acceptNewWaitForDialogForAcceptDialogStepReactionStep = ktutorial.newStep("acceptNewWaitForDialogForAcceptDialogStepReaction");
acceptNewWaitForDialogForAcceptDialogStepReactionStep.setText(t.i18nc("@info", "<para><link url=\"widget:addWaitForDialog/okButton\">Accept the dialog</link> to add the condition to wait for.</para>"));

function acceptNewWaitForDialogForAcceptDialogStepReactionStepSetup(step) {
    waitForClickedByAddWaitForDialogOkButton = ktutorial.newWaitFor("WaitForSignal");
    waitForClickedByAddWaitForDialogOkButton.setSignal(ktutorial.findObject("addWaitForDialog/okButton"), "clicked()");
    step.addWaitFor(waitForClickedByAddWaitForDialogOkButton, "editWaitForSignalInAcceptDialogStepReaction");
}
connect(acceptNewWaitForDialogForAcceptDialogStepReactionStep, "setup(QObject*)",
        this, "acceptNewWaitForDialogForAcceptDialogStepReactionStepSetup(QObject*)");

tutorial.addStep(acceptNewWaitForDialogForAcceptDialogStepReactionStep);

//Step editWaitForSignalInAcceptDialogStepReaction
editWaitForSignalInAcceptDialogStepReactionStep = ktutorial.newStep("editWaitForSignalInAcceptDialogStepReaction");
editWaitForSignalInAcceptDialogStepReactionStep.setText(t.i18nc("@info", "<para>Select the condition just added and <link url=\"widget:editButton\">edit</link> it.</para>"));

function editWaitForSignalInAcceptDialogStepReactionStepSetup(step) {
    waitForEditWaitForSignalDialogToBeShown = ktutorial.newWaitFor("WaitForWindow");
    waitForEditWaitForSignalDialogToBeShown.setWindowObjectName("editWaitForSignalDialog");
    step.addWaitFor(waitForEditWaitForSignalDialogToBeShown, "triggerEmitterNameCompletionBox");
}
connect(editWaitForSignalInAcceptDialogStepReactionStep, "setup(QObject*)",
        this, "editWaitForSignalInAcceptDialogStepReactionStepSetup(QObject*)");

tutorial.addStep(editWaitForSignalInAcceptDialogStepReactionStep);

//Step triggerEmitterNameCompletionBox
triggerEmitterNameCompletionBoxStep = ktutorial.newStep("triggerEmitterNameCompletionBox");
triggerEmitterNameCompletionBoxStep.setText(t.i18nc("@info", "<para>We are again in a point where we need to know the name of an object, in this case, the name of the <interface>Ok</interface> button. This time, though, we will not choose it from the list. We will see another little <application>KTutorial editor</application> feature instead.</para>\n\n<para>When there is a target application running (if you closed it, start it again clicking on <interface>Choose...</interface> and then closing the list), the field to set an object name offers text completion with the names of the objects in the target application. Like the list to select an object, the completion list is dynamic, so entries are added and removed based on objects added and removed in the target application.</para>\n\n<para>Ensure that the target application is running and that, in that target application, the dialog to add a new step is open. Then just write <emphasis>o</emphasis> in the <link url=\"widget:objectNameLineEdit\">emitter name field</link> and watch the completion list pop up.</para>"));

function triggerEmitterNameCompletionBoxStepSetup(step) {
    waitForCompletionByObjectNameLineEdit = ktutorial.newWaitFor("WaitForSignal");
    waitForCompletionByObjectNameLineEdit.setSignal(ktutorial.findObject("objectNameLineEdit"), "completion(QString)");
    step.addWaitFor(waitForCompletionByObjectNameLineEdit, self, "triggerEmitterNameCompletionBoxStepWaitForCompletionByObjectNameLineEditConditionMet()");
}
connect(triggerEmitterNameCompletionBoxStep, "setup(QObject*)",
        this, "triggerEmitterNameCompletionBoxStepSetup(QObject*)");

function triggerEmitterNameCompletionBoxStepWaitForCompletionByObjectNameLineEditConditionMet() {
    objectNameLineEdit = ktutorial.findObject("objectNameLineEdit");
    if (objectNameLineEdit.text == "o") {
        tutorial.nextStep("selectOkButtonInCompletionBox");
    }
}

tutorial.addStep(triggerEmitterNameCompletionBoxStep);

//Step selectOkButtonInCompletionBox
selectOkButtonInCompletionBoxStep = ktutorial.newStep("selectOkButtonInCompletionBox");
selectOkButtonInCompletionBoxStep.setText(t.i18nc("@info", "<para>As you can see, the completion list proposes <emphasis>okButton</emphasis>, as it is the only object in the target application whose name starts with <emphasis>o</impact>. The completion list proposes all the objects in the target application whose name starts with the text written in the emitter name field.</para>\n\n<para>You should set the emitter name to <emphasis>okButton</emphasis>, but instead of writing it use the completion list. To use some proposed completion click on it with the mouse, or traverse the list using the up and down keys pressing enter when you reach the desired one.</para>"));

function selectOkButtonInCompletionBoxStepSetup(step) {
    waitForTextChangedByObjectNameLineEdit = ktutorial.newWaitFor("WaitForSignal");
    waitForTextChangedByObjectNameLineEdit.setSignal(ktutorial.findObject("objectNameLineEdit"), "textChanged(QString)");
    step.addWaitFor(waitForTextChangedByObjectNameLineEdit, self, "selectOkButtonInCompletionBoxStepWaitForTextChangedByObjectNameLineEditConditionMet()");
}
connect(selectOkButtonInCompletionBoxStep, "setup(QObject*)",
        this, "selectOkButtonInCompletionBoxStepSetup(QObject*)");

function selectOkButtonInCompletionBoxStepWaitForTextChangedByObjectNameLineEditConditionMet() {
    objectNameLineEdit = ktutorial.findObject("objectNameLineEdit");
    if (objectNameLineEdit.text == "okButton") {
        tutorial.nextStep("triggerSignalNameCompletionBox");
    }
}

tutorial.addStep(selectOkButtonInCompletionBoxStep);

//Step triggerSignalNameCompletionBox
triggerSignalNameCompletionBoxStep = ktutorial.newStep("triggerSignalNameCompletionBox");
triggerSignalNameCompletionBoxStep.setText(t.i18nc("@info", "<para>The signal name field also provides completion when the target application is running and the emitter name was set.</para>\n\n<para>Note that the completion list can also be triggered using the standard shortcut <shortcut>Ctrl+T</shortcut> when the focus is on a text edition widget. Click on the <link url=\"widget:signalNameLineEdit\">signal name field</link> to give it the focus, then press <shortcut>Ctrl+T</shortcut> to show all the signals that can be emitted by the <interface>Ok</interface> button.</para>"));

function triggerSignalNameCompletionBoxStepSetup(step) {
    waitForSubstringCompletionBySignalNameLineEdit = ktutorial.newWaitFor("WaitForSignal");
    waitForSubstringCompletionBySignalNameLineEdit.setSignal(ktutorial.findObject("signalNameLineEdit"), "substringCompletion(QString)");
    step.addWaitFor(waitForSubstringCompletionBySignalNameLineEdit, "selectClickedSignalInCompletionBox");
}
connect(triggerSignalNameCompletionBoxStep, "setup(QObject*)",
        this, "triggerSignalNameCompletionBoxStepSetup(QObject*)");

tutorial.addStep(triggerSignalNameCompletionBoxStep);

//Step selectClickedSignalInCompletionBox
selectClickedSignalInCompletionBoxStep = ktutorial.newStep("selectClickedSignalInCompletionBox");
selectClickedSignalInCompletionBoxStep.setText(t.i18nc("@info", "<para>As it was explained before, we want to react when the <interface>Ok</interface> button is clicked, so select the <emphasis>clicked()</emphasis> signal from the completion list.</para>"));

function selectClickedSignalInCompletionBoxStepSetup(step) {
    waitForTextChangedBySignalNameLineEdit = ktutorial.newWaitFor("WaitForSignal");
    waitForTextChangedBySignalNameLineEdit.setSignal(ktutorial.findObject("signalNameLineEdit"), "textChanged(QString)");
    step.addWaitFor(waitForTextChangedBySignalNameLineEdit, self, "selectClickedSignalInCompletionBoxStepWaitForTextChangedBySignalNameLineEditConditionMet()");
}
connect(selectClickedSignalInCompletionBoxStep, "setup(QObject*)",
        this, "selectClickedSignalInCompletionBoxStepSetup(QObject*)");

function selectClickedSignalInCompletionBoxStepWaitForTextChangedBySignalNameLineEditConditionMet() {
    signalNameLineEdit = ktutorial.findObject("signalNameLineEdit");
    if (signalNameLineEdit.text == "clicked()") {
        tutorial.nextStep("acceptEditWaitForSignalDialogForAcceptDialogStepReaction");
    }
}

tutorial.addStep(selectClickedSignalInCompletionBoxStep);

//Step acceptEditWaitForSignalDialogForAcceptDialogStepReaction
acceptEditWaitForSignalDialogForAcceptDialogStepReactionStep = ktutorial.newStep("acceptEditWaitForSignalDialogForAcceptDialogStepReaction");
acceptEditWaitForSignalDialogForAcceptDialogStepReactionStep.setText(t.i18nc("@info", "<para>Now that the emitter and signal names were set <link url=\"widget:editWaitForSignalDialog/okButton\">accept the dialog</link>.</para>"));

function acceptEditWaitForSignalDialogForAcceptDialogStepReactionStepSetup(step) {
    waitForClickedByEditWaitForSignalDialogOkButton = ktutorial.newWaitFor("WaitForSignal");
    waitForClickedByEditWaitForSignalDialogOkButton.setSignal(ktutorial.findObject("editWaitForSignalDialog/okButton"), "clicked()");
    step.addWaitFor(waitForClickedByEditWaitForSignalDialogOkButton, "setNextStepResponseInAcceptDialogStepReaction");
}
connect(acceptEditWaitForSignalDialogForAcceptDialogStepReactionStep, "setup(QObject*)",
        this, "acceptEditWaitForSignalDialogForAcceptDialogStepReactionStepSetup(QObject*)");

tutorial.addStep(acceptEditWaitForSignalDialogForAcceptDialogStepReactionStep);

//Step setNextStepResponseInAcceptDialogStepReaction
setNextStepResponseInAcceptDialogStepReactionStep = ktutorial.newStep("setNextStepResponseInAcceptDialogStepReaction");
setNextStepResponseInAcceptDialogStepReactionStep.setText(t.i18nc("@info", "<para>The user accepting the dialog to add a new step will make our example tutorial to change to its last step.</para>\n\n<para>Set the <link url=\"widget:responseStepLineEdit\">step to change to</link> to <emphasis>end</emphasis>.</para>"));

function setNextStepResponseInAcceptDialogStepReactionStepSetup(step) {
    waitForTextChangedByResponseStepLineEdit = ktutorial.newWaitFor("WaitForSignal");
    waitForTextChangedByResponseStepLineEdit.setSignal(ktutorial.findObject("responseStepLineEdit"), "textChanged(QString)");
    step.addWaitFor(waitForTextChangedByResponseStepLineEdit, self, "setNextStepResponseInAcceptDialogStepReactionStepWaitForTextChangedByResponseStepLineEditConditionMet()");
}
connect(setNextStepResponseInAcceptDialogStepReactionStep, "setup(QObject*)",
        this, "setNextStepResponseInAcceptDialogStepReactionStepSetup(QObject*)");

function setNextStepResponseInAcceptDialogStepReactionStepWaitForTextChangedByResponseStepLineEditConditionMet() {
    responseStepLineEdit = ktutorial.findObject("responseStepLineEdit");
    if (responseStepLineEdit.text == "end") {
        tutorial.nextStep("acceptAcceptDialogStepReactionDialog");
    }
}

tutorial.addStep(setNextStepResponseInAcceptDialogStepReactionStep);

//Step acceptAcceptDialogStepReactionDialog
acceptAcceptDialogStepReactionDialogStep = ktutorial.newStep("acceptAcceptDialogStepReactionDialog");
acceptAcceptDialogStepReactionDialogStep.setText(t.i18nc("@info", "<para>And finally <link url=\"widget:okButton\">accept the dialog</link>.</para>"));

function acceptAcceptDialogStepReactionDialogStepSetup(step) {
    waitForClickedByOkButton = ktutorial.newWaitFor("WaitForSignal");
    waitForClickedByOkButton.setSignal(ktutorial.findObject("okButton"), "clicked()");
    step.addWaitFor(waitForClickedByOkButton, "addEndStep");
}
connect(acceptAcceptDialogStepReactionDialogStep, "setup(QObject*)",
        this, "acceptAcceptDialogStepReactionDialogStepSetup(QObject*)");

tutorial.addStep(acceptAcceptDialogStepReactionDialogStep);

//Step addEndStep
addEndStepStep = ktutorial.newStep("addEndStep");
addEndStepStep.setText(t.i18nc("@info", "<para>The last step in the tutorial will just inform the user that the tutorial is done.</para>\n\n<para>As with the previous ones, we have to start by <link url=\"widget:addStepToolButton\">adding a new step</link>.</para>"));

function addEndStepStepSetup(step) {
    waitForStepDataDialogToBeShown = ktutorial.newWaitFor("WaitForWindow");
    waitForStepDataDialogToBeShown.setWindowObjectName("StepDataDialog");
    step.addWaitFor(waitForStepDataDialogToBeShown, "setEndStepId");
}
connect(addEndStepStep, "setup(QObject*)",
        this, "addEndStepStepSetup(QObject*)");

tutorial.addStep(addEndStepStep);

//Step setEndStepId
setEndStepIdStep = ktutorial.newStep("setEndStepId");
setEndStepIdStep.setText(t.i18nc("@info", "<para>Set the <link url=\"widget:idLineEdit\">id of the step</link> to <emphasis>end</emphasis>.</para>"));

function setEndStepIdStepSetup(step) {
    waitForTextChangedByIdLineEdit = ktutorial.newWaitFor("WaitForSignal");
    waitForTextChangedByIdLineEdit.setSignal(ktutorial.findObject("idLineEdit"), "textChanged(QString)");
    step.addWaitFor(waitForTextChangedByIdLineEdit, self, "setEndStepIdStepWaitForTextChangedByIdLineEditConditionMet()");
}
connect(setEndStepIdStep, "setup(QObject*)",
        this, "setEndStepIdStepSetup(QObject*)");

function setEndStepIdStepWaitForTextChangedByIdLineEditConditionMet() {
    idLineEdit = ktutorial.findObject("idLineEdit");
    if (idLineEdit.text == "end") {
        tutorial.nextStep("setEndStepText");
    }
}

tutorial.addStep(setEndStepIdStep);

//Step setEndStepText
setEndStepTextStep = ktutorial.newStep("setEndStepText");
setEndStepTextStep.setText(t.i18nc("@info", "<para>Set the <link url=\"widget:textTextEdit\">text of the step</link> to:</para>\n\n<para><emphasis>And that's all!</emphasis></para>"));

function setEndStepTextStepSetup(step) {
    waitForTextChangedByTextTextEdit = ktutorial.newWaitFor("WaitForSignal");
    waitForTextChangedByTextTextEdit.setSignal(ktutorial.findObject("textTextEdit"), "textChanged()");
    step.addWaitFor(waitForTextChangedByTextTextEdit, self, "setEndStepTextStepWaitForTextChangedByTextTextEditConditionMet()");
}
connect(setEndStepTextStep, "setup(QObject*)",
        this, "setEndStepTextStepSetup(QObject*)");

function setEndStepTextStepWaitForTextChangedByTextTextEditConditionMet() {
    textTextEdit = ktutorial.findObject("textTextEdit");
    if (textTextEdit.plainText.toLowerCase() == t.i18n("And that's all!").toLowerCase()) {
        tutorial.nextStep("acceptAddEndStepDialog");
    }
}

tutorial.addStep(setEndStepTextStep);

//Step acceptAddEndStepDialog
acceptAddEndStepDialogStep = ktutorial.newStep("acceptAddEndStepDialog");
acceptAddEndStepDialogStep.setText(t.i18nc("@info", "<para>And finally <link url=\"widget:okButton\">accept the dialog</link>.</para>"));

function acceptAddEndStepDialogStepSetup(step) {
    waitForClickedByOkButton = ktutorial.newWaitFor("WaitForSignal");
    waitForClickedByOkButton.setSignal(ktutorial.findObject("okButton"), "clicked()");
    step.addWaitFor(waitForClickedByOkButton, "testTutorial");
}
connect(acceptAddEndStepDialogStep, "setup(QObject*)",
        this, "acceptAddEndStepDialogStepSetup(QObject*)");

tutorial.addStep(acceptAddEndStepDialogStep);

//Step testTutorial
testTutorialStep = ktutorial.newStep("testTutorial");
testTutorialStep.setText(t.i18nc("@info", "<para>Well, now that we have added that last step the example tutorial is done. But, when designing a tutorial, how can we know that it does what we expect it to do?</para>\n\n<para><application>KTutorial editor</application> is able to start the tutorial being designed in its target application without having to manually export and install the tutorial. This way you can truly check that the tutorial will behave as you expect.</para>\n\n<para>When testing a tutorial you are advised to use a target application in a clean state, like you would do when using a tutorial from that application.</para>\n\n<para>So cancel the dialog to add a new step in the target application if you haven't done it yet. Then, to start in the target application the tutorial we have been editing click on <interface>Edit|Test tutorial</interface>, or the <interface>Test tutorial</interface> button in the toolbar.</para>"));

function testTutorialStepSetup(step) {
    waitForTriggeredByTestTutorial = ktutorial.newWaitFor("WaitForSignal");
    waitForTriggeredByTestTutorial.setSignal(ktutorial.findObject("testTutorial"), "triggered()");
    step.addWaitFor(waitForTriggeredByTestTutorial, "end");
}
connect(testTutorialStep, "setup(QObject*)",
        this, "testTutorialStepSetup(QObject*)");

tutorial.addStep(testTutorialStep);

//Step end
endStep = ktutorial.newStep("end");
endStep.setText(t.i18nc("@info", "<para>The tutorial shown in the target application is the one we have been designing. If you add a new step, it will change to the second step, and if you accept the dialog it will change to the last step.</para>\n\n<para>Note, however, that when a tutorial is tested it is not installed. The tutorials must be installed to be available in the tutorial list of the target application.</para>\n\n<para>And that's all, the tutorial about the interaction with the target application ends here.</para>"));

tutorial.addStep(endStep);

