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

#ifndef REACTIONWIDGET_H
#define REACTIONWIDGET_H

#include <QPointer>

#include "CommandWidget.h"
#include "../data/Reaction.h"

class Step;
class Tutorial;
class WaitFor;
class WaitForWidget;

namespace Ui {
class ReactionWidget;
}

/**
 * Command widget for a Reaction.
 */
class ReactionWidget: public CommandWidget {
Q_OBJECT
public:

    /**
     * Creates a new ReactionWidget for the given Reaction.
     *
     * @param reaction The reaction to edit.
     * @param parent The parent QWidget.
     */
    explicit ReactionWidget(Reaction* reaction, QWidget* parent = 0);

    /**
     * Destroys this widget.
     */
    virtual ~ReactionWidget();

    /**
     * Enables the text completion for the id of the next step.
     * The proposed ids are the ids of every step, except for the one that the
     * reaction belongs to or is going to be added to.
     *
     * @param tutorial The tutorial to get the step ids from.
     * @param ownStep The step that the reaction is going to be added to.
     */
    void enableStepIdCompletion(const Tutorial* tutorial,
                                const Step* ownStep = 0);

protected:

    /**
     * Commands to save the data of the reaction.
     *
     * @param parent The parent command.
     * @return A list with the commands.
     */
    virtual QList<QUndoCommand*> createSaveCommands(QUndoCommand* parent);

private:

    /**
     * The reaction to edit.
     */
    Reaction* mReaction;

    /**
     * The cloned WaitFor to be edited in the WaitForWidget.
     * A cloned WaitFor instead of the one from the Reaction is used because
     * working on it would modify it even if saveChanges() wasn't called.
     * The pointer is automatically set to 0 if the object is deleted anywhere
     * (that is, not by deleting this pointer, but another one pointing to the
     * object).
     */
    QPointer<WaitFor> mWaitForClone;

    /**
     * The widget to edit the WaitFor.
     */
    WaitForWidget* mWaitForWidget;

    /**
     * The currently selected trigger type.
     */
    Reaction::TriggerType mCurrentTriggerType;

    /**
     * The currently selected response type.
     */
    Reaction::ResponseType mCurrentResponseType;

    /**
     * The Ui Designer generated class.
     */
    Ui::ReactionWidget* ui;

private Q_SLOTS:

    /**
     * When the condition radio button is selected, it enables and disables the
     * appropriate widgets, and updates the current trigger type.
     *
     * @param checked Whether the radio button was checked or unchecked.
     */
    void selectTriggerCondition(bool checked);

    /**
     * When the option radio button is selected, it enables and disables the
     * appropriate widgets, and updates the current trigger type.
     *
     * @param checked Whether the radio button was checked or unchecked.
     */
    void selectTriggerOption(bool checked);

    /**
     * When the custom code radio button is selected, it enables and disables
     * the appropriate widgets, and updates the current response type.
     *
     * @param checked Whether the radio button was checked or unchecked.
     */
    void selectResponseCode(bool checked);

    /**
     * When the next step radio button is selected, it enables and disables the
     * appropriate widgets, and updates the current response type.
     *
     * @param checked Whether the radio button was checked or unchecked.
     */
    void selectResponseStep(bool checked);

};

#endif
