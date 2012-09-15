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

#include "FileActions.h"

#include <KActionCollection>
#include <KFileDialog>
#include <KFileFilterCombo>
#include <KLocalizedString>
#include <KMessageBox>
#include <KRecentFilesAction>
#include <KStandardAction>

#include "KTutorialEditor.h"
#include "data/Tutorial.h"
#include "serialization/Serialization.h"

//public:

FileActions::FileActions(KTutorialEditor* tutorialEditor):
        QObject(tutorialEditor),
    mTutorialEditor(tutorialEditor),
    mClean(true),
    mRecentFilesAction(0) {

    setupActions();

    connect(tutorialEditor, SIGNAL(cleanChanged(bool)),
            this, SLOT(handleUndoStackCleanChanged(bool)));
}

FileActions::~FileActions() {
    mRecentFilesAction->saveEntries(KGlobal::config()->group("RecentFiles"));
}

const KUrl& FileActions::tutorialUrl() const {
    return mTutorialUrl;
}

bool FileActions::queryCloseTutorial() {
    if (mClean) {
        return true;
    }

    QString text = i18nc("@label", "The tutorial has been modified.<nl/>"
"Do you want to save your changes or discard them?");
    QString caption = i18nc("@title:window", "Close tutorial");
    int button = KMessageBox::warningYesNoCancel(mTutorialEditor, text, caption,
                                                 KStandardGuiItem::save(),
                                                 KStandardGuiItem::discard());

    if (button == KMessageBox::Cancel) {
         return false;
    }

    if (button == KMessageBox::Yes) {
        return saveTutorial();
    }

    return true;
}

//public slots:

//Don't use a reference to the URL, as if it is the argument in
//urlSelected(KUrl) signal emitted by mRecentFilesAction, it is removed in
//mRecentFilesAction->addUrl(KUrl), which leads to a crash
void FileActions::loadTutorialFromUrl(KUrl url) {
    if (!queryCloseTutorial()) {
        return;
    }

    Tutorial* tutorial;
    try {
        tutorial = Serialization(mTutorialEditor).loadTutorial(url);
    } catch (IOException e) {
        QString text = i18nc("@label", "There was a problem when trying to "
"open the file:<nl/>%1", e.message());
        QString caption = i18nc("@title:window", "File could not be read");
        KMessageBox::error(mTutorialEditor, text, caption);
        return;
    } catch (DeserializationException e) {
        QString text = i18nc("@label", "There was a problem when trying to "
"load the tutorial:<nl/>%1", e.message());
        QString caption = i18nc("@title:window", "Tutorial could not be "
"loaded");
        KMessageBox::error(mTutorialEditor, text, caption);
        return;
    }

    mTutorialEditor->setTutorialToBeEdited(tutorial);
    mRecentFilesAction->addUrl(url);
    mTutorialUrl = url;
    mTutorialEditor->setClean();
}

void FileActions::newTutorial() {
    if (!queryCloseTutorial()) {
        return;
    }

    mTutorialEditor->setTutorialToBeEdited();
    mTutorialUrl = KUrl();
    //Force clean state, as clearing an empty stack (in setTutorialToBeEdited())
    //would not emit cleanChanged()
    mTutorialEditor->setClean();
}

void FileActions::handleUndoStackCleanChanged(bool clean) {
    mClean = clean;

    KActionCollection* actionCollection = mTutorialEditor->actionCollection();
    if (clean && !mTutorialUrl.isEmpty()) {
        actionCollection->action("file_save")->setEnabled(false);
    } else {
        actionCollection->action("file_save")->setEnabled(true);
    }
}

//private:

void FileActions::setupActions() {
    KActionCollection* actionCollection = mTutorialEditor->actionCollection();

    KStandardAction::openNew(this, SLOT(newTutorial()), actionCollection);

    KStandardAction::open(this, SLOT(openTutorial()), actionCollection);

    mRecentFilesAction = KStandardAction::openRecent(
                    this, SLOT(loadTutorialFromUrl(KUrl)), actionCollection);
    mRecentFilesAction->loadEntries(KGlobal::config()->group("RecentFiles"));

    KStandardAction::save(this, SLOT(saveTutorial()), actionCollection);

    KStandardAction::saveAs(this, SLOT(saveTutorialAs()), actionCollection);

    KAction* action = new KAction(this);
    action->setText(i18nc("@action", "Export..."));
    action->setStatusTip(i18nc("@info:status", "Exports the tutorial to a "
"script."));
    action->setIcon(KIcon("document-export"));
    actionCollection->addAction("exportTutorial", action);
    connect(action, SIGNAL(triggered(bool)), this, SLOT(exportTutorial()));
}

