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

#include "SemanticMarkupParser.h"

class SemanticMarkupParserTest: public QObject {
Q_OBJECT

private slots:

    void testNoTags();
    void testStartTag();
    void testEndTag();
    void testEmptyElementTag();

    void testPairedTags();
    void testUnpairedTags();

    void testPairedNestedTags();
    void testPairedTagsWithChildStartTagNotClosed();
    void testPairedTagsWithChildStartTagClosedAfterParent();

    void testNestedDuplicatedTag();

    void testPreviouslyPairedTags();

    void testCursorInsideStartTag();
    void testCursorInsideEndTag();
    void testCursorInsideEmptyElementTag();

    void testParseTwice();

};

void SemanticMarkupParserTest::testNoTags() {
    SemanticMarkupParser parser;

    QString text("Some text without tags");
    parser.setCursorIndex(8);
    parser.parse(text);

    QVERIFY(!parser.isCursorInsideTag());
    QVERIFY(parser.openElementsAtCursor().isEmpty());
}

void SemanticMarkupParserTest::testStartTag() {
    SemanticMarkupParser parser;

    QString text("Before <element> After");
    parser.setCursorIndex(4);
    parser.parse(text);

    QVERIFY(!parser.isCursorInsideTag());
    QVERIFY(parser.openElementsAtCursor().isEmpty());

    parser.setCursorIndex(20);
    parser.parse(text);

    QVERIFY(!parser.isCursorInsideTag());
    QList<StartTag> openElements = parser.openElementsAtCursor();
    QCOMPARE(openElements.count(), 1);
    QCOMPARE(openElements[0].mName, QString("element"));
    QCOMPARE(openElements[0].mAttributes, QString(""));
    QCOMPARE(openElements[0].mIndex, 7);
    QVERIFY(!parser.isElementClosed(openElements[0]));

    text = "Before <element attribute1=\"value1\" attribute2=\"value2\"    "
           "attribute3=\"value3\"> After";

    parser.setCursorIndex(84);
    parser.parse(text);

    QVERIFY(!parser.isCursorInsideTag());
    openElements = parser.openElementsAtCursor();
    QCOMPARE(openElements.count(), 1);
    QCOMPARE(openElements[0].mName, QString("element"));
    QCOMPARE(openElements[0].mAttributes,
             QString("attribute1=\"value1\" attribute2=\"value2\"    "
                     "attribute3=\"value3\""));
    QCOMPARE(openElements[0].mIndex, 7);
    QVERIFY(!parser.isElementClosed(openElements[0]));
}

void SemanticMarkupParserTest::testEndTag() {
    SemanticMarkupParser parser;

    QString text("Before </element> After");
    parser.setCursorIndex(4);
    parser.parse(text);

    QVERIFY(!parser.isCursorInsideTag());
    QVERIFY(parser.openElementsAtCursor().isEmpty());

    parser.setCursorIndex(20);
    parser.parse(text);

    QVERIFY(!parser.isCursorInsideTag());
    QVERIFY(parser.openElementsAtCursor().isEmpty());
}

void SemanticMarkupParserTest::testEmptyElementTag() {
    SemanticMarkupParser parser;

    QString text("Before <element/> After");
    parser.setCursorIndex(4);
    parser.parse(text);

    QVERIFY(!parser.isCursorInsideTag());
    QVERIFY(parser.openElementsAtCursor().isEmpty());

    parser.setCursorIndex(20);
    parser.parse(text);

    QVERIFY(!parser.isCursorInsideTag());
    QVERIFY(parser.openElementsAtCursor().isEmpty());

    text = "Before <element attribute1=\"value1\" attribute2=\"value2\"    "
           "attribute3=\"value3\"/> After";

    parser.setCursorIndex(84);
    parser.parse(text);

    QVERIFY(!parser.isCursorInsideTag());
    QVERIFY(parser.openElementsAtCursor().isEmpty());
}

void SemanticMarkupParserTest::testPairedTags() {
    SemanticMarkupParser parser;

    QString text("Before <element> Between </element> After");
    parser.setCursorIndex(20);
    parser.parse(text);

    QVERIFY(!parser.isCursorInsideTag());
    QList<StartTag> openElements = parser.openElementsAtCursor();
    QCOMPARE(openElements.count(), 1);
    QCOMPARE(openElements[0].mName, QString("element"));
    QCOMPARE(openElements[0].mAttributes, QString(""));
    QCOMPARE(openElements[0].mIndex, 7);
    QVERIFY(parser.isElementClosed(openElements[0]));
}

