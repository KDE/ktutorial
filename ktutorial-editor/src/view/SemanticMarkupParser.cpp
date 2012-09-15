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

#include "SemanticMarkupParser.h"

#include <QRegExp>
#include <QStringList>

//public:

SemanticMarkupParser::SemanticMarkupParser():
    mCursorIndex(-1),
    mCursorInsideTag(false) {
}

void SemanticMarkupParser::setCursorIndex(int cursorIndex) {
    mCursorIndex = cursorIndex;
}

void SemanticMarkupParser::parse(const QString& text) {
    mCursorInsideTag = false;
    mOpenElements.clear();
    mOpenElementsAtCursor.clear();
    mNotClosedChildElements.clear();

    QRegExp startTagRegExp("<\\s*(\\w+)\\s*((\\w+=\"[^\"]*\"\\s*)*)>");
    QRegExp endTagRegExp("</\\s*(\\w+)\\s*>");
    QRegExp emptyTagRegExp("<\\s*(\\w+)\\s*((\\w+=\"[^\"]*\"\\s*)*)/>");
    QRegExp tagRegExp(startTagRegExp.pattern() + '|' +
                      endTagRegExp.pattern() + '|' +
                      emptyTagRegExp.pattern());

    bool parsingReachedCursor = false;

    int parsingIndex = tagRegExp.indexIn(text);
    while (parsingIndex >= 0) {
        QString tag = tagRegExp.capturedTexts().at(0);

        if (mCursorIndex > parsingIndex &&
            mCursorIndex < parsingIndex + tagRegExp.matchedLength()) {
            mCursorInsideTag = true;
            mOpenElementsAtCursor.clear();
            return;
        }

        if (!parsingReachedCursor && parsingIndex >= mCursorIndex) {
            parsingReachedCursor = true;
            mOpenElementsAtCursor = mOpenElements;
        }

        if (startTagRegExp.exactMatch(tag)) {
            StartTag startTag;
            startTag.mName = startTagRegExp.capturedTexts().at(1);
            startTag.mAttributes = startTagRegExp.capturedTexts().at(2);
            startTag.mIndex = parsingIndex;
            mOpenElements.insert(0, startTag);
        } else if (endTagRegExp.exactMatch(tag)) {
            QString endTagName = endTagRegExp.capturedTexts().at(1);

            int endElementIndex = indexOf(mOpenElements, endTagName);
            if (endElementIndex >= 0) {
                int index = 0;
                while (index < endElementIndex) {
                    mNotClosedChildElements.append(mOpenElements.first());
                    mOpenElements.removeFirst();
                    index++;
                }
                mOpenElements.removeFirst();
            }
        }

        parsingIndex = parsingIndex + tagRegExp.matchedLength();
        parsingIndex = tagRegExp.indexIn(text, parsingIndex);
    }

    if (!parsingReachedCursor) {
        mOpenElementsAtCursor = mOpenElements;
    }
}

bool SemanticMarkupParser::isElementClosed(const StartTag& startTag) const {
    if (indexOf(mOpenElements, startTag) > -1 ||
            indexOf(mNotClosedChildElements, startTag) > -1) {
        return false;
    }

    return true;
}

bool SemanticMarkupParser::isCursorInsideTag() const {
    return mCursorInsideTag;
}

QList<StartTag> SemanticMarkupParser::openElementsAtCursor() const {
    return mOpenElementsAtCursor;
}

//private:

int SemanticMarkupParser::indexOf(const QList<StartTag>& startTags,
                                  const QString& tagName) const {
    for (int i=0; i<startTags.count(); ++i) {
        if (startTags[i].mName == tagName) {
            return i;
        }
    }

    return -1;
}

int SemanticMarkupParser::indexOf(const QList<StartTag>& startTags,
                                  const StartTag& startTag) const {
    for (int i=0; i<startTags.count(); ++i) {
        if (startTags[i].mName == startTag.mName &&
            startTags[i].mAttributes == startTag.mAttributes &&
            startTags[i].mIndex == startTag.mIndex) {
            return i;
        }
    }

    return -1;
}
