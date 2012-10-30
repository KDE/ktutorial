/***************************************************************************
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

#include <QTest>

#include "SemanticMarkupLinkWidget.h"

#include <QRadioButton>

#include <KLineEdit>

class SemanticMarkupLinkWidgetTest: public QObject {
Q_OBJECT

private slots:

    void testConstructor();

    void testSelectLinkType();

    void testUrlWithGenericLink();
    void testUrlWithWidgetLink();

    void testSetUrlWithGenericLink();
    void testSetUrlWithWidgetLink();

private:

    QRadioButton* genericLinkRadioButton(SemanticMarkupLinkWidget* widget)
                                                                        const;
    QRadioButton* widgetLinkRadioButton(SemanticMarkupLinkWidget* widget) const;

    KLineEdit* genericLinkLineEdit(SemanticMarkupLinkWidget* widget) const;
    KLineEdit* widgetLinkLineEdit(SemanticMarkupLinkWidget* widget) const;

};

void SemanticMarkupLinkWidgetTest::testConstructor() {
    QWidget parent;
    SemanticMarkupLinkWidget* widget = new SemanticMarkupLinkWidget(&parent);

    QCOMPARE(widget->parentWidget(), &parent);
    QVERIFY(genericLinkRadioButton(widget)->isChecked());
    QVERIFY(genericLinkLineEdit(widget)->isEnabled());
    QVERIFY(!widgetLinkRadioButton(widget)->isChecked());
    QVERIFY(!widgetLinkLineEdit(widget)->isEnabled());
}

void SemanticMarkupLinkWidgetTest::testSelectLinkType() {
    SemanticMarkupLinkWidget widget;

    widgetLinkRadioButton(&widget)->click();

    QVERIFY(!genericLinkRadioButton(&widget)->isChecked());
    QVERIFY(!genericLinkLineEdit(&widget)->isEnabled());
    QVERIFY(widgetLinkRadioButton(&widget)->isChecked());
    QVERIFY(widgetLinkLineEdit(&widget)->isEnabled());

    genericLinkRadioButton(&widget)->click();

    QVERIFY(genericLinkRadioButton(&widget)->isChecked());
    QVERIFY(genericLinkLineEdit(&widget)->isEnabled());
    QVERIFY(!widgetLinkRadioButton(&widget)->isChecked());
    QVERIFY(!widgetLinkLineEdit(&widget)->isEnabled());
}

void SemanticMarkupLinkWidgetTest::testUrlWithGenericLink() {
    SemanticMarkupLinkWidget widget;

    genericLinkRadioButton(&widget)->click();
    genericLinkLineEdit(&widget)->setText("http://www.kde.org");

    QCOMPARE(widget.url(), QString("http://www.kde.org"));
}

void SemanticMarkupLinkWidgetTest::testUrlWithWidgetLink() {
    SemanticMarkupLinkWidget widget;

    widgetLinkRadioButton(&widget)->click();
    widgetLinkLineEdit(&widget)->setText("okButton");

    QCOMPARE(widget.url(), QString("widget:okButton"));
}

void SemanticMarkupLinkWidgetTest::testSetUrlWithGenericLink() {
    SemanticMarkupLinkWidget widget;

    widgetLinkRadioButton(&widget)->click();

    widget.setUrl("http://www.kde.org");

    QVERIFY(genericLinkRadioButton(&widget)->isChecked());
    QCOMPARE(genericLinkLineEdit(&widget)->text(),
             QString("http://www.kde.org"));
}

void SemanticMarkupLinkWidgetTest::testSetUrlWithWidgetLink() {
    SemanticMarkupLinkWidget widget;

    genericLinkRadioButton(&widget)->click();

    widget.setUrl("widget:okButton");

    QVERIFY(widgetLinkRadioButton(&widget)->isChecked());
    QCOMPARE(widgetLinkLineEdit(&widget)->text(), QString("okButton"));
}

/////////////////////////////////// Helpers ////////////////////////////////////

QRadioButton* SemanticMarkupLinkWidgetTest::genericLinkRadioButton(
                                    SemanticMarkupLinkWidget* widget) const {
    return widget->findChild<QRadioButton*>("genericLinkRadioButton");
}

QRadioButton* SemanticMarkupLinkWidgetTest::widgetLinkRadioButton(
                                    SemanticMarkupLinkWidget* widget) const {
    return widget->findChild<QRadioButton*>("widgetLinkRadioButton");
}

KLineEdit* SemanticMarkupLinkWidgetTest::genericLinkLineEdit(
                                    SemanticMarkupLinkWidget* widget) const {
    return widget->findChild<KLineEdit*>("genericLinkLineEdit");
}

KLineEdit* SemanticMarkupLinkWidgetTest::widgetLinkLineEdit(
                                    SemanticMarkupLinkWidget* widget) const {
#ifdef QT_QTDBUS_FOUND
    return widget->findChild<KLineEdit*>("objectNameLineEdit");
#else
    return widget->findChild<KLineEdit*>("widgetLinkLineEdit");
#endif
}

QTEST_MAIN(SemanticMarkupLinkWidgetTest)

#include "SemanticMarkupLinkWidgetTest.moc"
