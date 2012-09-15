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

#ifndef WAITFORWIDGET_H
#define WAITFORWIDGET_H

#include <QWidget>

class QItemSelection;
class TreeItem;
class WaitFor;

namespace Ui {
class WaitForWidget;
}

/**
 * Widget to create or edit a WaitFor.
 * The following operations can be done: add a WaitFor, edit a WaitFor or remove
 * a WaitFor.
 *
 * A WaitFor can be added only as the first WaitFor (when the list is empty), or
 * as a child of a composed WaitFor (selecting the one that will be its parent).
 *
 * A WaitFor can be edited if it is a plain condition, that is, it is not
 * composed from other WaitFors, and it is not a WaitForStepActivation.
 *
 * Finally, any WaitFor can be removed.
 *
 * The WaitFor, returned by waitFor() method, must be explicitly deleted.
 * However, the WaitFor set in the constructor may have been deleted by the
 * widget if the user removed the root WaitFor.
 */
class WaitForWidget: public QWidget {
Q_OBJECT
public:

    /**
     * Creates a new WaitForWidget.
     *
     * @param waitFor The initial WaitFor.
     * @param parent The parent QWidget.
     */
    explicit WaitForWidget(WaitFor* waitFor, QWidget* parent = 0);

    /**
     * Destroys this widget.
     */
    virtual ~WaitForWidget();

    /**
     * Returns the WaitFor.
     *
     * @return The WaitFor, if any.
     */
    WaitFor* waitFor() const;

private:

    /**
     * The WaitFor to edit.
     */
    WaitFor* mWaitFor;

    /**
     * The WaitFor currently selected.
     */
    WaitFor* mCurrentWaitFor;

    /**
     * The root tree item in the tree view.
     * It provides the header in the tree view, and acts as parent item for the
     * tree item that represents mWaitFor.
     */
    TreeItem* mRootItem;

    /**
     * The Ui Designer generated class.
     */
    Ui::WaitForWidget* ui;

    /**
     * Sets up the tree view for the WaitFor to be edited.
     *
     * @param waitFor The WaitFor to be edited.
     */
    void setupTreeView(WaitFor* waitFor);

    /**
     * Sets the current WaitFor and enables or disables the buttons as needed.
     *
     * @param selectedWaitFor The selected WaitFor.
     */
    void updateWaitForSelection(WaitFor* selectedWaitFor);

private Q_SLOTS:

    /**
     * Handles a change in the selection in the tree view.
     *
     * @param selected The item selection of selected items.
     * @param selected The item selection of deselected items.
     */
    void handleSelectionChanged(const QItemSelection& selected,
                                const QItemSelection& deselected);

    /**
     * Shows a dialog with a NewWaitForWidget and adds the new WaitFor.
     * The WaitFor is added to the selected WaitFor, or as the root WaitFor if
     * there is none (a new WaitForTreeItem is also added in that case).
     *
     * When a WaitFor is added the selection doesn't change, so some buttons are
     * updated as needed.
     */
    void addWaitFor();

    /**
     * Shows an EditionDialog for the selected WaitFor.
     */
    void editWaitFor();

    /**
     * Removes the selected WaitFor.
     * If the root WaitFor is removed, its tree item is also removed from the
     * root item.
     */
    void removeWaitFor();

};

#endif
