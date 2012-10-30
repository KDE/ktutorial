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

#include "EditorSupportAdaptor.h"

#include <QCoreApplication>

#include "EditorSupport.h"
#include "ObjectRegister.h"

namespace ktutorial {
namespace editorsupport {

//public:

EditorSupportAdaptor::EditorSupportAdaptor(EditorSupport* editorSupport):
        QDBusAbstractAdaptor(editorSupport),
    mEditorSupport(editorSupport) {
}

//public slots:

int EditorSupportAdaptor::mainWindowObjectId() const {
    return mEditorSupport->mainWindowObjectId();
}

int EditorSupportAdaptor::findObject(const QString& name) {
    return mEditorSupport->findObject(name);
}

void EditorSupportAdaptor::highlight(int objectId) {
    mEditorSupport->highlight(objectId);
}

void EditorSupportAdaptor::stopHighlighting(int objectId) {
    mEditorSupport->stopHighlighting(objectId);
}

void EditorSupportAdaptor::enableEventSpy() {
    mEditorSupport->enableEventSpy();
}

void EditorSupportAdaptor::disableEventSpy() {
    mEditorSupport->disableEventSpy();
}

void EditorSupportAdaptor::testScriptedTutorial(const QString& filename,
                                                const QString& stepId) {
    mEditorSupport->testScriptedTutorial(filename, stepId);
}

}
}
