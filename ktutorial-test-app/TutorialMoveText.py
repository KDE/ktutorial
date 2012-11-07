# -*- coding: utf-8 -*-

###########################################################################
#   Copyright (C) 2010 by Daniel Calviño Sánchez <danxuliu@gmail.com>     #
#   Copyright (C) 2012 by Daniel Calviño Sánchez <danxuliu@gmail.com>     #
#                                                                         #
#   This program is free software; you can redistribute it and/or modify  #
#   it under the terms of the GNU General Public License as published by  #
#   the Free Software Foundation; either version 2 of the License, or     #
#   (at your option) any later version.                                   #
#                                                                         #
#   This program is distributed in the hope that it will be useful,       #
#   but WITHOUT ANY WARRANTY; without even the implied warranty of        #
#   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         #
#   GNU General Public License for more details.                          #
#                                                                         #
#   You should have received a copy of the GNU General Public License     #
#   along with this program; If not, see <http://www.gnu.org/licenses/>.  #
###########################################################################

import Kross
t = Kross.module("kdetranslation")

tutorialName = t.i18nc("@info/plain", "Move text in the text area")
programmingLanguage = t.i18nc("@item:intext Programming language", "Python")
tutorial.tutorialInformationAsObject().setName(t.i18nc("@info/plain Tutorial name (Programming language name)", "%1 (%2)", [tutorialName, programmingLanguage]))
tutorial.tutorialInformationAsObject().setDescription(t.i18nc("@info/plain", "This tutorial shows how to move text in the text area"))

textArea = ktutorial.findObject("textArea")

import sip
from PyQt4 import QtCore, Qt
textAreaPyQt = sip.wrapinstance(textArea.__toPointer__(), Qt.QTextEdit)

import string
version = string.split(QtCore.PYQT_VERSION_STR, ".")
if map(int, version) < [4, 5]:
    raise ImportWarning('PyQt version is lower than 4.5, can not load "Move text in the text area (Python)" tutorial')

class MouseFilter(Qt.QObject):
    mousePressed = QtCore.pyqtSignal()
    mouseReleased = QtCore.pyqtSignal()

    def eventFilter(self, object, event):
        if event.type() == QtCore.QEvent.MouseButtonPress:
            if event.button() == QtCore.Qt.LeftButton:
                self.mousePressed.emit()
        elif event.type() == QtCore.QEvent.Drop:
                self.mouseReleased.emit()
        return False

mouseFilter = MouseFilter()
textAreaPyQt.viewport().installEventFilter(mouseFilter)

# Dictionaries are mutable, so they can be used in functions without declaring
# them as global. Boolean values, instead, need to be declared inside the
# function before being used (global variableName)
flags = { "secondPath":False, "mousePathSelected":False }

def tutorialSetup(tutorial):
    flags["secondPath"] = False
    flags["mousePathSelected"] = False

tutorial.connect("setup(QObject*)", tutorialSetup)

#Step start
startStep = ktutorial.newStep("start")
startStep.setText(t.i18nc("@info", "In this tutorial you will learn how to move text in the test application.<nl/>Text can be moved using the mouse or the keyboard. What do you prefer?"))

startStep.addOption(ktutorial.newOption(t.i18nc("@action", "Mouse")), self, "startMouseMove()")

def startMouseMove():
    flags["mousePathSelected"] = True
    tutorial.nextStep("write")

startStep.addOption(ktutorial.newOption(t.i18nc("@action", "Keyboard")), self, "startKeyboardMove()")

def startKeyboardMove():
    flags["mousePathSelected"] = False
    tutorial.nextStep("write")

tutorial.addStep(startStep)

#Step write
writeStep = ktutorial.newStep("write")
writeStep.setText(t.i18nc("@info", "First of all, clean the text area and write \"%1\" (without quotes)", [t.i18nc("@item:intext", "Hello world")]))

waitForTextChanged = ktutorial.newWaitFor("WaitForSignal")
waitForTextChanged.setSignal(textArea, "textChanged()")
writeStep.addWaitFor(waitForTextChanged, self, "write()")

def write():
    if textArea.plainText.lower() == t.i18nc("@item:intext", "Hello world").lower():
        tutorial.nextStep("select")

tutorial.addStep(writeStep)

#Step select
selectStep = ktutorial.newStep("select")
selectStep.setText(t.i18nc("@info", "Now, select the text \"%1\" in the text area", [t.i18nc("@item:intext As in 'Hello world'", "world")]))

waitForSelectionChanged = ktutorial.newWaitFor("WaitForSignal")
waitForSelectionChanged.setSignal(textArea, "selectionChanged()")
selectStep.addWaitFor(waitForSelectionChanged, self, "select()")

def select():
    if textAreaPyQt.textCursor().selectedText().toLower() == t.i18nc("@item:intext As in 'Hello world'", "world").lower():
        if flags["mousePathSelected"]:
            tutorial.nextStep("mousePress")
        else:
            tutorial.nextStep("keyboardCut")

tutorial.addStep(selectStep)

