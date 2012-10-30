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

#include <QTest>

#include <KStandardDirs>
#include <qtest_kde.h>

#define protected public
#define private public
#include "ScriptManager.h"
#undef private
#undef protected

#include "../TutorialInformation.h"
#include "../TutorialManager.h"

namespace ktutorial {
namespace scripting {

class ScriptManagerTest: public QObject {
Q_OBJECT

private slots:

    void initTestCase();
    void cleanupTestCase();

    void testLoadTutorials();

    void testLoadTutorialsFromDirectory();

private:

    QString mDirectory;
    QFile* mTutorialValid1;
    QFile* mTutorialValid2;
    QFile* mTutorialInvalid;

    bool contains(const QList<const TutorialInformation*>& tutorialInformations,
                  const QString& id);

};

void ScriptManagerTest::initTestCase() {
    mDirectory = KGlobal::dirs()->saveLocation("appdata", "tutorials/");

    QTextStream out;

    mTutorialValid1 = new QFile(mDirectory + "valid1.js");
    mTutorialValid1->open(QIODevice::WriteOnly);
    out.setDevice(mTutorialValid1);
    out << "i = 42;\n";
    mTutorialValid1->close();

    mTutorialValid2 = new QFile(mDirectory + "valid2.js");
    mTutorialValid2->open(QIODevice::WriteOnly);
    out.setDevice(mTutorialValid2);
    out << "i = 108;\n";
    mTutorialValid2->close();

    mTutorialInvalid = new QFile(mDirectory + "invalid.js");
    mTutorialInvalid->open(QIODevice::WriteOnly);
    out.setDevice(mTutorialInvalid);
    out << "unknownFunction();\n";
    mTutorialInvalid->close();
}

void ScriptManagerTest::cleanupTestCase() {
    mTutorialValid1->remove();
    delete mTutorialValid1;

    mTutorialValid2->remove();
    delete mTutorialValid2;

    mTutorialInvalid->remove();
    delete mTutorialInvalid;

    QDir().rmdir(mDirectory);
}

void ScriptManagerTest::testLoadTutorials() {
    ScriptManager scriptManager;
    TutorialManager tutorialManager;

    scriptManager.loadTutorials(&tutorialManager);

    //Tutorials are only written to user data directory, but not system data
    //directory, as the process may not have enough permissions. So it isn't
    //really tested if the system data directory is taken into account :(
    QCOMPARE(tutorialManager.tutorialInformations().size(), 2);
    QVERIFY(contains(tutorialManager.tutorialInformations(),
                     mDirectory + "valid1.js"));
    QVERIFY(contains(tutorialManager.tutorialInformations(),
                     mDirectory + "valid2.js"));
}

void ScriptManagerTest::testLoadTutorialsFromDirectory() {
    ScriptManager scriptManager;
    TutorialManager tutorialManager;

    scriptManager.loadTutorialsFromDirectory(&tutorialManager, mDirectory);

    QCOMPARE(tutorialManager.tutorialInformations().size(), 2);
    QVERIFY(contains(tutorialManager.tutorialInformations(),
                     mDirectory + "valid1.js"));
    QVERIFY(contains(tutorialManager.tutorialInformations(),
                     mDirectory + "valid2.js"));
}

/////////////////////////////////Helpers////////////////////////////////////////

bool ScriptManagerTest::contains(
                const QList<const TutorialInformation*>& tutorialInformations,
                const QString& id) {
    for (int i=0; i<tutorialInformations.size(); ++i) {
        if (tutorialInformations[i]->id() == id) {
            return true;
        }
    }

    return false;
}

}
}

QTEST_MAIN(ktutorial::scripting::ScriptManagerTest)

#include "ScriptManagerTest.moc"
