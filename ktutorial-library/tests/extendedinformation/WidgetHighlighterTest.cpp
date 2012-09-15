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

#include <QApplication>
#include <QWidget>

#define protected public
#define private public
#include "WidgetHighlighter.h"
#undef private
#undef protected

//WidgetHighlighter* must be declared as a metatype to be used in qvariant_cast
Q_DECLARE_METATYPE(ktutorial::extendedinformation::WidgetHighlighter*);

namespace ktutorial {
namespace extendedinformation {

class WidgetHighlighterTest: public QObject {
Q_OBJECT
private slots:

    void testConstructor();

    void testStart();
    void testStartAlreadyStarted();
    void testStartWhileStopAnimationIsRunning();

    void testUpdateProgress();
    void testUpdateProgressWhenProgressIsAlmostOne();
    void testUpdateProgressWhenProgressIsAlmostZero();

    void testStop();
    void testStopAfterStopAnimationEnded();
    void testStopImmediatelyAfterStart();

    void testParentWidgetResized();

};

void WidgetHighlighterTest::testConstructor() {
    QWidget widget;
    WidgetHighlighter* highlighter = new WidgetHighlighter(&widget);

    QCOMPARE(highlighter->parent(), &widget);
    QCOMPARE(highlighter->size(), widget.size());
    QCOMPARE(highlighter->mProgress, 0.0);
}

void WidgetHighlighterTest::testStart() {
    QWidget widget;
    WidgetHighlighter* highlighter = new WidgetHighlighter(&widget);

    highlighter->start();

    //Give it some time to update
    QTest::qWait(100);

    QVERIFY(highlighter->mProgress > 0);
}

void WidgetHighlighterTest::testStartAlreadyStarted() {
    QWidget widget;
    WidgetHighlighter* highlighter = new WidgetHighlighter(&widget);

    highlighter->start();

    //Give it some time to update
    QTest::qWait(100);

    qreal previousProgress = highlighter->mProgress;
    highlighter->start();

    //Ensure that progress is not reseted
    QCOMPARE(highlighter->mProgress, previousProgress);
}

void WidgetHighlighterTest::testStartWhileStopAnimationIsRunning() {
    QWidget widget;
    WidgetHighlighter* highlighter = new WidgetHighlighter(&widget);

    highlighter->mProgress = 0.5;
    highlighter->start();

    //Give it some time to update
    QTest::qWait(100);

    highlighter->stop();

    //Give it some time to update
    QTest::qWait(100);

    highlighter->start();

    //Give it some time to update
    QTest::qWait(100);

    QVERIFY(highlighter->mProgress > 0.5);
    QVERIFY(highlighter->mIncreasing);
    QVERIFY(!highlighter->mStopping);
}

void WidgetHighlighterTest::testUpdateProgress() {
    QWidget widget;
    WidgetHighlighter* highlighter = new WidgetHighlighter(&widget);
    highlighter->mTimer.stop();

    int previousProgress = highlighter->mProgress;

    highlighter->updateProgress();

    QCOMPARE(highlighter->mProgress,
             previousProgress + highlighter->mProgressForEachTick);
}

void WidgetHighlighterTest::testUpdateProgressWhenProgressIsAlmostOne() {
    QWidget widget;
    WidgetHighlighter* highlighter = new WidgetHighlighter(&widget);
    highlighter->mTimer.stop();

    highlighter->mProgress = 0.995;
    highlighter->mIncreasing = true;
    highlighter->updateProgress();

    QCOMPARE(highlighter->mProgress, 1.0);
    QCOMPARE(highlighter->mIncreasing, false);

    highlighter->updateProgress();

    QCOMPARE(highlighter->mProgress, 1 - highlighter->mProgressForEachTick);
    QCOMPARE(highlighter->mIncreasing, false);
}

void WidgetHighlighterTest::testUpdateProgressWhenProgressIsAlmostZero() {
    QWidget widget;
    WidgetHighlighter* highlighter = new WidgetHighlighter(&widget);
    highlighter->mTimer.stop();

    highlighter->mProgress = 0.005;
    highlighter->mIncreasing = false;
    highlighter->updateProgress();

    QCOMPARE(highlighter->mProgress, 0.0);
    QCOMPARE(highlighter->mIncreasing, true);

    highlighter->updateProgress();

    QCOMPARE(highlighter->mProgress, highlighter->mProgressForEachTick);
    QCOMPARE(highlighter->mIncreasing, true);
}

void WidgetHighlighterTest::testStop() {
    QWidget widget;
    WidgetHighlighter* highlighter = new WidgetHighlighter(&widget);

    highlighter->mProgress = 0.5;
    highlighter->start();

    //Give it some time to update
    QTest::qWait(100);

    highlighter->stop();

    //Give it some time to update
    QTest::qWait(200);

    QVERIFY(highlighter->mTimer.isActive());
    QVERIFY(highlighter->mProgress < 0.5);
    QVERIFY(!highlighter->mIncreasing);
    QVERIFY(highlighter->mStopping);
}

void WidgetHighlighterTest::testStopAfterStopAnimationEnded() {
    QWidget widget;
    WidgetHighlighter* highlighter = new WidgetHighlighter(&widget);

    highlighter->start();

    //Give it some time to update
    QTest::qWait(100);

    //extendedinformation::WidgetHighlighter* must be registered in order to be
    //used with QSignalSpy
    int widgetHighlighterStarType =
            qRegisterMetaType<extendedinformation::WidgetHighlighter*>(
                                    "extendedinformation::WidgetHighlighter*");
    QSignalSpy stoppedSpy(highlighter,
                    SIGNAL(stopped(extendedinformation::WidgetHighlighter*)));

    highlighter->stop();

    //Give it some time to update
    QTest::qWait(200);

    QVERIFY(!highlighter->mTimer.isActive());
    QCOMPARE(highlighter->mProgress, 0.0);
    QCOMPARE(stoppedSpy.count(), 1);
    QVariant argument = stoppedSpy.at(0).at(0);
    QCOMPARE(argument.userType(), widgetHighlighterStarType);
    QCOMPARE(qvariant_cast<extendedinformation::WidgetHighlighter*>(argument),
             highlighter);
}

void WidgetHighlighterTest::testStopImmediatelyAfterStart() {
    QWidget widget;
    WidgetHighlighter* highlighter = new WidgetHighlighter(&widget);

    highlighter->start();

    //extendedinformation::WidgetHighlighter* must be registered in order to be
    //used with QSignalSpy
    int widgetHighlighterStarType =
            qRegisterMetaType<extendedinformation::WidgetHighlighter*>(
                                    "extendedinformation::WidgetHighlighter*");
    QSignalSpy stoppedSpy(highlighter,
                    SIGNAL(stopped(extendedinformation::WidgetHighlighter*)));

    highlighter->stop();

    QVERIFY(!highlighter->mTimer.isActive());
    QCOMPARE(highlighter->mProgress, 0.0);
    QCOMPARE(stoppedSpy.count(), 1);
    QVariant argument = stoppedSpy.at(0).at(0);
    QCOMPARE(argument.userType(), widgetHighlighterStarType);
    QCOMPARE(qvariant_cast<extendedinformation::WidgetHighlighter*>(argument),
             highlighter);
}

void WidgetHighlighterTest::testParentWidgetResized() {
    QWidget widget;
    WidgetHighlighter* highlighter = new WidgetHighlighter(&widget);

    //The widget must be visible to ensure that it receives the resize event
    widget.show();

    //Resize twice to prevent a false test if the first size was the current
    //widget size
    widget.resize(4, 4);
    widget.resize(108, 108);

    QCOMPARE(highlighter->size(), widget.size());
}

}
}

QTEST_MAIN(ktutorial::extendedinformation::WidgetHighlighterTest)

#include "WidgetHighlighterTest.moc"
