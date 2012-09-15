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

#include "TutorialReader.h"

#include <QDomDocument>

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

//public:

TutorialReader::TutorialReader() {
}

Tutorial* TutorialReader::readTutorial(const QString& data)
throw (DeserializationException) {
    QDomDocument document;

    QString errorMessage;
    int errorLine;
    int errorColumn;

    if (!document.setContent(data, &errorMessage, &errorLine, &errorColumn)) {
        throw DeserializationException(i18n("XML document is not well formed: "
"%1, line %2, column %3", errorMessage, errorLine, errorColumn));
    }

    if (document.documentElement().tagName() != "tutorial") {
        throw DeserializationException(i18n("Unknown root element, "
"<emphasis>tutorial</emphasis> expected, got: %1",
document.documentElement().tagName()));
    }

    return readTutorial(document.documentElement());
}

//private:

Tutorial* TutorialReader::readTutorial(const QDomElement& element) {
    Tutorial* tutorial = new Tutorial();

    if (element.hasAttribute("name")) {
        tutorial->setName(element.attribute("name"));
    }

    QDomElement childElement = element.firstChildElement();
    while (!childElement.isNull()) {
        if (childElement.tagName() == "description") {
            tutorial->setDescription(childElement.text());
        } else if (childElement.tagName() == "license") {
            tutorial->setLicenseText(childElement.text());
        } else if (childElement.tagName() == "setup") {
            tutorial->setCustomSetupCode(childElement.text());
        } else if (childElement.tagName() == "tearDown") {
            tutorial->setCustomTearDownCode(childElement.text());
        } else if (childElement.tagName() == "step") {
            tutorial->addStep(readStep(childElement));
        }

        childElement = childElement.nextSiblingElement();
    }

    return tutorial;
}

Step* TutorialReader::readStep(const QDomElement& element) {
    Step* step = new Step;

    if (element.hasAttribute("id")) {
        step->setId(element.attribute("id"));
    }

    QDomElement childElement = element.firstChildElement();
    while (!childElement.isNull()) {
        if (childElement.tagName() == "text") {
            step->setText(childElement.text());
        } else if (childElement.tagName() == "setup") {
            step->setCustomSetupCode(childElement.text());
        } else if (childElement.tagName() == "tearDown") {
            step->setCustomTearDownCode(childElement.text());
        } else if (childElement.tagName() == "reaction") {
            step->addReaction(readReaction(childElement));
        }

        childElement = childElement.nextSiblingElement();
    }

    return step;
}

Reaction* TutorialReader::readReaction(const QDomElement& element) {
    Reaction* reaction = new Reaction();

    if (element.hasAttribute("triggerType")) {
        Reaction::TriggerType triggerType = Reaction::OptionSelected;
        if (element.attribute("triggerType") == "ConditionMet") {
            triggerType = Reaction::ConditionMet;
        }

        reaction->setTriggerType(triggerType);
    }

    if (element.hasAttribute("responseType")) {
        Reaction::ResponseType responseType = Reaction::NextStep;
        if (element.attribute("responseType") == "CustomCode") {
            responseType = Reaction::CustomCode;
        }

        reaction->setResponseType(responseType);
    }

    QDomElement childElement = element.firstChildElement();
    while (!childElement.isNull()) {
        if (childElement.tagName() == "option") {
            if (childElement.hasAttribute("name")) {
                reaction->setOptionName(childElement.attribute("name"));
            }
        } else if (isWaitForElement(childElement)) {
            reaction->setWaitFor(readWaitFor(childElement));
        } else if (childElement.tagName() == "customCode") {
            reaction->setCustomCode(childElement.text());
        } else if (childElement.tagName() == "nextStep") {
            if (childElement.hasAttribute("id")) {
                reaction->setNextStepId(childElement.attribute("id"));
            }
        }

        childElement = childElement.nextSiblingElement();
    }

    return reaction;
}

