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

#include "TutorialCustomCodeWidget.h"

#include <KLineEdit>
#include <KTextEdit>

#include "../data/Tutorial.h"

class TutorialCustomCodeWidgetTest: public QObject {
Q_OBJECT

private slots:

    void testConstructorSetup();
    void testConstructorSetupWithRichText();
    void testConstructorTearDown();
    void testConstructorTearDownWithRichText();

    void testSaveChangesSetup();
    void testSaveChangesTearDown();

private:

    KTextEdit* customCodeTextEdit(TutorialCustomCodeWidget* widget) const;

};

void TutorialCustomCodeWidgetTest::testConstructorSetup() {
    Tutorial tutorial;
    tutorial.setCustomSetupCode("The setup code");

    QWidget parent;
    TutorialCustomCodeWidget* widget =
                new TutorialCustomCodeWidget(&tutorial,
                                             TutorialCustomCodeWidget::Setup,
                                             &parent);

    QCOMPARE(widget->parentWidget(), &parent);
    QCOMPARE(customCodeTextEdit(widget)->toPlainText(),
             QString("The setup code"));
}

void TutorialCustomCodeWidgetTest::testConstructorSetupWithRichText() {
    Tutorial tutorial;
    tutorial.setCustomSetupCode("<p>The setup code</p>");

    QWidget parent;
    TutorialCustomCodeWidget* widget =
                new TutorialCustomCodeWidget(&tutorial,
                                             TutorialCustomCodeWidget::Setup,
                                             &parent);

    QCOMPARE(widget->parentWidget(), &parent);
    QCOMPARE(customCodeTextEdit(widget)->toPlainText(),
             QString("<p>The setup code</p>"));
}

void TutorialCustomCodeWidgetTest::testConstructorTearDown() {
    Tutorial tutorial;
    tutorial.setCustomTearDownCode("The tear down code");

    QWidget parent;
    TutorialCustomCodeWidget* widget =
                new TutorialCustomCodeWidget(&tutorial,
                                             TutorialCustomCodeWidget::TearDown,
                                             &parent);

    QCOMPARE(widget->parentWidget(), &parent);
    QCOMPARE(customCodeTextEdit(widget)->toPlainText(),
             QString("The tear down code"));
}

void TutorialCustomCodeWidgetTest::testConstructorTearDownWithRichText() {
    Tutorial tutorial;
    tutorial.setCustomTearDownCode("<p>The tear down code</p>");

    QWidget parent;
    TutorialCustomCodeWidget* widget =
                new TutorialCustomCodeWidget(&tutorial,
                                             TutorialCustomCodeWidget::TearDown,
                                             &parent);

    QCOMPARE(widget->parentWidget(), &parent);
    QCOMPARE(customCodeTextEdit(widget)->toPlainText(),
             QString("<p>The tear down code</p>"));
}

void TutorialCustomCodeWidgetTest::testSaveChangesSetup() {
    Tutorial tutorial;
    tutorial.setCustomSetupCode("The setup code");

    TutorialCustomCodeWidget widget(&tutorial, TutorialCustomCodeWidget::Setup);
    customCodeTextEdit(&widget)->setText("The new setup code");

    widget.saveChanges();

    QCOMPARE(tutorial.customSetupCode(), QString("The new setup code"));
}

void TutorialCustomCodeWidgetTest::testSaveChangesTearDown() {
    Tutorial tutorial;
    tutorial.setCustomTearDownCode("The tear down code");

    TutorialCustomCodeWidget widget(&tutorial,
                                    TutorialCustomCodeWidget::TearDown);
    customCodeTextEdit(&widget)->setText("The new tear down code");

    widget.saveChanges();

    QCOMPARE(tutorial.customTearDownCode(), QString("The new tear down code"));
}

/////////////////////////////////// Helpers ////////////////////////////////////

KTextEdit* TutorialCustomCodeWidgetTest::customCodeTextEdit(
                                    TutorialCustomCodeWidget* widget) const {
    return widget->findChild<KTextEdit*>("customCodeTextEdit");
}

QTEST_MAIN(TutorialCustomCodeWidgetTest)

#include "TutorialCustomCodeWidgetTest.moc"
