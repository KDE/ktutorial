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

#ifndef REACTION_H
#define REACTION_H

#include <QObject>

class WaitFor;

/**
 * Container for reaction data.
 * It stores the data necessary to call addWaitFor and addOption methods in a
 * KTutorial::Step. That is, it stores the data that defines how a step reacts.
 *
 * A reaction is composed by a trigger and a response. The trigger can be the
 * user selecting an option of the step, or a condition to wait for that is met.
 * The response can be changing to another step, or custom code for complexer
 * behavior.
 *
 * When any attribute is modified, dataChanged(Step*) signal is emitted.
 */
class Reaction: public QObject {
Q_OBJECT
public:

    enum TriggerType {
        OptionSelected,
        ConditionMet
    };

    enum ResponseType {
        NextStep,
        CustomCode
    };

    Reaction(QObject* parent = 0);
    virtual ~Reaction();

    TriggerType triggerType() const;
    void setTriggerType(TriggerType triggerType);

    QString optionName() const;
    void setOptionName(const QString& optionName);

    WaitFor* waitFor() const;

    /**
     * Sets the condition to wait for.
     * The WaitFor will be destroyed when this Reaction is destroyed. However,
     * if a WaitFor is set over a previous one, the previous one must be deleted
     * explicitly.
     *
     * @param waitFor The condition to wait for.
     */
    void setWaitFor(WaitFor* waitFor);

    ResponseType responseType() const;
    void setResponseType(ResponseType responseType);

    QString nextStepId() const;
    void setNextStepId(const QString& nextStepId);

    QString customCode() const;
    void setCustomCode(const QString& customCode);

Q_SIGNALS:

    /**
     * Emitted when any data in the reaction changed.
     *
     * @param reaction This reaction.
     */
    void dataChanged(Reaction* reaction);

private:

    TriggerType mTriggerType;
    QString mOptionName;
    WaitFor* mWaitFor;
    ResponseType mResponseType;
    QString mNextStepId;
    QString mCustomCode;

};

#endif
