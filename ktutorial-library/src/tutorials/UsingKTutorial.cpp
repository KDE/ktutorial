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

#include "UsingKTutorial.h"

#include <QMouseEvent>

#include <KLocalizedString>
#include <KTextEdit>

#include "../KTutorial.h"
#include "../Option.h"
#include "../Step.h"
#include "../TutorialInformation.h"
#include "../WaitForEvent.h"
#include "../WaitForSignal.h"
#include "../view/StepTextWidget.h"
#include "../view/StepWidget.h"

namespace ktutorial {

class WaitForLeftMouseButtonPressed: public WaitForEvent {
Q_OBJECT
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

class ClearTextStep: public Step {
public:

    ClearTextStep(UsingKTutorial* usingKTutorial):
            Step("clearText"),
        mUsingKTutorial(usingKTutorial) {
    }

    virtual void setup() {
        QTextEdit* textEdit = //krazy:exclude=qclasses
                    new KTextEdit(KTutorial::self()->mainApplicationWindow());
        textEdit->setAttribute(Qt::WA_DeleteOnClose);
        textEdit->setWindowFlags(Qt::Window | Qt::WindowStaysOnTopHint);
        textEdit->setObjectName("usingKTutorialTextEdit");
        textEdit->setText(i18nc("@info/plain Plain text in a KTextEdit",
                                "Look at me! I am the text area!"));
        textEdit->show();

        addWaitFor(new WaitForSignal(textEdit, SIGNAL(textChanged())),
                   mUsingKTutorial, SLOT(clearTextTextModified()));
    }

private:

    UsingKTutorial* mUsingKTutorial;

};

class HighlightTextEditStep: public Step {
Q_OBJECT
public:

    HighlightTextEditStep():
            Step("highlightTextEdit") {
    }

    virtual void setup() {
        KTextEdit* textEdit = KTutorial::self()->findObject<KTextEdit*>(
                                                    "usingKTutorialTextEdit");

        addWaitFor(new WaitForEvent(textEdit, QEvent::Close),
                   "moveWidgetPress");
    }

};

class MoveWidgetPressStep: public Step {
Q_OBJECT
public:

    MoveWidgetPressStep():
            Step("moveWidgetPress") {
    }

    virtual void setup() {
        QWidget* mStepWidget = KTutorial::self()->
                        findObject<view::StepWidget*>("ktutorial_StepWidget");

        addWaitFor(new WaitForLeftMouseButtonPressed(mStepWidget),
                   "moveWidgetRelease");
    }

};

class MoveWidgetReleaseStep: public Step {
Q_OBJECT
public:

    MoveWidgetReleaseStep():
            Step("moveWidgetRelease") {
    }

