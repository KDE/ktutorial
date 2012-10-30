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

#include "SemanticMarkupEdition.h"

#include <QRegExp>
#include <QTextEdit>

#include <KAction>
#include <KActionCollection>
#include <KDialog>
#include <KLocalizedString>
#include <KPushButton>

#include "DialogRunner.h"
#include "SemanticMarkupLinkWidget.h"
#include "SemanticMarkupParser.h"

//public:

SemanticMarkupEdition::SemanticMarkupEdition(
                                QTextEdit* textEdit): //krazy:exclude=qclasses
        QObject(textEdit),
    mTextEdit(textEdit) {
    Q_ASSERT(textEdit);

    connect(mTextEdit, SIGNAL(cursorPositionChanged()),
            this, SLOT(updateActionStates()));
    //TODO Needed to update the action states when the user deletes a character;
    //look how to do that without updating twice in any other text change.
    connect(mTextEdit, SIGNAL(textChanged()),
            this, SLOT(updateActionStates()));
}

void SemanticMarkupEdition::createActions(KActionCollection* actionCollection) {
    Q_ASSERT(actionCollection);

    mEmphasisAction = new KAction("emphasis", actionCollection);
    mEmphasisAction->setCheckable(true);
    mEmphasisAction->setToolTip(i18nc("@info:tooltip", "Emphasized text"));
    mEmphasisAction->setWhatsThis(i18nc("@info:whatsthis",
        "Phrase tag to emphasize a word or phrase in the text.<nl/>"
        "Example: <emphasis>emphasized text</emphasis>"));
    actionCollection->addAction("kuit-edition-phrase-emphasis",
                                mEmphasisAction);
    connect(mEmphasisAction, SIGNAL(triggered()), this, SLOT(emphasis()));
    mActions.append(mEmphasisAction);

    mEmphasisStrongAction = new KAction("emphasis (strong)", actionCollection);
    mEmphasisStrongAction->setCheckable(true);
    mEmphasisStrongAction->setToolTip(i18nc("@info:tooltip",
                                            "Strongly emphasized text"));
    mEmphasisStrongAction->setWhatsThis(i18nc("@info:whatsthis",
        "Phrase tag to strongly emphasize a word or phrase in the text.<nl/>"
        "Example: <emphasis strong=\"1\">strongly emphasized text</emphasis>"));
    actionCollection->addAction("kuit-edition-phrase-emphasis-strong",
                                mEmphasisStrongAction);
    connect(mEmphasisStrongAction, SIGNAL(triggered()),
            this, SLOT(emphasisStrong()));
    mActions.append(mEmphasisStrongAction);

    mFilenameAction = new KAction("filename", actionCollection);
    mFilenameAction->setCheckable(true);
    mFilenameAction->setToolTip(i18nc("@info:tooltip", "Filename or path"));
    mFilenameAction->setWhatsThis(i18nc("@info:whatsthis",
        "Phrase tag for file or folder name or path.<nl/>"
        "The path separators will be transformed into what is native to the "
        "platform.<nl/>"
        "Example: <filename>/home/user/Music/song.ogg</filename>"));
    actionCollection->addAction("kuit-edition-phrase-filename",
                                mFilenameAction);
    connect(mFilenameAction, SIGNAL(triggered()), this, SLOT(filename()));
    mActions.append(mFilenameAction);

    mInterfaceAction = new KAction("interface", actionCollection);
    mInterfaceAction->setCheckable(true);
    mInterfaceAction->setToolTip(i18nc("@info:tooltip",
                                       "GUI interface element"));
    mInterfaceAction->setWhatsThis(i18nc("@info:whatsthis",
        "Phrase tag for paths to GUI interface elements.<nl/>"
        "If there is more than one element in the path, use \"|\" to delimit "
        "elements, which will be converted into canonical delimiter.<nl/>"
        "Example: <interface>File|Open</interface>"));
    actionCollection->addAction("kuit-edition-phrase-interface",
                                mInterfaceAction);
    connect(mInterfaceAction, SIGNAL(triggered()), this, SLOT(interface()));
    mActions.append(mInterfaceAction);

    mLinkAction = new KAction("link", actionCollection);
    mLinkAction->setCheckable(true);
    mLinkAction->setToolTip(i18nc("@info:tooltip", "Link to URL or widget"));
    mLinkAction->setWhatsThis(i18nc("@info:whatsthis",
        "Phrase tag to link to a URL-addressable resource.<nl/>"
        "Widgets in the target application interface can be linked using "
        "<emphasis>widget:theObjectNameOfTheWidget</emphasis><nl/>"
        "Example: <link url=\"http://www.kde.org\">a link</link>"));
    actionCollection->addAction("kuit-edition-phrase-link", mLinkAction);
    connect(mLinkAction, SIGNAL(triggered()), this, SLOT(link()));
    mActions.append(mLinkAction);

    mNlAction = new KAction("nl", actionCollection);
    mNlAction->setToolTip(i18nc("@info:tooltip", "Line break"));
    mNlAction->setWhatsThis(i18nc("@info:whatsthis",
        "Phrase tag for line breaks.<nl/>"
        "Example: line<nl/>break"));
    actionCollection->addAction("kuit-edition-phrase-nl", mNlAction);
    connect(mNlAction, SIGNAL(triggered()), this, SLOT(nl()));
    mActions.append(mNlAction);

    mShortcutAction = new KAction("shortcut", actionCollection);
    mShortcutAction->setCheckable(true);
    mShortcutAction->setToolTip(i18nc("@info:tooltip",
                                      "Combination of keys to press"));
    mShortcutAction->setWhatsThis(i18nc("@info:whatsthis",
        "Phrase tag for combinations of keys to press.<nl/>"
        "Separate the keys by \"+\" or \"-\", and the shortcut will be "
        "converted into canonical form.<nl/>"
        "Example: <shortcut>Ctrl+N</shortcut>"));
    actionCollection->addAction("kuit-edition-phrase-shortcut",
                                mShortcutAction);
    connect(mShortcutAction, SIGNAL(triggered()), this, SLOT(shortcut()));
    mActions.append(mShortcutAction);

    mParaAction = new KAction("para", actionCollection);
    mParaAction->setCheckable(true);
    mParaAction->setToolTip(i18nc("@info:tooltip", "Paragraph"));
    mParaAction->setWhatsThis(i18nc("@info:whatsthis",
        "<para>Structure tag for text paragraphs.<nl/>"
        "Example: one paragraph</para><para>Other paragraph</para>"));
    actionCollection->addAction("kuit-edition-structure-para", mParaAction);
    connect(mParaAction, SIGNAL(triggered()), this, SLOT(para()));
    mActions.append(mParaAction);

    mListAction = new KAction("list", actionCollection);
    mListAction->setCheckable(true);
    mListAction->setToolTip(i18nc("@info:tooltip", "List of items"));
    mListAction->setWhatsThis(i18nc("@info:whatsthis",
        "<para>Structure tag for lists of items.<nl/>"
        "Can contain only &lt;item&gt; as subtags. List is considered an "
        "element of the paragraph, so the &lt;list&gt; must be found inside "
        "&lt;para&gt;.<nl/>"
        "Example: <list>"
        "            <item>One item</item>"
        "            <item>Other item</item>"
        "         </list></para>"));
    actionCollection->addAction("kuit-edition-structure-list", mListAction);
    connect(mListAction, SIGNAL(triggered()), this, SLOT(list()));
    mActions.append(mListAction);

    mItemAction = new KAction("item", actionCollection);
    mItemAction->setCheckable(true);
    mItemAction->setToolTip(i18nc("@info:tooltip", "List items"));
    mItemAction->setWhatsThis(i18nc("@info:whatsthis",
        "<para>Structure tag for list items.<nl/>"
        "Example: <list>"
        "            <item>One item</item>"
        "            <item>Other item</item>"
        "         </list></para>"));
    actionCollection->addAction("kuit-edition-structure-item", mItemAction);
    connect(mItemAction, SIGNAL(triggered()), this, SLOT(item()));
    mActions.append(mItemAction);

    updateActionStates();
}

