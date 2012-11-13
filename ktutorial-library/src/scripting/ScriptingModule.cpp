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

#include "ScriptingModule.h"

#include <QMetaMethod>

#include <KDebug>

#include "ScriptedStep.h"
#include "../KTutorial.h"
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
extern int debugArea();
}

namespace ktutorial {
namespace scripting {

ScriptingModule* ScriptingModule::self() {
    if (sSelf == 0) {
        sSelf = new ScriptingModule();

        //The WaitFor types are registered with an specific name instead of the
        //default one, as the default one includes a leading "ktutorial::".
        sSelf->registerWaitForMetaObject(WaitForAnd::staticMetaObject,
                                         "WaitForAnd");
        sSelf->registerWaitForMetaObject(WaitForEvent::staticMetaObject,
                                         "WaitForEvent");
        sSelf->registerWaitForMetaObject(WaitForNot::staticMetaObject,
                                         "WaitForNot");
        sSelf->registerWaitForMetaObject(WaitForOr::staticMetaObject,
                                         "WaitForOr");
        sSelf->registerWaitForMetaObject(WaitForProperty::staticMetaObject,
                                         "WaitForProperty");
        sSelf->registerWaitForMetaObject(WaitForSignal::staticMetaObject,
                                         "WaitForSignal");
        sSelf->registerWaitForMetaObject(
                                    WaitForStepActivation::staticMetaObject,
                                    "WaitForStepActivation");
        sSelf->registerWaitForMetaObject(WaitForWindow::staticMetaObject,
                                         "WaitForWindow");
    }

    return sSelf;
}

ScriptingModule::~ScriptingModule() {
}

bool ScriptingModule::registerWaitForMetaObject(
                            const QMetaObject& waitForMetaObject,
                            const QString& typeName /*= QString()*/) {
    if (mWaitForMetaObjects.contains(typeName)) {
        kWarning(debugArea()) << "Can't register"
                              << QString(waitForMetaObject.className())
                              << ", as" << typeName << "is already registered";
        return false;
    }

    if (!inheritsWaitFor(waitForMetaObject)) {
        kWarning(debugArea()) << "Can't register"
                              << QString(waitForMetaObject.className())
                              << ", as it does not inherit WaitFor";
        return false;
    }

    if (!hasInvokableDefaultConstructor(waitForMetaObject)) {
        kWarning(debugArea()) << "Can't register"
                              << QString(waitForMetaObject.className())
                              << ", as it does not have an invokable default"
                              << "constructor";
        return false;
    }

    QString key = typeName.isNull()? waitForMetaObject.className(): typeName;
    mWaitForMetaObjects.insert(key, waitForMetaObject);

    return true;
}

QObject* ScriptingModule::findObject(const QString& name) {
    return KTutorial::self()->findObject<QObject*>(name);
}

QObject* ScriptingModule::newOption(const QString& name) {
    return new Option(name);
}

QObject* ScriptingModule::newStep(const QString& id) {
    return new ScriptedStep(id);
}

QObject* ScriptingModule::newWaitFor(const QString& typeName) {
    if (!mWaitForMetaObjects.contains(typeName)) {
        return 0;
    }

    QObject* object = mWaitForMetaObjects.find(typeName).value().newInstance();
    return qobject_cast<WaitFor*>(object);
}

//private:

ScriptingModule* ScriptingModule::sSelf = 0;

ScriptingModule::ScriptingModule() {
}

bool ScriptingModule::inheritsWaitFor(const QMetaObject& metaObject) const {
    const QMetaObject* currentSuperClass = &metaObject;
    do {
        if (currentSuperClass->className() == QString("ktutorial::WaitFor")) {
            return true;
        }
    } while ((currentSuperClass = currentSuperClass->superClass()) != 0);

    return false;
}

bool ScriptingModule::hasInvokableDefaultConstructor(
                                        const QMetaObject& metaObject) const {
    for (int i=0; i<metaObject.constructorCount(); ++i) {
        QMetaMethod constructor = metaObject.constructor(i);
        if (constructor.parameterTypes().count() == 0 &&
                constructor.access() == QMetaMethod::Public) {
            return true;
        }
    }

    return false;
}

}
}
