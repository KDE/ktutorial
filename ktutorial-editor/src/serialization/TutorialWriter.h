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

#ifndef TUTORIALWRITER_H
#define TUTORIALWRITER_H

#include <QXmlStreamWriter>

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
 * Serializes a Tutorial in XML.
 * Each data class has its own XML element (Tutorial, Step...). Objects that
 * contain objects from other classes (for example, a Tutorial contains Steps)
 * follow a hierarchical XML structure (the Step XML element is child of
 * Tutorial XML element).
 *
 * Most object attributes are written as XML text elements (like the custom
 * code in a Tutorial). Some of them, which by their very own nature are small
 * strings, are written as XML attributes instead (like the id of a Step).
 *
 * When an object doesn't have some attribute set, be it a string or another
 * object, their XML attribute or XML element isn't written at all.
 *
 * For further details, a W3C XML Schema is provided in Tutorial.xsd file
 * (ktutorial/ktutorial-editor/src/serialization/Tutorial.xsd).
 */
class TutorialWriter {
public:

    /**
     * Creates a new TutorialWriter.
     */
    TutorialWriter();

    /**
     * Returns the XML serialization of the given tutorial.
     *
     * @param tutorial The tutorial to get its XML serialization.
     * @return The XML serialization of the given tutorial.
     */
    QString writeTutorial(const Tutorial* tutorial);

private:

    /**
     * The XML writer to use.
     */
    QXmlStreamWriter* mXmlWriter;

    /**
     * Writes the XML serialization of the given step.
     *
     * @param step The Step to get its XML serialization.
     */
    void write(const Step* step);

    /**
     * Writes the XML serialization of the given reaction.
     *
     * @param reaction The Reaction to get its XML serialization.
     */
    void write(const Reaction* reaction);

    /**
     * Writes the XML serialization of the given WaitFor.
     * It calls the appropriate write(const WaitForXXX*) method, depending on
     * the WaitFor subclass.
     *
     * @param waitFor The WaitFor to get its XML serialization.
     */
    void write(const WaitFor* waitFor);

    /**
     * Writes the XML serialization of the given WaitForComposed.
     *
     * @param waitForComposed The WaitForComposed to get its XML serialization.
     */
    void write(const WaitForComposed* waitForComposed);

    /**
     * Writes the XML serialization of the given WaitForEvent.
     *
     * @param waitForEvent The WaitForEvent to get its XML serialization.
     */
    void write(const WaitForEvent* waitForEvent);

    /**
     * Writes the XML serialization of the given WaitForNot.
     *
     * @param waitForNot The WaitForNot to get its XML serialization.
     */
    void write(const WaitForNot* waitForNot);

    /**
     * Writes the XML serialization of the given WaitForProperty.
     *
     * @param waitForProperty The WaitForProperty to get its XML serialization.
     */
    void write(const WaitForProperty* waitForProperty);

    /**
     * Writes the XML serialization of the given WaitForSignal.
     *
     * @param waitForSignal The WaitForSignal to get its XML serialization.
     */
    void write(const WaitForSignal* waitForSignal);

    /**
     * Writes the XML serialization of the given WaitForStepActivation.
     *
     * @param waitForStepActivation The WaitForStepActivation to get its XML
     *        serialization.
     */
    void write(const WaitForStepActivation* WaitForStepActivation);

    /**
     * Writes the XML serialization of the given WaitForWindow.
     *
     * @param waitForWindow The WaitForWindow to get its XML serialization.
     */
    void write(const WaitForWindow* waitForWindow);

};

#endif
