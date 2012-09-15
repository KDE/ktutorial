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

#ifndef EDITIONWIDGET_H
#define EDITIONWIDGET_H

#include <QWidget>

/**
 * Base abstract class for edition widgets.
 * Widgets mean to edit any data in the tutorial must inherit from this class.
 * A generic edition dialog with Ok and Cancel buttons is provided by
 * EditionDialog class. The dialog wraps an EditionWidget and saves the changes
 * using saveChanges() method when needed.
 *
 * Subclasses must implement saveChanges method to commit the changes to the
 * object being edited.
 *
 * @see EditionDialog
 */
class EditionWidget: public QWidget {
Q_OBJECT
public:

    /**
     * Creates a new EditionWidget with the given parent.
     *
     * @param parent The parent widget.
     */
    EditionWidget(QWidget* parent = 0);

    /**
     * Saves the changes to the object being edited.
     * This method must be implemented in subclasses.
     */
    virtual void saveChanges() = 0;

};

#endif
