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

#ifndef STEPDATAWIDGET_H
#define STEPDATAWIDGET_H

#include "CommandWidget.h"

class Step;
class Tutorial;

namespace Ui {
class StepDataWidget;
}

/**
 * Command widget for the id and text of a Step.
 */
class StepDataWidget: public CommandWidget {
Q_OBJECT
public:

    /**
     * Creates a new StepDataWidget for the given Step.
     *
     * @param step The step to edit.
     * @param parent The parent QWidget.
     */
    explicit StepDataWidget(Step* step, QWidget* parent = 0);

    /**
     * Destroys this widget.
     */
    virtual ~StepDataWidget();

    /**
     * Enables the text completion for the id of the step.
     * The proposed ids are the unused ids from all the reactions, except for
     * those that belong to the step of this StepDataWidget.
     * An id is unused if it is not the id of any step added in the tutorial.
     *
     * @param tutorial The tutorial to get the step ids from.
     */
    void enableStepIdCompletion(const Tutorial* tutorial);

protected:

    /**
     * Commands to save the id and text of the step.
     *
     * @param parent The parent command.
     * @return A list with the commands.
     */
    virtual QList<QUndoCommand*> createSaveCommands(QUndoCommand* parent);

private:

    /**
     * The step to edit.
     */
    Step* mStep;

    /**
     * The Ui Designer generated class.
     */
    Ui::StepDataWidget* ui;

};

#endif
