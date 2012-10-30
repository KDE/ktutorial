/***************************************************************************
 *   Copyright (C) 2009 by Daniel Calviño Sánchez <danxuliu@gmail.com>     *
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

#include <QTest>

#include <QSignalSpy>
#include <QTextStream>

#include <KTemporaryFile>
#include <kross/core/action.h>

#define protected public
#define private public
#include "ScriptedTutorial.h"
#undef private
#undef protected

#include "ScriptingModule.h"
#include "../TutorialInformation.h"

//ScriptedTutorial* must be declared as a metatype to be used in qvariant_cast
Q_DECLARE_METATYPE(ktutorial::scripting::ScriptedTutorial*);

namespace ktutorial {
namespace scripting {

class ScriptedTutorialTest: public QObject {
Q_OBJECT

private slots:

    void init();
    void cleanup();

    void testConstructor();
    void testConstructorInvalidFile();
    void testConstructorInvalidLanguage();
    void testConstructorInvalidScript();

    void testSetup();
    void testSetupObjectArgument();

    void testTearDown();
    void testTearDownObjectArgument();

private:

    KTemporaryFile* mTemporaryFile;

};

void ScriptedTutorialTest::init() {
    mTemporaryFile = new KTemporaryFile();
    mTemporaryFile->setSuffix(".js");
    mTemporaryFile->open();
}

void ScriptedTutorialTest::cleanup() {
    delete mTemporaryFile;
}

void ScriptedTutorialTest::testConstructor() {
    QTextStream out(mTemporaryFile);
    out << "function getTutorial() { return tutorial; }\n";
    out << "function getKTutorial() { return ktutorial; }\n";
    out.flush();

    ScriptedTutorial scriptedTutorial(mTemporaryFile->fileName());

    QVERIFY(scriptedTutorial.tutorialInformation());
    QCOMPARE(scriptedTutorial.tutorialInformation()->id(),
             mTemporaryFile->fileName());
    QVERIFY(scriptedTutorial.isValid());
    QVariant tutorial =
                    scriptedTutorial.mScriptAction->callFunction("getTutorial");
    QCOMPARE(qvariant_cast<QObject*>(tutorial), &scriptedTutorial);
    QVariant ktutorial =
                scriptedTutorial.mScriptAction->callFunction("getKTutorial");
    QCOMPARE(qvariant_cast<QObject*>(ktutorial), ScriptingModule::self());
}

void ScriptedTutorialTest::testConstructorInvalidFile() {
    ScriptedTutorial scriptedTutorial("someFileYouWillNotLikelyHave");

    QVERIFY(scriptedTutorial.tutorialInformation());
    QCOMPARE(scriptedTutorial.tutorialInformation()->id(),
             QString("someFileYouWillNotLikelyHave"));
    QVERIFY(!scriptedTutorial.isValid());
}

void ScriptedTutorialTest::testConstructorInvalidLanguage() {
    delete mTemporaryFile;
    mTemporaryFile = new KTemporaryFile();
    mTemporaryFile->setSuffix(".bishuo");
    mTemporaryFile->open();

    ScriptedTutorial scriptedTutorial(mTemporaryFile->fileName());

    QVERIFY(scriptedTutorial.tutorialInformation());
    QCOMPARE(scriptedTutorial.tutorialInformation()->id(), 
             mTemporaryFile->fileName());
    QVERIFY(!scriptedTutorial.isValid());
}

void ScriptedTutorialTest::testConstructorInvalidScript() {
    QTextStream out(mTemporaryFile);
    out << "someUnknownFunction();";
    out.flush();

    ScriptedTutorial scriptedTutorial(mTemporaryFile->fileName());

    QVERIFY(scriptedTutorial.tutorialInformation());
    QCOMPARE(scriptedTutorial.tutorialInformation()->id(), 
             mTemporaryFile->fileName());
    QVERIFY(!scriptedTutorial.isValid());
}

void ScriptedTutorialTest::testSetup() {
    QSKIP("Skipped until setup argument is set again to scripting::ScriptedTutorial*", SkipAll);

    ScriptedTutorial scriptedTutorial(mTemporaryFile->fileName());

    //ScriptedTutorial* must be registered in order to be used with QSignalSpy.
    //The name must contain the namespace, as it needs the full name used in the
    //signal (and the signal has to be declared using the full namespace so the
    //spied argument can be converted using qvariant_cast).
    int scriptedTutorialStarType =
        qRegisterMetaType<ScriptedTutorial*>("scripting::ScriptedTutorial*");
    QSignalSpy setupSpy(&scriptedTutorial,
                        SIGNAL(setup(scripting::ScriptedTutorial*)));

    scriptedTutorial.setup();

    QCOMPARE(setupSpy.count(), 1);
    QVariant argument = setupSpy.at(0).at(0);
    QCOMPARE(argument.userType(), scriptedTutorialStarType);
    QCOMPARE(qvariant_cast<ScriptedTutorial*>(argument), &scriptedTutorial);
}

void ScriptedTutorialTest::testSetupObjectArgument() {
    ScriptedTutorial scriptedTutorial(mTemporaryFile->fileName());

    QSignalSpy setupSpy(&scriptedTutorial, SIGNAL(setup(QObject*)));

    scriptedTutorial.setup();

    QCOMPARE(setupSpy.count(), 1);
    QVariant argument = setupSpy.at(0).at(0);
    //From QVariant::Type documentation
    //"Although this function is declared as returning QVariant::Type, the
    //return value should be interpreted as QMetaType::Type"
    QCOMPARE((QMetaType::Type)argument.type(), QMetaType::QObjectStar);
    QCOMPARE(qvariant_cast<QObject*>(argument), &scriptedTutorial);
}

void ScriptedTutorialTest::testTearDown() {
    QSKIP("Skipped until tearDown argument is set again to scripting::ScriptedTutorial*", SkipAll);

    ScriptedTutorial scriptedTutorial(mTemporaryFile->fileName());

    //ScriptedTutorial* must be registered in order to be used with QSignalSpy.
    //The name must contain the namespace, as it needs the full name used in the
    //signal (and the signal has to be declared using the full namespace so the
    //spied argument can be converted using qvariant_cast).
    int scriptedTutorialStarType =
        qRegisterMetaType<ScriptedTutorial*>("scripting::ScriptedTutorial*");
    QSignalSpy tearDownSpy(&scriptedTutorial,
                           SIGNAL(tearDown(scripting::ScriptedTutorial*)));

    scriptedTutorial.tearDown();

    QCOMPARE(tearDownSpy.count(), 1);
    QVariant argument = tearDownSpy.at(0).at(0);
    QCOMPARE(argument.userType(), scriptedTutorialStarType);
    QCOMPARE(qvariant_cast<ScriptedTutorial*>(argument), &scriptedTutorial);
}

void ScriptedTutorialTest::testTearDownObjectArgument() {
    ScriptedTutorial scriptedTutorial(mTemporaryFile->fileName());

    QSignalSpy tearDownSpy(&scriptedTutorial, SIGNAL(tearDown(QObject*)));

    scriptedTutorial.tearDown();

    QCOMPARE(tearDownSpy.count(), 1);
    QVariant argument = tearDownSpy.at(0).at(0);
    //From QVariant::Type documentation
    //"Although this function is declared as returning QVariant::Type, the
    //return value should be interpreted as QMetaType::Type"
    QCOMPARE((QMetaType::Type)argument.type(), QMetaType::QObjectStar);
    QCOMPARE(qvariant_cast<QObject*>(argument), &scriptedTutorial);
}

}
}

QTEST_MAIN(ktutorial::scripting::ScriptedTutorialTest)

#include "ScriptedTutorialTest.moc"
