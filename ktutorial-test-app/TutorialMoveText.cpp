/***************************************************************************
 *   Copyright (C) 2008 by Daniel Calviño Sánchez <danxuliu@gmail.com>     *
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

#include "TutorialMoveText.h"

#include <QMouseEvent>

#include <kaction.h>
#include <klocalizedstring.h>
#include <ktextedit.h>

#include <ktutorial/KTutorial.h>
#include <ktutorial/Option.h>
#include <ktutorial/Step.h>
#include <ktutorial/TutorialInformation.h>
#include <ktutorial/WaitForAnd.h>
#include <ktutorial/WaitForEvent.h>
#include <ktutorial/WaitForNot.h>
#include <ktutorial/WaitForSignal.h>

using ktutorial::KTutorial;
using ktutorial::Option;
using ktutorial::Step;
using ktutorial::TutorialInformation;
using ktutorial::WaitForEvent;
using ktutorial::WaitForSignal;

class WaitForLeftMouseButtonPressed: public WaitForEvent {
public:

    WaitForLeftMouseButtonPressed(QObject* object):
            WaitForEvent(object, QEvent::MouseButtonPress) {
    }

protected:

    virtual void handleEvent(QEvent* event) {
        QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(event);
        if (mouseEvent->button() == Qt::LeftButton) {
            WaitForEvent::handleEvent(event);
        }
    }

};

//public:

TutorialMoveText::TutorialMoveText(): Tutorial(0) {
    mTutorialInformation = new TutorialInformation("moveText");
    mTutorialInformation->setName(i18n("Move text in the text area"));
    mTutorialInformation->setDescription(i18n("This tutorial shows how to move text in the text area"));

    mTextArea = KTutorial::self()->findObject<KTextEdit*>("textArea");

    mSecondPath = false;

    //Step start
    Step* startStep = new Step("start");
    startStep->setText(i18nc("@info", "In this tutorial you will learn how to move text in the test application.<nl/>Text can be moved using the mouse or the keyboard. What do you prefer?"));

    startStep->addOption(new Option(i18n("Mouse")), this, SLOT(startMouseMove()));
    startStep->addOption(new Option(i18n("Keyboard")), this, SLOT(startKeyboardMove()));

    addStep(startStep);

    //Step write
    Step* writeStep = new Step("write");
    writeStep->setText(i18nc("@info", "First of all, clean the text area and write \"%1\" (without quotes)", i18n("Hello world")));

    writeStep->addWaitFor(new WaitForSignal(mTextArea, SIGNAL(textChanged())),
                          this, SLOT(write()));

    addStep(writeStep);

    //Step select
    Step* selectStep = new Step("select");
    selectStep->setText(i18nc("@info", "Now, select the text \"%1\" in the text area", i18n("world")));

    selectStep->addWaitFor(new WaitForSignal(mTextArea, SIGNAL(selectionChanged())),
                          this, SLOT(select()));

    addStep(selectStep);

    //Step keyboardCut
    Step* keyboardCutStep = new Step("keyboardCut");
    keyboardCutStep->setText(i18nc("@info", "The next thing to do is cut the selected text. Cutting it will remove it from the text area, but in some Steps we will make it appear again. To cut the selected text, click in <interface>Edit|Cut</interface> or in the button of the toolbar"));

    KAction* cutAction = KTutorial::self()->findObject<KAction*>("edit_cut");
    keyboardCutStep->addWaitFor(new WaitForSignal(cutAction, SIGNAL(triggered(bool))),
                                "keyboardMoveCursor");

    addStep(keyboardCutStep);

    //Step keyboardMoveCursor
    Step* keyboardMoveCursorStep = new Step("keyboardMoveCursor");
    keyboardMoveCursorStep->setText(i18nc("@info", "Move the cursor to the beginning of the document, before \"%1\" text", i18n("Hello")));

    keyboardMoveCursorStep->addWaitFor(new WaitForSignal(mTextArea, SIGNAL(cursorPositionChanged())),
                                       this, SLOT(keyboardMoveCursor()));

    addStep(keyboardMoveCursorStep);

    //Step keyboardPaste
    Step* keyboardPasteStep = new Step("keyboardPaste");
    keyboardPasteStep->setText(i18nc("@info", "Finally, paste the text you have cut previously. To do this, click in <interface>Edit|Paste</interface> or in the button in the toolbar"));

    KAction* pasteAction = KTutorial::self()->findObject<KAction*>("edit_paste");
    keyboardPasteStep->addWaitFor(new WaitForSignal(pasteAction, SIGNAL(triggered(bool))),
                                  this, SLOT(keyboardPaste()));

    addStep(keyboardPasteStep);

    //Step mousePress
    Step* mousePressStep = new Step("mousePress");
    mousePressStep->setText(i18nc("@info", "Press with the left button of the mouse on the selected text. You must press on it, or the selection will change, and you will have to select it again. Don't release the button yet."));

    mousePressStep->addWaitFor(new WaitForLeftMouseButtonPressed(mTextArea->viewport()),
                               "mouseRelease");

    addStep(mousePressStep);

    //Step mouseRelease
    Step* mouseReleaseStep = new Step("mouseRelease");
    mouseReleaseStep->setText(i18nc("@info", "Without releasing the mouse button, move the cursor to the desired position, for example the beginning of the text.<nl/>Once the cursor is there, you can release the mouse button, and the text will be moved."));

    mouseReleaseStep->addWaitFor(new WaitForEvent(mTextArea->viewport(), QEvent::Drop),
                                 this, SLOT(mouseRelease()));

    addStep(mouseReleaseStep);

    //Step showOtherWay
    Step* showOtherWayStep = new Step("showOtherWay");
    showOtherWayStep->setText(i18nc("@info", "As explained, there are two ways to move text in a text area: using the mouse and using the keyboard. Do you want to see the other way?"));

    showOtherWayStep->addOption(new Option(i18n("Yes, please")), this, SLOT(showOtherWay()));
    showOtherWayStep->addOption(new Option(i18n("No, thanks")), "end");

    addStep(showOtherWayStep);

    //Step end
    Step* endStep = new Step("end");
    endStep->setText(i18nc("@info", "That's all.<nl/>Now you can close the tutorial."));

    addStep(endStep);
}

//public slots:

void TutorialMoveText::startKeyboardMove() {
    mMousePathSelected = false;
    nextStep("write");
}

void TutorialMoveText::startMouseMove() {
    mMousePathSelected = true;
    nextStep("write");
}

void TutorialMoveText::write() {
    if (mTextArea->toPlainText().compare(i18n("Hello world"),
                                         Qt::CaseInsensitive) == 0) {
        nextStep("select");
    }
}

void TutorialMoveText::select(){
    if (mTextArea->textCursor().selectedText().compare(i18n("world"),
                                                       Qt::CaseInsensitive) == 0) {
        if (mMousePathSelected) {
            nextStep("mousePress");
        } else {
            nextStep("keyboardCut");
        }
    }
}

void TutorialMoveText::keyboardMoveCursor() {
    if (mTextArea->textCursor().position() == 0) {
        nextStep("keyboardPaste");
    }
}

void TutorialMoveText::keyboardPaste() {
    if (mSecondPath) {
        nextStep("end");
    } else {
        nextStep("showOtherWay");
    }
}

void TutorialMoveText::mouseRelease() {
    if (mSecondPath) {
        nextStep("end");
    } else {
        nextStep("showOtherWay");
    }
}

void TutorialMoveText::showOtherWay() {
    mSecondPath = true;

    mTextArea->clear();

    mMousePathSelected = !mMousePathSelected;
    nextStep("write");
}

//protected:

void TutorialMoveText::setup() {
    mSecondPath = false;
}