void SemanticMarkupParserTest::testUnpairedTags() {
    SemanticMarkupParser parser;

    QString text("Before <element> Between </otherElement> After");
    parser.setCursorIndex(20);
    parser.parse(text);

    QVERIFY(!parser.isCursorInsideTag());
    QList<StartTag> openElements = parser.openElementsAtCursor();
    QCOMPARE(openElements.count(), 1);
    QCOMPARE(openElements[0].mName, QString("element"));
    QCOMPARE(openElements[0].mAttributes, QString(""));
    QCOMPARE(openElements[0].mIndex, 7);
    QVERIFY(!parser.isElementClosed(openElements[0]));
}

void SemanticMarkupParserTest::testPairedNestedTags() {
    SemanticMarkupParser parser;

    QString text("Before <element> Between <child> Between child </child> "
                 "Between </element> After");
    parser.setCursorIndex(20);
    parser.parse(text);

    QVERIFY(!parser.isCursorInsideTag());
    QList<StartTag> openElements = parser.openElementsAtCursor();
    QCOMPARE(openElements.count(), 1);
    QCOMPARE(openElements[0].mName, QString("element"));
    QCOMPARE(openElements[0].mAttributes, QString(""));
    QCOMPARE(openElements[0].mIndex, 7);
    QVERIFY(parser.isElementClosed(openElements[0]));

    parser.setCursorIndex(42);
    parser.parse(text);

    QVERIFY(!parser.isCursorInsideTag());
    openElements = parser.openElementsAtCursor();
    QCOMPARE(openElements.count(), 2);
    QCOMPARE(openElements[0].mName, QString("child"));
    QCOMPARE(openElements[0].mAttributes, QString(""));
    QCOMPARE(openElements[0].mIndex, 25);
    QVERIFY(parser.isElementClosed(openElements[0]));
    QCOMPARE(openElements[1].mName, QString("element"));
    QCOMPARE(openElements[1].mAttributes, QString(""));
    QCOMPARE(openElements[1].mIndex, 7);
    QVERIFY(parser.isElementClosed(openElements[1]));

    parser.setCursorIndex(60);
    parser.parse(text);

    QVERIFY(!parser.isCursorInsideTag());
    openElements = parser.openElementsAtCursor();
    QCOMPARE(openElements.count(), 1);
    QCOMPARE(openElements[0].mName, QString("element"));
    QCOMPARE(openElements[0].mAttributes, QString(""));
    QCOMPARE(openElements[0].mIndex, 7);
    QVERIFY(parser.isElementClosed(openElements[0]));
}

void SemanticMarkupParserTest::testPairedTagsWithChildStartTagNotClosed() {
    SemanticMarkupParser parser;

    QString text("Before <element> Between <child> After child </element> "
                 "After");
    parser.setCursorIndex(20);
    parser.parse(text);

    QVERIFY(!parser.isCursorInsideTag());
    QList<StartTag> openElements = parser.openElementsAtCursor();
    QCOMPARE(openElements.count(), 1);
    QCOMPARE(openElements[0].mName, QString("element"));
    QCOMPARE(openElements[0].mAttributes, QString(""));
    QCOMPARE(openElements[0].mIndex, 7);
    QVERIFY(parser.isElementClosed(openElements[0]));

    parser.setCursorIndex(42);
    parser.parse(text);

    QVERIFY(!parser.isCursorInsideTag());
    openElements = parser.openElementsAtCursor();
    QCOMPARE(openElements.count(), 2);
    QCOMPARE(openElements[0].mName, QString("child"));
    QCOMPARE(openElements[0].mAttributes, QString(""));
    QCOMPARE(openElements[0].mIndex, 25);
    QVERIFY(!parser.isElementClosed(openElements[0]));
    QCOMPARE(openElements[1].mName, QString("element"));
    QCOMPARE(openElements[1].mAttributes, QString(""));
    QCOMPARE(openElements[1].mIndex, 7);
    QVERIFY(parser.isElementClosed(openElements[1]));

    parser.setCursorIndex(58);
    parser.parse(text);

    QVERIFY(!parser.isCursorInsideTag());
    QVERIFY(parser.openElementsAtCursor().isEmpty());
}

