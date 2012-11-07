# -*- coding: utf-8 -*-

###########################################################################
#   Copyright (C) 2009 by Daniel Calviño Sánchez <danxuliu@gmail.com>     #
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

tutorialName = t.i18nc("@info/plain", "Clear the text area")
programmingLanguage = t.i18nc("@item:intext Programming language", "Python")
tutorial.tutorialInformationAsObject().setName(t.i18nc("@info/plain Tutorial name (Programming language name)", "%1 (%2)", [tutorialName, programmingLanguage]))
tutorial.tutorialInformationAsObject().setDescription(t.i18nc("@info/plain", "This tutorial shows how to clean the text area with ease"))

#Step 1
startStep = ktutorial.newStep("start")
startStep.setText(t.i18nc("@info", "In this tutorial you will learn how to clear text in the test application.<nl/>First, write \"%1\" (without quotes) in the text area", [t.i18nc("@item:intext", "Hello world")]))

textArea = ktutorial.findObject("textArea")

waitForTextChanged = ktutorial.newWaitFor("WaitForSignal")
waitForTextChanged.setSignal(textArea, "textChanged()")
startStep.addWaitFor(waitForTextChanged, self, "startDone()")

def startDone():
    if textArea.plainText.lower() == t.i18nc("@item:intext", "Hello world").lower():
        tutorial.nextStep("clearText")

tutorial.addStep(startStep)

#Step 2
clearTextStep = ktutorial.newStep("clearText")
clearTextStep.setText(t.i18nc("@info", "Now, you must trigger the clear action.<nl/>To do this, you can click in <interface>File|Clear</interface>, or in the toolbar button called <interface>Clear</interface>"))

clearAction = ktutorial.findObject("clear")

waitForClearTriggered = ktutorial.newWaitFor("WaitForSignal")
waitForClearTriggered.setSignal(clearAction, "triggered(bool)")
clearTextStep.addWaitFor(waitForClearTriggered, "end")

tutorial.addStep(clearTextStep)

#Step 3
endStep = ktutorial.newStep("end")
endStep.setText(t.i18nc("@info", "And that's all.<nl/>Now you can close the tutorial."))

tutorial.addStep(endStep)
