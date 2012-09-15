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

#ifndef JAVASCRIPTEXPORTER_H
#define JAVASCRIPTEXPORTER_H

#include <QHash>
#include <QStringList>
#include <QTextStream>

class Reaction;
class Step;
class Tutorial;
class WaitFor;
class WaitForComposed;
class WaitForEvent;
class WaitForNot;
class WaitForProperty;
class WaitForSignal;
class WaitForStepActivation;
class WaitForWindow;

/**
 * Exporter of tutorials to Javascript code.
 * The generated Javascript code can be read by KTutorial from a tutorial script
 * file to create and initialize a KTutorial::Tutorial from it.
 *
 * The general structure of the code is:
 * -License (if any)
 * -Tutorial information
 * -Tutorial setup (if any)
 * -Tutorial tear down (if any)
 * -Steps
 *
 * For each step, the following structure is used:
 * -Step initialization (id and text)
 * -Step setup
 *   -Reactions (if any)
 *   -Custom setup code (if any)
 * -Functions for each custom code response in the reactions (if any)
 * -Step tear down (if any)
 *
 * If some necessary data is missing (for example, the id in a step), a comment
 * explaining the error is written instead of the code for that element.
 */
class JavascriptExporter {
public:

    /**
     * Creates a new JavascriptExporter.
     */
    JavascriptExporter();

    /**
     * Exports the tutorial to Javascript code.
     *
     * @param tutorial The tutorial to export.
     * @return The Javascript code.
     */
    QString exportTutorial(const Tutorial* tutorial);

private:

    /**
     * The text stream to write the Javascript code to.
     */
    QTextStream mOut;

    /**
     * The full code for the functions pending to be written.
     */
    QStringList mPendingFunctions;

    /**
     * Register for variable names and the number of times that it was used.
     */
    QHash<QString, int> mVariables;

    /**
     * The current indentation level.
     * Increment and decrement it when entering and exiting a code block.
     */
    int mIndentationLevel;

    /**
     * Writes the commented license text.
     * Nothing is written if there is no license.
     *
     * @param tutorial The tutorial to write its license.
     */
    void writeLicense(const Tutorial* tutorial);

    /**
     * Writes the name and description code of a tutorial.
     * An error message is written if the name or the tutorial is missing.
     *
     * @param tutorial The tutorial to write its name and description code.
     */
    void writeInformation(const Tutorial* tutorial);

    /**
     * Writes the setup function of a tutorial and its custom code.
     * Nothing is written if there is no setup code.
     *
     * @param tutorial The tutorial to write its setup function.
     */
    void writeSetup(const Tutorial* tutorial);

    /**
     * Writes the tear down function of a tutorial and its custom code.
     * Nothing is written if there is no tear down code.
     *
     * @param tutorial The tutorial to write its tear down function.
     */
    void writeTearDown(const Tutorial* tutorial);

    /**
     * Writes the full code (step creation, setup, tear down and adding to the
     * tutorial) of a step.
     * If the step id or name aren't set, an error message is written instead.
     * When the text seems to contain rich text or semantic markup, it is
     * formatted as rich text. Otherwise, it is formatted as plain text.
     *
     * @param step The step to write its code.
     */
    void writeStep(const Step* step);

    /**
     * Writes the setup code (including setting up reactions) of a Step.
     * Nothing is written if there is no custom setup code and no reactions.
     *
     * After the setup code all the pending functions added by custom code
     * responses in reactions are written.
     *
     * @param step The step to write its setup code.
     */
    void writeSetup(const Step* step);

    /**
     * Writes the tear down code of a Step.
     * Nothing is written if there is no tear down code.
     *
     * @param step The step to write its tear down code.
     */
    void writeTearDown(const Step* step);

    /**
     * Writes the reaction code.
     * An error message is written if the WaitFor, the option name or the next
     * step id is not set (and the trigger/response type needed them).
     *
     * @param step The step that the reaction is part of.
     * @param reaction The reaction to write its code.
     */
    void writeReaction(const Step* step, const Reaction* reaction);

    /**
     * Writes the code to create and set an option.
     * The option name can not be empty.
     *
     * @param optionName The name of the option.
     * @return The name of the variable that holds the option.
     */
    QString writeOption(const QString& optionName);

    /**
     * Writes the code to create and set a WaitFor.
     *
     * @param waitFor The WaitFor.
     * @return The name of the variable that holds the WaitFor, or an empty
     *         string if the WaitFor data is invalid.
     */
    QString writeWaitFor(const WaitFor* waitFor);

    /**
     * Writes the code to create and set a WaitForComposed.
     * The code for the child WaitFors is added before the code for the
     * WaitForComposed.
     *
     * @param waitForComposed The WaitForComposed.
     * @return The name of the variable that holds the WaitFor.
     */
    QString writeWaitFor(const WaitForComposed* waitForComposed);

