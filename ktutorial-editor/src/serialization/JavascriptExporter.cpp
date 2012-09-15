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

#include "JavascriptExporter.h"

#include <QRegExp>
#include <QSet>
#include <QStringList>
#include <QTextDocument>

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

JavascriptExporter::JavascriptExporter(): mIndentationLevel(0) {
}

QString JavascriptExporter::exportTutorial(const Tutorial* tutorial) {
    QString code;
    mOut.setString(&code);

    writeLicense(tutorial);
    out() << "t = Kross.module(\"kdetranslation\");\n\n";
    writeInformation(tutorial);
    writeSetup(tutorial);
    writeTearDown(tutorial);

    foreach (Step* step, tutorial->steps()) {
        writeStep(step);
    }

    mOut.setString(0);
    return code;
}

//private:

void JavascriptExporter::writeLicense(const Tutorial* tutorial) {
    if (tutorial->licenseText().isEmpty()) {
        return;
    }

    QStringList lines = tutorial->licenseText().split('\n');
    int maximumLineLength = 0;
    foreach (const QString& line, lines) {
        if (line.size() > maximumLineLength) {
            maximumLineLength = line.size();
        }
    }

    out() << "/****" << QString('*').repeated(maximumLineLength) << "****\n";
    foreach (QString line, lines) {
        int numberOfSpaces = maximumLineLength - line.length();
        QString filling = QString(' ').repeated(numberOfSpaces);
        out() << " *   " + line + filling + "   *\n";
    }
    out() << " ****" << QString('*').repeated(maximumLineLength) << "****/\n\n";
}

void JavascriptExporter::writeInformation(const Tutorial* tutorial) {
    if (tutorial->name().isEmpty()) {
        out() << "//Error: Tutorial without name!\n";
    } else {
        out() << "tutorial.tutorialInformationAsObject().setName(t.i18n(\""
              << escape(tutorial->name()) <<"\"));\n";
    }

    if (tutorial->description().isEmpty()) {
        out() << "//Error: Tutorial without description!\n";
    } else {
        out() << "tutorial.tutorialInformationAsObject().setDescription("
              << "t.i18n(\"" << escape(tutorial->description()) <<"\"));\n";
    }

    mOut << "\n";
}

void JavascriptExporter::writeSetup(const Tutorial* tutorial) {
    if (tutorial->customSetupCode().isEmpty()) {
        return;
    }

    out() << "function tutorialSetup(tutorial) {\n";
    mIndentationLevel++;
    mOut << toIndentedCode(tutorial->customSetupCode());
    mIndentationLevel--;
    out() << "}\n";
    out() << "connect(tutorial, \"setup(QObject*)\",\n"
          << "        this, \"tutorialSetup(QObject*)\");\n\n";
}

void JavascriptExporter::writeTearDown(const Tutorial* tutorial) {
    if (tutorial->customTearDownCode().isEmpty()) {
        return;
    }

    out() << "function tutorialTearDown(tutorial) {\n";
    mIndentationLevel++;
    mOut << toIndentedCode(tutorial->customTearDownCode());
    mIndentationLevel--;
    out() << "}\n";
    out() << "connect(tutorial, \"tearDown(QObject*)\",\n"
          << "        this, \"tutorialTearDown(QObject*)\");\n\n";
}

void JavascriptExporter::writeStep(const Step* step) {
    if (step->id().isEmpty()) {
        out() << "//Error: Step without id!\n\n";
        return;
    }

    out() << "//Step " << step->id() << "\n";

    QString stepVariable = toLowerCamelCase(step->id()) + "Step";
    out() << stepVariable << " = ktutorial.newStep(\"" << escape(step->id())
          << "\");\n";

    if (step->text().isEmpty()) {
        out() << "//Error: Step without text!\n";
    } else if (Qt::mightBeRichText(step->text()) ||
                   mightContainSemanticMarkup(step->text())) {
        out() << stepVariable << ".setText(t.i18nc(\"@info\", \""
              << escape(step->text()) << "\"));\n";
    } else {
        out() << stepVariable << ".setText(t.i18nc(\"@info/plain\", \""
              << escape(step->text()) << "\"));\n";
    }
    mOut << '\n';

    writeSetup(step);
    writeTearDown(step);

    out() << "tutorial.addStep(" << stepVariable << ");\n\n";
}

