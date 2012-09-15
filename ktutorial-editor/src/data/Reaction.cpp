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

#include "Reaction.h"
#include "WaitFor.h"

//public:

Reaction::Reaction(QObject* parent):
        QObject(parent),
    mTriggerType(OptionSelected),
    mWaitFor(0),
    mResponseType(NextStep) {
}

Reaction::~Reaction() {
    delete mWaitFor;
}

Reaction::TriggerType Reaction::triggerType() const {
    return mTriggerType;
}

void Reaction::setTriggerType(TriggerType triggerType) {
    mTriggerType = triggerType;

    emit dataChanged(this);
}

QString Reaction::optionName() const {
    return mOptionName;
}

void Reaction::setOptionName(const QString& optionName) {
    mOptionName = optionName;

    emit dataChanged(this);
}

WaitFor* Reaction::waitFor() const {
    return mWaitFor;
}

void Reaction::setWaitFor(WaitFor* waitFor) {
    mWaitFor = waitFor;

    emit dataChanged(this);
}

Reaction::ResponseType Reaction::responseType() const {
    return mResponseType;
}

void Reaction::setResponseType(ResponseType responseType) {
    mResponseType = responseType;

    emit dataChanged(this);
}

QString Reaction::nextStepId() const {
    return mNextStepId;
}

void Reaction::setNextStepId(const QString& nextStepId) {
    mNextStepId = nextStepId;

    emit dataChanged(this);
}

QString Reaction::customCode() const {
    return mCustomCode;
}

void Reaction::setCustomCode(const QString& customCode) {
    mCustomCode = customCode;

    emit dataChanged(this);
}