//private:

QAction* SemanticMarkupEdition::actionForElement(const StartTag& startTag)
                                                                        const {
    QAction* action = 0;

    QRegExp emphasisStrongRegExp("strong=\"yes|true|1\"");
    if (startTag.mName == "emphasis" &&
            startTag.mAttributes.indexOf(emphasisStrongRegExp) < 0) {
        action = mEmphasisAction;
    } else if (startTag.mName == "emphasis" &&
            startTag.mAttributes.indexOf(emphasisStrongRegExp) >= 0) {
        action = mEmphasisStrongAction;
    } else if (startTag.mName == "filename") {
        action = mFilenameAction;
    } else if (startTag.mName == "interface") {
        action = mInterfaceAction;
    } else if (startTag.mName == "link") {
        action = mLinkAction;
    } else if (startTag.mName == "nl") {
        action = mNlAction;
    } else if (startTag.mName == "shortcut") {
        action = mShortcutAction;
    } else if (startTag.mName == "para") {
        action = mParaAction;
    } else if (startTag.mName == "list") {
        action = mListAction;
    } else if (startTag.mName == "item") {
        action = mItemAction;
    }

    return action;
}

void SemanticMarkupEdition::writeSimpleTagFor(const QAction* action) {
    if (action->isChecked()) {
        mTextEdit->insertPlainText('<' + action->text() + '>');
    } else {
        mTextEdit->insertPlainText("</" + action->text() + '>');
    }
}