void SemanticMarkupParserTest::
                            testPairedTagsWithChildStartTagClosedAfterParent() {
    SemanticMarkupParser parser;

    QString text("Before <element> Between <child> After child </element> "
                 "After </child>");
    parser.setCursorIndex(20);
    parser.parse(text);

    QVERIFY(!parser.isCursorInsideTag());
    QList<StartTag> openElements = parser.openElementsAtCursor();
    QCOMPARE(openElements.count(), 1);
    QCOMPARE(openElements[0].mName, QString("element"));
    QCOMPARE(openElements[0].mAttributes, QString(""));
    QCOMPARE(openElements[0].mIndex, 7);
    QVERIFY(parser.isElementClosed(openElements[0]));

    parser.setCursorIndex(42);
    parser.parse(text);

    QVERIFY(!parser.isCursorInsideTag());
    openElements = parser.openElementsAtCursor();
    QCOMPARE(openElements.count(), 2);
    QCOMPARE(openElements[0].mName, QString("child"));
    QCOMPARE(openElements[0].mAttributes, QString(""));
    QCOMPARE(openElements[0].mIndex, 25);
    QVERIFY(!parser.isElementClosed(openElements[0]));
    QCOMPARE(openElements[1].mName, QString("element"));
    QCOMPARE(openElements[1].mAttributes, QString(""));
    QCOMPARE(openElements[1].mIndex, 7);
    QVERIFY(parser.isElementClosed(openElements[1]));

    parser.setCursorIndex(58);
    parser.parse(text);

    QVERIFY(!parser.isCursorInsideTag());
    QVERIFY(parser.openElementsAtCursor().isEmpty());
}

void SemanticMarkupParserTest::testNestedDuplicatedTag() {
    SemanticMarkupParser parser;

    QString text("Before <duplicated> Between <duplicated> Between child "
                 "</duplicated> After");
    parser.setCursorIndex(23);
    parser.parse(text);

    QVERIFY(!parser.isCursorInsideTag());
    QList<StartTag> openElements = parser.openElementsAtCursor();
    QCOMPARE(openElements.count(), 1);
    QCOMPARE(openElements[0].mName, QString("duplicated"));
    QCOMPARE(openElements[0].mAttributes, QString(""));
    QCOMPARE(openElements[0].mIndex, 7);
    QVERIFY(!parser.isElementClosed(openElements[0]));

    parser.setCursorIndex(42);
    parser.parse(text);

    QVERIFY(!parser.isCursorInsideTag());
    openElements = parser.openElementsAtCursor();
    QCOMPARE(openElements.count(), 2);
    QCOMPARE(openElements[0].mName, QString("duplicated"));
    QCOMPARE(openElements[0].mAttributes, QString(""));
    QCOMPARE(openElements[0].mIndex, 28);
    QVERIFY(parser.isElementClosed(openElements[0]));
    QCOMPARE(openElements[1].mName, QString("duplicated"));
    QCOMPARE(openElements[1].mAttributes, QString(""));
    QCOMPARE(openElements[1].mIndex, 7);
    QVERIFY(!parser.isElementClosed(openElements[1]));

    parser.setCursorIndex(72);
    parser.parse(text);

    QVERIFY(!parser.isCursorInsideTag());
    openElements = parser.openElementsAtCursor();
    QCOMPARE(openElements.count(), 1);
    QCOMPARE(openElements[0].mName, QString("duplicated"));
    QCOMPARE(openElements[0].mAttributes, QString(""));
    QCOMPARE(openElements[0].mIndex, 7);
    QVERIFY(!parser.isElementClosed(openElements[0]));
}

void SemanticMarkupParserTest::testPreviouslyPairedTags() {
    SemanticMarkupParser parser;

    QString text("<previous></previous> Before <element> After");
    parser.setCursorIndex(23);
    parser.parse(text);

    QVERIFY(!parser.isCursorInsideTag());
    QVERIFY(parser.openElementsAtCursor().isEmpty());

    parser.setCursorIndex(42);
    parser.parse(text);

    QVERIFY(!parser.isCursorInsideTag());
    QList<StartTag> openElements = parser.openElementsAtCursor();
    QCOMPARE(openElements.count(), 1);
    QCOMPARE(openElements[0].mName, QString("element"));
    QCOMPARE(openElements[0].mAttributes, QString(""));
    QCOMPARE(openElements[0].mIndex, 29);
    QVERIFY(!parser.isElementClosed(openElements[0]));
}

