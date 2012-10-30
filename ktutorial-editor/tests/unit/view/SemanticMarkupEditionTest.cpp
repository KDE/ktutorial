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

#include <QTest>

#include "SemanticMarkupEdition.h"

#include <QAction>
#include <QTextEdit>
#include <QTimer>

#include <KActionCollection>
#include <KPushButton>

#include "SemanticMarkupLinkWidget.h"

class SemanticMarkupEditionTest: public QObject {
Q_OBJECT

private slots:

    void init();
    void cleanup();

    void testConstructor();

    void testCreateActions();

    void testEmphasis();
    void testEmphasisStrong();
    void testFilename();
    void testInterface();
    void testLink();
    void testLinkUpdate();
    void testLinkUpdateWithoutUrlAttribute();
    void testNl();
    void testShortcut();
    void testPara();
    void testList();
    void testItem();

    void testCursorInStartTag();
    void testCursorInEndTag();
    void testCursorInEmptyElementTag();

    void testCursorBetweenPhraseTags();
    void testCursorBetweenLinkTags();
    void testCursorBetweenLinkTagsWithChildElement();
    void testCursorBetweenParaTags();
    void testCursorBetweenParaTagsWithPhraseTagNotClosed();
    void testCursorBetweenListTags();
    void testCursorBetweenListTagsWithItemTagNotClosed();
    void testCursorBetweenItemTags();
    void testCursorBetweenItemTagsWithPhraseTagNotClosed();

    void testCursorBetweenEndAndStartTags();
    void testCursorBetweenUnpairedTags();
    void testCursorBetweenUnpairedTagsClosedLater();
    void testCursorBetweenTwoStartTags();
    void testCursorBetweenTwoEndTags();

    void testChangeTextWithoutChangingCursorPosition();

private:

    SemanticMarkupEdition* mSemanticMarkupEdition;
    KActionCollection* mActionCollection;
    QTextEdit* mTextEdit;

    QAction* mEmphasisAction;
    QAction* mEmphasisStrongAction;
    QAction* mFilenameAction;
    QAction* mInterfaceAction;
    QAction* mLinkAction;
    QAction* mNlAction;
    QAction* mShortcutAction;
    QAction* mParaAction;
    QAction* mListAction;
    QAction* mItemAction;

    void queueAssertLinkUrl(const QString& url, int timeToWait);
    void queueSetLinkUrl(const QString& url, int timeToWait);
    void queueCancelSetLinkUrl(const QString& url, int timeToWait);

    void moveCursorTo(int position) const;

    void assertOnlyEnabled(const QList<QAction*>& enabledActions) const;
    void assertOnlyDisabled(const QList<QAction*>& disabledActions) const;
    void assertOnlyChecked(const QList<QAction*>& checkedActions) const;

    void assertTextStartTriggerTextEndTriggerText(QAction* action,
                                                  const QString& tagName) const;

};

void SemanticMarkupEditionTest::init() {
    mTextEdit = new QTextEdit();
    mSemanticMarkupEdition = new SemanticMarkupEdition(mTextEdit);
    mActionCollection = new KActionCollection(this);

    mSemanticMarkupEdition->createActions(mActionCollection);

    mEmphasisAction = mActionCollection->action("kuit-edition-phrase-emphasis");
    mEmphasisStrongAction =
            mActionCollection->action("kuit-edition-phrase-emphasis-strong");
    mFilenameAction = mActionCollection->action("kuit-edition-phrase-filename");
    mInterfaceAction =
                    mActionCollection->action("kuit-edition-phrase-interface");
    mLinkAction = mActionCollection->action("kuit-edition-phrase-link");
    mNlAction = mActionCollection->action("kuit-edition-phrase-nl");
    mShortcutAction = mActionCollection->action("kuit-edition-phrase-shortcut");
    mParaAction = mActionCollection->action("kuit-edition-structure-para");
    mListAction = mActionCollection->action("kuit-edition-structure-list");
    mItemAction = mActionCollection->action("kuit-edition-structure-item");
}

void SemanticMarkupEditionTest::cleanup() {
    delete mActionCollection;
    delete mTextEdit;
}

void SemanticMarkupEditionTest::testConstructor() {
    QTextEdit textEdit;
    SemanticMarkupEdition* semanticMarkupEdition =
                                        new SemanticMarkupEdition(&textEdit);

    QCOMPARE(semanticMarkupEdition->parent(), &textEdit);
}

