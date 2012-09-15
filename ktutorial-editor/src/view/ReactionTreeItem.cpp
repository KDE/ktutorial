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

#include "ReactionTreeItem.h"

#include <KLocalizedString>

#include "TextTreeItem.h"
#include "WaitForTreeItem.h"
#include "../data/Reaction.h"

//public:

ReactionTreeItem::ReactionTreeItem(Reaction* reaction, TreeItem* parent):
        TreeItem(parent),
    mReaction(reaction) {
    Q_ASSERT(reaction);

    mEmptyTriggerConditionItem = 0;
    mTriggerConditionItem = 0;
    mTriggerOptionItem = 0;
    mResponseCustomCodeItem = 0;
    mResponseNextStepItem = 0;

    //Add two dummy children, as update method expects always to child items
    appendChild(new TextTreeItem(this));
    appendChild(new TextTreeItem(this));
    update(reaction);

    connect(reaction, SIGNAL(dataChanged(Reaction*)),
            this, SLOT(update(Reaction*)));
}

QString ReactionTreeItem::text() const {
    return i18nc("@item", "Reaction");
}

Reaction* ReactionTreeItem::reaction() const {
    return mReaction;
}

//private:

void ReactionTreeItem::replaceItem(TreeItem* oldItem, TreeItem* newItem) {
    Q_ASSERT(oldItem);
    Q_ASSERT(newItem);
    Q_ASSERT(oldItem->parent());
    Q_ASSERT(oldItem->parent() == newItem->parent());

    int index = oldItem->childIndex();
    TreeItem* parent = oldItem->parent();

    parent->removeChild(oldItem);
    delete oldItem;

    parent->insertChild(newItem, index);
}

void ReactionTreeItem::updateConditionItem(Reaction* reaction) {
    if (!reaction->waitFor() && !mEmptyTriggerConditionItem) {
        mEmptyTriggerConditionItem = new TextTreeItem(this);
        mEmptyTriggerConditionItem->setText(
                                        i18nc("@item", "(No condition set)"));

        replaceItem(child(0), mEmptyTriggerConditionItem);
    } else if (reaction->waitFor()) {
        mTriggerConditionItem =
                WaitForTreeItem::treeItemForWaitFor(reaction->waitFor(), this);

        replaceItem(child(0), mTriggerConditionItem);
    }
}

void ReactionTreeItem::updateOptionItem(Reaction* reaction) {
    if (!mTriggerOptionItem) {
        mTriggerOptionItem = new TextTreeItem(this);

        replaceItem(child(0), mTriggerOptionItem);
    }

    QString optionName = reaction->optionName();
    if (optionName.isEmpty()) {
        optionName = i18nc("@item", "(option name not set)");
    } else {
        optionName = '"' + reaction->optionName() + '"';
    }

    mTriggerOptionItem->setText(
                i18nc("@item", "When the option %1 is selected", optionName));
}

void ReactionTreeItem::updateCustomCodeItem(Reaction* reaction) {
    if (!mResponseCustomCodeItem) {
        TextTreeItem* parent = new TextTreeItem(this);
        parent->setText(i18nc("@item", "Execute the following code:"));

        mResponseCustomCodeItem = new TextTreeItem(parent);
        parent->appendChild(mResponseCustomCodeItem);

        replaceItem(child(1), parent);
    }

    if (reaction->customCode().isEmpty()) {
        mResponseCustomCodeItem->setText(i18nc("@item", "(No code set)"));
    } else {
        mResponseCustomCodeItem->setText(reaction->customCode());
    }
}

void ReactionTreeItem::updateNextStepItem(Reaction* reaction) {
    if (!mResponseNextStepItem) {
        mResponseNextStepItem = new TextTreeItem(this);

        replaceItem(child(1), mResponseNextStepItem);
    }

    QString nextStepId = reaction->nextStepId();
    if (nextStepId.isEmpty()) {
        nextStepId = i18nc("@item", "(step id not set)");
    } else {
        nextStepId = '"' + reaction->nextStepId() + '"';
    }

    mResponseNextStepItem->setText(
                            i18nc("@item", "Change to step %1", nextStepId));
}

//private slots:

void ReactionTreeItem::update(Reaction* reaction) {
    Q_ASSERT(reaction);

    if (reaction->triggerType() == Reaction::ConditionMet) {
        updateConditionItem(reaction);
    }

    if (reaction->triggerType() == Reaction::OptionSelected) {
        updateOptionItem(reaction);
    }

    if (reaction->responseType() == Reaction::CustomCode) {
        updateCustomCodeItem(reaction);
    }

    if (reaction->responseType() == Reaction::NextStep) {
        updateNextStepItem(reaction);
    }
}