    virtual void setup() {
        QWidget* mStepWidget = KTutorial::self()->
                        findObject<view::StepWidget*>("ktutorial_StepWidget");

        addWaitFor(new WaitForEvent(mStepWidget, QEvent::MouseButtonRelease),
                   "end");
    }

};

//public:

UsingKTutorial::UsingKTutorial(): Tutorial(0) {
    TutorialInformation* tutorialInformation =
                                    new TutorialInformation("usingKTutorial");
    tutorialInformation->setName(i18nc("@info/plain", "Using the tutorials"));
    tutorialInformation->setDescription(i18nc("@info/plain", "This tutorial "
                                               "shows how the tutorial system "
                                               "works"));
    setTutorialInformation(tutorialInformation);

    //Step start
    Step* startStep = new Step("start");
    startStep->setText(i18nc("@info",
"<para>Welcome to the tutorial to learn how to use tutorials ;)</para>"
"<para>But, what is a tutorial? A tutorial is a little guide to help you to "
"learn how to use an application. For example, it shows you how some feature "
"works, or how to accomplish some task.</para>"));

    startStep->addOption(new Option(i18nc("@action", "Continue")),
                         "singleOption");

    addStep(startStep);

    //Step singleOption
    Step* singleOptionStep = new Step("singleOption");
    singleOptionStep->setText(i18nc("@info",
"<para>A tutorial is composed of several steps. Each step contains a bit of "
"information.</para>"
"<para>There are several ways to advance from one step to another. For "
"example, clicking on the button below the tutorial will change to the next "
"step.</para>"));

    singleOptionStep->addOption(new Option(i18nc("@action", "Continue")),
                                "severalOptions");

    addStep(singleOptionStep);

    //Step severalOptions
    Step* severalOptionsStep = new Step("severalOptions");
    severalOptionsStep->setText(i18nc("@info",
"<para>In other steps you may have more than one option to select. This can be "
"used, for example, to follow one path or another in the tutorial, skip some "
"part of the tutorial, etcetera. Which option do you prefer?</para>"));

    severalOptionsStep->addOption(new Option(i18nc("@action", "Option 1")),
                                  "option1Selected");
    severalOptionsStep->addOption(new Option(i18nc("@action", "Option 2")),
                                  "option2Selected");

    addStep(severalOptionsStep);

    //Step option1Selected
    Step* option1SelectedStep = new Step("option1Selected");
    option1SelectedStep->setText(i18nc("@info",
"<para>You have selected <emphasis>%1</emphasis>. Do not worry, as this is "
"just an example, selecting one option or the other makes no difference.</para>"
"<para>In the next step you will learn the last way to advance from one step "
"to another. To show this to you I will open a new window where text can be "
"written.</para>", i18nc("@action", "Option 1")));

    option1SelectedStep->addOption(new Option(i18nc("@action", "Continue")),
                                   "clearText");

    addStep(option1SelectedStep);

    //Step option2Selected
    Step* option2SelectedStep = new Step("option2Selected");
    option2SelectedStep->setText(i18nc("@info",
"<para>You have selected <emphasis>%1</emphasis>. Do not worry, as this is "
"just an example, selecting one option or the other makes no difference.</para>"
"<para>In the next step you will learn the last way to advance from one step "
"to another. To show this to you I will open a new window where text can be "
"written.</para>", i18nc("@action", "Option 2")));

    option2SelectedStep->addOption(new Option(i18nc("@action", "Continue")),
                                   "clearText");

    addStep(option2SelectedStep);

    //Step clearText
    Step* clearTextStep = new ClearTextStep(this);
    clearTextStep->setText(i18nc("@info",
"<para>Do you see the new window that has appeared? Yes, the one that says "
"that it is the text area. Well, the last way to advance from one step to "
"another is just doing what you are asked for.</para>"
"<para>In this case, empty the text area erasing all its text and once you "
"have done it look again to the tutorial.</para>"));

    //WaitFor is added in step setup, as it has to create a QTextEdit when the
    //step is activated

    addStep(clearTextStep);

    //Step textCleared
    Step* textClearedStep = new Step("textCleared");
    textClearedStep->setText(i18nc("@info",
"<para>Do you see? You are in a new step, but you did not tell the tutorial to "
"continue to the next step, and neither you had to select between several "
"options. The tutorial advanced automatically when you erased the text as "
"requested. This will be the most common way to advance from one step to "
"another.</para>"));

    textClearedStep->addOption(new Option(i18nc("@action", "Continue")),
                               "highlightTextEdit");

    addStep(textClearedStep);

    //Step highlightTextEdit
    //As changing to another step stops the highlighting in the currently
    //highlighted widget, it is not possible to use one step to show how to
    //highlight the widget and the next one to show how to stop highlighting it
    Step* highlightTextEditStep = new HighlightTextEditStep();
    highlightTextEditStep->setText(i18nc("@info",
"<para>Sometimes, a tutorial may provide a link to a <emphasis>widget"
"</emphasis> (an element of the graphical user interface, like a button). For "
"example, <link url=\"widget:usingKTutorialTextEdit\">this is a link to the "
"text area</link>.</para>"
"<para>When you click on a link to a widget, the widget is highlighted to help "
"you to spot it. If you click again on it, the highlighting will be stopped. "
"In this example it is pretty clear what the text area refers to, but it can "
"be useful in tutorials for more complex applications.</para>"
"<para>Ok, close the window with the text area to continue with the "
"tutorial.</para>"));

    //WaitFor is added in step setup, as it uses an object that isn't available
    //when the tutorial is created

    addStep(highlightTextEditStep);

    //Step moveWidgetPress
    Step* moveWidgetPressStep = new MoveWidgetPressStep();
    moveWidgetPressStep->setText(i18nc("@info",
"<para>You may have noticed that the tutorial window has no border. Does that "
"mean that it can not be moved? Not at all. It can be dragged using the mouse "
"like any other window, but pressing in a different place. Let's see "
"how.</para>"
"<para>Above these paragraphs you can see an empty space, next to the button "
"at the corner. Press with the left button of your mouse on that empty space. "
"Just press, do not release the mouse button yet.</para>"));

    //WaitFor is added in step setup, as it uses an object that isn't available
    //when the tutorial is created

    addStep(moveWidgetPressStep);

    //Step moveWidgetRelease
    Step* moveWidgetReleaseStep = new MoveWidgetReleaseStep();
    moveWidgetReleaseStep->setText(i18nc("@info",
"<para>Now, and without releasing the button, move the mouse and the window "
"will be moved. Once you release the button, the window will be kept in the "
"place it was. Of course, you can move it again dragging it like you "
"have just done.</para>"));

    //WaitFor is added in step setup, as it uses an object that isn't available
    //when the tutorial is created

    addStep(moveWidgetReleaseStep);

    //Step end
    Step* endStep = new Step("end");
    endStep->setText(i18nc("@info",
"<para>And that's all. You can now close the tutorial. In fact, you could have "
"closed the tutorial in any other of the previous steps, but if you do that "
"when the tutorial is started again it will not remember what you did. It will "
"start from the beginning again.</para>"
"<para>But how do you close the tutorial? Well, do you see that button above "
"this text, in the corner, with an icon that looks like a close icon? Do you "
"guess what it does? ;) Just click on it and you will return to the tutorial "
"selection dialog. Bye!</para>"));

    addStep(endStep);
}

//public slots:

void UsingKTutorial::clearTextTextModified() {
    KTextEdit* textEdit = KTutorial::self()->
                            findObject<KTextEdit*>("usingKTutorialTextEdit");

    if (textEdit->toPlainText().isEmpty()) {
        nextStep("textCleared");
    }
}

//protected:

void UsingKTutorial::tearDown() {
    KTextEdit* textEdit = KTutorial::self()->findObject<KTextEdit*>(
                                                    "usingKTutorialTextEdit");
    if (textEdit) {
        textEdit->deleteLater();
    }
}

}

#include "moc_UsingKTutorial.cpp"
#include "UsingKTutorial.moc"