void SemanticMarkupEditionTest::testCreateActions() {
    QTextEdit textEdit;
    SemanticMarkupEdition* semanticMarkupEdition =
                                        new SemanticMarkupEdition(&textEdit);
    KActionCollection actionCollection(this);

    semanticMarkupEdition->createActions(&actionCollection);

    QCOMPARE(actionCollection.count(), 10);
    QVERIFY(actionCollection.action("kuit-edition-phrase-emphasis"));
    QVERIFY(actionCollection.action("kuit-edition-phrase-emphasis-strong"));
    QVERIFY(actionCollection.action("kuit-edition-phrase-filename"));
    QVERIFY(actionCollection.action("kuit-edition-phrase-interface"));
    QVERIFY(actionCollection.action("kuit-edition-phrase-link"));
    QVERIFY(actionCollection.action("kuit-edition-phrase-nl"));
    QVERIFY(actionCollection.action("kuit-edition-phrase-shortcut"));
    QVERIFY(actionCollection.action("kuit-edition-structure-para"));
    QVERIFY(actionCollection.action("kuit-edition-structure-list"));
    QVERIFY(actionCollection.action("kuit-edition-structure-item"));
}

void SemanticMarkupEditionTest::testEmphasis() {
    assertTextStartTriggerTextEndTriggerText(mEmphasisAction, "emphasis");
}

void SemanticMarkupEditionTest::testEmphasisStrong() {
    QVERIFY(mEmphasisStrongAction);
    QVERIFY(mEmphasisStrongAction->isEnabled());
    QVERIFY(!mEmphasisStrongAction->isChecked());

    mTextEdit->insertPlainText("No, ");
    mEmphasisStrongAction->trigger();

    assertOnlyEnabled(QList<QAction*>() << mEmphasisStrongAction);
    assertOnlyChecked(QList<QAction*>() << mEmphasisStrongAction);

    mTextEdit->insertPlainText("I");
    mEmphasisStrongAction->trigger();

    QVERIFY(mEmphasisStrongAction->isEnabled());
    QVERIFY(!mEmphasisStrongAction->isChecked());

    mTextEdit->insertPlainText(" am your father");

    QCOMPARE(mTextEdit->toPlainText(),
             QString("No, <emphasis strong=\"yes\">I</emphasis> am your "
                     "father"));
}

void SemanticMarkupEditionTest::testFilename() {
    assertTextStartTriggerTextEndTriggerText(mFilenameAction, "filename");
}

void SemanticMarkupEditionTest::testInterface() {
    assertTextStartTriggerTextEndTriggerText(mInterfaceAction, "interface");
}

void SemanticMarkupEditionTest::testLink() {
    QVERIFY(mLinkAction);
    QVERIFY(mLinkAction->isEnabled());
    QVERIFY(!mLinkAction->isChecked());

    mTextEdit->insertPlainText("Before first trigger");
    queueAssertLinkUrl("", 500);
    queueCancelSetLinkUrl("http://www.kde.org", 500);
    mLinkAction->trigger();

    QVERIFY(mLinkAction->isEnabled());
    QVERIFY(!mLinkAction->isChecked());
    QCOMPARE(mTextEdit->toPlainText(), QString("Before first trigger"));

    queueAssertLinkUrl("", 500);
    queueSetLinkUrl("http://www.kde.org", 500);
    mLinkAction->trigger();

    assertOnlyEnabled(QList<QAction*>() << mLinkAction);
    assertOnlyChecked(QList<QAction*>() << mLinkAction);

    mTextEdit->insertPlainText("While checked");
    mLinkAction->trigger();

    QVERIFY(mLinkAction->isEnabled());
    QVERIFY(!mLinkAction->isChecked());

    mTextEdit->insertPlainText("After second trigger");

    QCOMPARE(mTextEdit->toPlainText(),
             QString("Before first trigger<link url=\"http://www.kde.org\">"
                     "While checked</link>After second trigger"));
}

