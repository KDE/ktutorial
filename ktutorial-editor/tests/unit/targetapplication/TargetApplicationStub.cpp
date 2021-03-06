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

#include <KApplication>
#include <KCmdLineArgs>
#include <KMainWindow>

#include "RemoteClassStubs.h"

int main (int argc, char *argv[]) {
    KCmdLineArgs::init(argc, argv, "TargetApplicationStub", "", ki18n(""), "");

    KApplication app;

    KMainWindow* window = new KMainWindow();
    window->show();

    StubEditorSupport* editorSupport = new StubEditorSupport();
    QDBusConnection::sessionBus().registerObject("/ktutorial",
                            editorSupport, QDBusConnection::ExportAllSlots);

    StubObjectRegister* objectRegister = new StubObjectRegister();
    QDBusConnection::sessionBus().registerObject("/ktutorial/ObjectRegister",
                            objectRegister, QDBusConnection::ExportAdaptors);

    return app.exec();
}
