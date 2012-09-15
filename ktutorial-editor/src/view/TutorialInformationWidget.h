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

#ifndef TUTORIALINFORMATIONWIDGET_H
#define TUTORIALINFORMATIONWIDGET_H

#include "CommandWidget.h"

class Tutorial;

namespace Ui {
class TutorialInformationWidget;
}

/**
 * Command widget for the name and description of a Tutorial.
 */
class TutorialInformationWidget: public CommandWidget {
Q_OBJECT
public:

    /**
     * Creates a new TutorialInformationWidget for the given Tutorial.
     *
     * @param tutorial The tutorial to edit.
     * @param parent The parent QWidget.
     */
    explicit TutorialInformationWidget(Tutorial* tutorial, QWidget* parent = 0);

    /**
     * Destroys this widget.
     */
    virtual ~TutorialInformationWidget();

protected:

    /**
     * Commands to save the name and description of the tutorial.
     *
     * @param parent The parent command.
     * @return A list with the commands.
     */
    virtual QList<QUndoCommand*> createSaveCommands(QUndoCommand* parent);

private:

    /**
     * The tutorial to edit.
     */
    Tutorial* mTutorial;

    /**
     * The Ui Designer generated class.
     */
    Ui::TutorialInformationWidget* ui;

};

#endif
