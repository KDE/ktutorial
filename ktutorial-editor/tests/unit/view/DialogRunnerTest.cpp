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

#include <QTest>

#include "DialogRunner.h"

#include <QDialog>
#include <QTimer>

class DialogRunnerTest: public QObject {
Q_OBJECT

public slots:

    void hideShowAndAcceptDialog();
    void destroyDialog();

private slots:

    void init();
    void cleanup();

    void testConstructor();

    void testExec();
    void testExecWhenDialogIsDestroyed();

private:

    QPointer<QDialog> mDialog;

};

void DialogRunnerTest::init() {
    mDialog = new QDialog();
}

void DialogRunnerTest::cleanup() {
    delete mDialog;
}

void DialogRunnerTest::testConstructor() {
    QObject parent;
    QDialog dialog;
    DialogRunner* runner = new DialogRunner(&dialog, &parent);

    QCOMPARE(runner->parent(), &parent);
}

void DialogRunnerTest::testExec() {
    DialogRunner runner(mDialog);

    //DialogRunner has its own event loop, so it won't return to the test code
    //until the dialog is closed. Thus, the commands must be "queued", as
    //executing them after the runner.exec() won't work.
    QTimer::singleShot(500, this, SLOT(hideShowAndAcceptDialog()));

    int result = runner.exec();

    QCOMPARE(result, (int)QDialog::Accepted);
}

void DialogRunnerTest::testExecWhenDialogIsDestroyed() {
    DialogRunner runner(mDialog);

    //DialogRunner has its own event loop, so it won't return to the test code
    //until the dialog is closed. Thus, the commands must be "queued", as
    //executing them after the runner.exec() won't work.
    QTimer::singleShot(500, this, SLOT(destroyDialog()));

    int result = runner.exec();

    QCOMPARE(result, (int)QDialog::Rejected);
}

/////////////////////////////////// Helpers ////////////////////////////////////

void DialogRunnerTest::hideShowAndAcceptDialog() {
    QVERIFY(mDialog->isVisible());
    QVERIFY(mDialog->isModal());

    mDialog->hide();
    mDialog->show();
    mDialog->accept();
}

void DialogRunnerTest::destroyDialog() {
    QVERIFY(mDialog->isVisible());
    QVERIFY(mDialog->isModal());

    delete mDialog;
}

QTEST_MAIN(DialogRunnerTest)

#include "DialogRunnerTest.moc"