QDialog* SemanticMarkupEdition::newLinkDialog( //krazy:exclude=qclasses
                                        const QString& url) const {
    KDialog* dialog = new KDialog(mTextEdit);
    dialog->setModal(true);

    dialog->setButtons(KDialog::Ok | KDialog::Cancel);

    dialog->button(KDialog::Ok)->setObjectName("okButton");
    dialog->button(KDialog::Cancel)->setObjectName("cancelButton");

    SemanticMarkupLinkWidget* linkWidget = new SemanticMarkupLinkWidget(dialog);
    linkWidget->setUrl(url);
    dialog->setMainWidget(linkWidget);
    dialog->setWindowTitle(linkWidget->windowTitle());

    return dialog;
}

//private slots:

void SemanticMarkupEdition::updateActionStates() {
    Q_ASSERT(!mActions.isEmpty());

    foreach (QAction* action, mActions) {
        action->setEnabled(true);
        action->setChecked(false);
    }

    mLinkElementClosed = false;

    SemanticMarkupParser parser;
    parser.setCursorIndex(mTextEdit->textCursor().position());
    parser.parse(mTextEdit->toPlainText());

    if (parser.isCursorInsideTag()) {
        foreach (QAction* action, mActions) {
            action->setEnabled(false);
        }

        return;
    }

    QList<StartTag> openElementsAtCursor = parser.openElementsAtCursor();
    if (openElementsAtCursor.isEmpty()) {
        mListAction->setEnabled(false);
        mItemAction->setEnabled(false);

        return;
    }

    QList<QAction*> phraseActions;
    phraseActions << mEmphasisAction << mEmphasisStrongAction << mFilenameAction
                  << mInterfaceAction << mLinkAction << mNlAction
                  << mShortcutAction;

    foreach (const StartTag& startTag, openElementsAtCursor) {
        QAction* action = actionForElement(startTag);
        if (!action) {
            continue;
        }

        action->setChecked(true);

        bool elementClosed = parser.isElementClosed(startTag);
        if (elementClosed && action != mLinkAction) {
            action->setEnabled(false);
        } else if (elementClosed && action == mLinkAction) {
            action->setEnabled(true);
            mLinkElementClosed = true;
        }

        if (phraseActions.contains(action)) {
            foreach (QAction* actionToDisable, mActions) {
                if (actionToDisable != action) {
                    actionToDisable->setEnabled(false);
                }
            }
        } else if (action == mParaAction) {
            if (!mListAction->isChecked()) {
                mItemAction->setEnabled(false);
            }
        } else if (action == mListAction) {
            mParaAction->setEnabled(false);
            if (!mItemAction->isChecked()) {
                foreach (QAction* phraseAction, phraseActions) {
                    phraseAction->setEnabled(false);
                }
            }
        } else if (action == mItemAction) {
            mParaAction->setEnabled(false);
            mListAction->setEnabled(false);
        }
    }
}

