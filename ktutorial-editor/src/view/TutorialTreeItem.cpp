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

#include "TutorialTreeItem.h"

#include <KLocalizedString>

#include "StepTreeItem.h"
#include "TextTreeItem.h"
#include "TreeItemUtil.h"
#include "../data/Tutorial.h"

//public:

TutorialTreeItem::TutorialTreeItem(Tutorial* tutorial, TreeItem* parent):
        TreeItem(parent) {
    Q_ASSERT(tutorial);

    mNameItem = new TextTreeItem(this);
    appendChild(mNameItem);
    mDescriptionItem = new TextTreeItem(this);
    appendChild(mDescriptionItem);
    mLicenseItem = 0;
    mSetupItem = 0;
    mTearDownItem = 0;

    update(tutorial);
    connect(tutorial, SIGNAL(dataChanged(Tutorial*)),
            this, SLOT(update(Tutorial*)));

    foreach(Step* step, tutorial->steps()) {
        addStep(step, mStepTreeItems.count());
    }
    connect(tutorial, SIGNAL(stepAdded(Step*,int)),
            this, SLOT(addStep(Step*,int)));
    connect(tutorial, SIGNAL(stepRemoved(Step*)),
            this, SLOT(removeStep(Step*)));
}

QString TutorialTreeItem::text() const {
    if (mTutorialId.isEmpty()) {
        return i18nc("@item", "Tutorial");
    }

    return i18nc("@item", "Tutorial %1", mTutorialId);
}

//private:

StepTreeItem* TutorialTreeItem::stepTreeItemForStep(Step* step) {
    foreach (StepTreeItem* stepTreeItem, mStepTreeItems) {
        if (stepTreeItem->step() == step) {
            return stepTreeItem;
        }
    }

    return 0;
}

//private slots:

void TutorialTreeItem::update(Tutorial* tutorial) {
    QString name;
    if (tutorial->name().isEmpty()) {
        name = i18nc("@item:intext", "(name not set)");

        if (!mTutorialId.isEmpty()) {
            mTutorialId.clear();
            emit dataChanged(this);
        }
    } else {
        name = tutorial->name();

        mTutorialId = tutorial->id();
        emit dataChanged(this);
    }
    mNameItem->setText(i18nc("@item Noun, the name of a tutorial",
                             "Name: %1", name));

    QString description;
    if (tutorial->description().isEmpty()) {
        description = i18nc("@item:intext", "(description not set)");
    } else {
        description = tutorial->description();
    }
    mDescriptionItem->setText(i18nc("@item", "Description: %1", description));

    int childIndex = 2;

    if (tutorial->licenseText().isEmpty()) {
        TreeItemUtil::removeNestedItemIfNeeded(mLicenseItem);
    } else {
        TreeItemUtil::addNestedItemIfNeeded(this, mLicenseItem, childIndex,
                                            i18nc("@item", "License:"));
        mLicenseItem->setText(tutorial->licenseText());

        childIndex++;
    }

    if (tutorial->customSetupCode().isEmpty()) {
        TreeItemUtil::removeNestedItemIfNeeded(mSetupItem);
    } else {
        TreeItemUtil::addNestedItemIfNeeded(this, mSetupItem, childIndex,
                                            i18nc("@item", "Setup:"));
        mSetupItem->setText(tutorial->customSetupCode());

        childIndex++;
    }

    if (tutorial->customTearDownCode().isEmpty()) {
        TreeItemUtil::removeNestedItemIfNeeded(mTearDownItem);
    } else {
        TreeItemUtil::addNestedItemIfNeeded(this, mTearDownItem, childIndex,
                                            i18nc("@item", "Tear down:"));
        mTearDownItem->setText(tutorial->customTearDownCode());

        childIndex++;
    }

    mStepTreeItemBaseIndex = childIndex;
}

void TutorialTreeItem::addStep(Step* step, int index) {
    StepTreeItem* stepTreeItem = new StepTreeItem(step, this);
    insertChild(stepTreeItem, mStepTreeItemBaseIndex + index);
    mStepTreeItems.insert(index, stepTreeItem);
}

void TutorialTreeItem::removeStep(Step* step) {
    StepTreeItem* stepTreeItem = stepTreeItemForStep(step);

    removeChild(stepTreeItem);
    mStepTreeItems.removeOne(stepTreeItem);
    delete stepTreeItem;
}