//private slots:

void FileActions::openTutorial() {
    KUrl url = mTutorialUrl;
    QPointer<KFileDialog> dialog = new KFileDialog(url, QString(),
                                                   mTutorialEditor);

    dialog->setCaption(i18nc("@title", "Open Tutorial"));
    dialog->setOperationMode(KFileDialog::Opening);
    dialog->setFilter(i18nc("@item:inlistbox A KFileDialog filter",
                            "*.xml|XML file"));

    if (dialog->exec() == QDialog::Rejected) {
        return;
    }

    loadTutorialFromUrl(dialog->selectedUrl());
}

bool FileActions::saveTutorial() {
    if (mTutorialUrl.isEmpty()) {
        return saveTutorialAs();
    }

    const Tutorial* tutorial = mTutorialEditor->tutorial();
    try {
        Serialization(mTutorialEditor).saveTutorial(tutorial, mTutorialUrl);
    } catch (IOException e) {
        QString text = i18nc("@label", "There was a problem when trying to "
"save the tutorial:<nl/>%1", e.message());
        QString caption = i18nc("@title:window", "Tutorial could not be saved");
        KMessageBox::error(mTutorialEditor, text, caption);
        return false;
    }

    mTutorialEditor->setClean();

    return true;
}

bool FileActions::saveTutorialAs() {
    const Tutorial* tutorial = mTutorialEditor->tutorial();

    KUrl url = mTutorialUrl;
    QPointer<KFileDialog> dialog = new KFileDialog(url, QString(),
                                                   mTutorialEditor);

    dialog->setSelection(tutorial->id());
    dialog->setCaption(i18nc("@title", "Save Tutorial"));
    dialog->setOperationMode(KFileDialog::Saving);
    dialog->setConfirmOverwrite(true);
    dialog->setFilter(i18nc("@item:inlistbox A KFileDialog filter",
                            "*.xml|XML file"));
    dialog->filterWidget()->setEditable(false);

    if (dialog->exec() == QDialog::Rejected) {
        return false;
    }

    try {
        Serialization(mTutorialEditor).saveTutorial(tutorial,
                                                    dialog->selectedUrl());
    } catch (IOException e) {
        QString text = i18nc("@label", "There was a problem when trying to "
"save the tutorial:<nl/>%1", e.message());
        QString caption = i18nc("@title:window", "Tutorial could not be saved");
        KMessageBox::error(mTutorialEditor, text, caption);
        return false;
    }

    mRecentFilesAction->addUrl(dialog->selectedUrl());
    mTutorialUrl = dialog->selectedUrl();
    mTutorialEditor->setClean();

    return true;
}

void FileActions::exportTutorial() {
    const Tutorial* tutorial = mTutorialEditor->tutorial();

    KUrl url;
    QPointer<KFileDialog> dialog = new KFileDialog(url, QString(),
                                                   mTutorialEditor);

    dialog->setSelection(tutorial->id());
    dialog->setCaption(i18nc("@title", "Export Tutorial"));
    dialog->setOperationMode(KFileDialog::Saving);
    dialog->setConfirmOverwrite(true);
    dialog->setFilter(Serialization(mTutorialEditor).availableExporterTypes());
    dialog->filterWidget()->setEditable(false);

    if (dialog->exec() == QDialog::Rejected) {
        return;
    }

    try {
        Serialization(mTutorialEditor).exportTutorial(tutorial,
                                                      dialog->currentFilter(),
                                                      dialog->selectedUrl());
    } catch (IOException e) {
        QString text = i18nc("@label", "There was a problem when trying to "
"save the exported tutorial:<nl/>%1", e.message());
        QString caption = i18nc("@title:window", "Exported tutorial could not "
"be saved");
        KMessageBox::error(mTutorialEditor, text, caption);
        return;
    }
}
