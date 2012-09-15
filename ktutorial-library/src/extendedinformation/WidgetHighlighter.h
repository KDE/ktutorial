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

#ifndef KTUTORIAL_EXTENDEDINFORMATION_WIDGETHIGHLIGHTER_H
#define KTUTORIAL_EXTENDEDINFORMATION_WIDGETHIGHLIGHTER_H

#include <QTimer>
#include <QWidget>

namespace ktutorial {
namespace extendedinformation {

/**
 * Utility class to highlight a widget.
 * WidgetHighlighter executes an animation that tints its parent widget,
 * changing its colors from normal to highlighted and back again.
 *
 * Once started, the animation goes on indefinitely until the stop slot is
 * called. The parent widget recovers its original appearance animating the
 * change from the current color of the widget to the normal one.
 * If the highlighting is started again while the stop animation is running, the
 * highlighting animation is started again from the current color (that is, the
 * stop animation is cancelled and a new highlight animation is started from
 * that point).
 *
 * To tint the widget, the WidgetHighlighter paints itself over the parent
 * widget with a semi-transparent highlight color. However, due to performance
 * reasons, only a frame is tinted instead of the whole widget. The center of
 * the frame is always transparent.
 *
 * WidgetHighlighter should not be created directly. Instead,
 * WidgetHighlighterManager should be used, as it takes care of deleting the
 * highlighter when no longer needed.
 *
 * @see WidgetHighlighterManager
 */
class WidgetHighlighter: public QWidget {
Q_OBJECT
public:

    /**
     * Creates a new WidgetHighlighter for the given widget.
     *
     * @param targetWidget The widget to highlight.
     */
    explicit WidgetHighlighter(QWidget* targetWidget);

    /**
     * Resizes this WidgetHighlighter to the size of its parent widget when it
     * receives a resize event.
     *
     * @param watched The filtered object that received an event.
     * @param event The event received.
     * @return False, to allow the event to be handled further.
     */
    virtual bool eventFilter(QObject* watched, QEvent* event);

public Q_SLOTS:

    /**
     * Starts highlighting the target widget.
     * If there is a normal animation already running nothing is done. However,
     * if there is a stop animation running, it is cancelled and the
     * highlighting animation is started again from the current color.
     */
    void start();

    /**
     * Stops highlighting the target widget.
     * The animation is not stopped sharply, but a stop animation (the widget
     * recovering its normal color) is executed.
     */
    void stop();

Q_SIGNALS:

    /**
     * Emitted when the animation has stopped.
     * Note that it is emitted when the animation has truly stopped, that is,
     * when there is not even an stop animation.
     *
     * @param widgetHighlighter This WidgetHighlighter.
     */
    void stopped(extendedinformation::WidgetHighlighter* widgetHighlighter);

protected:

    /**
     * Paints a frame with a semi-transparent highlight color.
     * The degree of opacity depends on the progress of the animation.
     *
     * @param event The paint event.
     */
    virtual void paintEvent(QPaintEvent* event);

private:

    /**
     * The widget to highlight.
     */
    QWidget* mTargetWidget;

    /**
     * Timer to update the colors.
     */
    QTimer mTimer;

    /**
     * The current progress from normal to highlighted colors.
     * Range [0-1].
     */
    qreal mProgress;

    /**
     * How much advances the progress when an update is done.
     */
    qreal mProgressForEachTick;

    /**
     * True if the widget is being highlighted, false it is being dehighlighted.
     */
    bool mIncreasing;

    /**
     * True if the stop animation is being run, false otherwise.
     */
    bool mStopping;

    /**
     * The width of the frame used to highlight the widget.
     */
    int mFrameWidth;

private Q_SLOTS:

    /**
     * Updates the animation progress.
     */
    void updateProgress();

};

}
}

#endif