void JavascriptExporter::writeSetup(const Step* step) {
    if (step->customSetupCode().isEmpty() && step->reactions().count() == 0) {
        return;
    }

    QString stepVariable = toLowerCamelCase(step->id()) + "Step";
    out() << "function " << stepVariable << "Setup(step) {\n";
    mIndentationLevel++;

    for (int i=0; i<step->reactions().count(); ++i) {
        Reaction* reaction = step->reactions()[i];
        writeReaction(step, reaction);

        if (i<step->reactions().count() - 1 ||
                !step->customSetupCode().isEmpty()) {
            mOut << '\n';
        }
    }

    mOut << toIndentedCode(step->customSetupCode());
    mIndentationLevel--;
    out() << "}\n";
    out() << "connect(" << stepVariable << ", \"setup(QObject*)\",\n"
          << "        this, \"" << stepVariable << "Setup(QObject*)\");\n\n";

    foreach (const QString& function, mPendingFunctions) {
        mOut << function << '\n';
    }
    mPendingFunctions.clear();
    mVariables.clear();
}

void JavascriptExporter::writeTearDown(const Step* step) {
    if (step->customTearDownCode().isEmpty()) {
        return;
    }

    QString stepVariable = toLowerCamelCase(step->id()) + "Step";
    out() << "function " << stepVariable << "TearDown(step) {\n";
    mIndentationLevel++;
    mOut << toIndentedCode(step->customTearDownCode());
    mIndentationLevel--;
    out() << "}\n";
    out() << "connect(" << stepVariable << ", \"tearDown(QObject*)\",\n"
          << "        this, \"" << stepVariable << "TearDown(QObject*)\");\n\n";
}

void JavascriptExporter::writeReaction(const Step* step,
                                       const Reaction* reaction) {
    if (reaction->triggerType() == Reaction::ConditionMet &&
            !reaction->waitFor()) {
        out() << "//Error: WaitFor not set!\n";
        return;
    }

    if (reaction->triggerType() == Reaction::OptionSelected &&
            reaction->optionName().isEmpty()) {
        out() << "//Error: Option without name!\n";
        return;
    }

    if (reaction->responseType() == Reaction::NextStep &&
            reaction->nextStepId().isEmpty()) {
        out() << "//Error: Next step id not set!\n";
        return;
    }

    if (reaction->triggerType() == Reaction::OptionSelected &&
            reaction->responseType() == Reaction::NextStep) {
        QString variable = writeOption(reaction->optionName());
        out() << "step.addOption(" << variable << ", \""
              << escape(reaction->nextStepId()) <<  "\");\n";
        return;
    }

    if (reaction->triggerType() == Reaction::OptionSelected &&
            reaction->responseType() == Reaction::CustomCode) {
        QString variable = writeOption(reaction->optionName());
        QString functionName = toLowerCamelCase(step->id()) + "Step" +
                                    toUpperCamelCase(reaction->optionName()) +
                                    "OptionSelected";
        out() << "step.addOption(" << variable << ", self, \""
              << functionName << "()\");\n";
        addFunction(functionName, reaction->customCode());
        return;
    }

    QString variableName = writeWaitFor(reaction->waitFor());
    if (variableName.isEmpty()) {
        return;
    }

    if (reaction->responseType() == Reaction::NextStep) {
        out() << "step.addWaitFor(" << variableName << ", \""
              << escape(reaction->nextStepId()) << "\");\n";
        return;
    }

    QString functionName = toLowerCamelCase(step->id()) + "Step" +
                                toUpperCamelCase(variableName) + "ConditionMet";
    out() << "step.addWaitFor(" << variableName << ", self, \""
          << functionName << "()\");\n";
    addFunction(functionName, reaction->customCode());
}

QString JavascriptExporter::writeOption(const QString& optionName) {
    Q_ASSERT(!optionName.isEmpty());

    QString variable = toLowerCamelCase(optionName) + "Option";
    variable = addVariable(variable);

    out() << variable << " = ktutorial.newOption(t.i18nc("
          << "\"@action Tutorial option\", \"" << escape(optionName)
          << "\"));\n";

    return variable;
}

QString JavascriptExporter::writeWaitFor(const WaitFor* waitFor) {
    if (qobject_cast<const WaitForComposed*>(waitFor)) {
        return writeWaitFor(static_cast<const WaitForComposed*>(waitFor));
    }
    if (qobject_cast<const WaitForEvent*>(waitFor)) {
        return writeWaitFor(static_cast<const WaitForEvent*>(waitFor));
    }
    if (qobject_cast<const WaitForNot*>(waitFor)) {
        return writeWaitFor(static_cast<const WaitForNot*>(waitFor));
    }
    if (qobject_cast<const WaitForProperty*>(waitFor)) {
        return writeWaitFor(static_cast<const WaitForProperty*>(waitFor));
    }
    if (qobject_cast<const WaitForSignal*>(waitFor)) {
        return writeWaitFor(static_cast<const WaitForSignal*>(waitFor));
    }
    if (qobject_cast<const WaitForStepActivation*>(waitFor)) {
        return writeWaitFor(static_cast<const WaitForStepActivation*>(waitFor));
    }
    if (qobject_cast<const WaitForWindow*>(waitFor)) {
        return writeWaitFor(static_cast<const WaitForWindow*>(waitFor));
    }

    return "";
}

