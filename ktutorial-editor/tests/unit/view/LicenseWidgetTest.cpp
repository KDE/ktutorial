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

#include "LicenseWidget.h"

#include <KLineEdit>
#include <KTextEdit>

#include "../data/Tutorial.h"

class LicenseWidgetTest: public QObject {
Q_OBJECT

private slots:

    void testConstructor();
    void testConstructorWithRichText();

    void testSaveChanges();

private:

    KTextEdit* licenseTextEdit(LicenseWidget* widget) const;

};

void LicenseWidgetTest::testConstructor() {
    Tutorial tutorial;
    tutorial.setLicenseText("The license");

    QWidget parent;
    LicenseWidget* widget = new LicenseWidget(&tutorial, &parent);

    QCOMPARE(widget->parentWidget(), &parent);
    QCOMPARE(licenseTextEdit(widget)->toPlainText(), QString("The license"));
}

void LicenseWidgetTest::testConstructorWithRichText() {
    Tutorial tutorial;
    tutorial.setLicenseText("<p>The license</p>");

    QWidget parent;
    LicenseWidget* widget = new LicenseWidget(&tutorial, &parent);

    QCOMPARE(widget->parentWidget(), &parent);
    QCOMPARE(licenseTextEdit(widget)->toPlainText(),
             QString("<p>The license</p>"));
}

void LicenseWidgetTest::testSaveChanges() {
    Tutorial tutorial;
    tutorial.setLicenseText("The license");

    LicenseWidget widget(&tutorial);
    licenseTextEdit(&widget)->setText("The new license");

    widget.saveChanges();

    QCOMPARE(tutorial.licenseText(), QString("The new license"));
}

/////////////////////////////////// Helpers ////////////////////////////////////

KTextEdit* LicenseWidgetTest::licenseTextEdit(LicenseWidget* widget) const {
    return widget->findChild<KTextEdit*>("licenseTextEdit");
}

QTEST_MAIN(LicenseWidgetTest)

#include "LicenseWidgetTest.moc"
