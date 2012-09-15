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

#ifndef ACTIONLISTWIDGET_H
#define ACTIONLISTWIDGET_H

#include <QWidget>

class QToolButton;

/**
 * QWidget to show the added actions as QToolButtons.
 * Standard widgets show the actions added to them in a menu. This widget shows
 * them in a list of QToolButtons. Adding, inserting and removing actions is
 * supported.
 *
 * Each QToolButton is named like its action, followed by "ToolButton".
 *
 * A special tool button subclass, that ensures that there is always an icon
 * even if it is empty, is used. When there is an icon, Oxygen and other styles
 * align the text with the icon, instead of centering it like it is done when
 * there is no icon and a size policy other than fixed is used. A minimum
 * expanding size policiy is used for the tool buttons to fill the whole width
 * of this widget, instead of having each one its own width.
 */
class ActionListWidget: public QWidget {
Q_OBJECT
public:

    /**
     * Creates a new empty ActionListWidget.
     *
     * @param parent The parent widget.
     */
    explicit ActionListWidget(QWidget* parent = 0);

protected:

    /**
     * Handles adding, changing or deleting an action in this ActionListWidget.
     *
     * @param event The action event.
     */
    virtual void actionEvent(QActionEvent* event);

private:

    /**
     * Returns the QToolButton in this widget that shows the given action.
     *
     * @param action The action to get its QToolButton.
     */
    QToolButton* toolButtonForAction(QAction* action);

    /**
     * Adds a new QToolButton for the given action.
     *
     * @param action The action to add a QToolButton for it.
     * @param before Add the action before this tool button, or append it if it
     *               is 0.
     */
    void addToolButtonForAction(QAction* action, QToolButton* before);

};

#endif
