/***************************************************************************
 *   Copyright (C) 2008 by KDE TechBase Developers                         *
 *   http://techbase.kde.org                                               *
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

#include "MainWindow.h"

#include <KApplication>
#include <KAction>
#include <KLocale>
#include <KActionCollection>
#include <KStandardAction>
#include <KFileDialog>
#include <KMessageBox>
#include <KIO/NetAccess>
#include <KSaveFile>
#include <QTextStream>

#include <ktutorial/KTutorial.h>

#include "TutorialClearText.h"
#include "TutorialMoveText.h"

MainWindow::MainWindow(QWidget* parent): KXmlGuiWindow(parent),
    fileName(QString()) {
    textArea = new KTextEdit();
    textArea->setObjectName("textArea");
    setCentralWidget(textArea);

    setupActions();

    ktutorial::KTutorial::self()->setup(this);
    ktutorial::KTutorial::self()->registerTutorial(new TutorialClearText());
    ktutorial::KTutorial::self()->registerTutorial(new TutorialMoveText());

    setupGUI();
}

void MainWindow::setupActions() {
    KAction* clearAction = new KAction(this);
    clearAction->setText(i18nc("@action Verb, to clear a text area", "Clear"));
    clearAction->setIcon(KIcon("document-new"));
    clearAction->setShortcut(Qt::CTRL + Qt::Key_W);
    actionCollection()->addAction("clear", clearAction);
    connect(clearAction, SIGNAL(triggered(bool)), textArea, SLOT(clear()));

//     KAction* redoAction = KStandardAction::redo(this, SLOT(newFile()), this);
//     actionCollection()->addAction("edit_redo", redoAction);
//     redoAction->setEnabled(false);
//     connect(redoAction, SIGNAL(triggered(bool)), textArea, SLOT(redo()));
//     connect(textArea, SIGNAL(redoAvailable(bool)), redoAction, SLOT(setEnabled(bool)));
//
//     KAction* undoAction = KStandardAction::undo(this, SLOT(newFile()), this);
//     actionCollection()->addAction("edit_undo", undoAction);
//     undoAction->setEnabled(false);
//     connect(undoAction, SIGNAL(triggered(bool)), textArea, SLOT(undo()));
//     connect(textArea, SIGNAL(undoAvailable(bool)), undoAction, SLOT(setEnabled(bool)));

    KAction* copyAction = KStandardAction::copy(textArea, SLOT(copy()), this);
    actionCollection()->addAction("edit_copy", copyAction);
    copyAction->setEnabled(false);
    connect(textArea, SIGNAL(copyAvailable(bool)), copyAction, SLOT(setEnabled(bool)));

    KAction* cutAction = KStandardAction::cut(textArea, SLOT(cut()), this);
    actionCollection()->addAction("edit_cut", cutAction);
    cutAction->setEnabled(false);
    connect(textArea, SIGNAL(copyAvailable(bool)), cutAction, SLOT(setEnabled(bool)));

    KAction* pasteAction = KStandardAction::paste(textArea, SLOT(paste()), this);
    actionCollection()->addAction("edit_paste", pasteAction);
//     pasteAction->setEnabled(false);
//     connect(textArea, SIGNAL(pasteAvailable(bool)), pasteAction, SLOT(setEnabled(bool)));


    KStandardAction::quit(kapp, SLOT(quit()), actionCollection());

    KStandardAction::open(this, SLOT(openFile()), actionCollection());

    KStandardAction::save(this, SLOT(saveFile()), actionCollection());

    KStandardAction::saveAs(this, SLOT(saveFileAs()), actionCollection());

    KStandardAction::openNew(this, SLOT(newFile()), actionCollection());
}

void MainWindow::newFile() {
    fileName.clear();
    textArea->clear();
}

void MainWindow::saveFileAs(const QString& outputFileName) {
    KSaveFile file(outputFileName);
    file.open();

    QByteArray outputByteArray;
    outputByteArray.append(textArea->toPlainText());
    file.write(outputByteArray);
    file.finalize();
    file.close();

    fileName = outputFileName;
}

void MainWindow::saveFileAs() {
    QString fileNameFromDialog = KFileDialog::getSaveFileName();

    if (fileNameFromDialog.isEmpty()) {
        return;
    }

    saveFileAs(fileNameFromDialog);
}

void MainWindow::saveFile() {
    if(!fileName.isEmpty()) {
        saveFileAs(fileName);
    } else {
        saveFileAs();
    }
}

void MainWindow::openFile() {
    QString fileNameFromDialog = KFileDialog::getOpenFileName();

    if (fileNameFromDialog.isEmpty()) {
        return;
    }

    QString tmpFile;
    if(KIO::NetAccess::download(fileNameFromDialog, tmpFile, this)) {
        QFile file(tmpFile);
        file.open(QIODevice::ReadOnly);
        textArea->setPlainText(QTextStream(&file).readAll());
        fileName = fileNameFromDialog;

        KIO::NetAccess::removeTempFile(tmpFile);
    } else {
        KMessageBox::error(this, KIO::NetAccess::lastErrorString());
    }
}