QString JavascriptExporter::writeWaitFor(const WaitForComposed* waitForComposed) {
    QString type;
    if (waitForComposed->compositionType() == WaitForComposed::And) {
        type = "And";
    } else if (waitForComposed->compositionType() == WaitForComposed::Or) {
        type = "Or";
    }

    QStringList childVariables;
    foreach (WaitFor* waitFor, waitForComposed->waitFors()) {
        childVariables.append(writeWaitFor(waitFor));
        mOut << '\n';
    }

    QString variable = addVariable("waitFor" + type);
    out() << variable << " = ktutorial.newWaitFor(\"WaitFor" << type
          << "\");\n";
    foreach (const QString& childVariable, childVariables) {
        if (!childVariable.isEmpty()) {
            out() << variable << ".add(" << childVariable << ");\n";
        }
    }

    return variable;
}

QString JavascriptExporter::writeWaitFor(const WaitForEvent* waitForEvent) {
    if (waitForEvent->receiverName().isEmpty()) {
        out() << "//Error: WaitForEvent without receiver name!\n";
        return "";
    }

    if (waitForEvent->eventName().isEmpty()) {
        out() << "//Error: WaitForEvent without event name!\n";
        return "";
    }

    QString variable = "waitFor" + toUpperCamelCase(waitForEvent->eventName()) +
                       "In" + toUpperCamelCase(waitForEvent->receiverName());
    variable = addVariable(variable);

    out() << variable << " = ktutorial.newWaitFor(\"WaitForEvent\");\n";
    out() << variable << ".setEvent(ktutorial.findObject(\""
          << escape(waitForEvent->receiverName()) << "\"), \""
          << escape(waitForEvent->eventName()) << "\");\n";

    return variable;
}

QString JavascriptExporter::writeWaitFor(const WaitForNot* waitForNot) {
    if (!waitForNot->negatedWaitFor()) {
        out() << "//Error: WaitForNot without negated WaitFor!\n";
        return "";
    }

    QString variable = addVariable("waitForNot");
    QString negatedVariable = writeWaitFor(waitForNot->negatedWaitFor());

    mOut << '\n';
    out() << variable << " = ktutorial.newWaitFor(\"WaitForNot\");\n";
    if (!negatedVariable.isEmpty()) {
        out() << variable << ".setNegatedWaitFor(" << negatedVariable << ");\n";
    }

    return variable;
}

QString JavascriptExporter::writeWaitFor(
                                    const WaitForProperty* waitForProperty) {
    if (waitForProperty->objectName().isEmpty()) {
        out() << "//Error: WaitForProperty without object name!\n";
        return "";
    }

    if (waitForProperty->propertyName().isEmpty()) {
        out() << "//Error: WaitForProperty without property name!\n";
        return "";
    }

    if (waitForProperty->value().isEmpty()) {
        out() << "//Error: WaitForProperty without value!\n";
        return "";
    }

    QString propertyName = waitForProperty->propertyName();
    propertyName.remove(QRegExp("\\(.*"));
    QString variable = "waitFor" + toUpperCamelCase(propertyName) +
                       "In" + toUpperCamelCase(waitForProperty->objectName());
    variable = addVariable(variable);

    out() << variable << " = ktutorial.newWaitFor(\"WaitForProperty\");\n";
    out() << variable << ".setProperty(ktutorial.findObject(\""
          << escape(waitForProperty->objectName()) << "\"), \""
          << escape(waitForProperty->propertyName()) << "\", "
          << waitForProperty->value() << ");\n";

    return variable;
}

QString JavascriptExporter::writeWaitFor(const WaitForSignal* waitForSignal) {
    if (waitForSignal->emitterName().isEmpty()) {
        out() << "//Error: WaitForSignal without emitter name!\n";
        return "";
    }

    if (waitForSignal->signalName().isEmpty()) {
        out() << "//Error: WaitForSignal without signal name!\n";
        return "";
    }

    QString signalName = waitForSignal->signalName();
    signalName.remove(QRegExp("\\(.*"));
    QString variable = "waitFor" + toUpperCamelCase(signalName) +
                       "By" + toUpperCamelCase(waitForSignal->emitterName());
    variable = addVariable(variable);

    out() << variable << " = ktutorial.newWaitFor(\"WaitForSignal\");\n";
    out() << variable << ".setSignal(ktutorial.findObject(\""
          << escape(waitForSignal->emitterName()) << "\"), \""
          << escape(waitForSignal->signalName()) << "\");\n";

    return variable;
}

