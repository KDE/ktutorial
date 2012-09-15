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

#include "DialogRunner.h"

#include <QDialog>
#include <QEventLoop>

//public:

DialogRunner::DialogRunner(QDialog* dialog, QObject* parent):
        QObject(parent),
    mDialog(dialog) {
    Q_ASSERT(dialog);
}

int DialogRunner::exec() {
    Q_ASSERT(mDialog);

    mDialog->setModal(true);
    mDialog->show();

    connect(mDialog, SIGNAL(finished(int)), this, SLOT(exitEventLoop()));
    connect(mDialog, SIGNAL(destroyed(QObject*)), this, SLOT(exitEventLoop()));

    mEventLoop = new QEventLoop();
    mEventLoop->exec(QEventLoop::DialogExec);
    delete mEventLoop;

    if (!mDialog) {
        return QDialog::Rejected;
    }

    int result = mDialog->result();
    mDialog->deleteLater();

    return result;
}

//private slots:

void DialogRunner::exitEventLoop() {
    mEventLoop->exit();
}
