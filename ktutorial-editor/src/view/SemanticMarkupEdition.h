/***************************************************************************
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

#ifndef SEMANTICMARKUPEDITION_H
#define SEMANTICMARKUPEDITION_H

#include <QObject>

class QAction;
class QDialog;
class QTextEdit;

class KActionCollection;

class StartTag;

/**
 * Helper to edit KUIT semantic markup in a QTextEdit widget.
 * SemanticMarkupEdition provides several actions that ease the edition of the
 * most common KUIT semantic markup tags. The actions are added to a
 * KActionCollection using createActions(KActionCollection*).
 *
 * Triggering an action will write its associated start tag in the current
 * position of the text edit cursor. Triggering the action again will write its
 * associated end tag. When a tag needs extra information from the user to be
 * written (for example, the URL of a link element), a dialog will be shown.
 * Also, triggering the action when the cursor is between two paired start and
 * end tags will show the dialog and update the attribute.
 *
 * The action states will be updated based on the cursor of the QTextEdit.
 * Depending on its position or the current selection some actions may be
 * disabled or checked.
 *
 * When the cursor is inside any tag all the actions will be disabled. If it is
 * between two paired start and end tags they will also be disabled (except the
 * ones that show a dialog), although the action for that tag (and its paired
 * parent elements) will be checked. If the cursor is after a start tag, the
 * action for that tag will be enabled and checked (and triggering it will write
 * the end tag).
 *
 * The actions may also be disabled when their element can not be written at the
 * current position of the cursor. For example, the list action will be disabled
 * unless the cursor is after a para tag. It will be disabled too if after the
 * para tag there is a phrase element tag, as list elements can be child only of
 * para elements.
 *
 * Only the cursor position is used to update the state of the actions.
 * Selections are not taken into account.
 */
class SemanticMarkupEdition: public QObject {
Q_OBJECT
public:

    /**
     * Creates a new SemanticMarkupEdition.
     * The QTextEdit will also act as parent of the SemanticMarkupEdition.
     *
     * @param textEdit The QTextEdit used for the edition of the text.
     */
    SemanticMarkupEdition(QTextEdit* textEdit); //krazy:exclude=qclasses

    /**
     * Creates the actions for markup edition and adds them to the given
     * collection.
     * The collection is made parent of the actions.
     *
     * @param actionCollection The collection to add the actions to.
     */
    void createActions(KActionCollection* actionCollection);

private:

    /**
     * All the edition actions.
     */
    QList<QAction*> mActions;

    /**
     * Action for "emphasis" phrase tag.
     */
    QAction* mEmphasisAction;

    /**
     * Action for "emphasis" phrase tag with "strong" attribute.
     */
    QAction* mEmphasisStrongAction;

    /**
     * Action for "filename" phrase tag.
     */
    QAction* mFilenameAction;

    /**
     * Action for "interface" phrase tag.
     */
    QAction* mInterfaceAction;

    /**
     * Action for "link" phrase tag.
     */
    QAction* mLinkAction;

    /**
     * Action for "nl" phrase tag.
     */
    QAction* mNlAction;

    /**
     * Action for "shortcut" phrase tag.
     */
    QAction* mShortcutAction;

    /**
     * Action for "para" structure tag.
     */
    QAction* mParaAction;

    /**
     * Action for "list" structure tag.
     */
    QAction* mListAction;

    /**
     * Action for "item" structure tag.
     */
    QAction* mItemAction;

    /**
     * The QTextEdit used for the edition of the text.
     */
    QTextEdit* mTextEdit; //krazy:exclude=qclasses

    /**
     * Whether the cursor is between two paired start and end link elements or
     * not.
     */
    bool mLinkElementClosed;

    /**
     * Returns the action that represents the given start tag.
     *
     * @param startTag The start tag to get its action.
     * @return The action for the tag.
     */
    QAction* actionForElement(const StartTag& startTag) const;

    /**
     * Writes a simple tag for the given action.
     * If the action is checked, a start tag for the element of the action is
     * inserted in the text edit. If the action is not checked, an end tag is
     * inserted.
     *
     * @param action The action to write its tag.
     */
    void writeSimpleTagFor(const QAction* action);

    /**
     * Creates a new dialog for link elements.
     * The dialog contains a SemanticMarkupLinkWidget and two dialog buttons: Ok
     * and Cancel.
     *
     * @param url The URL to show in the link widget of the dialog.
     * @return The new dialog.
     */
    QDialog* newLinkDialog(const QString& url) const; //krazy:exclude=qclasses

private Q_SLOTS:

    /**
     * Updates the state of the edition actions based on the cursor position.
     */
    void updateActionStates();

    /**
     * Behavior for "emphasis" phrase tag edition.
     */
    void emphasis();

    /**
     * Behavior for "emphasis" phrase tag with "strong" attribute edition.
     */
    void emphasisStrong();

    /**
     * Behavior for "filename" phrase tag edition.
     */
    void filename();

    /**
     * Behavior for "interface" phrase tag edition.
     */
    void interface();

    /**
     * Behavior for "link" phrase tag edition.
     * Unlike other tags, to write the start link" element the user has to
     * provide some information. A dialog is shown to the user to set the URL of
     * the link.
     *
     * Moreover, the URL can be modified even on a closed "link" element. The
     * "link" action can be triggered even on a closed element, and when the
     * action is triggered, the dialog is shown to modify the URL.
     */
    void link();

    /**
     * Behavior for "nl" phrase tag edition.
     */
    void nl();

    /**
     * Behavior for "shortcut" phrase tag edition.
     */
    void shortcut();

    /**
     * Behavior for "para" structure tag edition.
     */
    void para();

    /**
     * Behavior for "list" structure tag edition.
     */
    void list();

    /**
     * Behavior for "item" structure tag edition.
     */
    void item();

};

#endif
