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

#include <QTest>

#include <QWidget>

#include "WidgetHighlighterManager.h"
#define protected public
#define private public
#include "WidgetHighlighter.h"
#undef private
#undef protected

namespace ktutorial {
namespace extendedinformation {

class WidgetHighlighterManagerTest: public QObject {
Q_OBJECT
private slots:

    void testSelf();

    void testHighlight();
    void testHighlightWidgetAlreadyHighlighted();
    void testHighlightChildAlreadyHighlighted();
    void testHighlightAfterStopHighlighting();

    void testStopHighlighting();
    void testStopHighlightingWhenChildIsBeingHighlighted();

    void testDeleteWidgetWhileHighlighting();

private:

    WidgetHighlighter* highlighterOf(const QWidget* widget) const;

};

void WidgetHighlighterManagerTest::testSelf() {
    WidgetHighlighterManager* manager1 = WidgetHighlighterManager::self();
    WidgetHighlighterManager* manager2 = WidgetHighlighterManager::self();

    QVERIFY(manager1);
    QVERIFY(manager2);
    QVERIFY(manager1 == manager2);
}

void WidgetHighlighterManagerTest::testHighlight() {
    QWidget widget;
    WidgetHighlighterManager* manager = WidgetHighlighterManager::self();

    manager->highlight(&widget);

    //Give it some time to update
    QTest::qWait(100);

    QCOMPARE(widget.findChildren<WidgetHighlighter*>().count(), 1);
    QVERIFY(highlighterOf(&widget));
    QVERIFY(highlighterOf(&widget)->mProgress > 0);
}

void WidgetHighlighterManagerTest::testHighlightWidgetAlreadyHighlighted() {
    QWidget widget;
    WidgetHighlighterManager* manager = WidgetHighlighterManager::self();

    manager->highlight(&widget);

    WidgetHighlighter* highlighter = highlighterOf(&widget);

    manager->highlight(&widget);

    QCOMPARE(widget.findChildren<WidgetHighlighter*>().count(), 1);
    QCOMPARE(highlighterOf(&widget), highlighter);
}

void WidgetHighlighterManagerTest::testHighlightChildAlreadyHighlighted() {
    QWidget widget;
    QWidget* child = new QWidget(&widget);
    WidgetHighlighterManager* manager = WidgetHighlighterManager::self();

    manager->highlight(child);

    //If a normal pointer is used it can happen that the old highligther object
    //is deleted and the new one is created at the same memory address, thus
    //causing the assert to fail (even if the behavior was right). Using a
    //QPointer prevents this, as it is set to 0 when its object is deleted.
    QPointer<WidgetHighlighter> childHighlighter = highlighterOf(child);

    manager->highlight(&widget);

    QCOMPARE(widget.findChildren<WidgetHighlighter*>().count(), 1);
    QVERIFY(highlighterOf(&widget));
    QVERIFY(highlighterOf(&widget) != childHighlighter);
    QVERIFY(!highlighterOf(child));
}

void WidgetHighlighterManagerTest::testHighlightAfterStopHighlighting() {
    QWidget widget;
    WidgetHighlighterManager* manager = WidgetHighlighterManager::self();

    manager->highlight(&widget);

    QPointer<WidgetHighlighter> highlighter = highlighterOf(&widget);
    QVERIFY(highlighter);

    manager->stopHighlighting(&widget);

    QVERIFY(!highlighter);

    manager->highlight(&widget);

    //Give it some time to update
    QTest::qWait(100);

    QCOMPARE(widget.findChildren<WidgetHighlighter*>().count(), 1);
    QVERIFY(highlighterOf(&widget));
    QVERIFY(highlighterOf(&widget)->mProgress > 0);
}

void WidgetHighlighterManagerTest::testStopHighlighting() {
    QWidget widget;
    WidgetHighlighterManager* manager = WidgetHighlighterManager::self();

    manager->highlight(&widget);

    //Give it some time to update
    QTest::qWait(100);

    manager->stopHighlighting(&widget);

    //Give it some time to update
    QTest::qWait(200);

    QCOMPARE(widget.findChildren<WidgetHighlighter*>().count(), 0);
}

void WidgetHighlighterManagerTest::
                            testStopHighlightingWhenChildIsBeingHighlighted() {
    QWidget widget;
    QWidget* child = new QWidget(&widget);
    WidgetHighlighterManager* manager = WidgetHighlighterManager::self();

    manager->highlight(child);

    //Give it some time to update
    QTest::qWait(100);

    manager->stopHighlighting(&widget);

    //Give it some time to update
    QTest::qWait(200);

    //The highlighter should have not been stopped, as it was not highlighting
    //the parent widget, but the child
    QCOMPARE(widget.findChildren<WidgetHighlighter*>().count(), 1);
    QVERIFY(highlighterOf(child));
    QVERIFY(highlighterOf(child)->mProgress > 0);
    QVERIFY(highlighterOf(child)->mIncreasing);
}

void WidgetHighlighterManagerTest::testDeleteWidgetWhileHighlighting() {
    QWidget* widget = new QWidget();
    WidgetHighlighterManager* manager = WidgetHighlighterManager::self();

    manager->highlight(widget);

    //Give it some time to update
    QTest::qWait(100);

    delete widget;

    //No explicit check is made, if it does not crash everything is fine ;)
}

WidgetHighlighter* WidgetHighlighterManagerTest::highlighterOf(
                                                const QWidget* widget) const {
    WidgetHighlighter* highlighter = widget->findChild<WidgetHighlighter*>();

    //Ensure that it is a direct child
    if (widget->children().contains(highlighter)) {
        return highlighter;
    }

    return 0;
}

}
}

QTEST_MAIN(ktutorial::extendedinformation::WidgetHighlighterManagerTest)

#include "WidgetHighlighterManagerTest.moc"
