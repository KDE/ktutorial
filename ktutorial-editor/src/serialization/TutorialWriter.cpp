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

//public:

TutorialWriter::TutorialWriter(): mXmlWriter(0) {
}

QString TutorialWriter::writeTutorial(const Tutorial* tutorial) {
    QString serializedTutorial;

    mXmlWriter = new QXmlStreamWriter(&serializedTutorial);
    mXmlWriter->setAutoFormatting(true);
    mXmlWriter->setAutoFormattingIndent(4);

    mXmlWriter->writeStartDocument();
    mXmlWriter->writeStartElement("tutorial");

    if (!tutorial->name().isEmpty()) {
        mXmlWriter->writeAttribute("name", tutorial->name());
    }
    if (!tutorial->description().isEmpty()) {
        mXmlWriter->writeTextElement("description", tutorial->description());
    }
    if (!tutorial->licenseText().isEmpty()) {
        mXmlWriter->writeTextElement("license", tutorial->licenseText());
    }
    if (!tutorial->customSetupCode().isEmpty()) {
        mXmlWriter->writeTextElement("setup", tutorial->customSetupCode());
    }
    if (!tutorial->customTearDownCode().isEmpty()) {
        mXmlWriter->writeTextElement("tearDown",
                                     tutorial->customTearDownCode());
    }
    foreach (const Step* step, tutorial->steps()) {
        write(step);
    }

    mXmlWriter->writeEndElement();
    mXmlWriter->writeEndDocument();

    delete mXmlWriter;
    mXmlWriter = 0;

    //QXmlStreamWriter doesn't write the encoding information when a string is
    //used instead of a QIODevice
    serializedTutorial.replace("<?xml version=\"1.0\"?>\n",
                               "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n");
    return serializedTutorial;
}

//private:

void TutorialWriter::write(const Step* step) {
    mXmlWriter->writeStartElement("step");

    if (!step->id().isEmpty()) {
        mXmlWriter->writeAttribute("id", step->id());
    }
    if (!step->text().isEmpty()) {
        mXmlWriter->writeTextElement("text", step->text());
    }
    if (!step->customSetupCode().isEmpty()) {
        mXmlWriter->writeTextElement("setup", step->customSetupCode());
    }
    if (!step->customTearDownCode().isEmpty()) {
        mXmlWriter->writeTextElement("tearDown", step->customTearDownCode());
    }
    foreach (const Reaction* reaction, step->reactions()) {
        write(reaction);
    }

    mXmlWriter->writeEndElement();
}

void TutorialWriter::write(const Reaction* reaction) {
    mXmlWriter->writeStartElement("reaction");

    QString triggerTypeValue;
    if (reaction->triggerType() == Reaction::ConditionMet) {
        triggerTypeValue = "ConditionMet";
    } else {
        triggerTypeValue = "OptionSelected";
    }
    mXmlWriter->writeAttribute("triggerType", triggerTypeValue);

    QString responseTypeValue;
    if (reaction->responseType() == Reaction::CustomCode) {
        responseTypeValue = "CustomCode";
    } else {
        responseTypeValue = "NextStep";
    }
    mXmlWriter->writeAttribute("responseType", responseTypeValue);

    if (!reaction->optionName().isEmpty()) {
        mXmlWriter->writeEmptyElement("option");
        mXmlWriter->writeAttribute("name", reaction->optionName());
    }
    if (reaction->waitFor()) {
        write(reaction->waitFor());
    }
    if (!reaction->customCode().isEmpty()) {
        mXmlWriter->writeTextElement("customCode", reaction->customCode());
    }
    if (!reaction->nextStepId().isEmpty()) {
        mXmlWriter->writeEmptyElement("nextStep");
        mXmlWriter->writeAttribute("id", reaction->nextStepId());
    }

    mXmlWriter->writeEndElement();
}

