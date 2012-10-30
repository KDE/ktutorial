/***************************************************************************
 *   Copyright (C) 2009 by Daniel Calviño Sánchez <danxuliu@gmail.com>     *
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

#include <QTest>

#include <QSharedPointer>

#define protected public
#define private public
#include "ScriptingModule.h"
#undef private
#undef protected

#include "ScriptedStep.h"
#include "../Option.h"
#include "../WaitFor.h"
#include "../WaitForAnd.h"
#include "../WaitForEvent.h"
#include "../WaitForNot.h"
#include "../WaitForOr.h"
#include "../WaitForProperty.h"
#include "../WaitForSignal.h"
#include "../WaitForStepActivation.h"
#include "../WaitForWindow.h"

namespace ktutorial {

class MockWaitForDefaultNamespace: public WaitFor {
Q_OBJECT
public:

    Q_INVOKABLE MockWaitForDefaultNamespace(): WaitFor() {
    }

    virtual bool conditionMet() const {
        return false;
    }

};

namespace scripting {

class ScriptingModuleTest: public QObject {
Q_OBJECT

private slots:

    void testSelf();

    void testRegisterWaitForMetaObject();
    void testRegisterWaitForMetaObjectWithDefaultName();
    void testRegisterWaitForMetaObjectSeveralTypes();
    void testRegisterWaitForMetaObjectTwiceWithSameName();
    void testRegisterWaitForMetaObjectNotWaitForSubclass();
    void testRegisterWaitForMetaObjectNoDefaultConstructor();
    void testRegisterWaitForMetaObjectConstructorNotInvokable();
    void testRegisterWaitForMetaObjectConstructorNotPublic();

    void testNewOption();

    void testNewStep();

    void testNewWaitFor();
    void testNewWaitForInNamespace();
    void testNewWaitForNotRegistered();

private:

    bool containsMetaObject(ScriptingModule* scriptingModule,
                            const QString& typeName);
    QMetaObject& metaObject(ScriptingModule* scriptingModule,
                            const QString& typeName);

};

class MockWaitFor: public WaitFor {
Q_OBJECT
public:

    Q_INVOKABLE MockWaitFor(): WaitFor() {
    }

    virtual bool conditionMet() const {
        return false;
    }

};

class MockWaitFor2: public MockWaitFor {
Q_OBJECT
public:

    Q_INVOKABLE MockWaitFor2(): MockWaitFor() {
    }

};

class MockWaitFor3: public MockWaitFor {
Q_OBJECT
public:

    Q_INVOKABLE MockWaitFor3(const QString&): MockWaitFor() {
    }

};

class MockWaitFor4: public MockWaitFor {
Q_OBJECT
public:

    MockWaitFor4(): MockWaitFor() {
    }

};

class MockWaitFor5: public MockWaitFor {
Q_OBJECT
public:

    Q_INVOKABLE MockWaitFor5(const QString&): MockWaitFor() {
    }

private:

    Q_INVOKABLE MockWaitFor5(): MockWaitFor() {
    }

};

class MockObject: public QObject {
Q_OBJECT
public:

    Q_INVOKABLE MockObject(): QObject() {
    }

};

void ScriptingModuleTest::testSelf() {
    ScriptingModule* scriptingModule = ScriptingModule::self();

    QVERIFY(scriptingModule);
    QCOMPARE(ScriptingModule::self(), scriptingModule);

    QVERIFY(containsMetaObject(scriptingModule, "WaitForAnd"));
    QMetaObject& type = metaObject(scriptingModule, "WaitForAnd");
    QCOMPARE(type.className(), WaitForAnd::staticMetaObject.className());

    QVERIFY(containsMetaObject(scriptingModule, "WaitForEvent"));
    type = metaObject(scriptingModule, "WaitForEvent");
    QCOMPARE(type.className(), WaitForEvent::staticMetaObject.className());

    QVERIFY(containsMetaObject(scriptingModule, "WaitForNot"));
    type = metaObject(scriptingModule, "WaitForNot");
    QCOMPARE(type.className(), WaitForNot::staticMetaObject.className());

    QVERIFY(containsMetaObject(scriptingModule, "WaitForOr"));
    type = metaObject(scriptingModule, "WaitForOr");
    QCOMPARE(type.className(), WaitForOr::staticMetaObject.className());

    QVERIFY(containsMetaObject(scriptingModule, "WaitForProperty"));
    type = metaObject(scriptingModule, "WaitForProperty");
    QCOMPARE(type.className(), WaitForProperty::staticMetaObject.className());

    QVERIFY(containsMetaObject(scriptingModule, "WaitForSignal"));
    type = metaObject(scriptingModule, "WaitForSignal");
    QCOMPARE(type.className(), WaitForSignal::staticMetaObject.className());

    QVERIFY(containsMetaObject(scriptingModule, "WaitForStepActivation"));
    type = metaObject(scriptingModule, "WaitForStepActivation");
    QCOMPARE(type.className(),
             WaitForStepActivation::staticMetaObject.className());

    QVERIFY(containsMetaObject(scriptingModule, "WaitForWindow"));
    type = metaObject(scriptingModule, "WaitForWindow");
    QCOMPARE(type.className(), WaitForWindow::staticMetaObject.className());
}

void ScriptingModuleTest::testRegisterWaitForMetaObject() {
    ScriptingModule scriptingModule;

    QVERIFY(scriptingModule.registerWaitForMetaObject(
                                                MockWaitFor::staticMetaObject,
                                                "MockWaitFor"));
    QVERIFY(containsMetaObject(&scriptingModule, "MockWaitFor"));
    QMetaObject& type = metaObject(&scriptingModule, "MockWaitFor");
    QCOMPARE(type.className(), MockWaitFor::staticMetaObject.className());
}

void ScriptingModuleTest::testRegisterWaitForMetaObjectWithDefaultName() {
    ScriptingModule scriptingModule;

    QVERIFY(scriptingModule.registerWaitForMetaObject(
                                                MockWaitFor::staticMetaObject));
    QVERIFY(containsMetaObject(&scriptingModule,
                               "ktutorial::scripting::MockWaitFor"));
    QMetaObject& type = metaObject(&scriptingModule,
                                   "ktutorial::scripting::MockWaitFor");
    QCOMPARE(type.className(), MockWaitFor::staticMetaObject.className());
}

void ScriptingModuleTest::testRegisterWaitForMetaObjectSeveralTypes() {
    ScriptingModule scriptingModule;

    QVERIFY(scriptingModule.registerWaitForMetaObject(
                                                MockWaitFor::staticMetaObject,
                                                "MockWaitFor"));
    QVERIFY(scriptingModule.registerWaitForMetaObject(
                                                MockWaitFor::staticMetaObject));
    QVERIFY(scriptingModule.registerWaitForMetaObject(
                                                MockWaitFor2::staticMetaObject,
                                                "DummyName"));

    QVERIFY(containsMetaObject(&scriptingModule, "MockWaitFor"));
    QMetaObject& type = metaObject(&scriptingModule, "MockWaitFor");
    QCOMPARE(type.className(), MockWaitFor::staticMetaObject.className());

    QVERIFY(containsMetaObject(&scriptingModule,
                               "ktutorial::scripting::MockWaitFor"));
    type = metaObject(&scriptingModule, "ktutorial::scripting::MockWaitFor");
    QCOMPARE(type.className(), MockWaitFor::staticMetaObject.className());

    QVERIFY(containsMetaObject(&scriptingModule, "DummyName"));
    type = metaObject(&scriptingModule, "DummyName");
    QCOMPARE(type.className(), MockWaitFor2::staticMetaObject.className());
}

void ScriptingModuleTest::testRegisterWaitForMetaObjectTwiceWithSameName() {
    ScriptingModule scriptingModule;
    scriptingModule.registerWaitForMetaObject(MockWaitFor::staticMetaObject,
                                              "MockWaitFor");

    QVERIFY(!scriptingModule.registerWaitForMetaObject(
                                                MockWaitFor2::staticMetaObject,
                                                "MockWaitFor"));
    QVERIFY(containsMetaObject(&scriptingModule, "MockWaitFor"));
    QMetaObject& type = metaObject(&scriptingModule, "MockWaitFor");
    QCOMPARE(type.className(), MockWaitFor::staticMetaObject.className());
}

void ScriptingModuleTest::testRegisterWaitForMetaObjectNotWaitForSubclass() {
    ScriptingModule scriptingModule;

    QVERIFY(!scriptingModule.registerWaitForMetaObject(
                                                MockObject::staticMetaObject,
                                                "MockObject"));
    QVERIFY(!containsMetaObject(&scriptingModule, "MockObject"));
}

void ScriptingModuleTest::testRegisterWaitForMetaObjectNoDefaultConstructor() {
    ScriptingModule scriptingModule;

    QVERIFY(!scriptingModule.registerWaitForMetaObject(
                                                MockWaitFor3::staticMetaObject,
                                                "MockWaitFor3"));
    QVERIFY(!containsMetaObject(&scriptingModule, "MockWaitFor3"));
}

void
ScriptingModuleTest::testRegisterWaitForMetaObjectConstructorNotInvokable() {
    ScriptingModule scriptingModule;

    QVERIFY(!scriptingModule.registerWaitForMetaObject(
                                                MockWaitFor4::staticMetaObject,
                                                "MockWaitFor4"));
    QVERIFY(!containsMetaObject(&scriptingModule, "MockWaitFor4"));
}

void ScriptingModuleTest::testRegisterWaitForMetaObjectConstructorNotPublic() {
    ScriptingModule scriptingModule;

    QVERIFY(!scriptingModule.registerWaitForMetaObject(
                                                MockWaitFor5::staticMetaObject,
                                                "MockWaitFor5"));
    QVERIFY(!containsMetaObject(&scriptingModule, "MockWaitFor5"));
}

void ScriptingModuleTest::testNewOption() {
    ScriptingModule scriptingModule;
    //TODO Remove cast when return type is changed
    QSharedPointer<Option> option((Option*)scriptingModule.
                                        newOption("The name of the option"));

    QVERIFY(!option.isNull());
    QCOMPARE(option->name(), QString("The name of the option"));
}

void ScriptingModuleTest::testNewStep() {
    ScriptingModule scriptingModule;
    //TODO Remove cast when return type is changed
    QSharedPointer<ScriptedStep> step((ScriptedStep*)scriptingModule.
                                        newStep("theIdOfTheScriptedStep"));

    QVERIFY(!step.isNull());
    QCOMPARE(step->id(), QString("theIdOfTheScriptedStep"));
}

void ScriptingModuleTest::testNewWaitFor() {
    ScriptingModule scriptingModule;
    scriptingModule.registerWaitForMetaObject(
                                MockWaitForDefaultNamespace::staticMetaObject,
                                "DummyWaitFor");
    //TODO Remove cast when return type is changed
    QSharedPointer<WaitFor> waitFor((WaitFor*)scriptingModule.
                                        newWaitFor("DummyWaitFor"));

    QVERIFY(!waitFor.isNull());
    QVERIFY(!waitFor.dynamicCast<MockWaitForDefaultNamespace>().isNull());
}

void ScriptingModuleTest::testNewWaitForInNamespace() {
    ScriptingModule scriptingModule;
    scriptingModule.registerWaitForMetaObject(MockWaitFor::staticMetaObject,
                                              "DummyWaitFor");
    //TODO Remove cast when return type is changed
    QSharedPointer<WaitFor> waitFor((WaitFor*)scriptingModule.
                                        newWaitFor("DummyWaitFor"));

    QVERIFY(!waitFor.isNull());
    QVERIFY(!waitFor.dynamicCast<MockWaitFor>().isNull());
}

void ScriptingModuleTest::testNewWaitForNotRegistered() {
    ScriptingModule scriptingModule;
    //TODO Remove cast when return type is changed
    QSharedPointer<WaitFor> waitFor((WaitFor*)scriptingModule.
                                        newWaitFor("UnknownWaitFor"));

    QVERIFY(waitFor.isNull());
}

/////////////////////////////////////Helpers////////////////////////////////////

bool ScriptingModuleTest::containsMetaObject(ScriptingModule* scriptingModule,
                                             const QString& typeName) {
    return scriptingModule->mWaitForMetaObjects.contains(typeName);
}

QMetaObject& ScriptingModuleTest::metaObject(ScriptingModule* scriptingModule,
                                             const QString& typeName) {
    return scriptingModule->mWaitForMetaObjects.find(typeName).value();
}

}
}

QTEST_MAIN(ktutorial::scripting::ScriptingModuleTest)

//Needed as the generated moc file tries to create objects not qualified with
//their namespace (when Q_INVOKABLE is set in constructors)
using ktutorial::scripting::MockWaitFor;
using ktutorial::scripting::MockWaitFor2;
using ktutorial::scripting::MockWaitFor3;
using ktutorial::scripting::MockWaitFor5;
using ktutorial::scripting::MockObject;

#include "ScriptingModuleTest.moc"
