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

#ifndef KTUTORIAL_VIEW_STEPTEXTWIDGET_H
#define KTUTORIAL_VIEW_STEPTEXTWIDGET_H

#include <QPointer>

#include <KTextEdit>

namespace ktutorial {
namespace view {

/**
 * A TextEdit with the appearance of a label to show the text of a step.
 * It behaves like a QLabel with custom behavior for links. However, as QLabel
 * doesn't provide a way to easily extend its link related behavior, a KTextEdit
 * with a QLabel appearance (read only, size fit to text, no scroll bars...) is
 * used instead.
 *
 * Links that exhibit a custom behavior are those with the format
 * "widget:nameOfTheWidget". When the link is pressed with the left mouse
 * button, the widget with the specified name is highlighted. When it is pressed
 * again, the highlighting is stopped. If the right button is pressed, a context
 * menu with an item to highlight or stop highlighting is shown.
 *
 * If a widget was being highlighted when the highglighting is started in
 * another widget, the highlighting in the previous widget is stopped. The
 * highlighting is also stopped when the widget gets the focus, or when the text
 * is changed in the StepTextWidget.
 *
 * When the mouse cursor is moved over a "widget:" link, the arrow cursor is
 * changed to a pointing hand cursor.
 *
 * The links are specified using HTML markup, for example,
 * &lt;a href="widget:theNameOfTheWidget"&gt;the text of the link&lt;/a&gt;
 */
class StepTextWidget: public KTextEdit {
Q_OBJECT
public:

    /**
     * Creates a new StepTextWidget.
     *
     * @param parent The parent widget.
     */
    explicit StepTextWidget(QWidget* parent = 0);

    /**
     * Destroys this StepTextWidget.
     * If there is a widget being highlighted, the highlighting is stopped.
     */
    virtual ~StepTextWidget();

    /**
     * Watches the widget currently being highlighted and stops the highlighting
     * when it is focused.
     *
     * @param watched The filtered object that received an event.
     * @param event The event received.
     * @return False, to allow the event to be handled further.
     */
    virtual bool eventFilter(QObject* watched, QEvent* event);

    /**
     * Returns the height of the size for the given width.
     *
     * @param width The width to get its height.
     * @return The height for the width.
     */
    virtual int heightForWidth(int width) const;

    /**
     * Returns the recommended minimum size for this StepTextWidget.
     * The size has the width of the longest word in the text and the height of
     * a single line.
     *
     * @return The recommended minimum size.
     */
    virtual QSize minimumSizeHint() const;

    /**
     * Returns the recommended size for this StepTextWidget.
     * The size is adjusted to fit the text as a rectangle, with a width bigger
     * then the height.
     *
     * @return The recommended size.
     */
    virtual QSize sizeHint() const;

protected:

    /**
     * Shows a custom context menu when a context menu is requested on a
     * "widget:" anchor.
     * The context menu will contain a "Stop highlighting" or a "Highlight" item
     * depending on whether the widget is the one currently being highlighted or
     * not.
     *
     * @param event The context menu event.
     */
    virtual void contextMenuEvent(QContextMenuEvent* event);

    /**
     * Changes the cursor to a pointing hand when it is over a "widget:" anchor.
     *
     * @param event The mouse move event.
     */
    virtual void mouseMoveEvent(QMouseEvent* event);

    /**
     * When the mouse is pressed on a "widget:" anchor, it is highlighted or
     * stopped being highlighted.
     *
     * @param event The mouse press event.
     */
    virtual void mousePressEvent(QMouseEvent* event);

private:

    /**
     * The last widget referenced by a link activated or selected in this
     * StepTextWidget.
     */
    QPointer<QWidget> mLastReferencedWidget;

    /**
     * The widget being highlighted.
     */
    QPointer<QWidget> mWidgetBeingHighlighted;

    /**
     * Returns the size for the given text width.
     * If the width is < 0, the size is adjusted to the text as a rectangle,
     * with a width bigger than the height.
     * If the width is >= 0, the size is the given width and a height bigger
     * enough to show the whole text. The width may be bigger than the given one
     * if the given one is not big enough to show the longest word in the text.
     *
     * @param width The width to get its size.
     * @return The size for the width.
     */
    QSize sizeForWidth(int width) const;

    /**
     * Returns the widget referenced in the given anchor.
     * The anchor must have a "widget:name" format. The widget is looked for
     * using its name in KTutorial::findObject(QString).
     *
     * @param anchor The anchor to get its widget.
     * @return The widget referenced in the anchor.
     */
    QWidget* widgetForAnchor(const QString& anchor);

private Q_SLOTS:

    /**
     * Notifies the layout that the geometry of the widget has changed and stops
     * the widget currently being highlighted, if any.
     */
    void updateText();

    /**
     * Starts highlighting the last referenced widget.
     * If there is any widget currently being highlighted, it is stopped.
     */
    void highlightLastReferencedWidget();

    /**
     * Stops highlighting the widget currently being highlighted.
     */
    void stopHighlightingCurrentWidget();

};

}
}

#endif
