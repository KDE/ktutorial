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

#ifndef REACTIONTREEITEM_H
#define REACTIONTREEITEM_H

#include <QPointer>
#include "TreeItem.h"

class Reaction;
class TextTreeItem;
class WaitForTreeItem;

/**
 * A TreeItem that represents a Reaction.
 * The tree representation of a reaction is:
 * Reaction
 *   |-Trigger
 *   --Response
 *
 * Trigger can be either:
 * -When the option "optionName" is selected
 * or the representation of a WaitFor, depending on the type of trigger. If the
 * trigger is an option and there is no option name set, or if the trigger is a
 * condition but there is no WaitFor, a placeholder is used instead. Option
 * placeholder is "(option name not set)", and condition placeholder is "(No
 * condition set)" (without quotes, but with parenthesis).
 *
 * Response can be either:
 * -Change to step "stepId"
 * or:
 * -Execute the following code:
 *   -Custom code
 * depending on the type of response. If there is no stepId or code set, a
 * placeholder is used instead. Step id placeholder is "(step id not set)", and
 * code placeholder is "(No code set)" (without quotes, but with parenthesis).
 *
 * Whenever the reaction data changes, the ReactionTreeItem and its child items
 * are updated as needed.
 */
class ReactionTreeItem: public TreeItem {
Q_OBJECT
public:

    /**
     * Creates a new ReactionTreeItem for the given Reaction and with the given
     * parent.
     *
     * @param reaction The reaction to represent.
     * @param parent The parent TreeItem.
     */
    explicit ReactionTreeItem(Reaction* reaction, TreeItem* parent = 0);

    /**
     * Returns "Reaction".
     *
     * @return The text for this TreeItem.
     */
    virtual QString text() const;

    /**
     * Returns the Reaction.
     *
     * @return The Reaction.
     */
    Reaction* reaction() const;

private:

    /**
     * The Reaction.
     */
    Reaction* mReaction;

    /**
     * The child item containing the warning text when there is no condition
     * but a condition type is used.
     * The pointer is automatically set to 0 if the object is deleted anywhere
     * (that is, not by deleting this pointer, but another one pointing to the
     * object).
     */
    QPointer<TextTreeItem> mEmptyTriggerConditionItem;

    /**
     * The child item containing the WaitForTreeItem for the condition, when a
     * condition type is used.
     * The pointer is automatically set to 0 if the object is deleted anywhere
     * (that is, not by deleting this pointer, but another one pointing to the
     * object).
     */
    QPointer<WaitForTreeItem> mTriggerConditionItem;

    /**
     * The child item containing the trigger text, when an option type is used.
     * The pointer is automatically set to 0 if the object is deleted anywhere
     * (that is, not by deleting this pointer, but another one pointing to the
     * object).
     */
    QPointer<TextTreeItem> mTriggerOptionItem;

    /**
     * The child item containing the response text, when the custom code type is
     * used.
     * This is a nested item. Its parent (and the direct child of this
     * ReactionTreeItem) contains the "Execute the following code:" text.
     * The pointer is automatically set to 0 if the object is deleted anywhere
     * (that is, not by deleting this pointer, but another one pointing to the
     * object).
     */
    QPointer<TextTreeItem> mResponseCustomCodeItem;

    /**
     * The child item containing the response text, when the next step type is
     * used.
     * The pointer is automatically set to 0 if the object is deleted anywhere
     * (that is, not by deleting this pointer, but another one pointing to the
     * object).
     */
    QPointer<TextTreeItem> mResponseNextStepItem;

    /**
     * Replaces the old item with the new item.
     * The old item is destroyed.
     *
     * @param oldItem The item to remove.
     * @param newItem The item to insert.
     */
    void replaceItem(TreeItem* oldItem, TreeItem* newItem);

    /**
     * Sets a new WaitForTreeItem for the WaitFor of the reaction, or a
     * TextTreeItem if there is no WaitFor.
     *
     * @param reaction The reaction to get the WaitFor from.
     */
    void updateConditionItem(Reaction* reaction);

    /**
     * Sets a new TextTreeItem or modifies the current one with the option name.
     *
     * @param reaction The reaction to get the option name from.
     */
    void updateOptionItem(Reaction* reaction);

    /**
     * Sets a new nested TextTreeItem or modifies the current one with the
     * custom code.
     *
     * @param reaction The reaction to get the custom code from.
     */
    void updateCustomCodeItem(Reaction* reaction);

    /**
     * Sets a new TextTreeItem or modifies the current one with the step id.
     *
     * @param reaction The reaction to get the step id from.
     */
    void updateNextStepItem(Reaction* reaction);

private Q_SLOTS:

    /**
     * Updates this ReactionTreeItem when the data of its reaction changed.
     * If a child item is needed to show some data, it is inserted or updated
     * (depending on whether it existed previously or not).
     * If the child item is no longer needed, it is removed.
     *
     * Items may be flat or nested, depending on the data to show.
     *
     * @param reaction The reaction.
     */
    void update(Reaction* reaction);

};

#endif