QString JavascriptExporter::writeWaitFor(
                        const WaitForStepActivation* waitForStepActivation) {
    Q_UNUSED(waitForStepActivation);

    QString variable = addVariable("waitForStepActivation");

    out() << variable
          << " = ktutorial.newWaitFor(\"WaitForStepActivation\");\n";
    out() << variable << ".setStep(tutorial, step);\n";

    return variable;
}

QString JavascriptExporter::writeWaitFor(const WaitForWindow* waitForWindow) {
    if (waitForWindow->windowObjectName().isEmpty()) {
        out() << "//Error: WaitForWindow without window object name!\n";
        return "";
    }

    QString variable = "waitFor" +
                       toUpperCamelCase(waitForWindow->windowObjectName()) +
                       "ToBeShown";
    variable = addVariable(variable);

    out() << variable << " = ktutorial.newWaitFor(\"WaitForWindow\");\n";
    out() << variable << ".setWindowObjectName(\""
          << escape(waitForWindow->windowObjectName()) << "\");\n";

    return variable;
}

QTextStream& JavascriptExporter::out() {
    mOut << indentation();
    return mOut;
}

QString JavascriptExporter::indentation() {
    QString text;
    for (int i=0; i<mIndentationLevel; ++i) {
        text += "    ";
    }

    return text;
}

void JavascriptExporter::addFunction(const QString& functionName,
                                     const QString& code) {
    int previousIndentationLevel = mIndentationLevel;
    mIndentationLevel = 0;

    QString function;
    function += "function " + functionName + "() {\n";
    mIndentationLevel++;
    if (code.isEmpty()) {
        function += toIndentedCode("//Error: No code set!\n");
    } else {
        function += toIndentedCode(code);
    }
    mIndentationLevel--;
    function += "}\n";

    mPendingFunctions.append(function);

    mIndentationLevel = previousIndentationLevel;
}

QString JavascriptExporter::addVariable(const QString& variable) {
    if (!mVariables.contains(variable)) {
        mVariables.insert(variable, 1);
        return variable;
    }

    mVariables.insert(variable, mVariables.value(variable) + 1);
    return variable + QString().setNum(mVariables.value(variable));
}

QString JavascriptExporter::escape(QString text) {
    text.replace('\n', "\\n");
    text.replace('\t', "\\t");
    text.replace('"', "\\\"");

    return text;
}

QString JavascriptExporter::toIndentedCode(const QString& code) {
    if (code.isEmpty()) {
        return "";
    }

    QString indentedCode;

    QStringList lines = code.split('\n');
    for (int i=0; i<lines.count()-1; ++i) {
        indentedCode += indentation() + lines[i] + '\n';
    }

    //If the code ends with '\n', the split code will contain an empty end
    //element that should be ignored
    if (!code.endsWith('\n')) {
        indentedCode += indentation() + lines[lines.count()-1] + '\n';
    }

    return indentedCode;
}

QString JavascriptExporter::toLowerCamelCase(const QString& text) const {
    if (text.isEmpty()) {
        return "";
    }

    QString lowerCamelCase = toUpperCamelCase(text);
    lowerCamelCase[0] = lowerCamelCase[0].toLower();

    return lowerCamelCase;
}

QString JavascriptExporter::toUpperCamelCase(QString text) const {
    if (text.isEmpty()) {
        return "";
    }

    text.replace(QRegExp("[^\\w ]"), " ");
    QStringList words = text.split(' ', QString::SkipEmptyParts);

    QString upperCamelCase;
    for (int i=0; i<words.count(); ++i) {
        words[i][0] = words[i][0].toUpper();
        upperCamelCase += words[i];
    }

    return upperCamelCase;
}

bool JavascriptExporter::mightContainSemanticMarkup(const QString& text) const {
    QSet<QString> semanticMarkupTags;
    semanticMarkupTags << "application" << "bcode" << "command" << "email"
                       << "emphasis" << "envar" << "filename" << "icode"
                       << "interface" << "link" << "message" << "nl"
                       << "numid" << "placeholder" << "resource" << "shortcut";
    semanticMarkupTags << "para" << "title" << "subtitle" << "list" << "item";
    semanticMarkupTags << "note" << "warning";

    QRegExp tagRegExp(QString::fromLatin1("<\\s*(\\w+)[^>]*>"));

    int index = text.indexOf(tagRegExp);
    while (index >= 0) {
         QString tagName = tagRegExp.capturedTexts().at(1).toLower();
         if (semanticMarkupTags.contains(tagName)) {
             return true;
         }
         index = text.indexOf(tagRegExp, index + tagRegExp.matchedLength());
    }

    return false;
}
