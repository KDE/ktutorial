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

#include <QtTest>

#include "EditionDialog.h"

#include <KPushButton>

#include "EditionWidget.h"

class EditionDialogTest: public QObject {
Q_OBJECT

private slots:

    void testConstructor();

    void testOk();
    void testCancel();

};

class MockEditionWidget: public EditionWidget {
public:

    int mSaveChangesCallCount;

    MockEditionWidget(QWidget* parent = 0): EditionWidget(parent),
        mSaveChangesCallCount(0) {
        setWindowTitle("The edition widget title");
    }

    virtual void saveChanges() {
        mSaveChangesCallCount++;
    }

};

void EditionDialogTest::testConstructor() {
    EditionWidget* editionWidget = new MockEditionWidget();
    QWidget parentWidget;
    EditionDialog* dialog = new EditionDialog(editionWidget, &parentWidget);

    QCOMPARE(dialog->mainWidget(), editionWidget);
    QCOMPARE(dialog->mainWidget()->parent(), dialog);
    QCOMPARE(dialog->parentWidget(), &parentWidget);
    QVERIFY(dialog->isModal());
    QCOMPARE(dialog->windowTitle(), QString("The edition widget title"));
    QVERIFY(dialog->button(KDialog::Ok));
    QVERIFY(dialog->button(KDialog::Cancel));
}

void EditionDialogTest::testOk() {
    MockEditionWidget* editionWidget = new MockEditionWidget();
    EditionDialog dialog(editionWidget);

    dialog.button(KDialog::Ok)->click();

    QCOMPARE(editionWidget->mSaveChangesCallCount, 1);
    QVERIFY(dialog.isHidden());
}

void EditionDialogTest::testCancel() {
    MockEditionWidget* editionWidget = new MockEditionWidget();
    EditionDialog dialog(editionWidget);

    dialog.button(KDialog::Cancel)->click();

    QCOMPARE(editionWidget->mSaveChangesCallCount, 0);
    QVERIFY(dialog.isHidden());
}

QTEST_MAIN(EditionDialogTest)

#include "EditionDialogTest.moc"