void SemanticMarkupEditionTest::testLinkUpdate() {
    mTextEdit->setPlainText("Before first trigger<link "
                            "url=\"http://www.kde.org\">While checked</link>"
                            "After second trigger");

    moveCursorTo(58);

    queueAssertLinkUrl("http://www.kde.org", 500);
    queueSetLinkUrl("widget:okButton", 500);
    mLinkAction->trigger();

    QVERIFY(mLinkAction->isEnabled());
    QVERIFY(mLinkAction->isChecked());

    QCOMPARE(mTextEdit->toPlainText(),
             QString("Before first trigger<link url=\"widget:okButton\">"
                     "While checked</link>After second trigger"));
    QCOMPARE(mTextEdit->textCursor().position(), 55);

    //Cancel update link
    queueAssertLinkUrl("widget:okButton", 500);
    queueCancelSetLinkUrl("http://www.kde-look.org", 500);
    mLinkAction->trigger();

    QVERIFY(mLinkAction->isEnabled());
    QVERIFY(mLinkAction->isChecked());

    QCOMPARE(mTextEdit->toPlainText(),
             QString("Before first trigger<link url=\"widget:okButton\">"
                     "While checked</link>After second trigger"));
    QCOMPARE(mTextEdit->textCursor().position(), 55);
}

void SemanticMarkupEditionTest::testLinkUpdateWithoutUrlAttribute() {
    mTextEdit->setPlainText("Before<link>Between</link>After");

    moveCursorTo(16);

    queueAssertLinkUrl("", 500);
    queueSetLinkUrl("widget:okButton", 500);
    mLinkAction->trigger();

    QVERIFY(mLinkAction->isEnabled());
    QVERIFY(mLinkAction->isChecked());

    QCOMPARE(mTextEdit->toPlainText(),
             QString("Before<link url=\"widget:okButton\">Between</link>"
                     "After"));
    QCOMPARE(mTextEdit->textCursor().position(), 38);
}

void SemanticMarkupEditionTest::testNl() {
    QVERIFY(mNlAction);
    QVERIFY(mNlAction->isEnabled());
    QVERIFY(!mNlAction->isChecked());

    mTextEdit->insertPlainText("Before line break");
    mNlAction->trigger();

    QVERIFY(mNlAction->isEnabled());
    QVERIFY(!mNlAction->isChecked());

    mTextEdit->insertPlainText("After line break");

    QCOMPARE(mTextEdit->toPlainText(),
             QString("Before line break<nl/>\nAfter line break"));
}

void SemanticMarkupEditionTest::testShortcut() {
    assertTextStartTriggerTextEndTriggerText(mShortcutAction, "shortcut");
}

void SemanticMarkupEditionTest::testPara() {
    QVERIFY(mParaAction);
    QVERIFY(mParaAction->isEnabled());
    QVERIFY(!mParaAction->isChecked());

    mParaAction->trigger();

    assertOnlyDisabled(QList<QAction*>() << mItemAction);
    assertOnlyChecked(QList<QAction*>() << mParaAction);

    mTextEdit->insertPlainText("I have a ");
    QVERIFY(mEmphasisAction);
    mEmphasisAction->trigger();

    assertOnlyEnabled(QList<QAction*>() << mEmphasisAction);
    assertOnlyChecked(QList<QAction*>() << mParaAction << mEmphasisAction);

    mTextEdit->insertPlainText("bad");
    mEmphasisAction->trigger();

    assertOnlyDisabled(QList<QAction*>() << mItemAction);
    assertOnlyChecked(QList<QAction*>() << mParaAction);

    mTextEdit->insertPlainText(" feeling about this");
    mParaAction->trigger();

    QVERIFY(mParaAction->isEnabled());
    QVERIFY(!mParaAction->isChecked());

    QCOMPARE(mTextEdit->toPlainText(),
             QString("<para>I have a <emphasis>bad</emphasis> feeling about "
                     "this</para>\n\n"));
}

void SemanticMarkupEditionTest::testList() {
    QVERIFY(mListAction);
    QVERIFY(!mListAction->isEnabled());
    QVERIFY(!mListAction->isChecked());

    mTextEdit->insertPlainText("<para>");

    QVERIFY(mListAction->isEnabled());
    QVERIFY(!mListAction->isChecked());

    mListAction->trigger();

    assertOnlyEnabled(QList<QAction*>() << mListAction << mItemAction);
    assertOnlyChecked(QList<QAction*>() << mParaAction << mListAction);

    mListAction->trigger();

    QVERIFY(mListAction->isEnabled());
    QVERIFY(!mListAction->isChecked());

    mTextEdit->insertPlainText("</para>");

    QVERIFY(!mListAction->isEnabled());
    QVERIFY(!mListAction->isChecked());

    QCOMPARE(mTextEdit->toPlainText(),
             QString("<para><list>\n</list>\n</para>"));
}

