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

#ifndef TUTORIALTREEITEM_H
#define TUTORIALTREEITEM_H

#include "TreeItem.h"

class Step;
class StepTreeItem;
class TextTreeItem;
class Tutorial;

/**
 * A TreeItem that represents a Tutorial.
 * The tree representation of a tutorial is:
 * Tutorial theId
 *   |-Name: the name
 *   |-Description: the description
 *   |-License:
 *   |   -The license text
 *   |-Setup:
 *   |   -The custom setup code
 *   |-Tear down:
 *   |   -The custom tear down code
 *   |-Step first step
 *   |   ...
 *   |-Step second step
 *   |   ...
 *   ...
 *
 * If the name or the description aren't set yet, a placeholder is used instead.
 * Name placeholder is "(name not set)", and the description placeholder is
 * "(description not set)" (without quotes, but with parenthesis).
 *
 * The other items only appear if they have some data to show. For example, if
 * only the name of the Tutorial is set, its representation is:
 * Tutorial theName
 *   -Name: the name
 *   -Description: (description not set)
 *
 * Note that composed elements like license don't appear at all, not even the
 * parent item with just "License:".
 *
 * Whenever the tutorial data changes, the TutorialTreeItem and its child items
 * are updated as needed.
 *
 * Also note that the order of the child elements is always the same. Even if,
 * for example, the tear down code is set first and then the license, the
 * license item will appear first and then the tear down code item.
 *
 * @see StepTreeItem
 */
class TutorialTreeItem: public TreeItem {
Q_OBJECT
public:

    /**
     * Creates a new TutorialTreeItem for the given Tutorial and with the given
     * parent.
     *
     * @param tutorial The tutorial to represent.
     * @param parent The parent TreeItem.
     */
    explicit TutorialTreeItem(Tutorial* tutorial, TreeItem* parent = 0);

    /**
     * Returns "Tutorial " and the id of the tutorial, or just "Tutorial" if
     * there is no id.
     *
     * @return The text for this TreeItem.
     */
    virtual QString text() const;

private:

    /**
     * The id of the tutorial.
     */
    QString mTutorialId;

    /**
     * The child item containing the name.
     */
    TextTreeItem* mNameItem;

    /**
     * The child item containing the description.
     */
    TextTreeItem* mDescriptionItem;

    /**
     * The child item containing the license.
     * This is a nested item. Its parent (and the direct child of this
     * TutorialTreeItem) contains the "License:" text.
     */
    TextTreeItem* mLicenseItem;

    /**
     * The child item containing the custom setup code.
     * This is a nested item. Its parent (and the direct child of this
     * TutorialTreeItem) contains the "Setup:" text.
     */
    TextTreeItem* mSetupItem;

    /**
     * The child item containing the custom tear down code.
     * This is a nested item. Its parent (and the direct child of this
     * TutorialTreeItem) contains the "Tear down:" text.
     */
    TextTreeItem* mTearDownItem;

    /**
     * The StepTreeItems for each Step in the Tutorial.
     */
    QList<StepTreeItem*> mStepTreeItems;

    /**
     * The base index to add new StepTreeItems.
     * It is the next index to the last optional data item (license, setup or
     * tear down code).
     */
    int mStepTreeItemBaseIndex;

    /**
     * Returns the StepTreeItem for the given Step.
     *
     * @param step The Step to get its StepTreeItem.
     * @return The StepTreeItem.
     */
    StepTreeItem* stepTreeItemForStep(Step* step);

private Q_SLOTS:

    /**
     * Updates this TutorialTreeItem when the data of its tutorial changed.
     * If a child item is needed to show some data, it is inserted or updated
     * (depending on whether it existed previously or not).
     * If the child item is no longer needed, it is removed.
     *
     * Items may be flat or nested, depending on the data to show.
     *
     * @param tutorial The tutorial.
     */
    void update(Tutorial* tutorial);

    /**
     * Adds a new StepTreeItem when a Step is added in the tutorial.
     *
     * @param step The Step added in the Tutorial.
     * @param index The index where the Step was added in the Tutorial.
     */
    void addStep(Step* step, int index);

    /**
     * Removes the StepTreeItem for the Step removed in the tutorial.
     *
     * @param step The Step removed in the Tutorial.
     */
    void removeStep(Step* step);

};

#endif
