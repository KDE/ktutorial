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

#include "Step.h"
#include "Reaction.h"

//public:

Step::Step(QObject* parent): QObject(parent) {
}

Step::~Step() {
    qDeleteAll(mReactions);
}

QString Step::id() const {
    return mId;
}

void Step::setId(const QString& id) {
    mId = id;

    emit dataChanged(this);
}

QString Step::text() const {
    return mText;
}

void Step::setText(const QString& text) {
    mText = text;

    emit dataChanged(this);
}

QString Step::customSetupCode() const {
    return mCustomSetupCode;
}

void Step::setCustomSetupCode(const QString& code) {
    mCustomSetupCode = code;

    emit dataChanged(this);
}

QString Step::customTearDownCode() const {
    return mCustomTearDownCode;
}

void Step::setCustomTearDownCode(const QString& code) {
    mCustomTearDownCode = code;

    emit dataChanged(this);
}

void Step::addReaction(Reaction* reaction) {
    addReaction(reaction, mReactions.count());
}

void Step::addReaction(Reaction* reaction, int index) {
    Q_ASSERT(!mReactions.contains(reaction));

    mReactions.insert(index, reaction);

    emit reactionAdded(reaction, index);
}

QList<Reaction*> Step::reactions() const {
    return mReactions;
}

void Step::removeReaction(Reaction* reaction) {
    Q_ASSERT(mReactions.contains(reaction));

    mReactions.removeOne(reaction);

    emit reactionRemoved(reaction);
}