void SemanticMarkupEdition::emphasis() {
    writeSimpleTagFor(mEmphasisAction);
}

void SemanticMarkupEdition::emphasisStrong() {
    if (mEmphasisStrongAction->isChecked()) {
        mTextEdit->insertPlainText("<emphasis strong=\"yes\">");
    } else {
        mTextEdit->insertPlainText("</emphasis>");
    }
}

void SemanticMarkupEdition::filename() {
    writeSimpleTagFor(mFilenameAction);
}

void SemanticMarkupEdition::interface() {
    writeSimpleTagFor(mInterfaceAction);
}

void SemanticMarkupEdition::link() {
    if (!mLinkAction->isChecked() && !mLinkElementClosed) {
        mTextEdit->insertPlainText("</link>");
        return;
    }

    if (!mLinkElementClosed) {
        QScopedPointer<QDialog> dialog(newLinkDialog(""));
        if (DialogRunner(dialog.data()).exec() == QDialog::Rejected) {
            mLinkAction->setChecked(false);
            return;
        }

        QString newUrl = dialog->findChild<SemanticMarkupLinkWidget*>()->url();
        mTextEdit->insertPlainText(QString("<link url=\"%1\">").arg(newUrl));

        return;
    }

    mLinkAction->setChecked(true);

    QString text = mTextEdit->toPlainText();
    int cursorIndex = mTextEdit->textCursor().position();
    QRegExp linkRegExp("<\\s*link\\s*(\\w+=\"[^\"]*\")*\\s*>");
    int linkElementIndex = text.lastIndexOf(linkRegExp, cursorIndex);

    QString linkText = linkRegExp.capturedTexts().at(0);
    QRegExp urlRegExp(" url=\"([^\"]*)\"");
    int urlAttributeIndex = linkText.indexOf(urlRegExp);

    if (urlAttributeIndex == -1) {
        urlAttributeIndex = QString("<link").length();
    }

    int absoluteUrlAttributeIndex = linkElementIndex + urlAttributeIndex;
    QString oldUrlAttribute = urlRegExp.capturedTexts().at(0);
    QString oldUrl = urlRegExp.capturedTexts().at(1);

    QScopedPointer<QDialog> dialog(newLinkDialog(oldUrl));
    if (DialogRunner(dialog.data()).exec() == QDialog::Rejected) {
        return;
    }

    QString newUrl = dialog->findChild<SemanticMarkupLinkWidget*>()->url();

    QTextCursor cursor = mTextEdit->textCursor();
    cursor.setPosition(absoluteUrlAttributeIndex);
    cursor.movePosition(QTextCursor::NextCharacter, QTextCursor::KeepAnchor,
                        oldUrlAttribute.length());
    cursor.insertText(QString(" url=\"%1\"").arg(newUrl));
}

void SemanticMarkupEdition::nl() {
    mTextEdit->insertPlainText("<nl/>\n");
}

void SemanticMarkupEdition::shortcut() {
    writeSimpleTagFor(mShortcutAction);
}

void SemanticMarkupEdition::para() {
    if (mParaAction->isChecked()) {
        mTextEdit->insertPlainText("<para>");
    } else {
        mTextEdit->insertPlainText("</para>\n\n");
    }
}

void SemanticMarkupEdition::list() {
    if (mListAction->isChecked()) {
        mTextEdit->insertPlainText("<list>\n");
    } else {
        mTextEdit->insertPlainText("</list>\n");
    }
}

void SemanticMarkupEdition::item() {
    if (mItemAction->isChecked()) {
        mTextEdit->insertPlainText("<item>");
    } else {
        mTextEdit->insertPlainText("</item>\n");
    }
}
