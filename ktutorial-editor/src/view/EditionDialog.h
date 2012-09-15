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

#ifndef EDITIONDIALOG_H
#define EDITIONDIALOG_H

#include <KDialog>

class EditionWidget;

/**
 * Wrapper dialog for EditionWidgets that provides Ok and Cancel buttons.
 * When the user clicks the "Ok" button, the changes are saved to the object
 * being edited by the EditionWidget.
 *
 * By default, the dialog is modal and gets its window title from the edition
 * widget own window title.
 *
 * @see EditionWidgdet
 */
class EditionDialog: public KDialog {
Q_OBJECT
public:

    /**
     * Creates and shows a new TutorialManagerDialog.
     * The EditionWidget is reparented to this dialog and destroyed when this
     * EditionDialog is destroyed.
     *
     * @param editionWidget The EditionWidget to wrap.
     * @param parent The parent widget of this dialog, defaults to null.
     */
    explicit EditionDialog(EditionWidget* editionWidget, QWidget* parent = 0);

protected Q_SLOTS:

    /**
     * Hides the dialog, saving the changes if the button clicked was "Ok"
     * button.
     *
     * @param button The button clicked.
     */
    virtual void slotButtonClicked(int button);

private:

    /**
     * The EditionWidget wrapped.
     */
    EditionWidget* mEditionWidget;

};

#endif
