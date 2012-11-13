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

#include "EditorSupport.h"

#include <QWidget>
#include <QtDBus/QtDBus>

#include <KDebug>

#include "ClassRegisterAdaptor.h"
#include "EditorSupportAdaptor.h"
#include "EventSpy.h"
#include "EventSpyAdaptor.h"
#include "ObjectRegister.h"
#include "ObjectRegisterAdaptor.h"
#include "../ObjectFinder.h"
#include "../extendedinformation/WidgetHighlighterManager.h"
#include "../scripting/ScriptedTutorial.h"

using ktutorial::extendedinformation::WidgetHighlighterManager;
using ktutorial::scripting::ScriptedTutorial;

namespace ktutorial {
extern int debugArea();
}

namespace ktutorial {
namespace editorsupport {

//public:

EditorSupport::EditorSupport(QObject* parent /*= 0*/): QObject(parent),
    mObjectRegister(0),
    mEventSpy(0),
    mObjectFinder(0) {
}

void EditorSupport::setObjectFinder(ObjectFinder* objectFinder) {
    mObjectFinder = objectFinder;
}

void EditorSupport::setup(QObject* window) {
    if (!QDBusConnection::sessionBus().isConnected()) {
        kWarning(debugArea()) << "Cannot connect to the D-Bus session bus!\n"
                              << "KTutorial editor support will not be enabled";
        return;
    }

    mWindow = window;

    new EditorSupportAdaptor(this);
    QDBusConnection::sessionBus().registerObject("/ktutorial", this);

    mObjectRegister = new ObjectRegister(this);
    new ClassRegisterAdaptor(mObjectRegister);
    new ObjectRegisterAdaptor(mObjectRegister);

    QDBusConnection::sessionBus().registerObject("/ktutorial/ObjectRegister",
                                                 mObjectRegister);
}

int EditorSupport::mainWindowObjectId() {
    return mObjectRegister->idForObject(mWindow);
}

int EditorSupport::findObject(const QString& name) {
    Q_ASSERT(mObjectFinder);

    QObject* object = mObjectFinder->findObject<QObject*>(name, mWindow);
    return mObjectRegister->idForObject(object);
}

void EditorSupport::highlight(int objectId) {
    QObject* object = mObjectRegister->objectForId(objectId);
    QWidget* widget = qobject_cast<QWidget*>(object);
    if (!widget) {
        return;
    }

    WidgetHighlighterManager::self()->highlight(widget);
}

void EditorSupport::stopHighlighting(int objectId) {
    QObject* object = mObjectRegister->objectForId(objectId);
    QWidget* widget = qobject_cast<QWidget*>(object);
    if (!widget) {
        return;
    }

    WidgetHighlighterManager::self()->stopHighlighting(widget);
}

void EditorSupport::enableEventSpy() {
    mEventSpy = new EventSpy(this);
    mEventSpy->addObjectToSpy(mWindow);
    new EventSpyAdaptor(mEventSpy, mObjectRegister);

    QDBusConnection::sessionBus().registerObject("/ktutorial/EventSpy",
                                                 mEventSpy);
}

void EditorSupport::disableEventSpy() {
    QDBusConnection::sessionBus().unregisterObject("/ktutorial/EventSpy");

    delete mEventSpy;
    mEventSpy = 0;
}

void EditorSupport::testScriptedTutorial(const QString& filename,
                                         const QString& stepId) {
    ScriptedTutorial* scriptedTutorial = new ScriptedTutorial(filename);

    if (!scriptedTutorial->isValid()) {
        kWarning(debugArea()) << "Cannot test the scripted tutorial stored in "
                              << filename << ": the script is invalid";
        delete scriptedTutorial;
        return;
    }

    connect(scriptedTutorial, SIGNAL(finished(Tutorial*)),
            this, SLOT(deleteFinishedTestTutorial(Tutorial*)));

    emit started(scriptedTutorial);

    scriptedTutorial->start();

    if (!stepId.isEmpty()) {
        scriptedTutorial->nextStep(stepId);
    }
}

//private:

void EditorSupport::deleteFinishedTestTutorial(Tutorial* tutorial) {
    tutorial->deleteLater();
}


}
}
