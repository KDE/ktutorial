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

#include <QWidget>

#include "WindowVisibilitySpy.h"

namespace ktutorial {
namespace common {

class WindowVisibilitySpyTest: public QObject {
Q_OBJECT
private slots:

    void testConstructor();

    void testShowWidgetAsWindow();
    void testHideWidgetAsWindow();
    void testDeleteWidgetShownAsWindow();
    void testShowWidgetAsWidget();
    void testHideWidgetAsWidget();
    void testDeleteWidgetShownAsWidget();

    void testShowChildWidgetAddedBeforeSpy();
    void testShowChildWidgetAddedAfterSpy();

};

void WindowVisibilitySpyTest::testConstructor() {
    QObject parent;
    WindowVisibilitySpy* spy = new WindowVisibilitySpy(&parent);

    QCOMPARE(spy->parent(), &parent);
}

void WindowVisibilitySpyTest::testShowWidgetAsWindow() {
    QWidget widget;
    widget.setWindowFlags(Qt::Window);

    WindowVisibilitySpy spy;
    spy.addWidgetToSpy(&widget);

    QSignalSpy shownSpy(&spy, SIGNAL(windowShown(QWidget*)));

    widget.show();

    QCOMPARE(shownSpy.count(), 1);
    QVariant argument = shownSpy.at(0).at(0);
    QCOMPARE(argument.userType(), (int)QMetaType::QWidgetStar);
    QCOMPARE(qvariant_cast<QWidget*>(argument), &widget);
}

void WindowVisibilitySpyTest::testHideWidgetAsWindow() {
    QWidget widget;
    widget.setWindowFlags(Qt::Window);
    widget.show();

    WindowVisibilitySpy spy;
    spy.addWidgetToSpy(&widget);

    QSignalSpy hiddenSpy(&spy, SIGNAL(windowHidden(QWidget*)));

    widget.hide();

    QCOMPARE(hiddenSpy.count(), 1);
    QVariant argument = hiddenSpy.at(0).at(0);
    QCOMPARE(argument.userType(), (int)QMetaType::QWidgetStar);
    QCOMPARE(qvariant_cast<QWidget*>(argument), &widget);
}

void WindowVisibilitySpyTest::testDeleteWidgetShownAsWindow() {
    QWidget* widget = new QWidget();
    widget->setWindowFlags(Qt::Window);
    widget->show();

    WindowVisibilitySpy spy;
    spy.addWidgetToSpy(widget);

    QSignalSpy hiddenSpy(&spy, SIGNAL(windowHidden(QWidget*)));

    delete widget;

    QCOMPARE(hiddenSpy.count(), 1);
    QVariant argument = hiddenSpy.at(0).at(0);
    QCOMPARE(argument.userType(), (int)QMetaType::QWidgetStar);
    QCOMPARE(qvariant_cast<QWidget*>(argument), widget);
}

void WindowVisibilitySpyTest::testShowWidgetAsWidget() {
    QWidget window;
    window.show();

    QWidget* widget = new QWidget(&window);

    WindowVisibilitySpy spy;
    spy.addWidgetToSpy(widget);

    QSignalSpy shownSpy(&spy, SIGNAL(windowShown(QWidget*)));

    widget->show();

    QCOMPARE(shownSpy.count(), 0);
}

void WindowVisibilitySpyTest::testHideWidgetAsWidget() {
    QWidget window;
    window.show();

    QWidget* widget = new QWidget(&window);
    widget->show();

    WindowVisibilitySpy spy;
    spy.addWidgetToSpy(widget);

    QSignalSpy hiddenSpy(&spy, SIGNAL(windowHidden(QWidget*)));

    widget->hide();

    QCOMPARE(hiddenSpy.count(), 0);
}

void WindowVisibilitySpyTest::testDeleteWidgetShownAsWidget() {
    QWidget window;
    window.show();

    QWidget* widget = new QWidget(&window);
    widget->show();

    WindowVisibilitySpy spy;
    spy.addWidgetToSpy(widget);

    QSignalSpy shownSpy(&spy, SIGNAL(windowShown(QWidget*)));

    delete widget;

    QCOMPARE(shownSpy.count(), 0);
}

void WindowVisibilitySpyTest::testShowChildWidgetAddedBeforeSpy() {
    QWidget window;
    QWidget* childWidget = new QWidget(&window);
    window.show();

    QWidget* grandChildWindow = new QWidget(childWidget);
    grandChildWindow->setWindowFlags(Qt::Window);

    WindowVisibilitySpy spy;
    spy.addWidgetToSpy(&window);

    QSignalSpy shownSpy(&spy, SIGNAL(windowShown(QWidget*)));

    grandChildWindow->show();

    QCOMPARE(shownSpy.count(), 1);
    QVariant argument = shownSpy.at(0).at(0);
    QCOMPARE(argument.userType(), (int)QMetaType::QWidgetStar);
    QCOMPARE(qvariant_cast<QWidget*>(argument), grandChildWindow);
}

void WindowVisibilitySpyTest::testShowChildWidgetAddedAfterSpy() {
    QWidget window;
    QWidget* childWidget = new QWidget(&window);
    window.show();

    WindowVisibilitySpy spy;
    spy.addWidgetToSpy(&window);

    QSignalSpy shownSpy(&spy, SIGNAL(windowShown(QWidget*)));

    QWidget* grandChildWindow = new QWidget(childWidget);
    grandChildWindow->setWindowFlags(Qt::Window);
    grandChildWindow->show();

    QCOMPARE(shownSpy.count(), 1);
    QVariant argument = shownSpy.at(0).at(0);
    QCOMPARE(argument.userType(), (int)QMetaType::QWidgetStar);
    QCOMPARE(qvariant_cast<QWidget*>(argument), grandChildWindow);
}

}
}

QTEST_MAIN(ktutorial::common::WindowVisibilitySpyTest)

#include "WindowVisibilitySpyTest.moc"