void SemanticMarkupParserTest::testCursorInsideStartTag() {
    SemanticMarkupParser parser;

    QString text("Before <element> After");

    //Before "<"
    parser.setCursorIndex(7);
    parser.parse(text);

    QVERIFY(!parser.isCursorInsideTag());

    //After "<"
    parser.setCursorIndex(8);
    parser.parse(text);

    QVERIFY(parser.isCursorInsideTag());

    //Inside "element"
    parser.setCursorIndex(12);
    parser.parse(text);

    QVERIFY(parser.isCursorInsideTag());

    //Before ">"
    parser.setCursorIndex(15);
    parser.parse(text);

    QVERIFY(parser.isCursorInsideTag());

    //After ">"
    parser.setCursorIndex(16);
    parser.parse(text);

    QVERIFY(!parser.isCursorInsideTag());

    //Inside attributes
    text = "Before <element attribute1=\"value1\" attribute2=\"value2\"    "
           "attribute3=\"value3\"> After";

    parser.setCursorIndex(42);
    parser.parse(text);

    QVERIFY(parser.isCursorInsideTag());
}

void SemanticMarkupParserTest::testCursorInsideEndTag() {
    SemanticMarkupParser parser;

    QString text("Before </element> After");

    //Before "<"
    parser.setCursorIndex(7);
    parser.parse(text);

    QVERIFY(!parser.isCursorInsideTag());

    //After "<"
    parser.setCursorIndex(8);
    parser.parse(text);

    QVERIFY(parser.isCursorInsideTag());

    //Inside "element"
    parser.setCursorIndex(12);
    parser.parse(text);

    QVERIFY(parser.isCursorInsideTag());

    //Before ">"
    parser.setCursorIndex(16);
    parser.parse(text);

    QVERIFY(parser.isCursorInsideTag());

    //After ">"
    parser.setCursorIndex(17);
    parser.parse(text);

    QVERIFY(!parser.isCursorInsideTag());
}

void SemanticMarkupParserTest::testCursorInsideEmptyElementTag() {
    SemanticMarkupParser parser;

    QString text("Before <element/> After");

    //Before "<"
    parser.setCursorIndex(7);
    parser.parse(text);

    QVERIFY(!parser.isCursorInsideTag());

    //After "<"
    parser.setCursorIndex(8);
    parser.parse(text);

    QVERIFY(parser.isCursorInsideTag());

    //Inside "element"
    parser.setCursorIndex(12);
    parser.parse(text);

    QVERIFY(parser.isCursorInsideTag());

    //Before ">"
    parser.setCursorIndex(16);
    parser.parse(text);

    QVERIFY(parser.isCursorInsideTag());

    //After ">"
    parser.setCursorIndex(17);
    parser.parse(text);

    QVERIFY(!parser.isCursorInsideTag());

    //Inside attributes
    text = "Before <element attribute1=\"value1\" attribute2=\"value2\"    "
           "attribute3=\"value3\"/> After";

    parser.setCursorIndex(42);
    parser.parse(text);

    QVERIFY(parser.isCursorInsideTag());
}

void SemanticMarkupParserTest::testParseTwice() {
    SemanticMarkupParser parser;

    QString text("Before <element> After");

    parser.setCursorIndex(4);
    parser.parse(text);

    QVERIFY(!parser.isCursorInsideTag());
    QVERIFY(parser.openElementsAtCursor().isEmpty());

    parser.setCursorIndex(12);

    QVERIFY(!parser.isCursorInsideTag());

    parser.parse(text);

    QVERIFY(parser.isCursorInsideTag());

    parser.setCursorIndex(20);
    parser.parse(text);

    QVERIFY(!parser.isCursorInsideTag());
    QCOMPARE(parser.openElementsAtCursor().count(), 1);
}

QTEST_MAIN(SemanticMarkupParserTest)

#include "SemanticMarkupParserTest.moc"
