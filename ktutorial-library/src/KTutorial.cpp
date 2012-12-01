/***************************************************************************
 *   Copyright (C) 2008 by Daniel Calviño Sánchez <danxuliu@gmail.com>     *
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

#include "KTutorial.h"
#include "KTutorial_p.h"

#include <KDebug>

#include "TutorialManager.h"
#include "customization/DefaultKdeCustomization.h"
#include "scripting/ScriptingModule.h"
#include "scripting/ScriptManager.h"

#ifdef QT_QTDBUS_FOUND
#include "editorsupport/EditorSupport.h"
#endif

using ktutorial::customization::DefaultKdeCustomization;
using ktutorial::scripting::ScriptingModule;
using ktutorial::scripting::ScriptManager;

namespace ktutorial {

//The function is used in subdirectories, and each subdirectory has its own
//static library. The unit tests for the subdirectories are linked against the
//static library of its subdirectory and the main shared library (where this
//function is defined). Thus, the function must be exported for the linker to be
//able to find it when the unit tests for the subdirectories are built.
KTUTORIAL_EXPORT int debugArea() {
    static int s_area = KDebug::registerArea("ktutorial");
    return s_area;
}

//public:

KTutorial* KTutorial::self() {
    return sSelf;
}

KTutorial::~KTutorial() {
    delete d;
}

bool KTutorial::registerWaitForMetaObject(const QMetaObject& waitForMetaObject,
                                    const QString& typeName /*= QString()*/) {
    return ScriptingModule::self()->registerWaitForMetaObject(waitForMetaObject,
                                                              typeName);
}

void KTutorial::setup(KXmlGuiWindow* window) {
    DefaultKdeCustomization* defaultKdeCustomization =
                                            new DefaultKdeCustomization(window);
    setup(defaultKdeCustomization);
}

void KTutorial::setup(KTutorialCustomization* ktutorialCustomization) {
    d->mCustomization = ktutorialCustomization;
    d->mCustomization->setParent(this);

    d->mCustomization->setup(d->mTutorialmanager);

    ScriptManager().loadTutorials(d->mTutorialmanager);

#ifdef QT_QTDBUS_FOUND
    editorsupport::EditorSupport* editorSupport =
                                        new editorsupport::EditorSupport(this);
    editorSupport->setObjectFinder(d->mObjectFinder);
    editorSupport->setup(mainApplicationWindow());
    connect(editorSupport, SIGNAL(started(Tutorial*)),
            ktutorialCustomization, SLOT(showTutorialUI(Tutorial*)));
#endif
}

bool KTutorial::registerTutorial(Tutorial* tutorial) {
    return d->mTutorialmanager->registerTutorial(tutorial);
}

QWidget* KTutorial::mainApplicationWindow() const {
    return d->mCustomization->mainApplicationWindow();
}

//private:

KTutorial* KTutorial::sSelf = new KTutorial();

KTutorial::KTutorial():
    d(new KTutorialPrivate()) {
    d->mTutorialmanager = new TutorialManager();
    d->mTutorialmanager->setParent(this);
    d->mObjectFinder = new ObjectFinder(this);
    d->mCustomization = 0;
}

ObjectFinder* KTutorial::objectFinder() const {
    return d->mObjectFinder;
}

}