    /**
     * Writes the code to create and set a WaitForEvent.
     * If the receiver name or the event name aren't set, an error message is
     * written instead, and an empty string returned.
     *
     * @param waitForEvent The WaitForEvent.
     * @return The name of the variable that holds the WaitFor.
     */
    QString writeWaitFor(const WaitForEvent* waitForEvent);

    /**
     * Writes the code to create and set a WaitForNot.
     * If the negated WaitFor isn't set, an error message is written instead,
     * and an empty string returned.
     *
     * @param waitForNot The WaitForNot.
     * @return The name of the variable that holds the WaitFor.
     */
    QString writeWaitFor(const WaitForNot* waitForNot);

    /**
     * Writes the code to create and set a WaitForProperty.
     * If the object name, the property name or the value aren't set, an error
     * message is written instead, and an empty string returned.
     *
     * The value is written as is. It is neither escaped nor wrapped in quotes.
     *
     * @param waitForProperty The WaitForProperty.
     * @return The name of the variable that holds the WaitFor.
     */
    QString writeWaitFor(const WaitForProperty* waitForProperty);

    /**
     * Writes the code to create and set a WaitForSignal.
     * If the emitter name or the signal name aren't set, an error message is
     * written instead, and an empty string returned.
     *
     * @param waitForSignal The WaitForSignal.
     * @return The name of the variable that holds the WaitFor.
     */
    QString writeWaitFor(const WaitForSignal* waitForSignal);

    /**
     * Writes the code to create and set a WaitForStepActivation.
     *
     * @param waitForStepActivation The WaitForStepActivation.
     * @return The name of the variable that holds the WaitFor.
     */
    QString writeWaitFor(const WaitForStepActivation* waitForStepActivation);

    /**
     * Writes the code to create and set a WaitForWindow.
     * If the window object name isn't set, an error message is written instead,
     * and an empty string returned.
     *
     * @param waitForWindow The WaitForWindow.
     * @return The name of the variable that holds the WaitFor.
     */
    QString writeWaitFor(const WaitForWindow* waitForWindow);

    /**
     * Returns the output stream after adding the identation text.
     * It is used as a shortcut to "mOut << indentation()". Use it to output any
     * line that has no previous indentation.
     *
     * Use mOut directly to output empty new lines, pending functions, or code
     * returned by indentedText
     *
     * @return The output stream.
     */
    QTextStream& out();

    /**
     * Returns the current indentation text.
     * Each level of indentation are four blank spaces.
     *
      @return The current indentation text.
     */
    QString indentation();

    /**
     * Adds a new function to mPendingFunctions.
     * The body will be automatically indented.
     *
     * It is used to queue the writing of functions with custom code specified
     * in reactions, as reactions are written into the setup function of its
     * step.
     *
     * @param functionName The name of the function.
     * @param code The body of the function.
     */
    void addFunction(const QString& functionName, const QString& code);

    /**
     * Returns a unique variable name for the given variable name.
     * The same variable name may be used by several WaitFors in the same step
     * setup (for example, two WaitForAnd would have the same variable name).
     * When a variable is added, it is ensured that there are no other variables
     * with the same name. If there are, a fixed unique variable name is
     * returned.
     *
     * The register of variables must be cleared after exiting the step setup
     * generation to avoid unneeded renaming of variables in other step setups.
     *
     * @param variable The variable to add.
     * @return The unique variable name.
     */
    QString addVariable(const QString& variable);

    /**
     * Returns the same text, but with Javascript escape sequences where needed.
     * Some special characters (new line, tab and quotes) are written as their
     * escape squence.
     *
     * @param text The text to escape.
     * @return The Javascript text.
     */
    QString escape(QString text);

    /**
     * Returns the same code, but with each line indented and ending in a new
     * line.
     *
     * @param code The code to indent.
     * @return The indented code.
     */
    QString toIndentedCode(const QString& code);

    /**
     * Returns the lowerCamelCase version of the given text.
     * Note that the returned text contains only letters, digits or underscores.
     * Any other character is removed, and the next valid character is upper
     * cased.
     *
     * @param text The string to get its lowerCamelCase version.
     * @return The lowerCamelCase version of the text.
     */
    QString toLowerCamelCase(const QString& text) const;

    /**
     * Returns the UpperCamelCase version of the given text.
     * Note that the returned text contains only letters, digits or underscores.
     * Any other character is removed, and the next valid character is upper
     * cased.
     *
     * @param text The string to get its UpperCamelCase version.
     * @return The UpperCamelCase version of the text.
     */
    QString toUpperCamelCase(QString text) const;

    /**
     * Returns true if the string text is likely to contain semantic markup.
     * It just checks whether there is something that looks like a KUIT tag in
     * the text. Although the result may be correct for common cases, there is
     * no guarantee.
     *
     * @param text The string to check.
     * @return True if the string text is likely to contain semantic markup,
     *         false otherwise.
     */
    bool mightContainSemanticMarkup(const QString& text) const;

};

#endif
