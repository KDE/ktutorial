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

#include <QtTest>

#include "Serialization.h"

#include <QFile>

#include <KLocalizedString>
#include <KStandardDirs>
#include <KUrl>

#include "../data/Tutorial.h"

#define EXPECT_EXCEPTION(statement, exception) \
do {\
    try {\
        statement;\
        QFAIL("Expected " #exception " not thrown");\
    } catch (exception e) {\
    } catch (Exception e) {\
        QFAIL("Expected " #exception " not thrown");\
    }\
} while (0)

class SerializationTest: public QObject {
Q_OBJECT

private slots:

    void init();
    void cleanup();

    void testSaveAndLoad();

    void testLoadFromUnreadableUrl();
    void testLoadFromDirectory();
    void testLoadNotAnXmlFile();
    void testLoadXmlNotWellFormed();
    void testLoadXmlWithoutRootTutorialElement();

    void testSaveToExistingUrl();
    void testSaveToUnwritableUrl();
    void testSaveToDirectory();

    void testAvailableExporterTypes();

    void testExportJavascript();

    void testExportToExistingUrl();
    void testExportToUnwritableUrl();
    void testExportToDirectory();

private:

    QFile* mFile;

    QString readFile(QFile* file);
    void writeFile(QFile* file, const QString& contents);

};

void SerializationTest::init() {
    mFile = 0;
}

void SerializationTest::cleanup() {
    if (mFile) {
        mFile->remove();
    }
    delete mFile;
}

void SerializationTest::testSaveAndLoad() {
    Tutorial tutorial;
    tutorial.setName("The name");
    tutorial.setDescription("The description");

    KUrl url = KGlobal::dirs()->saveLocation("tmp") + "/SerializationTest.xml";

    Serialization().saveTutorial(&tutorial, url);
    QScopedPointer<Tutorial> loadedTutorial(Serialization().loadTutorial(url));

    QVERIFY(loadedTutorial);
    QCOMPARE(loadedTutorial->name(), tutorial.name());
    QCOMPARE(loadedTutorial->description(), tutorial.description());
}

void SerializationTest::testLoadFromUnreadableUrl() {
    KUrl url = KGlobal::dirs()->saveLocation("tmp") + "/SerializationTest.xml";
    mFile = new QFile(url.toLocalFile());
    mFile->open(QIODevice::WriteOnly | QIODevice::Text);
    writeFile(mFile, "<tutorial name=\"The name\"></tutorial>");
    QVERIFY(QFile::setPermissions(mFile->fileName(), 0));

    EXPECT_EXCEPTION(Serialization().loadTutorial(url), IOException);
}

void SerializationTest::testLoadFromDirectory() {
    KUrl url = KGlobal::dirs()->saveLocation("tmp");

    EXPECT_EXCEPTION(Serialization().loadTutorial(url), IOException);
}

void SerializationTest::testLoadNotAnXmlFile() {
    KUrl url = KGlobal::dirs()->saveLocation("tmp") + "/SerializationTest.txt";
    mFile = new QFile(url.toLocalFile());
    mFile->open(QIODevice::WriteOnly | QIODevice::Text);
    writeFile(mFile, "Not an XML file");

    EXPECT_EXCEPTION(Serialization().loadTutorial(url),
                     DeserializationException);
}

void SerializationTest::testLoadXmlNotWellFormed() {
    KUrl url = KGlobal::dirs()->saveLocation("tmp") + "/SerializationTest.xml";
    mFile = new QFile(url.toLocalFile());
    mFile->open(QIODevice::WriteOnly | QIODevice::Text);
    writeFile(mFile, "<tutorial><step></invalidEndElement></tutorial>");

    EXPECT_EXCEPTION(Serialization().loadTutorial(url),
                     DeserializationException);
}

void SerializationTest::testLoadXmlWithoutRootTutorialElement() {
    KUrl url = KGlobal::dirs()->saveLocation("tmp") + "/SerializationTest.xml";
    mFile = new QFile(url.toLocalFile());
    mFile->open(QIODevice::WriteOnly | QIODevice::Text);
    writeFile(mFile, "<unknownRootElement></unknownRootElement>");

    EXPECT_EXCEPTION(Serialization().loadTutorial(url),
                     DeserializationException);
}

void SerializationTest::testSaveToExistingUrl() {
    Tutorial tutorial;
    tutorial.setName("The name");
    tutorial.setDescription("The description");

    KUrl url = KGlobal::dirs()->saveLocation("tmp") + "/SerializationTest.xml";
    mFile = new QFile(url.toLocalFile());
    mFile->open(QIODevice::WriteOnly | QIODevice::Text);
    writeFile(mFile, "Hello world!");

    Serialization().saveTutorial(&tutorial, url);

    QVERIFY(mFile->exists());
    QVERIFY(mFile->open(QIODevice::ReadOnly | QIODevice::Text));

    QString actualContents = readFile(mFile);
    QString expectedContents =
"<?xml version=\"1.0\" encoding=\"utf-8\"?>\n"
"<tutorial name=\"The name\">\n"
"    <description>The description</description>\n"
"</tutorial>\n";

    QCOMPARE(actualContents, expectedContents);
}

void SerializationTest::testSaveToUnwritableUrl() {
    Tutorial tutorial;
    tutorial.setName("The name");
    tutorial.setDescription("The description");

    KUrl url = KGlobal::dirs()->saveLocation("tmp") + "/SerializationTest.xml";
    mFile = new QFile(url.toLocalFile());
    mFile->open(QIODevice::WriteOnly | QIODevice::Text);
    writeFile(mFile, "Hello world!");
    QVERIFY(QFile::setPermissions(mFile->fileName(), 0));

    EXPECT_EXCEPTION(Serialization().saveTutorial(&tutorial, url), IOException);
}

void SerializationTest::testSaveToDirectory() {
    Tutorial tutorial;
    tutorial.setName("The name");
    tutorial.setDescription("The description");

    KUrl url = KGlobal::dirs()->saveLocation("tmp");

    EXPECT_EXCEPTION(Serialization().saveTutorial(&tutorial, url), IOException);
}

void SerializationTest::testAvailableExporterTypes() {
    QString types = Serialization().availableExporterTypes();

    QCOMPARE(types, i18nc("@item:combobox A KFileDialog filter",
                          "*.js|Javascript file"));
}

void SerializationTest::testExportJavascript() {
    Tutorial tutorial;
    tutorial.setName("The name");
    tutorial.setDescription("The description");

    KUrl url = KGlobal::dirs()->saveLocation("tmp") + "/SerializationTest.js";

    Serialization().exportTutorial(&tutorial, "*.js", url);

    mFile = new QFile(url.toLocalFile());
    QVERIFY(mFile->exists());
    QVERIFY(mFile->open(QIODevice::ReadOnly | QIODevice::Text));

    QString actualContents = readFile(mFile);
    QString expectedContents =
"t = Kross.module(\"kdetranslation\");\n"
"\n"
"tutorial.tutorialInformationAsObject().setName(t.i18n(\"The name\"));\n"
"tutorial.tutorialInformationAsObject().setDescription("
                                            "t.i18n(\"The description\"));\n"
"\n";

    QCOMPARE(actualContents, expectedContents);
}

void SerializationTest::testExportToExistingUrl() {
    Tutorial tutorial;
    tutorial.setName("The name");
    tutorial.setDescription("The description");

    KUrl url = KGlobal::dirs()->saveLocation("tmp") + "/SerializationTest.js";
    mFile = new QFile(url.toLocalFile());
    mFile->open(QIODevice::WriteOnly | QIODevice::Text);
    writeFile(mFile, "Hello world!");

    Serialization().exportTutorial(&tutorial, "*.js", url);

    QVERIFY(mFile->exists());
    QVERIFY(mFile->open(QIODevice::ReadOnly | QIODevice::Text));

    QString actualContents = readFile(mFile);
    QString expectedContents =
"t = Kross.module(\"kdetranslation\");\n"
"\n"
"tutorial.tutorialInformationAsObject().setName(t.i18n(\"The name\"));\n"
"tutorial.tutorialInformationAsObject().setDescription("
                                            "t.i18n(\"The description\"));\n"
"\n";

    QCOMPARE(actualContents, expectedContents);
}

void SerializationTest::testExportToUnwritableUrl() {
    Tutorial tutorial;
    tutorial.setName("The name");
    tutorial.setDescription("The description");

    KUrl url = KGlobal::dirs()->saveLocation("tmp") + "/SerializationTest.js";
    mFile = new QFile(url.toLocalFile());
    mFile->open(QIODevice::WriteOnly | QIODevice::Text);
    writeFile(mFile, "Hello world!");
    QVERIFY(QFile::setPermissions(mFile->fileName(), 0));

    EXPECT_EXCEPTION(Serialization().exportTutorial(&tutorial, "*.js", url),
                     IOException);
}

void SerializationTest::testExportToDirectory() {
    Tutorial tutorial;
    tutorial.setName("The name");
    tutorial.setDescription("The description");

    KUrl url = KGlobal::dirs()->saveLocation("tmp");

    EXPECT_EXCEPTION(Serialization().exportTutorial(&tutorial, "*.js", url),
                     IOException);
}

/////////////////////////////////// Helpers ////////////////////////////////////

QString SerializationTest::readFile(QFile* file) {
    QString data;

    QTextStream in(file);
    while (!in.atEnd()) {
        data += in.readAll();
    }
    file->close();

    return data;
}

void SerializationTest::writeFile(QFile* file, const QString& contents) {
    QTextStream out(file);
    out << contents;

    file->close();
}

QTEST_MAIN(SerializationTest)

#include "SerializationTest.moc"