void TutorialWriter::write(const WaitFor* waitFor) {
    if (qobject_cast<const WaitForComposed*>(waitFor)) {
        write(static_cast<const WaitForComposed*>(waitFor));
        return;
    }
    if (qobject_cast<const WaitForEvent*>(waitFor)) {
        write(static_cast<const WaitForEvent*>(waitFor));
        return;
    }
    if (qobject_cast<const WaitForNot*>(waitFor)) {
        write(static_cast<const WaitForNot*>(waitFor));
        return;
    }
    if (qobject_cast<const WaitForProperty*>(waitFor)) {
        write(static_cast<const WaitForProperty*>(waitFor));
        return;
    }
    if (qobject_cast<const WaitForSignal*>(waitFor)) {
        write(static_cast<const WaitForSignal*>(waitFor));
        return;
    }
    if (qobject_cast<const WaitForStepActivation*>(waitFor)) {
        write(static_cast<const WaitForStepActivation*>(waitFor));
        return;
    }
    if (qobject_cast<const WaitForWindow*>(waitFor)) {
        write(static_cast<const WaitForWindow*>(waitFor));
        return;
    }
}

void TutorialWriter::write(const WaitForComposed* waitForComposed) {
    mXmlWriter->writeStartElement("waitForComposed");

    QString compositionTypeValue;
    if (waitForComposed->compositionType() == WaitForComposed::And) {
        compositionTypeValue = "And";
    } else {
        compositionTypeValue = "Or";
    }
    mXmlWriter->writeAttribute("compositionType", compositionTypeValue);

    foreach (const WaitFor* waitFor, waitForComposed->waitFors()) {
        write(waitFor);
    }

    mXmlWriter->writeEndElement();
}

void TutorialWriter::write(const WaitForEvent* waitForEvent) {
    mXmlWriter->writeEmptyElement("waitForEvent");

    if (!waitForEvent->receiverName().isEmpty()) {
        mXmlWriter->writeAttribute("receiverName",
                                   waitForEvent->receiverName());
    }
    if (!waitForEvent->eventName().isEmpty()) {
        mXmlWriter->writeAttribute("eventName", waitForEvent->eventName());
    }
}

void TutorialWriter::write(const WaitForNot* waitForNot) {
    mXmlWriter->writeStartElement("waitForNot");

    if (waitForNot->negatedWaitFor()) {
        write(waitForNot->negatedWaitFor());
    }

    mXmlWriter->writeEndElement();
}

void TutorialWriter::write(const WaitForProperty* waitForProperty) {
    mXmlWriter->writeEmptyElement("waitForProperty");

    if (!waitForProperty->objectName().isEmpty()) {
        mXmlWriter->writeAttribute("objectName", waitForProperty->objectName());
    }
    if (!waitForProperty->propertyName().isEmpty()) {
        mXmlWriter->writeAttribute("propertyName",
                                   waitForProperty->propertyName());
    }
    if (!waitForProperty->value().isEmpty()) {
        mXmlWriter->writeAttribute("value", waitForProperty->value());
    }
}

void TutorialWriter::write(const WaitForSignal* waitForSignal) {
    mXmlWriter->writeEmptyElement("waitForSignal");

    if (!waitForSignal->emitterName().isEmpty()) {
        mXmlWriter->writeAttribute("emitterName", waitForSignal->emitterName());
    }
    if (!waitForSignal->signalName().isEmpty()) {
        mXmlWriter->writeAttribute("signalName", waitForSignal->signalName());
    }
}

void TutorialWriter::write(const WaitForStepActivation* waitForStepActivation) {
    Q_UNUSED(waitForStepActivation);

    mXmlWriter->writeEmptyElement("waitForStepActivation");
}

void TutorialWriter::write(const WaitForWindow* waitForWindow) {
    mXmlWriter->writeEmptyElement("waitForWindow");

    if (!waitForWindow->windowObjectName().isEmpty()) {
        mXmlWriter->writeAttribute("windowObjectName",
                                   waitForWindow->windowObjectName());
    }
}
