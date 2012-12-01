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

#include "TutorialClearText.h"

#include <klocalizedstring.h>
#include <ktextedit.h>

#include <ktutorial/KTutorial.h>
#include <ktutorial/Step.h>
#include <ktutorial/TutorialInformation.h>
#include <ktutorial/WaitForSignal.h>

using ktutorial::KTutorial;
using ktutorial::Step;
using ktutorial::TutorialInformation;
using ktutorial::WaitForSignal;

//public:

TutorialClearText::TutorialClearText(): Tutorial(0) {
    TutorialInformation* tutorialInformation = new TutorialInformation("clearText");
    tutorialInformation->setName(i18nc("@info/plain", "Clear the text area"));
    tutorialInformation->setDescription(i18nc("@info/plain", "This tutorial shows how to clean the text area with ease"));
    setTutorialInformation(tutorialInformation);

    //Step 1
    Step* startStep = new Step("start");
    startStep->setText(i18nc("@info", "In this tutorial you will learn how to clear text in the test application.<nl/>First, write \"%1\" (without quotes) in the text area", i18nc("@item:intext", "Hello world")));

    QObject* textArea = KTutorial::self()->findObject<QObject*>("textArea");
    startStep->addWaitFor(new WaitForSignal(textArea, SIGNAL(textChanged())),
                          this, SLOT(startDone()));

    addStep(startStep);

    //Step 2
    Step* clearTextStep = new Step("clearText");
    clearTextStep->setText(i18nc("@info", "Now, you must trigger the clear action.<nl/>To do this, you can click in <interface>File|Clear</interface>, or in the toolbar button called <interface>Clear</interface>"));

    QObject* clearAction = KTutorial::self()->findObject<QObject*>("clear");
    clearTextStep->addWaitFor(new WaitForSignal(clearAction, SIGNAL(triggered(bool))),
                              "end");

    addStep(clearTextStep);

    //Step 3
    Step* endStep = new Step("end");
    endStep->setText(i18nc("@info", "And that's all.<nl/>Now you can close the tutorial."));

    addStep(endStep);
}

//public slots:

void TutorialClearText::startDone() {
    KTextEdit* textArea = KTutorial::self()->findObject<KTextEdit*>("textArea");

    if (textArea->toPlainText().compare(i18nc("@item:intext", "Hello world"),
                                        Qt::CaseInsensitive) == 0) {
            nextStep("clearText");
    }
}
