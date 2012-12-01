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

#include "DefaultKdeCustomization.h"

#include <KAction>
#include <KActionCollection>
#include <KLocalizedString>
#include <KXmlGuiWindow>

#include "../Tutorial.h"
#include "../TutorialInformation.h"
#include "../TutorialManager.h"
#include "../tutorials/UsingKTutorial.h"
#include "../view/StepWidget.h"
#include "../view/TutorialManagerDialog.h"

using ktutorial::view::StepWidget;
using ktutorial::view::TutorialManagerDialog;

namespace ktutorial {
namespace customization {

//public:

//It inherits from QObject to be automatically destroyed when the parent
//KXmlGuiWindow is destroyed. It uses the same strategy as
//KDEPrivate::ToolBarHandler.
//Usually I do not like multiple inheritance, but it comes very handy here :)
class TutorialKXmlGuiClient: public QObject, public KXMLGUIClient {
Q_OBJECT
public:
    TutorialKXmlGuiClient(KXmlGuiWindow* parent):
            QObject(parent), KXMLGUIClient(parent) {
        setComponentData(KComponentData("ktutorial"));
        setXMLFile("ktutorialui.rc");
    }
};

DefaultKdeCustomization::DefaultKdeCustomization(KXmlGuiWindow* window):
    mWindow(window) {
}

void DefaultKdeCustomization::setup(TutorialManager* tutorialManager) {
    mTutorialManager = tutorialManager;

    TutorialKXmlGuiClient* tutorialClient= new TutorialKXmlGuiClient(mWindow);

    mTutorialsAction = new KAction(mWindow);
    mTutorialsAction->setText(i18nc("@action:inmenu", "Tutorials..."));
    tutorialClient->actionCollection()->addAction("tutorials", mTutorialsAction);
    connect(mTutorialsAction, SIGNAL(triggered(bool)),
            this, SLOT(showTutorialManagerDialog()));

    connect(mTutorialManager, SIGNAL(started(Tutorial*)),
            this, SLOT(showTutorialUI(Tutorial*)));

    connect(mTutorialManager, SIGNAL(started(Tutorial*)),
            this, SLOT(disableTutorialsAction()));
    connect(mTutorialManager, SIGNAL(finished()),
            this, SLOT(enableTutorialsAction()));

    mTutorialManager->registerTutorial(new UsingKTutorial());
}

QWidget* DefaultKdeCustomization::mainApplicationWindow() {
    return mWindow;
}

//public slots:

void DefaultKdeCustomization::showTutorialUI(Tutorial* tutorial) {
    QString tutorialName = tutorial->tutorialInformation()->name();

    StepWidget* stepWidget = new StepWidget(tutorialName, mWindow);
    stepWidget->setMainApplicationWindow(mWindow);
    stepWidget->setObjectName("ktutorial_StepWidget");
    connect(tutorial, SIGNAL(stepActivated(Step*)),
            stepWidget, SLOT(setStep(Step*)));
    connect(stepWidget, SIGNAL(finished()), tutorial, SLOT(finish()));
    //Invalid tutorials finish just after being started. Deleting the StepWidget
    //when the tutorial finishes ensures that it is deleted in those cases and,
    //as deleteLater() is used, it does not interfere with the deletion when the
    //StepWidget is closed by the user.
    connect(tutorial, SIGNAL(finished(Tutorial*)),
            stepWidget, SLOT(deleteLater()));
}

//private slots:

void DefaultKdeCustomization::showTutorialManagerDialog() const {
    QDialog* dialog = //krazy:exclude=qclasses
                new TutorialManagerDialog(mTutorialManager, mWindow);
    dialog->setAttribute(Qt::WA_DeleteOnClose);
    dialog->setModal(true);
    dialog->setObjectName("ktutorial_TutorialManagerDialog");

    dialog->show();
}

void DefaultKdeCustomization::disableTutorialsAction() {
    mTutorialsAction->setEnabled(false);
}

void DefaultKdeCustomization::enableTutorialsAction() {
    mTutorialsAction->setEnabled(true);
}

}
}

#include "moc_DefaultKdeCustomization.cpp"
#include "DefaultKdeCustomization.moc"
