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

#ifndef STEPTREEITEM_H
#define STEPTREEITEM_H

#include "TreeItem.h"

class Reaction;
class ReactionTreeItem;
class Step;
class TextTreeItem;

/**
 * A TreeItem that represents a Step.
 * The tree representation of a step is:
 * Step theId
 *   |-Text: the text
 *   |-Setup:
 *   |   -The custom setup code
 *   --Tear down:
 *   |   -The custom tear down code
 *   |-Reaction
 *   |   ...
 *   |-Reaction
 *   |  ...
 *   ...
 *
 * If the id or the text aren't set yet, a placeholder is used instead. Id
 * placeholder is "(id not set)", and the text placeholder is "(text not set)"
 * (without quotes, but with parenthesis).
 *
 * The other items only appear if they have some data to show. For example, if
 * only the text of the Step is set, its representation is:
 * Step (id not set)
 *   -The text
 *
 * Note that composed elements like custom setup code don't appear at all, not
 * even the parent item with just "Setup:".
 *
 * Whenever the step data changes, the StepTreeItem and its child items
 * are updated as needed.
 *
 * Also note that the order of the child elements is always the same. Even if,
 * for example, the tear down code is set first and then the setup code, the
 * setup code item will appear first and then the tear down code item.
 *
 * @see ReactionTreeItem
 */
class StepTreeItem: public TreeItem {
Q_OBJECT
public:

    /**
     * Creates a new StepTreeItem for the given Step and with the given
     * parent.
     *
     * @param step The step to represent.
     * @param parent The parent TreeItem.
     */
    explicit StepTreeItem(Step* step, TreeItem* parent = 0);

    /**
     * Returns "Step " and the id of the step, or "Step (id not set)" if there
     * is no id.
     *
     * @return The text for this TreeItem.
     */
    virtual QString text() const;

    /**
     * Returns the Step.
     *
     * @return The Step.
     */
    Step* step() const;

private:

    /**
     * The Step.
     */
    Step* mStep;

    /**
     * The id of the step.
     */
    QString mStepId;

    /**
     * The child item containing the text.
     */
    TextTreeItem* mTextItem;

    /**
     * The child item containing the custom setup code.
     * This is a nested item. Its parent (and the direct child of this
     * StepTreeItem) contains the "Setup:" text.
     */
    TextTreeItem* mSetupItem;

    /**
     * The child item containing the custom tear down code.
     * This is a nested item. Its parent (and the direct child of this
     * StepTreeItem) contains the "Tear down:" text.
     */
    TextTreeItem* mTearDownItem;

    /**
     * The ReactionTreeItems for each Reaction in the Step.
     */
    QList<ReactionTreeItem*> mReactionTreeItems;

    /**
     * The base index to add new ReactionTreeItems.
     * It is the next index to the last optional data item (setup or tear down
     * code).
     */
    int mReactionTreeItemBaseIndex;

    /**
     * Returns the ReactionTreeItem for the given Reaction.
     *
     * @param reaction The Reaction to get its ReactionTreeItem.
     * @return The ReactionTreeItem.
     */
    ReactionTreeItem* reactionTreeItemForReaction(Reaction* reaction) const;

private Q_SLOTS:

    /**
     * Updates this StepTreeItem when the data of its step changed.
     * If a child item is needed to show some data, it is inserted or updated
     * (depending on whether it existed previously or not).
     * If the child item is no longer needed, it is removed.
     *
     * Items may be flat or nested, depending on the data to show.
     *
     * @param step The step.
     */
    void update(Step* step);

    /**
     * Adds a new ReactionTreeItem when a Reaction is added in the step.
     *
     * @param step The Reaction added in the Step.
     * @param index The index where the Reaction was added in the Step.
     */
    void addReaction(Reaction* reaction, int index);

    /**
     * Removes the ReactionTreeItem for the Reaction removed in the step.
     *
     * @param reaction The Reaction removed in the Step.
     */
    void removeReaction(Reaction* reaction);

};

#endif