void SemanticMarkupEditionTest::testItem() {
    QVERIFY(mItemAction);
    QVERIFY(!mItemAction->isEnabled());
    QVERIFY(!mItemAction->isChecked());

    mTextEdit->insertPlainText("<para><list>");

    QVERIFY(mItemAction->isEnabled());
    QVERIFY(!mItemAction->isChecked());

    mItemAction->trigger();

    assertOnlyDisabled(QList<QAction*>() << mParaAction << mListAction);
    assertOnlyChecked(QList<QAction*>() << mParaAction << mListAction
                                        << mItemAction);

    QVERIFY(mEmphasisAction);
    mEmphasisAction->trigger();

    assertOnlyEnabled(QList<QAction*>() << mEmphasisAction);
    assertOnlyChecked(QList<QAction*>() << mParaAction << mListAction
                                        << mItemAction << mEmphasisAction);

    mEmphasisAction->trigger();

    assertOnlyDisabled(QList<QAction*>() << mParaAction << mListAction);
    assertOnlyChecked(QList<QAction*>() << mParaAction << mListAction
                                        << mItemAction);

    mItemAction->trigger();

    QVERIFY(mItemAction->isEnabled());
    QVERIFY(!mItemAction->isChecked());

    mTextEdit->insertPlainText("</list></para>");

    QVERIFY(!mItemAction->isEnabled());
    QVERIFY(!mItemAction->isChecked());

    QCOMPARE(mTextEdit->toPlainText(),
             QString("<para><list><item><emphasis></emphasis></item>\n</list>"
                     "</para>"));
}

void SemanticMarkupEditionTest::testCursorInStartTag() {
    mTextEdit->setPlainText("<para>Some text <shortcut> more text");

    //Just before <shortcut>
    moveCursorTo(16);

    assertOnlyDisabled(QList<QAction*>() << mItemAction);
    assertOnlyChecked(QList<QAction*>() << mParaAction);

    //Just after <
    moveCursorTo(17);

    assertOnlyEnabled(QList<QAction*>());
    assertOnlyChecked(QList<QAction*>());

    //Middle of tag
    moveCursorTo(21);

    assertOnlyEnabled(QList<QAction*>());
    assertOnlyChecked(QList<QAction*>());

    //Just before >
    moveCursorTo(25);

    assertOnlyEnabled(QList<QAction*>());
    assertOnlyChecked(QList<QAction*>());

    //Just after <shortcut>
    moveCursorTo(26);

    assertOnlyEnabled(QList<QAction*>() << mShortcutAction);
    assertOnlyChecked(QList<QAction*>() << mParaAction << mShortcutAction);
}

void SemanticMarkupEditionTest::testCursorInEndTag() {
    mTextEdit->setPlainText("<para>Some text </interface> more text");

    //Just before </interface>
    moveCursorTo(16);

    assertOnlyDisabled(QList<QAction*>() << mItemAction);
    assertOnlyChecked(QList<QAction*>() << mParaAction);

    //Just after <
    moveCursorTo(17);

    assertOnlyEnabled(QList<QAction*>());
    assertOnlyChecked(QList<QAction*>());

    //Middle of tag
    moveCursorTo(21);

    assertOnlyEnabled(QList<QAction*>());
    assertOnlyChecked(QList<QAction*>());

    //Just before >
    moveCursorTo(27);

    assertOnlyEnabled(QList<QAction*>());
    assertOnlyChecked(QList<QAction*>());

    //Just after </interface>
    moveCursorTo(28);

    assertOnlyDisabled(QList<QAction*>() << mItemAction);
    assertOnlyChecked(QList<QAction*>() << mParaAction);
}

