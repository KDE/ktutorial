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

#ifndef TUTORIALTREESELECTIONMANAGER_H
#define TUTORIALTREESELECTIONMANAGER_H

#include <QItemSelectionModel>
#include <QObject>

class Reaction;
class Step;
class TreeItem;

/**
 * Watches the QItemSelectionModel of a TreeModel for changes in the selection.
 * When an item is selected in the TreeModel, it is checked what kind of data it
 * represents. An item can represent several types of data (for example, a
 * reaction item represents a reaction, but also a step as it is part of it).
 *
 * When the data represented by the item is a Step, stepSelected(Step*) signal
 * is emitted. When it is a Reaction, reactionSelected(Reaction*) signal is
 * emitted.
 *
 * Only single item selections are supported.
 */
class TutorialTreeSelectionManager: public QObject {
Q_OBJECT
public:

    /**
     * Creates a new TutorialTreeSelectionManager that watchs the given
     * selection model.
     *
     * @param itemSelectionModel The selection model to watch for changes in the
     *                           selection.
     * @param parent The parent object.
     */
    explicit TutorialTreeSelectionManager(
                QItemSelectionModel* itemSelectionModel, QObject* parent = 0);

Q_SIGNALS:

    /**
     * Emitted when a Step (or any of its child items) is selected.
     * If the Step is deselected and the new selected item isn't a Step, the
     * signal is emitted with a null pointer.
     * No signal is emitted if the selected item changes to another item that
     * selects the same Step already selected.
     *
     * @param step The selected Step, or null if it was deselected.
     */
    void stepSelected(Step* step);

    /**
     * Emitted when a Reaction (or any of its child items) is selected.
     * If the Reaction is deselected and the new selected item isn't a Reaction,
     * the signal is emitted with a null pointer.
     * No signal is emitted if the selected item changes to another item that
     * selects the same Reaction already selected.
     *
     * @param reaction The selected Reaction, or null if it was deselected.
     */
    void reactionSelected(Reaction* reaction);

private:

    /**
     * Emits stepSelected(Step*) signal based on the selected and deselected
     * items.
     *
     * @param selected The selected item, if any.
     * @param deselected The deselected item, if any.
     */
    void updateStepSelection(TreeItem* selected, TreeItem* deselected);

    /**
     * Returns the Step represented by the given item.
     * Any recursive child item of a StepTreeItem, or a StepTreeItem itself,
     * represents a Step.
     *
     * If the item doesn't represent a Step or there is no item, a null pointer
     * is returned.
     *
     * @param item The item to get its represented Step.
     * @return The Step.
     */
    Step* getStepForTreeItem(TreeItem* item);

    /**
     * Emits reactionSelected(Reaction*) signal based on the selected and
     * deselected items.
     *
     * @param selected The selected item, if any.
     * @param deselected The deselected item, if any.
     */
    void updateReactionSelection(TreeItem* selected, TreeItem* deselected);

    /**
     * Returns the Reaction represented by the given item.
     * Any recursive child item of a ReactionTreeItem, or a ReactionTreeItem
     * itself, represents a Reaction.
     *
     * If the item doesn't represent a Reaction or there is no item, a null
     * pointer is returned.
     *
     * @param item The item to get its represented Reaction.
     * @return The Reaction.
     */
    Reaction* getReactionForTreeItem(TreeItem* item);

private Q_SLOTS:

    /**
     * Handles a change in the selection in the watched selection model.
     * Signals are emitted as needed based on the selected and deselected items.
     *
     * @param selected The item selection of selected items.
     * @param selected The item selection of deselected items.
     */
    void handleSelectionChanged(const QItemSelection& selected,
                                const QItemSelection& deselected);

};

#endif
