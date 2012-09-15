/***************************************************************************
 *   Copyright (C) 2008 by Daniel Calviño Sánchez <danxuliu@gmail.com>     *
 *   Copyright (C) 2009 by Daniel Calviño Sánchez <danxuliu@gmail.com>     *
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

#ifndef KTUTORIAL_VIEW_STEPWIDGET_H
#define KTUTORIAL_VIEW_STEPWIDGET_H

#include <QFrame>
#include <QList>

class QCloseEvent;
class QHBoxLayout;
class QKeyEvent;
class QMouseEvent;
class QPaintEvent;

namespace ktutorial {
class Option;
class Step;
}

namespace Ui {
class StepWidget;
}

namespace ktutorial {
namespace view {

/**
 * Widget to show tutorials.
 * This widget shows the Steps of a Tutorial. When a new Step is activated,
 * setStep(Step*) is called to show the text and options of that Step to the
 * user. When the user closes the StepWidget, through the close button or using
 * Esc key, the Tutorial is finished.
 *
 * The StepWidget is shown as a floating widget, always on top of the
 * application (but not on top of other applications). It contains a close
 * button, the text of the active Step and one button for each Option of the
 * Step, if any.
 *
 * It can be moved from its initial position clicking with the left button and
 * dragging it.
 */
class StepWidget: public QFrame {
Q_OBJECT
public:

    /**
     * Creates a new StepWidget.
     *
     * @param tutorialName The name of the Tutorial to show.
     * @param parent The parent object.
     */
    explicit StepWidget(const QString& tutorialName, QWidget* parent = 0);

    /**
     * Destroys this StepWidget.
     */
    virtual ~StepWidget();

    /**
     * Sets the main application window to spy for child dialogs.
     * When a modal dialog is shown, this StepWidget will be reparented to the
     * dialog to avoid being blocked by it. The previous parent will be restored
     * when the modal dialog is hidden.
     *
     * @param mainApplicationWindow The main application window.
     */
    void setMainApplicationWindow(QWidget* mainApplicationWindow);

public slots:

    /**
     * Sets another Step to be shown.
     * The text is set and as many buttons as options added.
     *
     * The size is adjusted and, if it was hidden, this StepWidget is shown.
     *
     * @param step The step to be shown.
     */
    void setStep(Step* step);

signals:

    /**
     * This signal is emitted when the Tutorial is closed by the user.
     */
    void finished();

protected:

    /**
     * Executes parent handler, emits finished() signal and schedules this 
     * StepWidget for deletion.
     * close() can't be redefined to emit finished() signal, as it doesn't get a 
     * close triggered by the window manager (like using ALT+F4). Moreover, 
     * close() itself sends a QCloseEvent, which is handled by this method.
     * Likely, Qt::WA_DeleteOnClose isn't used as it will delete this StepWidget 
     * only through close(), but not through a QCloseEvent. Instead, this
     * StepWidget is scheduled for deletion after receiving a QCloseEvent.
     *
     * @param event The QCloseEvent.
     */
    virtual void closeEvent(QCloseEvent* event);

    /**
     * Adds some common dialog behaviour of KDE.
     * If Esc key is pressed, the tutorial finishes. It also enters "WhatIsThis"
     * mode if Shift+F1 is pressed. Any other case, executes parent handler.
     *
     * @param event The QKeyEvent.
     */
    virtual void keyPressEvent(QKeyEvent* event);

    /**
     * Moves this StepWidget if it is being dragged.
     * Movement is made using the data of the event and the drag offset.
     *
     * @param event The QMouseEvent.
     */
    virtual void mouseMoveEvent(QMouseEvent* event);

    /**
     * Enters in drag state if needed.
     * If left button is clicked it enters in drag state. Further movement of
     * the cursor will move this StepWidget.
     *
     * @param event The QMouseEvent.
     */
    virtual void mousePressEvent(QMouseEvent* event);

    /**
     * Exits drag state if needed.
     * If left button is clicked it exits drag state.
     *
     * @param event The QMouseEvent.
     */
    virtual void mouseReleaseEvent(QMouseEvent* event);

    /**
     * Adjusts the size of the widget and its children before painting.
     *
     * FIXME: isn't a better way to adjust the size than in paintEvent? It seems
     * that adjusting the size in setStep doesn't work as expected. Even if optionsWidget
     * is removed (so it isn't due to the dynamic creation of buttons), if the new
     * size of the widget is smaller than the previous one (due to a smaller text),
     * the previous one is kept.
     * See: http://lists.trolltech.com/qt-interest/2007-01/thread00715-0.html#msg00845
     *
     * Maybe the problem is that the slot setStep(Step*) is called from a signal
     * and how it is managed it in Qt. I thought I read something about adding
     * widgets runtime and when to do it. Would posting an event in setStep and
     * make the current tasks of that method when handling the event be the solution
     * to the problem?
     *
     * @param event The QPaintEvent.
     */
    virtual void paintEvent(QPaintEvent* event);

private:

    /**
     * Whether this StepWidget is currently being dragged or not.
     */
    bool mDragging;

    /**
     * The offset of the cursor to the top left corner.
     */
    QPoint mDragOffset;

    /**
     * The user interface elements.
     */
    Ui::StepWidget* ui;

    /**
     * The layout where buttons for Options are added.
     */
    QHBoxLayout* mOptionsLayout;

    /**
     * Sets the specified options.
     * A new button is added in the layout for Options for each Option in the
     * list. If there was any previous button for an Option of the previous
     * Step, it is removed.
     *
     * @param options The Options to set.
     */
    void setOptions(const QList<Option*>& options);

};

}
}

#endif
