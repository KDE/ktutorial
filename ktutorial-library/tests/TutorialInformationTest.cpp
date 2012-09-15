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

#include "TutorialInformation.h"

namespace ktutorial {

class TutorialInformationTest: public QObject {
Q_OBJECT

private slots:

    void testConstructor();

    void testSetName();

    void testSetDescription();

};

void TutorialInformationTest::testConstructor() {
    TutorialInformation tutorialInformation("theId");

    QCOMPARE(tutorialInformation.id(), QString("theId"));
    QCOMPARE(tutorialInformation.name(), QString(""));
    QCOMPARE(tutorialInformation.description(), QString(""));
}

void TutorialInformationTest::testSetName() {
    TutorialInformation tutorialInformation("someId");
    tutorialInformation.setName("The name");

    QCOMPARE(tutorialInformation.name(), QString("The name"));
}

void TutorialInformationTest::testSetDescription() {
    TutorialInformation tutorialInformation("screwLightbulbs");
    tutorialInformation.setDescription("The description");

    QCOMPARE(tutorialInformation.description(), QString("The description"));
}

}

QTEST_MAIN(ktutorial::TutorialInformationTest)

#include "TutorialInformationTest.moc"