void SemanticMarkupEditionTest::testCursorInEmptyElementTag() {
    mTextEdit->setPlainText("<para>I love you<nl/>I know");

    //Just before <nl/>
    moveCursorTo(16);

    assertOnlyDisabled(QList<QAction*>() << mItemAction);
    assertOnlyChecked(QList<QAction*>() << mParaAction);

    //Just after <
    moveCursorTo(17);

    assertOnlyEnabled(QList<QAction*>());
    assertOnlyChecked(QList<QAction*>());

    //Middle of tag
    moveCursorTo(19);

    assertOnlyEnabled(QList<QAction*>());
    assertOnlyChecked(QList<QAction*>());

    //Just before >
    moveCursorTo(20);

    assertOnlyEnabled(QList<QAction*>());
    assertOnlyChecked(QList<QAction*>());

    //Just after <nl/>
    moveCursorTo(21);

    assertOnlyDisabled(QList<QAction*>() << mItemAction);
    assertOnlyChecked(QList<QAction*>() << mParaAction);
}

//Just test with filename instead of testing every phrase tag
void SemanticMarkupEditionTest::testCursorBetweenPhraseTags() {
    mTextEdit->setPlainText("<filename>/usr/bin/ktutorial-editor</filename>");

    //Just after <filename>
    moveCursorTo(10);

    assertOnlyEnabled(QList<QAction*>());
    assertOnlyChecked(QList<QAction*>() << mFilenameAction);

    //Middle of text
    moveCursorTo(23);

    assertOnlyEnabled(QList<QAction*>());
    assertOnlyChecked(QList<QAction*>() << mFilenameAction);

    //Just before </filename>
    moveCursorTo(35);

    assertOnlyEnabled(QList<QAction*>());
    assertOnlyChecked(QList<QAction*>() << mFilenameAction);
}

//Link is a special phrase tag and shows a special behavior in this case
void SemanticMarkupEditionTest::testCursorBetweenLinkTags() {
    mTextEdit->setPlainText("<link url=\"widget:okButton\">Ok button</link>");
    moveCursorTo(32);

    assertOnlyEnabled(QList<QAction*>() << mLinkAction);
    assertOnlyChecked(QList<QAction*>() << mLinkAction);
}

//Link does not contain child elements... but just in case
void SemanticMarkupEditionTest::testCursorBetweenLinkTagsWithChildElement() {
    mTextEdit->setPlainText("<link url=\"widget:okButton\"><interface>Ok button"
                            "</interface></link>");
    moveCursorTo(42);

    assertOnlyEnabled(QList<QAction*>() << mLinkAction);
    assertOnlyChecked(QList<QAction*>() << mLinkAction << mInterfaceAction);
}

void SemanticMarkupEditionTest::testCursorBetweenParaTags() {
    mTextEdit->setPlainText("<para>The source file is <filename>"
                            "SemanticMarkupEditionTest.cpp</filename></para>");
    moveCursorTo(15);

    assertOnlyDisabled(QList<QAction*>() << mParaAction << mItemAction);
    assertOnlyChecked(QList<QAction*>() << mParaAction);

    moveCursorTo(42);

    assertOnlyEnabled(QList<QAction*>());
    assertOnlyChecked(QList<QAction*>() << mParaAction << mFilenameAction);
}

void SemanticMarkupEditionTest::
                            testCursorBetweenParaTagsWithPhraseTagNotClosed() {
    mTextEdit->setPlainText("<para>The source file is <filename>"
                            "SemanticMarkupEditionTest.cpp</para>");
    moveCursorTo(42);

    assertOnlyEnabled(QList<QAction*>() << mFilenameAction);
    assertOnlyChecked(QList<QAction*>() << mParaAction << mFilenameAction);
}

void SemanticMarkupEditionTest::testCursorBetweenListTags() {
    mTextEdit->setPlainText("<para><list></list></para>");
    moveCursorTo(12);

    assertOnlyEnabled(QList<QAction*>() << mItemAction);
    assertOnlyChecked(QList<QAction*>() << mParaAction << mListAction);
}

void SemanticMarkupEditionTest::
                            testCursorBetweenListTagsWithItemTagNotClosed() {
    mTextEdit->setPlainText("<para><list><item>Item text</list></para>");
    moveCursorTo(23);

    assertOnlyDisabled(QList<QAction*>() << mParaAction << mListAction);
    assertOnlyChecked(QList<QAction*>() << mParaAction << mListAction
                                        << mItemAction);
}

void SemanticMarkupEditionTest::testCursorBetweenItemTags() {
    mTextEdit->setPlainText("<para><list><item>A <emphasis>rubber "
                            "chicken</emphasis> with a pulley in the "
                            "middle</item></list></para>");
    moveCursorTo(64);

    assertOnlyDisabled(QList<QAction*>() << mParaAction << mListAction
                                         << mItemAction);
    assertOnlyChecked(QList<QAction*>() << mParaAction << mListAction
                                        << mItemAction);

    moveCursorTo(42);

    assertOnlyEnabled(QList<QAction*>());
    assertOnlyChecked(QList<QAction*>() << mParaAction << mListAction
                                        << mItemAction << mEmphasisAction);
}