WaitFor* TutorialReader::readWaitFor(const QDomElement& element) {
    if (element.tagName() == "waitForComposed") {
        return readWaitForComposed(element);
    }
    if (element.tagName() == "waitForEvent") {
        return readWaitForEvent(element);
    }
    if (element.tagName() == "waitForNot") {
        return readWaitForNot(element);
    }
    if (element.tagName() == "waitForProperty") {
        return readWaitForProperty(element);
    }
    if (element.tagName() == "waitForSignal") {
        return readWaitForSignal(element);
    }
    if (element.tagName() == "waitForStepActivation") {
        return readWaitForStepActivation(element);
    }
    if (element.tagName() == "waitForWindow") {
        return readWaitForWindow(element);
    }

    Q_ASSERT(false);
    return 0;
}

WaitFor* TutorialReader::readWaitForComposed(const QDomElement& element) {
    WaitForComposed* waitForComposed = new WaitForComposed();

    if (element.hasAttribute("compositionType")) {
        WaitForComposed::CompositionType compositionType = WaitForComposed::And;
        if (element.attribute("compositionType") == "Or") {
            compositionType = WaitForComposed::Or;
        }

        waitForComposed->setCompositionType(compositionType);
    }

    QDomElement childElement = element.firstChildElement();
    while (!childElement.isNull()) {
        if (isWaitForElement(childElement)) {
            waitForComposed->addWaitFor(readWaitFor(childElement));
        }

        childElement = childElement.nextSiblingElement();
    }

    return waitForComposed;
}

WaitFor* TutorialReader::readWaitForEvent(const QDomElement& element) {
    WaitForEvent* waitForEvent = new WaitForEvent();

    if (element.hasAttribute("receiverName")) {
        waitForEvent->setReceiverName(element.attribute("receiverName"));
    }
    if (element.hasAttribute("eventName")) {
        waitForEvent->setEventName(element.attribute("eventName"));
    }

    return waitForEvent;
}

WaitFor* TutorialReader::readWaitForNot(const QDomElement& element) {
    WaitForNot* waitForNot = new WaitForNot();

    QDomElement childElement = element.firstChildElement();
    while (!childElement.isNull()) {
        if (isWaitForElement(childElement)) {
            delete waitForNot->negatedWaitFor();
            waitForNot->setNegatedWaitFor(readWaitFor(childElement));
        }

        childElement = childElement.nextSiblingElement();
    }

    return waitForNot;
}

WaitFor* TutorialReader::readWaitForProperty(const QDomElement& element) {
    WaitForProperty* waitForProperty = new WaitForProperty();

    if (element.hasAttribute("objectName")) {
        waitForProperty->setObjectName(element.attribute("objectName"));
    }
    if (element.hasAttribute("propertyName")) {
        waitForProperty->setPropertyName(element.attribute("propertyName"));
    }
    if (element.hasAttribute("value")) {
        waitForProperty->setValue(element.attribute("value"));
    }

    return waitForProperty;
}

WaitFor* TutorialReader::readWaitForSignal(const QDomElement& element) {
    WaitForSignal* waitForSignal = new WaitForSignal();

    if (element.hasAttribute("emitterName")) {
        waitForSignal->setEmitterName(element.attribute("emitterName"));
    }
    if (element.hasAttribute("signalName")) {
        waitForSignal->setSignalName(element.attribute("signalName"));
    }

    return waitForSignal;
}

WaitFor* TutorialReader::readWaitForStepActivation(const QDomElement& element) {
    Q_UNUSED(element);

    return new WaitForStepActivation();
}

WaitFor* TutorialReader::readWaitForWindow(const QDomElement& element) {
    WaitForWindow* waitForWindow = new WaitForWindow();

    if (element.hasAttribute("windowObjectName")) {
        waitForWindow->setWindowObjectName(
                                        element.attribute("windowObjectName"));
    }

    return waitForWindow;
}

bool TutorialReader::isWaitForElement(const QDomElement& element) {
    if (element.tagName() != "waitForComposed" &&
            element.tagName() != "waitForEvent" &&
            element.tagName() != "waitForNot" &&
            element.tagName() != "waitForProperty" &&
            element.tagName() != "waitForSignal" &&
            element.tagName() != "waitForStepActivation" &&
            element.tagName() != "waitForWindow") {
        return false;
    }

    return true;
}
