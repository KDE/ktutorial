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

#include "StepTreeItem.h"

#include <KLocalizedString>

#include "ReactionTreeItem.h"
#include "TextTreeItem.h"
#include "TreeItemUtil.h"
#include "../data/Step.h"

//public:

StepTreeItem::StepTreeItem(Step* step, TreeItem* parent):
        TreeItem(parent),
    mStep(step) {
    Q_ASSERT(step);

    mTextItem = new TextTreeItem(this);
    appendChild(mTextItem);
    mSetupItem = 0;
    mTearDownItem = 0;

    update(step);
    connect(step, SIGNAL(dataChanged(Step*)),
            this, SLOT(update(Step*)));

    foreach(Reaction* reaction, step->reactions()) {
        addReaction(reaction, mReactionTreeItems.count());
    }
    connect(step, SIGNAL(reactionAdded(Reaction*, int)),
            this, SLOT(addReaction(Reaction*, int)));
    connect(step, SIGNAL(reactionRemoved(Reaction*)),
            this, SLOT(removeReaction(Reaction*)));
}

QString StepTreeItem::text() const {
    QString id = mStepId;
    if (mStepId.isEmpty()) {
        id = i18nc("@item:intext", "(id not set)");
    }

    return i18nc("@item Noun, a step in a tutorial", "Step %1", id);
}

Step* StepTreeItem::step() const {
    return mStep;
}

//private:

ReactionTreeItem* StepTreeItem::reactionTreeItemForReaction(
                                                    Reaction* reaction) const {
    foreach (ReactionTreeItem* reactionTreeItem, mReactionTreeItems) {
        if (reactionTreeItem->reaction() == reaction) {
            return reactionTreeItem;
        }
    }

    return 0;
}

//private slots:

void StepTreeItem::update(Step* step) {
    if (step->id().isEmpty()) {
        if (!mStepId.isEmpty()) {
            mStepId.clear();
            emit dataChanged(this);
        }
    } else {
        mStepId = step->id();
        emit dataChanged(this);
    }

    QString text;
    if (step->text().isEmpty()) {
        text = i18nc("@item:intext", "(text not set)");
    } else {
        text = step->text();
    }
    mTextItem->setText(i18nc("@item", "Text: %1", text));

    int childIndex = 1;

    if (step->customSetupCode().isEmpty()) {
        TreeItemUtil::removeNestedItemIfNeeded(mSetupItem);
    } else {
        TreeItemUtil::addNestedItemIfNeeded(this, mSetupItem, childIndex,
                                            i18nc("@item", "Setup:"));
        mSetupItem->setText(step->customSetupCode());

        childIndex++;
    }

    if (step->customTearDownCode().isEmpty()) {
        TreeItemUtil::removeNestedItemIfNeeded(mTearDownItem);
    } else {
        TreeItemUtil::addNestedItemIfNeeded(this, mTearDownItem, childIndex,
                                            i18nc("@item", "Tear down:"));
        mTearDownItem->setText(step->customTearDownCode());

        childIndex++;
    }

    mReactionTreeItemBaseIndex = childIndex;
}

void StepTreeItem::addReaction(Reaction* reaction, int index) {
    ReactionTreeItem* reactionTreeItem = new ReactionTreeItem(reaction, this);
    insertChild(reactionTreeItem, mReactionTreeItemBaseIndex + index);
    mReactionTreeItems.insert(index, reactionTreeItem);
}

void StepTreeItem::removeReaction(Reaction* reaction) {
    ReactionTreeItem* reactionTreeItem = reactionTreeItemForReaction(reaction);

    removeChild(reactionTreeItem);
    mReactionTreeItems.removeOne(reactionTreeItem);
    delete reactionTreeItem;
}