void SemanticMarkupEditionTest::testCursorBetweenItemTagsWithPhraseTagNotClosed() {
    mTextEdit->setPlainText("<para><list><item>A <emphasis>rubber "
                            "chicken with a pulley in the "
                            "middle</item></list></para>");

    moveCursorTo(42);

    assertOnlyEnabled(QList<QAction*>() << mEmphasisAction);
    assertOnlyChecked(QList<QAction*>() << mParaAction << mListAction
                                        << mItemAction << mEmphasisAction);
}

void SemanticMarkupEditionTest::testCursorBetweenEndAndStartTags() {
    mTextEdit->setPlainText("</interface>Some text<shortcut>");
    moveCursorTo(16);

    assertOnlyDisabled(QList<QAction*>() << mListAction << mItemAction);
    assertOnlyChecked(QList<QAction*>());
}

void SemanticMarkupEditionTest::testCursorBetweenUnpairedTags() {
    mTextEdit->setPlainText("<interface>Some text</shortcut>");
    moveCursorTo(16);

    assertOnlyEnabled(QList<QAction*>() << mInterfaceAction);
    assertOnlyChecked(QList<QAction*>() << mInterfaceAction);
}

void SemanticMarkupEditionTest::testCursorBetweenUnpairedTagsClosedLater() {
    mTextEdit->setPlainText("<interface>Some text</shortcut></interface>");
    moveCursorTo(16);

    assertOnlyEnabled(QList<QAction*>());
    assertOnlyChecked(QList<QAction*>() << mInterfaceAction);
}

void SemanticMarkupEditionTest::testCursorBetweenTwoStartTags() {
    mTextEdit->setPlainText("<interface>Some text<shortcut>");
    moveCursorTo(16);

    assertOnlyEnabled(QList<QAction*>() << mInterfaceAction);
    assertOnlyChecked(QList<QAction*>() << mInterfaceAction);
}

void SemanticMarkupEditionTest::testCursorBetweenTwoEndTags() {
    mTextEdit->setPlainText("</interface>Some text</shortcut>");
    moveCursorTo(16);

    assertOnlyDisabled(QList<QAction*>() << mListAction << mItemAction);
    assertOnlyChecked(QList<QAction*>());
}

void SemanticMarkupEditionTest::testChangeTextWithoutChangingCursorPosition() {
    mTextEdit->setPlainText("<emphasis>Some text</emphasis>");
    moveCursorTo(19);

    assertOnlyEnabled(QList<QAction*>());
    assertOnlyChecked(QList<QAction*>() << mEmphasisAction);

    QTest::keyClick(mTextEdit, Qt::Key_Delete);

    assertOnlyEnabled(QList<QAction*>() << mEmphasisAction);
    assertOnlyChecked(QList<QAction*>() << mEmphasisAction);
}

/////////////////////////////////// Helpers ////////////////////////////////////

//The dialogs are modal, so they won't return to the test code until they are
//closed. Thus, the actions to be performed while the dialogs are being shown
//(like setting the URL of a link element) must be "queued".
class QueuedActionsHelper: public QObject {
Q_OBJECT
public:

    QueuedActionsHelper(QObject* parent = 0): QObject(parent) {
    }

    void setTextEdit(const QTextEdit* textEdit) {
        mTextEdit = textEdit;
    }

    void setUrl(const QString& url) {
        mUrl = url;
    }

public slots:

    void assertLinkUrl() {
        SemanticMarkupLinkWidget* linkWidget =
                            mTextEdit->findChild<SemanticMarkupLinkWidget*>();
        QCOMPARE(linkWidget->url(), mUrl);
    }

    void setLinkUrl() {
        SemanticMarkupLinkWidget* linkWidget =
                            mTextEdit->findChild<SemanticMarkupLinkWidget*>();
        KPushButton* okButton = mTextEdit->findChild<KPushButton*>("okButton");

        linkWidget->setUrl(mUrl);
        okButton->click();
    }

