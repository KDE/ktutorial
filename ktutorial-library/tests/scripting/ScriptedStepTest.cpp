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

#include <QtTest>

#include <KTemporaryFile>
#include <kross/core/action.h>

#define protected public
#define private public
#include "ScriptedStep.h"
#undef private
#undef protected

#include "../WaitForSignal.h"

//ScriptedStep* must be declared as a metatype to be used in qvariant_cast
Q_DECLARE_METATYPE(ktutorial::scripting::ScriptedStep*);

namespace ktutorial {
namespace scripting {

class ScriptedStepTest: public QObject {
Q_OBJECT

public slots:

    void dummySlot() {
        mDummySlotCallCount++;
    }

signals:

    void dummySignal();

private slots:

    void init();
    void cleanup();

    void testConstructor();

    void testSetup();
    void testSetupObjectArgument();

    void testTearDown();
    void testTearDownObjectArgument();

    void testConnectWaitFor();
    void testConnectWaitForFunctionNameWithParenthesis();
    void testConnectWaitForNotKrossAction();

    void testDisconnectWaitFor();
    void testDisconnectWaitForNotKrossAction();

private:

    int mDummySlotCallCount;

    KTemporaryFile* mTemporaryFile;

};

void ScriptedStepTest::init() {
    mDummySlotCallCount = 0;

    mTemporaryFile = new KTemporaryFile();
    mTemporaryFile->setSuffix(".js");
    mTemporaryFile->open();
}

void ScriptedStepTest::cleanup() {
    delete mTemporaryFile;
}

void ScriptedStepTest::testConstructor() {
    ScriptedStep scriptedStep("theIdOfTheScriptedStep");

    QCOMPARE(scriptedStep.id(), QString("theIdOfTheScriptedStep"));
}

void ScriptedStepTest::testSetup() {
    QSKIP("Skipped until setup argument is set again to scripting::ScriptedStep*", SkipAll);

    ScriptedStep scriptedStep("theIdOfTheScriptedStep");

    //ScriptedStep* must be registered in order to be used with QSignalSpy.
    //The name must contain the namespace, as it needs the full name used in the
    //signal (and the signal has to be declared using the full namespace so the
    //spied argument can be converted using qvariant_cast).
    int scriptedStepStarType =
                qRegisterMetaType<ScriptedStep*>("scripting::ScriptedStep*");
    QSignalSpy setupSpy(&scriptedStep, SIGNAL(setup(scripting::ScriptedStep*)));

    scriptedStep.setup();

    QCOMPARE(setupSpy.count(), 1);
    QVariant argument = setupSpy.at(0).at(0);
    QCOMPARE(argument.userType(), scriptedStepStarType);
    QCOMPARE(qvariant_cast<ScriptedStep*>(argument), &scriptedStep);
}

void ScriptedStepTest::testSetupObjectArgument() {
    ScriptedStep scriptedStep("theIdOfTheScriptedStep");

    QSignalSpy setupSpy(&scriptedStep, SIGNAL(setup(QObject*)));

    scriptedStep.setup();

    QCOMPARE(setupSpy.count(), 1);
    QVariant argument = setupSpy.at(0).at(0);
    //From QVariant::Type documentation
    //"Although this function is declared as returning QVariant::Type, the
    //return value should be interpreted as QMetaType::Type"
    QCOMPARE((QMetaType::Type)argument.type(), QMetaType::QObjectStar);
    QCOMPARE(qvariant_cast<QObject*>(argument), &scriptedStep);
}

void ScriptedStepTest::testTearDown() {
    QSKIP("Skipped until tearDown argument is set again to scripting::ScriptedStep*", SkipAll);

    ScriptedStep scriptedStep("theIdOfTheScriptedStep");

    //ScriptedStep* must be registered in order to be used with QSignalSpy.
    //The name must contain the namespace, as it needs the full name used in the
    //signal (and the signal has to be declared using the full namespace so the
    //spied argument can be converted using qvariant_cast).
    int scriptedStepStarType =
                qRegisterMetaType<ScriptedStep*>("scripting::ScriptedStep*");
    QSignalSpy tearDownSpy(&scriptedStep, 
                           SIGNAL(tearDown(scripting::ScriptedStep*)));

    scriptedStep.tearDown();

    QCOMPARE(tearDownSpy.count(), 1);
    QVariant argument = tearDownSpy.at(0).at(0);
    QCOMPARE(argument.userType(), scriptedStepStarType);
    QCOMPARE(qvariant_cast<ScriptedStep*>(argument), &scriptedStep);
}

void ScriptedStepTest::testTearDownObjectArgument() {
    ScriptedStep scriptedStep("theIdOfTheScriptedStep");

    QSignalSpy tearDownSpy(&scriptedStep, SIGNAL(tearDown(QObject*)));

    scriptedStep.tearDown();

    QCOMPARE(tearDownSpy.count(), 1);
    QVariant argument = tearDownSpy.at(0).at(0);
    //From QVariant::Type documentation
    //"Although this function is declared as returning QVariant::Type, the
    //return value should be interpreted as QMetaType::Type"
    QCOMPARE((QMetaType::Type)argument.type(), QMetaType::QObjectStar);
    QCOMPARE(qvariant_cast<QObject*>(argument), &scriptedStep);
}

void ScriptedStepTest::testConnectWaitFor() {
    QTextStream out(mTemporaryFile);
    out << "function dummyFunction() { testObject.dummySlot(); }\n";
    out.flush();

    Kross::Action scriptAction(this, "TestKrossAction");
    scriptAction.setFile(mTemporaryFile->fileName());
    scriptAction.addObject(this, "testObject");
    scriptAction.trigger();

    ScriptedStep scriptedStep("theIdOfTheScriptedStep");

    WaitForSignal waitFor(this, SIGNAL(dummySignal()));
    waitFor.setActive(true);

    scriptedStep.connectWaitFor(&waitFor, &scriptAction, "dummyFunction");

    QCOMPARE(mDummySlotCallCount, 0);
    emit dummySignal();
    QCOMPARE(mDummySlotCallCount, 1);
}

void ScriptedStepTest::testConnectWaitForFunctionNameWithParenthesis() {
    QTextStream out(mTemporaryFile);
    out << "function dummyFunction() { testObject.dummySlot(); }\n";
    out.flush();

    Kross::Action scriptAction(this, "TestKrossAction");
    scriptAction.setFile(mTemporaryFile->fileName());
    scriptAction.addObject(this, "testObject");
    scriptAction.trigger();

    ScriptedStep scriptedStep("theIdOfTheScriptedStep");

    WaitForSignal waitFor(this, SIGNAL(dummySignal()));
    waitFor.setActive(true);

    scriptedStep.connectWaitFor(&waitFor, &scriptAction, "dummyFunction()");

    QCOMPARE(mDummySlotCallCount, 0);
    emit dummySignal();
    QCOMPARE(mDummySlotCallCount, 1);
}

void ScriptedStepTest::testConnectWaitForNotKrossAction() {
    ScriptedStep scriptedStep("theIdOfTheScriptedStep");

    WaitForSignal waitFor(this, SIGNAL(dummySignal()));
    waitFor.setActive(true);

    scriptedStep.connectWaitFor(&waitFor, this, SLOT(dummySlot()));

    QCOMPARE(mDummySlotCallCount, 0);
    emit dummySignal();
    QCOMPARE(mDummySlotCallCount, 1);
}

void ScriptedStepTest::testDisconnectWaitFor() {
    QTextStream out(mTemporaryFile);
    out << "function dummyFunction() { testObject.dummySlot(); }\n";
    out.flush();

    Kross::Action scriptAction(this, "TestKrossAction");
    scriptAction.setFile(mTemporaryFile->fileName());
    scriptAction.addObject(this, "testObject");
    scriptAction.trigger();

    ScriptedStep scriptedStep("theIdOfTheScriptedStep");

    WaitForSignal waitFor(this, SIGNAL(dummySignal()));
    waitFor.setActive(true);
    scriptedStep.connectWaitFor(&waitFor, &scriptAction, "dummyFunction");

    scriptedStep.disconnectWaitFor(&waitFor);

    QCOMPARE(mDummySlotCallCount, 0);
    emit dummySignal();
    QCOMPARE(mDummySlotCallCount, 0);
}

void ScriptedStepTest::testDisconnectWaitForNotKrossAction() {
    ScriptedStep scriptedStep("theIdOfTheScriptedStep");

    WaitForSignal waitFor(this, SIGNAL(dummySignal()));
    waitFor.setActive(true);
    scriptedStep.connectWaitFor(&waitFor, this, SLOT(dummySlot()));

    scriptedStep.disconnectWaitFor(&waitFor);

    QCOMPARE(mDummySlotCallCount, 0);
    emit dummySignal();
    QCOMPARE(mDummySlotCallCount, 0);
}

}
}

QTEST_MAIN(ktutorial::scripting::ScriptedStepTest)

#include "ScriptedStepTest.moc"