#Step keyboardCut
keyboardCutStep = ktutorial.newStep("keyboardCut")
keyboardCutStep.setText(t.i18nc("@info", "The next thing to do is cut the selected text. Cutting it will remove it from the text area, but in some Steps we will make it appear again. To cut the selected text, click in <interface>Edit|Cut</interface> or in the button of the toolbar"))

cutAction = ktutorial.findObject("edit_cut")
waitForCutTriggered = ktutorial.newWaitFor("WaitForSignal")
waitForCutTriggered.setSignal(cutAction, "triggered(bool)")
keyboardCutStep.addWaitFor(waitForCutTriggered, "keyboardMoveCursor")

tutorial.addStep(keyboardCutStep)

#Step keyboardMoveCursor
keyboardMoveCursorStep = ktutorial.newStep("keyboardMoveCursor")
keyboardMoveCursorStep.setText(t.i18nc("@info", "Move the cursor to the beginning of the document, before \"%1\" text", [t.i18nc("@item:intext As in 'Hello world'", "Hello")]))

waitForCursorPositionChanged = ktutorial.newWaitFor("WaitForSignal")
waitForCursorPositionChanged.setSignal(textArea, "cursorPositionChanged()")
keyboardMoveCursorStep.addWaitFor(waitForCursorPositionChanged, self, "keyboardMoveCursor()")

def keyboardMoveCursor():
    if textAreaPyQt.textCursor().position() == 0:
        tutorial.nextStep("keyboardPaste")

tutorial.addStep(keyboardMoveCursorStep)

#Step keyboardPaste
keyboardPasteStep = ktutorial.newStep("keyboardPaste")
keyboardPasteStep.setText(t.i18nc("@info", "Finally, paste the text you have cut previously. To do this, click in <interface>Edit|Paste</interface> or in the button in the toolbar"))

pasteAction = ktutorial.findObject("edit_paste")
waitForPasteTriggered = ktutorial.newWaitFor("WaitForSignal")
waitForPasteTriggered.setSignal(pasteAction, "triggered(bool)")
keyboardPasteStep.addWaitFor(waitForPasteTriggered, self, "keyboardPaste()")

def keyboardPaste():
    if flags["secondPath"]:
        tutorial.nextStep("end")
    else:
        tutorial.nextStep("showOtherWay")

tutorial.addStep(keyboardPasteStep)

#Step mousePress
mousePressStep = ktutorial.newStep("mousePress")
mousePressStep.setText(t.i18nc("@info", "<para><emphasis>Warning:</emphasis> due to <link url=\"https://bugs.kde.org/show_bug.cgi?id=219787\">Kross bug 219787</link>, this step will crash KTutorial test app. Hopefully, soon the patch given in the bug will be applied and there will be no more crashes ;)</para>\
<para>If you are wondering why a tutorial that crashes was included in KTutorial test app, the answer is because although it crashes, it also contains examples of advanced tutorial scripting using Python.</para>\
<para>Press with the left button of the mouse on the selected text. You must press on it, or the selection will change, and you will have to select it again. Do not release the button yet.</para>"))

waitForMousePressed = ktutorial.newWaitFor("WaitForSignal")
waitForMousePressed.setSignal(mouseFilter, "mousePressed()")
mousePressStep.addWaitFor(waitForMousePressed, "mouseRelease")

tutorial.addStep(mousePressStep)

#Step mouseRelease
mouseReleaseStep = ktutorial.newStep("mouseRelease")
mouseReleaseStep.setText(t.i18nc("@info", "Without releasing the mouse button, move the cursor to the desired position, for example the beginning of the text.<nl/>Once the cursor is there, you can release the mouse button, and the text will be moved."))

waitForMouseReleased = ktutorial.newWaitFor("WaitForSignal")
waitForMouseReleased.setSignal(mouseFilter, "mouseReleased()")
mouseReleaseStep.addWaitFor(waitForMouseReleased, self, "mouseRelease()")

def mouseRelease():
    if flags["secondPath"]:
        tutorial.nextStep("end")
    else:
        tutorial.nextStep("showOtherWay")

tutorial.addStep(mouseReleaseStep)

#Step showOtherWay
showOtherWayStep = ktutorial.newStep("showOtherWay")
showOtherWayStep.setText(t.i18nc("@info", "As explained, there are two ways to move text in a text area: using the mouse and using the keyboard. Do you want to see the other way?"))

showOtherWayStep.addOption(ktutorial.newOption(t.i18nc("@action", "Yes, please")), self, "showOtherWay()")

def showOtherWay():
    flags["secondPath"] = True

    textArea.clear()

    flags["mousePathSelected"] = not flags["mousePathSelected"]
    tutorial.nextStep("write")

showOtherWayStep.addOption(ktutorial.newOption(t.i18nc("@action", "No, thanks")), "end")

tutorial.addStep(showOtherWayStep)

#Step end
endStep = ktutorial.newStep("end")
endStep.setText(t.i18nc("@info", "That's all.<nl/>Now you can close the tutorial."))

tutorial.addStep(endStep)
