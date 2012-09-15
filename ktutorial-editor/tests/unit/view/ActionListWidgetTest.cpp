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

#include "ActionListWidget.h"

#include <QAction>
#include <QLayout>
#include <QToolButton>

#include <KIcon>

class ActionListWidgetTest: public QObject {
Q_OBJECT

private slots:

    void testConstructor();

    void testAddAction();
    void testAddActionSeveralActions();

    void testInsertAction();
    void testInsertActionSeveralActions();

    void testRemoveAction();
    void testRemoveActionSeveralActions();

private:

    QToolButton* toolButton(const ActionListWidget* widget,
                            const QAction* action) const;

    void assertToolButton(const ActionListWidget* widget, int index,
                          const QAction* action) const;

};

void ActionListWidgetTest::testConstructor() {
    QWidget parent;
    ActionListWidget* widget = new ActionListWidget(&parent);

    QCOMPARE(widget->parent(), &parent);
    QVERIFY(widget->layout()->itemAt(widget->layout()->count()-1)->spacerItem());
}

void ActionListWidgetTest::testAddAction() {
    ActionListWidget widget;
    QAction* action = new QAction(this);
    action->setObjectName("action");

    widget.addAction(action);

    assertToolButton(&widget, 0, action);
    QVERIFY(widget.layout()->itemAt(widget.layout()->count()-1)->spacerItem());
}

void ActionListWidgetTest::testAddActionSeveralActions() {
    ActionListWidget widget;
    QAction* action1 = new QAction(this);
    action1->setObjectName("action1");
    QAction* action2 = new QAction(this);
    action2->setObjectName("action2");
    QAction* action3 = new QAction(this);
    action3->setObjectName("action3");
    action3->setIcon(KIcon("kde"));

    widget.addAction(action1);
    widget.addAction(action2);
    widget.addAction(action3);

    assertToolButton(&widget, 0, action1);
    assertToolButton(&widget, 1, action2);
    assertToolButton(&widget, 2, action3);
    QVERIFY(widget.layout()->itemAt(widget.layout()->count()-1)->spacerItem());
}

void ActionListWidgetTest::testInsertAction() {
    ActionListWidget widget;
    QAction* action = new QAction(this);
    action->setObjectName("action");

    widget.insertAction(0, action);

    assertToolButton(&widget, 0, action);
    QVERIFY(widget.layout()->itemAt(widget.layout()->count()-1)->spacerItem());
}

void ActionListWidgetTest::testInsertActionSeveralActions() {
    ActionListWidget widget;
    QAction* action1 = new QAction(this);
    action1->setObjectName("action1");
    QAction* action2 = new QAction(this);
    action2->setObjectName("action2");
    QAction* action3 = new QAction(this);
    action3->setObjectName("action3");
    action3->setIcon(KIcon("kde"));

    widget.insertAction(0, action3);
    widget.insertAction(action3, action1);
    widget.insertAction(action3, action2);

    assertToolButton(&widget, 0, action1);
    assertToolButton(&widget, 1, action2);
    assertToolButton(&widget, 2, action3);
    QVERIFY(widget.layout()->itemAt(widget.layout()->count()-1)->spacerItem());
}

void ActionListWidgetTest::testRemoveAction() {
    ActionListWidget widget;
    QAction* action = new QAction(this);
    action->setObjectName("action");

    widget.addAction(action);
    widget.removeAction(action);

    QCOMPARE(widget.layout()->count(), 1);
    QVERIFY(widget.layout()->itemAt(0)->spacerItem());
}

void ActionListWidgetTest::testRemoveActionSeveralActions() {
    ActionListWidget widget;
    QAction* action1 = new QAction(this);
    action1->setObjectName("action1");
    QAction* action2 = new QAction(this);
    action2->setObjectName("action2");
    QAction* action3 = new QAction(this);
    action3->setObjectName("action3");
    action3->setIcon(KIcon("kde"));

    widget.addAction(action1);
    widget.addAction(action2);
    widget.addAction(action3);

    widget.removeAction(action2);

    assertToolButton(&widget, 0, action1);
    assertToolButton(&widget, 1, action3);

    widget.removeAction(action1);
    widget.removeAction(action3);

    QCOMPARE(widget.layout()->count(), 1);
    QVERIFY(widget.layout()->itemAt(0)->spacerItem());
}

/////////////////////////////////// Helpers ////////////////////////////////////

QToolButton* ActionListWidgetTest::toolButton(const ActionListWidget* widget,
                                              const QAction* action) const {
    QString buttonName = action->objectName() + "ToolButton";
    return widget->findChild<QToolButton*>(buttonName);
}

void ActionListWidgetTest::assertToolButton(const ActionListWidget* widget,
                                    int index, const QAction* action) const {
    QToolButton* button = toolButton(widget, action);

    QVERIFY(button);
    QCOMPARE(button->parent(), widget);
    QCOMPARE(button->defaultAction(), action);
    QCOMPARE(widget->layout()->indexOf(button), index);
}

QTEST_MAIN(ActionListWidgetTest)

#include "ActionListWidgetTest.moc"
