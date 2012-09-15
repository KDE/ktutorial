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

#include <QtTest>

#include "WaitForEventWidget.h"

#include <KLineEdit>

#include "../data/WaitForEvent.h"

class WaitForEventWidgetTest: public QObject {
Q_OBJECT

private slots:

    void testConstructor();

    void testSaveChanges();

    void testEventNameCompletion();

private:

    KLineEdit* receiverNameLineEdit(WaitForEventWidget* widget) const;
    KLineEdit* eventNameLineEdit(WaitForEventWidget* widget) const;

};

void WaitForEventWidgetTest::testConstructor() {
    WaitForEvent waitFor;
    waitFor.setReceiverName("The receiver name");
    waitFor.setEventName("The event name");

    QWidget parent;
    WaitForEventWidget* widget = new WaitForEventWidget(&waitFor, &parent);

    QCOMPARE(widget->parentWidget(), &parent);
    QCOMPARE(receiverNameLineEdit(widget)->text(),
             QString("The receiver name"));
    QCOMPARE(eventNameLineEdit(widget)->text(), QString("The event name"));
}

void WaitForEventWidgetTest::testSaveChanges() {
    WaitForEvent waitFor;
    waitFor.setReceiverName("The receiver name");
    waitFor.setEventName("The event name");

    WaitForEventWidget widget(&waitFor);
    receiverNameLineEdit(&widget)->setText("The new receiver name");
    eventNameLineEdit(&widget)->setText("The new event name");

    widget.saveChanges();

    QCOMPARE(waitFor.receiverName(), QString("The new receiver name"));
    QCOMPARE(waitFor.eventName(), QString("The new event name"));
}

void WaitForEventWidgetTest::testEventNameCompletion() {
    WaitForEvent waitFor;

    WaitForEventWidget widget(&waitFor);

    KCompletion* completion = eventNameLineEdit(&widget)->completionObject();
    QCOMPARE(completion->order(), KCompletion::Sorted);
    QCOMPARE(completion->allMatches("dest").count(), 1);
    QCOMPARE(completion->allMatches("dest")[0], QString("Destroy"));
}

/////////////////////////////////// Helpers ////////////////////////////////////

KLineEdit* WaitForEventWidgetTest::receiverNameLineEdit(
                                            WaitForEventWidget* widget) const {
#ifdef QT_QTDBUS_FOUND
    return widget->findChild<KLineEdit*>("objectNameLineEdit");
#else
    return widget->findChild<KLineEdit*>("receiverNameLineEdit");
#endif
}

KLineEdit* WaitForEventWidgetTest::eventNameLineEdit(
                                            WaitForEventWidget* widget) const {
    return widget->findChild<KLineEdit*>("eventNameLineEdit");
}

QTEST_MAIN(WaitForEventWidgetTest)

#include "WaitForEventWidgetTest.moc"
