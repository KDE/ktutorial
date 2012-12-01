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

#include "TutorialTreeSelectionManager.h"

#include <QItemSelectionModel>

#include "ReactionTreeItem.h"
#include "StepTreeItem.h"
#include "TreeItem.h"

//public:

TutorialTreeSelectionManager::TutorialTreeSelectionManager(
                                    QItemSelectionModel* itemSelectionModel,
                                    QObject* parent): QObject(parent) {
    connect(itemSelectionModel,
            SIGNAL(selectionChanged(QItemSelection,QItemSelection)),
            this,
            SLOT(handleSelectionChanged(QItemSelection,QItemSelection)));
}

//private:

void TutorialTreeSelectionManager::updateStepSelection(TreeItem* selected,
                                                       TreeItem* deselected) {
    Step* selectedStep = getStepForTreeItem(selected);
    Step* deselectedStep = getStepForTreeItem(deselected);

    if (selectedStep && selectedStep != deselectedStep) {
        emit stepSelected(selectedStep);
        return;
    }

    if (!selectedStep && deselectedStep) {
        emit stepSelected(0);
        return;
    }
}

Step* TutorialTreeSelectionManager::getStepForTreeItem(TreeItem* item) {
    if (qobject_cast<StepTreeItem*>(item)) {
        return static_cast<StepTreeItem*>(item)->step();
    }

    if (item == 0 || item->parent() == 0) {
        return 0;
    }

    return getStepForTreeItem(item->parent());
}

void TutorialTreeSelectionManager::updateReactionSelection(TreeItem* selected,
                                                        TreeItem* deselected) {
    Reaction* selectedReaction = getReactionForTreeItem(selected);
    Reaction* deselectedReaction = getReactionForTreeItem(deselected);

    if (selectedReaction && selectedReaction != deselectedReaction) {
        emit reactionSelected(selectedReaction);
        return;
    }

    if (!selectedReaction && deselectedReaction) {
        emit reactionSelected(0);
        return;
    }
}

Reaction* TutorialTreeSelectionManager::getReactionForTreeItem(TreeItem* item) {
    if (qobject_cast<ReactionTreeItem*>(item)) {
        return static_cast<ReactionTreeItem*>(item)->reaction();
    }

    if (item == 0 || item->parent() == 0) {
        return 0;
    }

    return getReactionForTreeItem(item->parent());
}

//private slots:

void TutorialTreeSelectionManager::handleSelectionChanged(
                                            const QItemSelection& selected,
                                            const QItemSelection& deselected) {
    //Only single selections are supported
    Q_ASSERT(selected.count() <= 1);
    Q_ASSERT(deselected.count() <= 1);

    TreeItem* selectedItem = 0;
    TreeItem* deselectedItem = 0;

    if (selected.count() == 1) {
        Q_ASSERT(selected.at(0).indexes().count() == 1);

        QModelIndex index = selected.at(0).indexes().at(0);
        selectedItem = static_cast<TreeItem*>(index.internalPointer());
    }

    if (deselected.count() == 1) {
        Q_ASSERT(deselected.at(0).indexes().count() == 1);

        QModelIndex index = deselected.at(0).indexes().at(0);
        deselectedItem = static_cast<TreeItem*>(index.internalPointer());
    }

    updateStepSelection(selectedItem, deselectedItem);
    updateReactionSelection(selectedItem, deselectedItem);
}
