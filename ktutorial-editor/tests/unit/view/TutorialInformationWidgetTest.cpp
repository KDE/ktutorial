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

#include "TutorialInformationWidget.h"

#include <KLineEdit>
#include <KTextEdit>

#include "../data/Tutorial.h"

class TutorialInformationWidgetTest: public QObject {
Q_OBJECT

private slots:

    void testConstructor();
    void testConstructorWithRichText();

    void testSaveChanges();

private:

    KLineEdit* nameLineEdit(TutorialInformationWidget* widget) const;
    KTextEdit* descriptionTextEdit(TutorialInformationWidget* widget) const;

};

void TutorialInformationWidgetTest::testConstructor() {
    Tutorial tutorial;
    tutorial.setName("The name");
    tutorial.setDescription("The description");

    QWidget parent;
    TutorialInformationWidget* widget = new TutorialInformationWidget(&tutorial,
                                                                      &parent);

    QCOMPARE(widget->parentWidget(), &parent);
    QCOMPARE(nameLineEdit(widget)->text(), QString("The name"));
    QCOMPARE(descriptionTextEdit(widget)->toPlainText(),
             QString("The description"));
}

void TutorialInformationWidgetTest::testConstructorWithRichText() {
    Tutorial tutorial;
    tutorial.setName("The name");
    tutorial.setDescription("<p>The description</p>");

    QWidget parent;
    TutorialInformationWidget* widget = new TutorialInformationWidget(&tutorial,
                                                                      &parent);

    QCOMPARE(widget->parentWidget(), &parent);
    QCOMPARE(nameLineEdit(widget)->text(), QString("The name"));
    QCOMPARE(descriptionTextEdit(widget)->toPlainText(),
             QString("<p>The description</p>"));
}

void TutorialInformationWidgetTest::testSaveChanges() {
    Tutorial tutorial;
    tutorial.setName("The name");
    tutorial.setDescription("The description");

    TutorialInformationWidget widget(&tutorial);
    nameLineEdit(&widget)->setText("The new name");
    descriptionTextEdit(&widget)->setText("The new description");

    widget.saveChanges();

    QCOMPARE(tutorial.name(), QString("The new name"));
    QCOMPARE(tutorial.description(), QString("The new description"));
}

/////////////////////////////////// Helpers ////////////////////////////////////

KLineEdit* TutorialInformationWidgetTest::nameLineEdit(
                                    TutorialInformationWidget* widget) const {
    return widget->findChild<KLineEdit*>("nameLineEdit");
}

KTextEdit* TutorialInformationWidgetTest::descriptionTextEdit(
                                    TutorialInformationWidget* widget) const {
    return widget->findChild<KTextEdit*>("descriptionTextEdit");
}

QTEST_MAIN(TutorialInformationWidgetTest)

#include "TutorialInformationWidgetTest.moc"
