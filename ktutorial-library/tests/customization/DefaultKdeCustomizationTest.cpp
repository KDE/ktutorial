/***************************************************************************
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

#include <QSignalSpy>

#include <KAction>
#include <KActionCollection>
#include <KXmlGuiWindow>

#include "DefaultKdeCustomization.h"
#include "../Step.h"
#include "../Tutorial.h"
#include "../TutorialInformation.h"
#include "../TutorialManager.h"
#include "../view/StepTextWidget.h"
#include "../view/StepWidget.h"
#include "../view/TutorialManagerDialog.h"

using ktutorial::Step;
using ktutorial::Tutorial;
using ktutorial::TutorialInformation;
using ktutorial::view::StepTextWidget;
using ktutorial::view::StepWidget;
using ktutorial::view::TutorialManagerDialog;

namespace ktutorial {
namespace customization {

class DefaultKdeCustomizationTest: public QObject {
Q_OBJECT
private slots:

    void initTestCase();

    void testSetup();

    void testMainApplicationWindow();

    void testShowTutorialUI();

    void testShowTutorialManagerUsingAction();

};

void DefaultKdeCustomizationTest::initTestCase() {
    //Set the application name to prevent a QtDBus assert complaining that
    //"Invalid object path given", as KXmlGuiWindow seems to register itself in
    //DBus.
    QApplication::setApplicationName("DefaultKdeCustomizationTest");
}

void DefaultKdeCustomizationTest::testSetup() {
    KXmlGuiWindow window;
    DefaultKdeCustomization customization(&window);
    TutorialManager tutorialManager;

    QVERIFY(!window.actionCollection()->action("tutorials"));
    QCOMPARE(tutorialManager.tutorialInformations().size(), 0);

    customization.setup(&tutorialManager);

    QAction* action = window.findChild<QAction*>("tutorials");
    QVERIFY(action);
    QVERIFY(action->isEnabled());

    QCOMPARE(tutorialManager.tutorialInformations().size(), 1);
    QCOMPARE(tutorialManager.tutorialInformations()[0]->id(),
             QString("usingKTutorial"));
}

void DefaultKdeCustomizationTest::testMainApplicationWindow() {
    KXmlGuiWindow window;
    DefaultKdeCustomization customization(&window);

    QCOMPARE(customization.mainApplicationWindow(), &window);
}

void DefaultKdeCustomizationTest::testShowTutorialUI() {
    KXmlGuiWindow window;
    DefaultKdeCustomization customization(&window);

    Tutorial tutorial(new TutorialInformation("test"));

    //Tutorial* must be registered in order to be used with QSignalSpy
    qRegisterMetaType<Tutorial*>("Tutorial*");
    QSignalSpy finishedSpy(&tutorial, SIGNAL(finished(Tutorial*)));

    Step* startStep = new Step("start");
    startStep->setText("Start step");
    tutorial.addStep(startStep);

    Step* endStep = new Step("end");
    endStep->setText("End step");
    tutorial.addStep(endStep);

    customization.showTutorialUI(&tutorial);
    tutorial.start();

    StepWidget* stepWidget = window.findChild<StepWidget*>();
    QVERIFY(stepWidget);

    StepTextWidget* stepTextWidget = stepWidget->findChild<StepTextWidget*>();
    QVERIFY(stepTextWidget);
    QCOMPARE(stepTextWidget->toPlainText(), QString("Start step"));

    tutorial.nextStep("end");

    QCOMPARE(stepTextWidget->toPlainText(), QString("End step"));

    QCOMPARE(finishedSpy.count(), 0);

    stepWidget->close();

    QCOMPARE(finishedSpy.count(), 1);
}

void DefaultKdeCustomizationTest::testShowTutorialManagerUsingAction() {
    KXmlGuiWindow window;
    DefaultKdeCustomization customization(&window);
    TutorialManager tutorialManager;
    customization.setup(&tutorialManager);

    QAction* action = window.findChild<QAction*>("tutorials");
    QVERIFY(action);
    QVERIFY(action->isEnabled());

    action->trigger();
    TutorialManagerDialog* dialog = window.findChild<TutorialManagerDialog*>();

    QVERIFY(dialog);
    QVERIFY(dialog->isModal());
    QVERIFY(dialog->testAttribute(Qt::WA_DeleteOnClose));
}

}
}

QTEST_MAIN(ktutorial::customization::DefaultKdeCustomizationTest)

#include "DefaultKdeCustomizationTest.moc"
