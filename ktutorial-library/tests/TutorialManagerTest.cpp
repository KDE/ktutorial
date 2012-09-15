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

#include <QtTest>
#include <qtest_kde.h>

#define protected public
#define private public
#include "TutorialManager.h"
#undef private
#undef protected

#include "Step.h"
#include "Tutorial.h"
#include "TutorialInformation.h"

//Tutorial* must be declared as a metatype to be used in qvariant_cast
Q_DECLARE_METATYPE(ktutorial::Tutorial*);

namespace ktutorial {

class TutorialManagerTest: public QObject {
Q_OBJECT

private slots:

    void init();
    void cleanup();

    void testRegisterTutorial();
    void testRegisterTutorialSeveralTutorials();
    void testRegisterTutorialTwice();
    void testRegisterTutorialDifferentTutorialWithSameId();

    void testStart();
    void testStartWithInvalidId();

private:

    TutorialManager* mTutorialManager;
    TutorialInformation* mTutorialInformation1;
    Tutorial* mTutorial1;
    TutorialInformation* mTutorialInformation2;
    Tutorial* mTutorial2;

};

class MockTutorial: public Tutorial {
public:
    MockTutorial(TutorialInformation* tutorialInformation):
        Tutorial(tutorialInformation) {
    }

    void emitFinished() {
        emit finished(this);
    }
};

void TutorialManagerTest::init() {
    mTutorialManager = new TutorialManager();

    mTutorialInformation1 = new TutorialInformation("firstIdentifier");
    mTutorial1 = new Tutorial(mTutorialInformation1);

    mTutorialInformation2 = new TutorialInformation("secondIdentifier");
    mTutorial2 = new Tutorial(mTutorialInformation2);
}

void TutorialManagerTest::cleanup() {
    if (!mTutorialManager->tutorialInformations().contains(
                                                    mTutorialInformation1)) {
        delete mTutorial1;
    }

    if (!mTutorialManager->tutorialInformations().contains(
                                                    mTutorialInformation2)) {
        delete mTutorial2;
    }

    delete mTutorialManager;
}

void TutorialManagerTest::testRegisterTutorial() {
    QVERIFY(mTutorialManager->registerTutorial(mTutorial1));

    QCOMPARE(mTutorialManager->tutorialInformations().size(), 1);
    QVERIFY(mTutorialManager->tutorialInformations().contains(
                                                        mTutorialInformation1));
    QCOMPARE(mTutorialManager->mTutorials.size(), 1);
    QCOMPARE(mTutorialManager->mTutorials.value(mTutorialInformation1), 
             mTutorial1);
    QCOMPARE(mTutorialManager->mTutorialInformations.size(), 1);
    QCOMPARE(mTutorialManager->mTutorialInformations.value("firstIdentifier"),
             mTutorialInformation1);
}

void TutorialManagerTest::testRegisterTutorialSeveralTutorials() {
    QVERIFY(mTutorialManager->registerTutorial(mTutorial1));

    QVERIFY(mTutorialManager->registerTutorial(mTutorial2));

    QCOMPARE(mTutorialManager->tutorialInformations().size(), 2);
    QVERIFY(mTutorialManager->tutorialInformations().contains(
                                                        mTutorialInformation1));
    QVERIFY(mTutorialManager->tutorialInformations().contains(
                                                        mTutorialInformation2));
    QCOMPARE(mTutorialManager->mTutorials.size(), 2);
    QCOMPARE(mTutorialManager->mTutorials.value(mTutorialInformation1), 
             mTutorial1);
    QCOMPARE(mTutorialManager->mTutorials.value(mTutorialInformation2), 
             mTutorial2);
    QCOMPARE(mTutorialManager->mTutorialInformations.size(), 2);
    QCOMPARE(mTutorialManager->mTutorialInformations.value("firstIdentifier"),
             mTutorialInformation1);
    QCOMPARE(mTutorialManager->mTutorialInformations.value("secondIdentifier"),
             mTutorialInformation2);
}

void TutorialManagerTest::testRegisterTutorialTwice() {
    mTutorialManager->registerTutorial(mTutorial1);
    mTutorialManager->registerTutorial(mTutorial2);

    QVERIFY(!mTutorialManager->registerTutorial(mTutorial1));

    QCOMPARE(mTutorialManager->tutorialInformations().size(), 2);
    QVERIFY(mTutorialManager->tutorialInformations().contains(
                                                        mTutorialInformation1));
    QVERIFY(mTutorialManager->tutorialInformations().contains(
                                                        mTutorialInformation2));
    QCOMPARE(mTutorialManager->mTutorials.size(), 2);
    QCOMPARE(mTutorialManager->mTutorials.value(mTutorialInformation1), 
             mTutorial1);
    QCOMPARE(mTutorialManager->mTutorials.value(mTutorialInformation2),
             mTutorial2);
    QCOMPARE(mTutorialManager->mTutorialInformations.size(), 2);
    QCOMPARE(mTutorialManager->mTutorialInformations.value("firstIdentifier"),
             mTutorialInformation1);
    QCOMPARE(mTutorialManager->mTutorialInformations.value("secondIdentifier"),
             mTutorialInformation2);
}

void TutorialManagerTest::testRegisterTutorialDifferentTutorialWithSameId() {
    mTutorialManager->registerTutorial(mTutorial1);
    mTutorialManager->registerTutorial(mTutorial2);

    //It will not be added and thus not deleted by TutorialManager, so it is 
    //created in stack
    Tutorial tutorial3(new TutorialInformation("firstIdentifier"));

    QVERIFY(!mTutorialManager->registerTutorial(&tutorial3));

    QCOMPARE(mTutorialManager->tutorialInformations().size(), 2);
    QVERIFY(mTutorialManager->tutorialInformations().contains(
                                                        mTutorialInformation1));
    QVERIFY(mTutorialManager->tutorialInformations().contains(
                                                        mTutorialInformation2));
    QCOMPARE(mTutorialManager->mTutorials.size(), 2);
    QCOMPARE(mTutorialManager->mTutorials.value(mTutorialInformation1), 
             mTutorial1);
    QCOMPARE(mTutorialManager->mTutorials.value(mTutorialInformation2),
             mTutorial2);
    QCOMPARE(mTutorialManager->mTutorialInformations.size(), 2);
    QCOMPARE(mTutorialManager->mTutorialInformations.value("firstIdentifier"),
             mTutorialInformation1);
    QCOMPARE(mTutorialManager->mTutorialInformations.value("secondIdentifier"),
             mTutorialInformation2);
}

void TutorialManagerTest::testStart() {
    mTutorialManager->registerTutorial(mTutorial1);

    TutorialInformation* tutorialInformation2 =
                                    new TutorialInformation("secondIdentifier");
    MockTutorial* tutorial2 = new MockTutorial(tutorialInformation2);
    mTutorialManager->registerTutorial(tutorial2);

    Step* startStep = new Step("start");
    tutorial2->addStep(startStep);

    //Tutorial* must be registered in order to be used with QSignalSpy
    int tutorialStarType = qRegisterMetaType<Tutorial*>("Tutorial*");
    QSignalSpy startedSpy(mTutorialManager, SIGNAL(started(Tutorial*)));

    mTutorialManager->start("secondIdentifier");

    QCOMPARE(startedSpy.count(), 1);
    QVariant argument = startedSpy.at(0).at(0);
    QCOMPARE(argument.userType(), tutorialStarType);
    QCOMPARE(qvariant_cast<Tutorial*>(argument), tutorial2);
    QVERIFY(startStep->isActive());

    QSignalSpy finishedSpy(mTutorialManager, SIGNAL(finished()));

    tutorial2->emitFinished();

    QCOMPARE(finishedSpy.count(), 1);
}

void TutorialManagerTest::testStartWithInvalidId() {
    mTutorialManager->registerTutorial(mTutorial1);
    mTutorialManager->registerTutorial(mTutorial2);

    QSignalSpy startedSpy(mTutorialManager, SIGNAL(started(Tutorial*)));
    QSignalSpy finishedSpy(mTutorialManager, SIGNAL(finished()));

    mTutorialManager->start("identifierNotRegistered");

    QCOMPARE(startedSpy.count(), 0);
    QCOMPARE(finishedSpy.count(), 1);
}

}

QTEST_MAIN(ktutorial::TutorialManagerTest)

#include "TutorialManagerTest.moc"
