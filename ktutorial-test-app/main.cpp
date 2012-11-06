/***************************************************************************
 *   Copyright (C) 2007 by Matt Williams <matt@milliams.com>               *
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

#include "MainWindow.h"

int main (int argc, char *argv[]) {
  KAboutData aboutData("ktutorial-test-app", 0,
      ki18nc("@title", "Test application for <application>KTutorial</application>"), "1.0",
      ki18nc("@title", "A simple test application for <application>KTutorial</application>."),
      KAboutData::License_GPL,
      ki18nc("@info:credit", "Copyright (c) 2007 Matt Williams"),
      ki18nc("@info", "Code borrowed from <link>http://techbase.kde.org/index.php?title=Development/Tutorials/Saving_and_loading</link>"));
  aboutData.addAuthor(ki18nc("@info:credit", "Daniel Calviño Sánchez"),
                      ki18nc("@info:credit", "KTutorial integration and tutorials"), 
                      "danxuliu@gmail.com");
  KCmdLineArgs::init(argc, argv, &aboutData);

  KApplication app;

  MainWindow* window = new MainWindow();
  window->show();

  return app.exec();
}
