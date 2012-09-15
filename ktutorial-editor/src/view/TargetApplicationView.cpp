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

#include "TargetApplicationView.h"

#include <QApplication>
#include <QScopedPointer>

#include <KLocalizedString>
#include <KMessageBox>
#include <KOpenWithDialog>
#include <KShell>
#include <KStandardDirs>

//public:

TargetApplicationView::TargetApplicationView(
                        TargetApplication* targetApplication, QWidget* parent):
        QObject(parent),
    mTargetApplication(targetApplication),
    mParent(parent) {
}

void TargetApplicationView::start() {
    if (mTargetApplication->remoteEditorSupport()) {
        return;
    }

    if (mTargetApplication->targetApplicationFilePath().isEmpty()) {
        QString path = askApplicationFilePath();

        if (path.isEmpty()) {
            return;
        }

        mTargetApplication->setTargetApplicationFilePath(path);
    }

    startExpectingTheTargetApplicationToStart();

    mTargetApplication->start();
}

//private:

QString TargetApplicationView::askApplicationFilePath() {
    QScopedPointer<KOpenWithDialog> dialog(new KOpenWithDialog(mParent));
    dialog->hideNoCloseOnExit();
    dialog->hideRunInTerminal();

    if (dialog->exec() == QDialog::Rejected) {
        return "";
    }

    //The executable returned by the services may contain arguments like "%U".
    //We are interested only in the executable itself.
    QString executable = KShell::splitArgs(dialog->text())[0];

    //The executable may be just a name, not the absolute path to the executable
    if (KUrl(executable).isRelative()) {
        executable = KStandardDirs::findExe(executable);
    }

    return executable;
}

void TargetApplicationView::startExpectingTheTargetApplicationToStart() {
    //Only knowing if the TargetApplication failed to start is relevant in order
    //to show the error message. However, once started the signal must be
    //disconnected to avoid showing several error messages (for example, if the
    //target application fails after being started several times by this view).
    connect(mTargetApplication, SIGNAL(started()),
            this, SLOT(stopExpectingTheTargetApplicationToStart()));
    connect(mTargetApplication, SIGNAL(startFailed(TargetApplication::Error)),
            this, SLOT(showErrorMessage(TargetApplication::Error)));

    QApplication::setOverrideCursor(QCursor(Qt::BusyCursor));
}

//private slots:

void TargetApplicationView::showErrorMessage(TargetApplication::Error error) {
    stopExpectingTheTargetApplicationToStart();

    QString caption = i18nc("@title:window", "Problem starting the target \
application");

    QString message;
    if (error == TargetApplication::InvalidPath) {
        message = i18nc("@label", "The selected application could not be \
executed. Check the executable file permissions.");
    } else if (error == TargetApplication::InvalidApplication) {
        message = i18nc("@label", "The selected application does not seem to \
have the KTutorial editor support module enabled. This can happen because the \
application does not use KTutorial, or because it took it too much time to \
start.");
    } else if (error == TargetApplication::NoDBusConnection) {
        message = i18nc("@label", "Cannot connect to the D-Bus session bus.\
There is no way to communicate with the target application.");
    }

    KMessageBox::sorry(mParent, message, caption);

    mTargetApplication->setTargetApplicationFilePath("");
}

void TargetApplicationView::stopExpectingTheTargetApplicationToStart() {
    disconnect(mTargetApplication, SIGNAL(started()),
            this, SLOT(stopExpectingTheTargetApplicationToStart()));
    disconnect(mTargetApplication, SIGNAL(startFailed(TargetApplication::Error)),
            this, SLOT(showErrorMessage(TargetApplication::Error)));

    QApplication::restoreOverrideCursor();
}
