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

#include "WidgetHighlighter.h"

#include <QPainter>
#include <QPaintEvent>

namespace ktutorial {
namespace extendedinformation {

//public:

WidgetHighlighter::WidgetHighlighter(QWidget* targetWidget):
        QWidget(targetWidget),
    mTargetWidget(targetWidget) {
    Q_ASSERT(targetWidget);

    setAttribute(Qt::WA_TransparentForMouseEvents);
    setFocusPolicy(Qt::NoFocus);

    resize(mTargetWidget->size());
    mTargetWidget->installEventFilter(this);

    //TODO Use QPropertyAnimation instead? Increase Qt version requirement in
    //CMakeLists.txt to Qt 4.6 if done.
    mProgress = 0;
    mIncreasing = true;
    mStopping = false;

    int interval = 80;
    int duration = 1000;
    mProgressForEachTick = interval / (qreal)duration;

    mTimer.setInterval(interval);
    connect(&mTimer, SIGNAL(timeout()), this, SLOT(updateProgress()));

    mFrameWidth = 6;

    show();
}

bool WidgetHighlighter::eventFilter(QObject* watched, QEvent* event) {
    if (watched != mTargetWidget || event->type() != QEvent::Resize) {
        return false;
    }

    QResizeEvent* resizeEvent = static_cast<QResizeEvent*>(event);
    resize(resizeEvent->size());

    return false;
}

//public slots:

void WidgetHighlighter::start() {
    mStopping = false;
    mIncreasing = true;

    mTimer.start();
}

void WidgetHighlighter::stop() {
    mStopping = true;
    mIncreasing = false;

    if (mProgress == 0) {
        mTimer.stop();
        emit stopped(this);
    }
}

//protected:

void WidgetHighlighter::paintEvent(QPaintEvent* event) {
    Q_UNUSED(event);

    //Painting the WidgetHighlighter over its parent widget with a
    //semi-transparent color is the best I could get. However, it has some
    //flaws. For example, a QMenu does not highlight its menu button. And some
    //widgets may be hardly highlighted if their background color is almost the
    //same as the highlight color (although it should not happen very often).
    //
    //Changing the parent widget palette does not work because some widgets
    //(like tool buttons) don't paint a background, only paint the text and get
    //its parent widget background. Forcing the painting of the background with
    //some color role does not help, as it may look ugly in some styles that use
    //a gradient for the background. Changing the palette has one benefit,
    //though, as it also changes the palette from the children widgets, which
    //means that a QMenu would highlight its menu button.
    //
    //Ideally, the highlighter should lighten its parent widget but when it is
    //too bright (for example, the white background of a text edit). In that
    //case, the parent widget should be darkened. To do this, however, the
    //WidgetHighlighter must know how its parent widget is painted.
    //
    //Calling QPixmap::grabWidget from the WidgetHighlighter::paintEvent is not
    //good, as it triggers a recursive paint event in its parent (provided the
    //WidgetHighlighter paintEvent is guarded against a recursive call, else the
    //application would directly hang). Calling it from the updateProgress and
    //storing the QPixmap in memory would theoretically work, but it showed some
    //strange artifacts.
    //
    //Setting a custom QGraphicsEffect does not seem like a good idea, as Qt can
    //be compiled without them, and because, as far as I know, only one effect
    //can  be used on a widget at a time (as making a Composite design pattern
    //with something like QComposedGraphicsEffect class is pretty easy and there
    //is no such class, I presume that it is not a good idea to use several
    //effects on the same widget, at least for now).
    //
    //Anyway, I do not know how to check whether a picture is light or dark
    //quickly enough to be done in a realtime animation, so... a
    //semi-transparent colored child widget is good enough until someone makes
    //something better ;)
    //
    //However, filling the whole widget and animating it is too CPU intensive
    //when tinting a parent widget with lots of child widgets (for example, the
    //main widget in showfoto). So, instead of tinting the whole parent only
    //a frame is painted and animated. In this case, the frame gets its full
    //opacity at the peak of the animation, instead of half the opacity as used
    //when tinting the whole widget.

    //The inactive palette does not usually have a lot of contrast between
    //normal color and highlight color (for example, a QStatusBar in Oxygen
    //style). The highlight color from the active palette is used in every case
    //to ensure that the widget is clearly highlighted, even if the window is
    //not the active one.
    QColor color = mTargetWidget->palette().color(QPalette::Active,
                                                  QPalette::Highlight);

    QPainter painter(this);
    painter.setOpacity(mProgress);

    QPen pen;
    pen.setWidth(mFrameWidth);
    pen.setColor(color);
    painter.setPen(pen);

    //Third and fourth arguments are width and height, not end coordinates
    painter.drawRect(pen.width() / 2, pen.width() / 2,
                     width() - pen.width(), height() - pen.width());
    painter.end();
}

//private slots:

void WidgetHighlighter::updateProgress(){
    if (mIncreasing) {
        mProgress += mProgressForEachTick;
        mProgress = qMin<qreal>(1, mProgress);
        mIncreasing = mProgress < 1;
    } else {
        mProgress -= mProgressForEachTick;
        mProgress = qMax<qreal>(0, mProgress);
        mIncreasing = mProgress == 0;
    }

    //Update just the frame of the widget instead of the whole widget for
    //performance
    update(0, 0, width(), mFrameWidth);
    update(0, 0, mFrameWidth, height());
    update(width() - mFrameWidth, 0, width(), height());
    update(0, height() - mFrameWidth, width(), height());

    if (mStopping && mProgress == 0) {
        mTimer.stop();
        emit stopped(this);
    }
}

}
}
