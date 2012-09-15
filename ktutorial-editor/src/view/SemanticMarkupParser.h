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

#ifndef SEMANTICMARKUPPARSER_H
#define SEMANTICMARKUPPARSER_H

#include <QList>
#include <QString>

/**
 * Structure to store the data of a XML start tag.
 */
struct StartTag {

    /**
     * The tag name.
     */
    QString mName;

    /**
     * The attributes, if any.
     */
    QString mAttributes;

    /**
     * The index of the tag in the text (the position of the '<' character).
     */
    int mIndex;

    /**
     * Creates a new StartTag.
     */
    StartTag():
        mName(""),
        mAttributes(""),
        mIndex(-1) {
    }

};

/**
 * Simple parser for texts containing KUIT semantic markup.
 * The parser is meant to provide SemanticMarkupEdition with information about
 * the markup in the text. Thus, it is designed just for that purpose; it is not
 * a general purpose parser.
 *
 * The parser supports XML where not all the elements have been closed. For
 * example, it can parse a text with just a start tag, or a text where there
 * is an unpaired start tag inside a valid element (one with paired start and
 * end tags).
 *
 * The parser stores which elements have been opened, and when it reaches an end
 * tag it regardes as closed the newest element that matches the end tag. For
 * example, in &lt;a&gt;&lt;a&gt;&lt;b&gt;&lt;/a&gt;&lt;/b&gt;, when &lt;/a&gt;
 * is reached the second "a" element is regarded as closed. The &lt;/b&gt; will
 * be ignored, as the "b" element is no longer taken into account since its
 * parent element was closed. Note, however, that the "b" element is not closed,
 * and isElementClosed(StartTag) with that "b" element would return false.
 * Passing the first "a" element would also return false; true would be returned
 * only for the second "a" element.
 *
 * Before parsing a text, the cursor index has to be set using
 * setCursorIndex(int). Then the text can be parsed using parse(QString).
 *
 * Once parsed, the SemanticMarkupParser object provides information about the
 * parsed text regarding the cursor position. To update the information provided
 * the text must be parsed again; that is, even if the only change is in the
 * cursor index, parse(QString) has to be called again.
 */
class SemanticMarkupParser {
public:

    /**
     * Creates a new SemanticMarkupParser.
     */
    SemanticMarkupParser();

    /**
     * Sets the index of the cursor.
     *
     * @param cursorIndex The index to set.
     */
    void setCursorIndex(int cursorIndex);

    /**
     * Parses the given text, updating the state of this parser.
     *
     * @param text The text to parse.
     */
    void parse(const QString& text);

    /**
     * Returns whether the cursor is inside a tag or not.
     *
     * @return True if the cursor is inside a tag, false otherwise.
     */
    bool isCursorInsideTag() const;

    /**
     * Returns whether the given start tag was closed or not.
     *
     * @param startTag The tag to check.
     * @return True if the start tag was closed, false otherwise.
     */
    bool isElementClosed(const StartTag& startTag) const;

    /**
     * Returns a list with all the start tags that were open at the cursor
     * position.
     * The order of the tags goes from the deepest one to the more general one.
     * That is, for &lt;a&gt;&lt;b&gt;, the first tag is "b" and the second one
     * is "a".
     *
     * @return The start tags open at the cursor.
     */
    QList<StartTag> openElementsAtCursor() const;

private:

    /**
     * The position of the cursor in the text.
     */
    int mCursorIndex;

    /**
     * Whether the cursor was inside a tag or not in the last parsed text.
     */
    bool mCursorInsideTag;

    /**
     * A list with all the open elements at the end of the last parsed text.
     */
    QList<StartTag> mOpenElements;

    /**
     * A list with all the open elements at the cursor of the last parsed text.
     */
    QList<StartTag> mOpenElementsAtCursor;

    /**
     * A list with all the elements that were not closed but some ancestor
     * element was at the end of the last parsed text.
     */
    QList<StartTag> mNotClosedChildElements;

    /**
     * The position in the given list of a tag with the given name.
     *
     * @param startTags The list to check.
     * @param tagName The name of the tag to find.
     * @return The position of the tag name, or -1 if it is not found.
     */
    int indexOf(const QList<StartTag>& startTags, const QString& tagName) const;

    /**
     * The position in the given list of the given tag.
     *
     * @param startTags The list to check.
     * @param startTag The tag to find.
     * @return The position of the tag, or -1 if it is not found.
     */
    int indexOf(const QList<StartTag>& startTags,
                const StartTag& startTag) const;

};

#endif
