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

#ifndef TUTORIAL_H
#define TUTORIAL_H

#include <QList>
#include <QObject>

class Step;

/**
 * Container for tutorial data.
 * It stores the data used in KTutorial tutorials, but it has nothing to do with
 * it (they don't even know each other). Its purpose is store the data needed to
 * generate the code to create a true KTutorial::Tutorial.
 *
 * When any attribute is modified, dataChanged(Tutorial*) signal is emitted.
 * When steps are added or removed, stepAdded(Step*, int) and stepRemoved(Step*)
 * are emitted.
 */
class Tutorial: public QObject {
Q_OBJECT
public:

    explicit Tutorial(QObject* parent = 0);

    /**
     * Destroys this Tutorial and all its steps.
     */
    virtual ~Tutorial();

    /**
     * Returns the id of this Tutorial.
     * The id is a lowerCamelCase version of the name.
     *
     * @return The id of this Tutorial.
     */
    QString id() const;

    QString name() const;
    void setName(const QString& name);

    QString description() const;
    void setDescription(const QString& description);

    QString licenseText() const;
    void setLicenseText(const QString& licenseText);

    QString customSetupCode() const;
    void setCustomSetupCode(const QString& code);

    QString customTearDownCode() const;
    void setCustomTearDownCode(const QString& code);

    /**
     * Adds a new step to this Tutorial.
     * The Tutorial gets ownership of the Step, so it is deleted when the
     * Tutorial is deleted.
     *
     * @param step The step to add.
     */
    void addStep(Step* step);

    /**
     * Adds a new step to this Tutorial at the given position.
     * The Tutorial gets ownership of the Step, so it is deleted when the
     * Tutorial is deleted.
     *
     * @param step The step to add.
     * @param index The index to add the step at.
     */
    void addStep(Step* step, int index);

    QList<Step*> steps() const;

    /**
     * Removes a step from this Tutorial.
     * The Step must be deleted explicitly.
     *
     * @param step The step to remove.
     */
    void removeStep(Step* step);

Q_SIGNALS:

    /**
     * Emitted when any data in the tutorial changed.
     *
     * @param tutorial This tutorial.
     */
    void dataChanged(Tutorial* tutorial);

    /**
     * Emitted when the step is added to the tutorial.
     *
     * @param step The step added.
     * @param index The index where the step was added.
     */
    void stepAdded(Step* step, int index);

    /**
     * Emitted when the step is removed from the tutorial.
     *
     * @param step The step removed.
     */
    void stepRemoved(Step* step);

private:

    QString mName;
    QString mDescription;
    QString mLicenseText;
    QString mCustomSetupCode;
    QString mCustomTearDownCode;
    QList<Step*> mSteps;

    /**
     * Returns the lowerCamelCase version of the given text.
     *
     * @param text The string to get its lowerCamelCase version.
     * @return The lowerCamelCase version of the text.
     */
    QString toLowerCamelCase(const QString& text) const;

};

#endif
