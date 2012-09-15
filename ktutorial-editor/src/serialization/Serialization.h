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

#ifndef SERIALIZATION_H
#define SERIALIZATION_H

#include <QStringList>

#include "DeserializationException.h"
#include "IOException.h"

class KUrl;
class QWidget;
class Tutorial;

/**
 * Facade for serialization system.
 * It provides the methods needed to save and load a tutorial to a XML file, and
 * to know the available exporters and export a tutorial to a script file using
 * one of them.
 */
class Serialization {
public:

    /**
     * Creates a new Serialization.
     * The given window will be used as the parent window if any dialog is shown
     * to request some information (for example, the user password when saving a
     * file in a remote computer). It will be also used to automatically cache
     * and discard that information.
     *
     * @param window The window associated with the input/output jobs, if any.
     */
    explicit Serialization(QWidget* window = 0);

    /**
     * Loads a tutorial from the file specified by the given url.
     * The file must be a XML file that validates against the W3C Schema in
     * Tutorial.xsd. The url can be local or remote.
     *
     * @param url The url to load the tutorial from.
     * @return The loaded tutorial.
     * @throw DeserializationException If there was a problem deserializing the
     *        tutorial.
     * @throw IOException If there was a problem reading the contents from the
     *        file.
     * @see TutorialReader
     */
    Tutorial* loadTutorial(const KUrl& url) throw (DeserializationException,
                                                   IOException);

    /**
     * Saves the tutorial to the file specified by the given url.
     * The file will be a XML file that validates against the W3C Schema in
     * Tutorial.xsd. The url can be local or remote.
     *
     * @param tutorial The tutorial to save.
     * @param url The url to save the tutorial to.
     * @throw IOException If there was a problem writing the contents to the
     *        file.
     * @see TutorialWriter
     */
    void saveTutorial(const Tutorial* tutorial, const KUrl& url)
    throw (IOException);

    /**
     * Returns the available exporter types.
     * The string has the format expected by KFileDialog setFilter method, that
     * is, "*.fileExtension1|Human readable name1\n*.fileExtension2|Human
     * readable name2\n...".
     * The first type in the list is the default type, Javascript.
     *
     * @return The available exporter types.
     */
    QString availableExporterTypes();

    /**
     * Exports the tutorial to the file specified by the url using the given
     * exporter type.
     * The type must be the extension fragment of one of the types returned by
     * availableExporterTypes(). It is the value returned by KDialog
     * currentFilter() method.
     * The url can be local or remote.
     *
     * @param tutorial The tutorial to export.
     * @param type The type of the exporter to use.
     * @param url The url to save the script file to.
     * @throw IOException If there was a problem writing the contents to the
     *        file.
     */
    void exportTutorial(const Tutorial* tutorial, const QString& type,
                        const KUrl& url) throw (IOException);

private:

    QWidget* mWindow;

    /**
     * Returns a list of strings with the available exporter types.
     *
     * @return A list with the available exporter types.
     */
    QStringList availableExporterTypeList();

    /**
     * Writes the data to the file specified by the given url.
     * The url can be local or remote.
     *
     * @param data The data to write to the file.
     * @param url The url of the file to save the data to.
     * @throw IOException If there was a problem writing the contents to the
     *        file.
     */
    void writeFile(const QString& data, const KUrl& url) throw (IOException);

};

#endif
