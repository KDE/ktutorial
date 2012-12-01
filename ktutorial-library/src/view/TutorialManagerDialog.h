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

#ifndef KTUTORIAL_VIEW_TUTORIALMANAGERDIALOG_H
#define KTUTORIAL_VIEW_TUTORIALMANAGERDIALOG_H

#include <KDialog>

class QItemSelection;

namespace ktutorial {
class TutorialInformation;
class TutorialManager;
}

namespace Ui {
class TutorialManagerDialog;
}

namespace ktutorial {
namespace view {

/**
 * A dialog to show the available tutorials and start them.
 * The tutorials are shown in a plain list containing their names. When a
 * tutorial is selected, its description is shown in a field designed for this
 * purpose.
 *
 * The dialog contains a user defined and a close button. The user defined
 * button is a start button, enabled when a tutorial is selected. Once a
 * tutorial is started, the dialog is hidden until it is notified that it has
 * finished.
 */
class TutorialManagerDialog: public KDialog {
Q_OBJECT
public:

    /**
     * Creates and shows a new TutorialManagerDialog.
     *
     * @param tutorialManager The TutorialManager to use.
     * @param parent The parent widget of this dialog, defaults to null.
     */
    explicit TutorialManagerDialog(TutorialManager* tutorialManager,
                                   QWidget* parent = 0);

    /**
     * Destroys this TutorialManagerDialog.
     */
    virtual ~TutorialManagerDialog();

public slots:

    /**
     * Receive notification that the tutorial being executed has finished.
     * It just shows again the dialog.
     */
    void finish();

private:

    /**
     * The TutorialManager to use.
     */
    TutorialManager* mTutorialManager;

    /**
     * The user interface elements.
     */
    Ui::TutorialManagerDialog* ui;

    /**
     * The TutorialInformation of the last activated tutorial in the list.
     */
    const TutorialInformation* mCurrentTutorialInformation;

    /**
     * Selects the tutorial identified by its TutorialInformation.
     * The description of the tutorial is shown in the Description field of this
     * dialog.
     *
     * @param tutorialInformation The TutorialInformation of the selected
     *        tutorial.
     */
    void select(const TutorialInformation* tutorialInformation);

private slots:

    /**
     * Selects the tutorial identified by its index in the list.
     * It is a wrapper for select(const TutorialInformation*) to get the
     * appropriate TutorialInformation from the index selected in the list.
     *
     * @param index The QItemSelection containing the selected item in the list.
     */
    void select(const QItemSelection& selected);

    /**
     * Starts the selected tutorial.
     * It hides the dialog before executing it.
     */
    void start();

};

}
}

#endif
