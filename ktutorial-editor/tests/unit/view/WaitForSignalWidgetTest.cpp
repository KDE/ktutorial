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

#include "WaitForSignalWidget.h"

#include <KLineEdit>

#include "../data/WaitForSignal.h"

class WaitForSignalWidgetTest: public QObject {
Q_OBJECT

private slots:

    void testConstructor();

    void testSaveChanges();

private:

    KLineEdit* emitterNameLineEdit(WaitForSignalWidget* widget) const;
    KLineEdit* signalNameLineEdit(WaitForSignalWidget* widget) const;

};

void WaitForSignalWidgetTest::testConstructor() {
    WaitForSignal waitFor;
    waitFor.setEmitterName("The emitter name");
    waitFor.setSignalName("The signal name");

    QWidget parent;
    WaitForSignalWidget* widget = new WaitForSignalWidget(&waitFor, &parent);

    QCOMPARE(widget->parentWidget(), &parent);
    QCOMPARE(emitterNameLineEdit(widget)->text(), QString("The emitter name"));
    QCOMPARE(signalNameLineEdit(widget)->text(), QString("The signal name"));
}

void WaitForSignalWidgetTest::testSaveChanges() {
    WaitForSignal waitFor;
    waitFor.setEmitterName("The emitter name");
    waitFor.setSignalName("The signal name");

    WaitForSignalWidget widget(&waitFor);
    emitterNameLineEdit(&widget)->setText("The new emitter name");
    signalNameLineEdit(&widget)->setText("The new signal name");

    widget.saveChanges();

    QCOMPARE(waitFor.emitterName(), QString("The new emitter name"));
    QCOMPARE(waitFor.signalName(), QString("The new signal name"));
}

/////////////////////////////////// Helpers ////////////////////////////////////

KLineEdit* WaitForSignalWidgetTest::emitterNameLineEdit(
                                            WaitForSignalWidget* widget) const {
#ifdef QT_QTDBUS_FOUND
    return widget->findChild<KLineEdit*>("objectNameLineEdit");
#else
    return widget->findChild<KLineEdit*>("emitterNameLineEdit");
#endif
}

KLineEdit* WaitForSignalWidgetTest::signalNameLineEdit(
                                            WaitForSignalWidget* widget) const {
    return widget->findChild<KLineEdit*>("signalNameLineEdit");
}

QTEST_MAIN(WaitForSignalWidgetTest)

#include "WaitForSignalWidgetTest.moc"