    void cancelSetLinkUrl() {
        SemanticMarkupLinkWidget* linkWidget =
                            mTextEdit->findChild<SemanticMarkupLinkWidget*>();
        KPushButton* cancelButton =
                            mTextEdit->findChild<KPushButton*>("cancelButton");

        linkWidget->setUrl(mUrl);
        cancelButton->click();
    }

private:

    const QWidget* mTextEdit;
    QString mUrl;

};

void SemanticMarkupEditionTest::queueAssertLinkUrl(const QString& url,
                                                   int timeToWait) {
    QueuedActionsHelper* helper = new QueuedActionsHelper(this);
    helper->setTextEdit(mTextEdit);
    helper->setUrl(url);
    QTimer::singleShot(timeToWait, helper, SLOT(assertLinkUrl()));
}

void SemanticMarkupEditionTest::queueSetLinkUrl(const QString& url,
                                                int timeToWait) {
    QueuedActionsHelper* helper = new QueuedActionsHelper(this);
    helper->setTextEdit(mTextEdit);
    helper->setUrl(url);
    QTimer::singleShot(timeToWait, helper, SLOT(setLinkUrl()));
}

void SemanticMarkupEditionTest::queueCancelSetLinkUrl(const QString& url,
                                                      int timeToWait) {
    QueuedActionsHelper* helper = new QueuedActionsHelper(this);
    helper->setTextEdit(mTextEdit);
    helper->setUrl(url);
    QTimer::singleShot(timeToWait, helper, SLOT(cancelSetLinkUrl()));
}

void SemanticMarkupEditionTest::moveCursorTo(int position) const {
    mTextEdit->moveCursor(QTextCursor::Start);

    for (int i=0; i<position; ++i) {
        mTextEdit->moveCursor(QTextCursor::NextCharacter,
                              QTextCursor::MoveAnchor);
    }
}

void SemanticMarkupEditionTest::assertOnlyEnabled(
                                const QList<QAction*>& enabledActions) const {
    foreach (QAction* action, mActionCollection->actions()) {
        if (enabledActions.contains(action)) {
            QVERIFY2(action->isEnabled(),
                     QString("Action %1 is not enabled").arg(action->text())
                            .toStdString().c_str());
        } else {
            QVERIFY2(!action->isEnabled(),
                     QString("Action %1 is enabled").arg(action->text())
                            .toStdString().c_str());
        }
    }
}

void SemanticMarkupEditionTest::assertOnlyDisabled(
                                const QList<QAction*>& disabledActions) const {
    foreach (QAction* action, mActionCollection->actions()) {
        if (disabledActions.contains(action)) {
            QVERIFY2(!action->isEnabled(),
                     QString("Action %1 is not disabled").arg(action->text())
                            .toStdString().c_str());
        } else {
            QVERIFY2(action->isEnabled(),
                    QString("Action %1 disabled").arg(action->text())
                            .toStdString().c_str());
        }
    }
}

void SemanticMarkupEditionTest::assertOnlyChecked(
                                const QList<QAction*>& checkedActions) const {
    foreach (QAction* action, mActionCollection->actions()) {
        if (checkedActions.contains(action)) {
            QVERIFY2(action->isChecked(),
                     QString("Action %1 is not checked").arg(action->text())
                            .toStdString().c_str());
        } else {
            QVERIFY2(!action->isChecked(),
                     QString("Action %1 is checked").arg(action->text())
                            .toStdString().c_str());
        }
    }
}

void SemanticMarkupEditionTest::assertTextStartTriggerTextEndTriggerText(
                                QAction* action, const QString& tagName) const {
    QVERIFY(action);
    QVERIFY(action->isEnabled());
    QVERIFY(!action->isChecked());

    mTextEdit->insertPlainText("Before first trigger");
    action->trigger();

    assertOnlyEnabled(QList<QAction*>() << action);
    assertOnlyChecked(QList<QAction*>() << action);

    mTextEdit->insertPlainText("While checked");
    action->trigger();

    QVERIFY(action->isEnabled());
    QVERIFY(!action->isChecked());

    mTextEdit->insertPlainText("After second trigger");

    QCOMPARE(mTextEdit->toPlainText(),
             QString("Before first trigger<%1>While checked</%1>After second "
                     "trigger").arg(tagName));
}

QTEST_MAIN(SemanticMarkupEditionTest)

#include "SemanticMarkupEditionTest.moc"
