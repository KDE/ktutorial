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

#include "Serialization.h"

#include <KLocalizedString>
#include <KTemporaryFile>
#include <KUrl>
#include <KIO/NetAccess>

#include "JavaScriptExporter.h"
#include "TutorialReader.h"
#include "TutorialWriter.h"

//public:

Serialization::Serialization(QWidget* window):
    mWindow(window) {
}

Tutorial* Serialization::loadTutorial(const KUrl& url)
throw (DeserializationException, IOException) {
    Q_ASSERT(url.isValid());

    if (url.fileName(KUrl::ObeyTrailingSlash).isEmpty()) {
        throw IOException(i18nc("@info/plain", "A file was expected, but '%1' "
                                "is a folder", url.prettyUrl()));
    }

    QString temporaryFileName;
    if (!KIO::NetAccess::download(url, temporaryFileName, mWindow)) {
        throw IOException(KIO::NetAccess::lastErrorString());
    }

    QFile temporaryFile(temporaryFileName);
    temporaryFile.open(QIODevice::ReadOnly | QIODevice::Text);

    QString data;
    QTextStream in(&temporaryFile);
    while (!in.atEnd()) {
        data += in.readAll();
    }
    temporaryFile.close();

    return TutorialReader().readTutorial(data);
}

void Serialization::saveTutorial(const Tutorial* tutorial, const KUrl& url)
throw (IOException) {
    Q_ASSERT(tutorial);
    Q_ASSERT(url.isValid());

    QString serializedTutorial = TutorialWriter().writeTutorial(tutorial);
    writeFile(serializedTutorial, url);
}

QString Serialization::availableExporterTypes() {
    QStringList typeList = availableExporterTypeList();

    QString types;
    for (int i=0; i<typeList.count() - 1; ++i) {
        types += typeList[i] + '\n';
    }
    types += typeList[typeList.count()-1];

    return types;
}

void Serialization::exportTutorial(const Tutorial* tutorial,
                                   const QString& type, const KUrl& url)
throw (IOException) {
    Q_ASSERT(tutorial);
    Q_ASSERT(url.isValid());

    QString exportedCode;
    if (type == "*.js") {
        exportedCode = JavaScriptExporter().exportTutorial(tutorial);
    } else {
        Q_ASSERT(false);
    }

    writeFile(exportedCode, url);
}

//private:

QStringList Serialization::availableExporterTypeList() {
    QStringList types;
    types << i18nc("@item:inlistbox A KFileDialog filter",
                   "*.js|JavaScript file");
    return types;
}

void Serialization::writeFile(const QString& data, const KUrl& url)
throw (IOException) {
    KTemporaryFile temporaryFile;
    temporaryFile.open();
    QTextStream out(&temporaryFile);
    out << data;
    out.flush();
    temporaryFile.close();

    if (!KIO::NetAccess::upload(temporaryFile.fileName(), url, mWindow)) {
        throw IOException(KIO::NetAccess::lastErrorString());
    }
}
