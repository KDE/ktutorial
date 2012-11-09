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

//public:

KTutorial* KTutorial::self() {
    return sSelf;
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
    mCustomization = ktutorialCustomization;
    mCustomization->setParent(this);

    mCustomization->setup(mTutorialmanager);

    ScriptManager().loadTutorials(mTutorialmanager);

#ifdef QT_QTDBUS_FOUND
    editorsupport::EditorSupport* editorSupport =
                                        new editorsupport::EditorSupport(this);
    editorSupport->setObjectFinder(mObjectFinder);
    editorSupport->setup(mainApplicationWindow());
    connect(editorSupport, SIGNAL(started(Tutorial*)),
            ktutorialCustomization, SLOT(showTutorialUI(Tutorial*)));
#endif
}

bool KTutorial::registerTutorial(Tutorial* tutorial) {
    return mTutorialmanager->registerTutorial(tutorial);
}

QWidget* KTutorial::mainApplicationWindow() const {
    return mCustomization->mainApplicationWindow();
}

//private:

KTutorial* KTutorial::sSelf = new KTutorial();

}
