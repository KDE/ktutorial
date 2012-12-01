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

#include <KLocalizedString>

#define protected public
#define private public
#include "TutorialListModel.h"
#undef private
#undef protected

#include "../Tutorial.h"
#include "../TutorialInformation.h"
#include "../TutorialManager.h"

namespace ktutorial {
namespace view {

class TutorialListModelTest: public QObject {
Q_OBJECT

private slots:

    void testConstructor();

    void testRowCount();
    void testRowCountNoTutorials();
    void testRowCountWithAnIndex();

    void testDataDisplayRoleSingleTutorial();
    void testDataDisplayRoleSeveralTutorials();
    void testDataWithInvalidIndex();
    void testDataWithInvalidRole();

    void testHeaderData();
    void testHeaderDataWithInvalidSection();
    void testHeaderDataWithInvalidRole();

    void getTutorialInformationForIndex();
    void getTutorialInformationForIndexWithInvalidIndex();

};

void TutorialListModelTest::testConstructor() {
    TutorialManager tutorialManager;
    TutorialListModel tutorialListModel(&tutorialManager);

    QCOMPARE(tutorialListModel.mTutorialManager, &tutorialManager);
}

void TutorialListModelTest::testRowCount() {
    TutorialManager tutorialManager;

    Tutorial* tutorial1 = new Tutorial(new TutorialInformation("tutorial1"));
    tutorialManager.registerTutorial(tutorial1);

    Tutorial* tutorial2 = new Tutorial(new TutorialInformation("tutorial2"));
    tutorialManager.registerTutorial(tutorial2);

    TutorialListModel tutorialListModel(&tutorialManager);

    QCOMPARE(tutorialListModel.rowCount(), 2);
}

void TutorialListModelTest::testRowCountNoTutorials() {
    TutorialManager tutorialManager;
    TutorialListModel tutorialListModel(&tutorialManager);

    QCOMPARE(tutorialListModel.rowCount(), 0);
}

void TutorialListModelTest::testRowCountWithAnIndex() {
    TutorialManager tutorialManager;

    Tutorial* tutorial1 = new Tutorial(new TutorialInformation("tutorial1"));
    tutorialManager.registerTutorial(tutorial1);

    Tutorial* tutorial2 = new Tutorial(new TutorialInformation("tutorial2"));
    tutorialManager.registerTutorial(tutorial2);

    TutorialListModel tutorialListModel(&tutorialManager);

    QCOMPARE(tutorialListModel.rowCount(tutorialListModel.index(0, 0)), 2);
    QCOMPARE(tutorialListModel.rowCount(tutorialListModel.index(4, 8)), 2);
}

void TutorialListModelTest::testDataDisplayRoleSingleTutorial() {
    TutorialManager tutorialManager;

    TutorialInformation* tutorialInformation1 = 
                                        new TutorialInformation("tutorial1");
    tutorialInformation1->setName("First tutorial");
    Tutorial* tutorial1 = new Tutorial(tutorialInformation1);
    tutorialManager.registerTutorial(tutorial1);

    TutorialListModel tutorialListModel(&tutorialManager);

    QModelIndex index = tutorialListModel.index(0, 0);
    QCOMPARE(tutorialListModel.data(index, Qt::DisplayRole), 
             QVariant("First tutorial"));
}

void TutorialListModelTest::testDataDisplayRoleSeveralTutorials() {
    TutorialManager tutorialManager;

    TutorialInformation* tutorialInformation1 =
                                        new TutorialInformation("tutorial1");
    tutorialInformation1->setName("First tutorial");
    Tutorial* tutorial1 = new Tutorial(tutorialInformation1);
    tutorialManager.registerTutorial(tutorial1);

    TutorialInformation* tutorialInformation2 =
                                        new TutorialInformation("tutorial2");
    tutorialInformation2->setName("Second tutorial");
    Tutorial* tutorial2 = new Tutorial(tutorialInformation2);
    tutorialManager.registerTutorial(tutorial2);

    TutorialInformation* tutorialInformation3 =
                                        new TutorialInformation("tutorial3");
    tutorialInformation3->setName("Third tutorial");
    Tutorial* tutorial3 = new Tutorial(tutorialInformation3);
    tutorialManager.registerTutorial(tutorial3);

    TutorialListModel tutorialListModel(&tutorialManager);

    QModelIndex index = tutorialListModel.index(0, 0);
    QCOMPARE(tutorialListModel.data(index, Qt::DisplayRole),
             QVariant("First tutorial"));
    index = tutorialListModel.index(1, 0);
    QCOMPARE(tutorialListModel.data(index, Qt::DisplayRole),
             QVariant("Second tutorial"));
    index = tutorialListModel.index(2, 0);
    QCOMPARE(tutorialListModel.data(index, Qt::DisplayRole),
             QVariant("Third tutorial"));
}

void TutorialListModelTest::testDataWithInvalidIndex() {
    TutorialManager tutorialManager;

    TutorialInformation* tutorialInformation1 =
                                        new TutorialInformation("tutorial1");
    tutorialInformation1->setName("First tutorial");
    Tutorial* tutorial1 = new Tutorial(tutorialInformation1);
    tutorialManager.registerTutorial(tutorial1);

    TutorialListModel tutorialListModel(&tutorialManager);

    QModelIndex index = tutorialListModel.index(1, 0);
    QCOMPARE(tutorialListModel.data(index, Qt::DisplayRole), QVariant());
}

void TutorialListModelTest::testDataWithInvalidRole() {
    TutorialManager tutorialManager;

    TutorialInformation* tutorialInformation1 =
                                        new TutorialInformation("tutorial1");
    tutorialInformation1->setName("First tutorial");
    Tutorial* tutorial1 = new Tutorial(tutorialInformation1);
    tutorialManager.registerTutorial(tutorial1);

    TutorialListModel tutorialListModel(&tutorialManager);

    QModelIndex index = tutorialListModel.index(0, 0);
    QCOMPARE(tutorialListModel.data(index, Qt::EditRole), QVariant());
}

void TutorialListModelTest::testHeaderData() {
    TutorialManager tutorialManager;
    TutorialListModel tutorialListModel(&tutorialManager);

    QCOMPARE(tutorialListModel.headerData(0, Qt::Horizontal), 
             QVariant(i18n("Tutorials name")));
    QCOMPARE(tutorialListModel.headerData(0, Qt::Vertical), 
             QVariant(i18n("Tutorials name")));
}

void TutorialListModelTest::testHeaderDataWithInvalidSection() {
    TutorialManager tutorialManager;
    TutorialListModel tutorialListModel(&tutorialManager);

    QCOMPARE(tutorialListModel.headerData(-1, Qt::Horizontal), QVariant());
    QCOMPARE(tutorialListModel.headerData(1, Qt::Horizontal), QVariant());
}

void TutorialListModelTest::testHeaderDataWithInvalidRole() {
    TutorialManager tutorialManager;
    TutorialListModel tutorialListModel(&tutorialManager);

    QCOMPARE(tutorialListModel.headerData(0, Qt::Horizontal, Qt::EditRole), 
             QVariant());
}

void TutorialListModelTest::getTutorialInformationForIndex() {
    TutorialManager tutorialManager;

    Tutorial* tutorial1 = new Tutorial(new TutorialInformation("tutorial1"));
    tutorialManager.registerTutorial(tutorial1);

    Tutorial* tutorial2 = new Tutorial(new TutorialInformation("tutorial2"));
    tutorialManager.registerTutorial(tutorial2);

    TutorialListModel tutorialListModel(&tutorialManager);

    QModelIndex index = tutorialListModel.index(0, 0);
    QCOMPARE(tutorialListModel.getTutorialInformationForIndex(index), 
             tutorial1->tutorialInformation());
    index = tutorialListModel.index(1, 0);
    QCOMPARE(tutorialListModel.getTutorialInformationForIndex(index),
             tutorial2->tutorialInformation());
}

void TutorialListModelTest::getTutorialInformationForIndexWithInvalidIndex() {
    TutorialManager tutorialManager;

    Tutorial* tutorial1 = new Tutorial(new TutorialInformation("tutorial1"));
    tutorialManager.registerTutorial(tutorial1);

    Tutorial* tutorial2 = new Tutorial(new TutorialInformation("tutorial2"));
    tutorialManager.registerTutorial(tutorial2);

    TutorialListModel tutorialListModel(&tutorialManager);

    QModelIndex index;
    QCOMPARE(tutorialListModel.getTutorialInformationForIndex(index),
             (TutorialInformation*)0);
    index = tutorialListModel.index(4, 8);
    QCOMPARE(tutorialListModel.getTutorialInformationForIndex(index),
             (TutorialInformation*)0);
}

}
}

QTEST_MAIN(ktutorial::view::TutorialListModelTest)

#include "TutorialListModelTest.moc"
