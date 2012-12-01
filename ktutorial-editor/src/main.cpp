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
#include <KAboutData>
#include <KCmdLineArgs>
#include <KUrl>

#include "KTutorialEditor.h"

int main (int argc, char *argv[]) {
    KAboutData aboutData("ktutorial-editor", 0,
        ki18nc("@title", "KTutorial editor"),
        "0.5",
        ki18nc("@title", "An editor to create tutorials for "
                         "<application>KTutorial</application>."),
        KAboutData::License_GPL_V2,
        ki18nc("@info:credit", "Copyright (C) 2010-2012 Daniel Calviño Sánchez"));
    aboutData.addAuthor(ki18nc("@info:credit", "Daniel Calviño Sánchez"),
                        ki18nc("@info:credit", "Main developer"),
                        "danxuliu@gmail.com");
    KCmdLineArgs::init(argc, argv, &aboutData);

    KCmdLineOptions options;
    options.add("+[URL]", ki18nc("@info:shell", "The tutorial to open"));

    KCmdLineArgs::addCmdLineOptions(options);

    KApplication app;
    if (app.isSessionRestored()) {
        kRestoreMainWindows<KTutorialEditor>();
        return app.exec();
    }

    KTutorialEditor* window = new KTutorialEditor();
    window->show();

    KCmdLineArgs* args = KCmdLineArgs::parsedArgs();
    if (args->count() == 1) {
        window->loadTutorialFromUrl(args->url(0));
    }

    return app.exec();
}
