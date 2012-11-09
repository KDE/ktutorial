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

#ifndef STEP_H
#define STEP_H

#include <QObject>

class Reaction;

/**
 * Container for step data.
 * It stores the data used in KTutorial steps, but it has nothing to do with
 * them (they don't even know each other). Its purpose is store the data needed to
 * generate the code to create a true KTutorial::Step.
 *
 * When any attribute is modified, dataChanged(Step*) signal is emitted. When
 * reactions are added or removed, reactionAdded(Reaction*, int) and
 * reactionRemoved(Reaction*) are emitted.
 */
class Step: public QObject {
Q_OBJECT
public:

    explicit Step(QObject* parent = 0);

    /**
     * Destroys this Step and all its reactions.
     */
    virtual ~Step();

    QString id() const;
    void setId(const QString& id);

    QString text() const;
    void setText(const QString& text);

    QString customSetupCode() const;
    void setCustomSetupCode(const QString& code);

    QString customTearDownCode() const;
    void setCustomTearDownCode(const QString& code);

    /**
     * Adds a new reaction to this Step.
     * The Step gets ownership of the Reaction, so it is deleted when the
     * Step is deleted.
     *
     * @param reaction The reaction to add.
     */
    void addReaction(Reaction* reaction);

    /**
     * Adds a new reaction to this Step at the given position.
     * The Step gets ownership of the Reaction, so it is deleted when the
     * Step is deleted.
     *
     * @param reaction The reaction to add.
     * @param index The index to add the step at.
     */
    void addReaction(Reaction* reaction, int index);

    QList<Reaction*> reactions() const;

    /**
     * Removes a reaction from this Step.
     * The Reaction must be deleted explicitly.
     *
     * @param reaction The reaction to remove.
     */
    void removeReaction(Reaction* reaction);

Q_SIGNALS:

    /**
     * Emitted when any data in the step changed.
     *
     * @param step This step.
     */
    void dataChanged(Step* step);

    /**
     * Emitted when the reaction is added to this Step.
     *
     * @param reaction The reaction added.
     * @param index The index where the reaction was added.
     */
    void reactionAdded(Reaction* reaction, int index);

    /**
     * Emitted when the reaction is removed from this Step.
     *
     * @param reaction The reaction removed.
     */
    void reactionRemoved(Reaction* reaction);

private:

    QString mId;
    QString mText;
    QString mCustomSetupCode;
    QString mCustomTearDownCode;
    QList<Reaction*> mReactions;

};

#endif
