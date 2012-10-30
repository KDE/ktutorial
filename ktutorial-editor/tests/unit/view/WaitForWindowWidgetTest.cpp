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

#include "WaitForWindowWidget.h"

#include <KLineEdit>

#include "../data/WaitForWindow.h"

class WaitForWindowWidgetTest: public QObject {
Q_OBJECT

private slots:

    void testConstructor();

    void testSaveChanges();

private:

    KLineEdit* windowObjectNameLineEdit(WaitForWindowWidget* widget) const;

};

void WaitForWindowWidgetTest::testConstructor() {
    WaitForWindow waitFor;
    waitFor.setWindowObjectName("The window object name");

    QWidget parent;
    WaitForWindowWidget* widget = new WaitForWindowWidget(&waitFor, &parent);

    QCOMPARE(widget->parentWidget(), &parent);
    QCOMPARE(windowObjectNameLineEdit(widget)->text(),
             QString("The window object name"));
}

void WaitForWindowWidgetTest::testSaveChanges() {
    WaitForWindow waitFor;
    waitFor.setWindowObjectName("The window object name");

    WaitForWindowWidget widget(&waitFor);
    windowObjectNameLineEdit(&widget)->setText("The new window object name");

    widget.saveChanges();

    QCOMPARE(waitFor.windowObjectName(), QString("The new window object name"));
}

/////////////////////////////////// Helpers ////////////////////////////////////

KLineEdit* WaitForWindowWidgetTest::windowObjectNameLineEdit(
                                            WaitForWindowWidget* widget) const {
#ifdef QT_QTDBUS_FOUND
    return widget->findChild<KLineEdit*>("objectNameLineEdit");
#else
    return widget->findChild<KLineEdit*>("windowObjectNameLineEdit");
#endif
}

QTEST_MAIN(WaitForWindowWidgetTest)

#include "WaitForWindowWidgetTest.moc"
