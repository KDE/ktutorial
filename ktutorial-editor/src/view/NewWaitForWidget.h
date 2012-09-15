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

#ifndef NEWWAITFORWIDGET_H
#define NEWWAITFORWIDGET_H

#include <QWidget>

class WaitFor;

namespace Ui {
class NewWaitForWidget;
}

/**
 * Widget to create a new condition.
 * Note that this is not an edition widget, as it has no data to edit. It
 * creates a new object, which can be got using waitFor() method.
 */
class NewWaitForWidget: public QWidget {
Q_OBJECT
public:

    /**
     * Creates a new NewWaitForWidget.
     *
     * @param parent The parent QWidget.
     */
    explicit NewWaitForWidget(QWidget* parent = 0);

    /**
     * Destroys this widget.
     */
    virtual ~NewWaitForWidget();

    /**
     * Return the new WaitFor.
     *
     * @return The new WaitFor.
     */
    WaitFor* waitFor() const;

private:

    /**
     * The Ui Designer generated class.
     */
    Ui::NewWaitForWidget* ui;

};

#endif
