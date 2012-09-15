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

#ifndef STEPCUSTOMCODEWIDGET_H
#define STEPCUSTOMCODEWIDGET_H

#include "CommandWidget.h"

class Step;

namespace Ui {
class CustomCodeWidget;
}

/**
 * Command widget for the setup and tear down code of a Step.
 * The code that is edited (setup or tear down) is specified when the widget
 * is created.
 */
class StepCustomCodeWidget: public CommandWidget {
Q_OBJECT
public:

    enum Type {
        Setup,
        TearDown
    };

    /**
     * Creates a new StepCustomCodeWidget for the given type of code of the
     * given step.
     *
     * @param step The step to edit its code.
     * @param type The type of code to edit.
     * @param parent The parent QWidget.
     */
    StepCustomCodeWidget(Step* step, Type type, QWidget* parent = 0);

    /**
     * Destroys this widget.
     */
    virtual ~StepCustomCodeWidget();

protected:

    /**
     * Commands to save the code of the step.
     *
     * @param parent The parent command.
     * @return A list with the commands.
     */
    virtual QList<QUndoCommand*> createSaveCommands(QUndoCommand* parent);

private:

    /**
     * The step to set its code.
     */
    Step* mStep;

    /**
     * The type of custom code to edit.
     */
    Type mType;

    /**
     * The Ui Designer generated class.
     */
    Ui::CustomCodeWidget* ui;

};

#endif
