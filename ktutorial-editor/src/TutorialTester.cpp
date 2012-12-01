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

#include "TutorialTester.h"

#include <KLocalizedString>
#include <KMessageBox>
#include <KTemporaryFile>
#include <KUrl>

#include "KTutorialEditor.h"
#include "serialization/Serialization.h"
#include "targetapplication/RemoteEditorSupport.h"
#include "targetapplication/TargetApplication.h"
#include "view/TargetApplicationView.h"

//public:

TutorialTester::TutorialTester(KTutorialEditor* tutorialEditor):
        QObject(tutorialEditor),
    mTutorialEditor(tutorialEditor) {
}

void TutorialTester::setStepToTestFrom(const QString& stepId) {
    mStepId = stepId;
}

void TutorialTester::testTutorial() {
    if (TargetApplication::self()->remoteEditorSupport()) {
        sendTutorialToTargetApplication();
    } else {
        connect(TargetApplication::self(), SIGNAL(started()),
                this, SLOT(sendTutorialToTargetApplication()));
        connect(TargetApplication::self(),
                SIGNAL(startFailed(TargetApplication::Error)),
                this, SLOT(deleteLater()));

        TargetApplicationView* targetApplicationView =
                            new TargetApplicationView(TargetApplication::self(),
                                                      mTutorialEditor);
        targetApplicationView->start();
    }
}

//private slots:

void TutorialTester::sendTutorialToTargetApplication() {
    disconnect(TargetApplication::self(), SIGNAL(started()),
               this, SLOT(sendTutorialToTargetApplication()));

    //As this TutorialTester is set as parent of the KTemporaryFile object, the
    //file will be automatically removed when this TutorialTester is destroyed
    KTemporaryFile* temporaryFile = new KTemporaryFile();
    temporaryFile->setAutoRemove(true);
    temporaryFile->setParent(this);
    temporaryFile->setSuffix(".js");
    temporaryFile->open();

    const Tutorial* tutorial = mTutorialEditor->tutorial();
    try {
        Serialization(mTutorialEditor).
                    exportTutorial(tutorial, "*.js", temporaryFile->fileName());
    } catch (IOException e) {
        QString text = i18nc("@label", "There was a problem when trying to "
"save the tutorial to a temporary file (to be used by the target application "
"to test the tutorial):<nl/>%1", e.message());
        QString caption = i18nc("@title:window", "Tutorial could not be saved");
        KMessageBox::error(mTutorialEditor, text, caption);
        delete temporaryFile;
        return;
    }

    try {
        if (mStepId.isEmpty()) {
            TargetApplication::self()->remoteEditorSupport()->
                                testScriptedTutorial(temporaryFile->fileName());
        } else {
            TargetApplication::self()->remoteEditorSupport()->
                    testScriptedTutorial(temporaryFile->fileName(), mStepId);
        }
    } catch (DBusException e) {
        QString text = i18nc("@label", "There was a problem when trying to "
"tell the target application to start the tutorial:<nl/>%1", e.message());
        QString caption = i18nc("@title:window", "Tutorial could not be "
"started");
        KMessageBox::error(mTutorialEditor, text, caption);
    }
}
