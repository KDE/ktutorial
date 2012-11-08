/***************************************************************************
 *   Copyright (C) 2008 by Daniel Calviño Sánchez <danxuliu@gmail.com>     *
 *   Copyright (C) 2009 by Daniel Calviño Sánchez <danxuliu@gmail.com>     *
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

#ifndef KTUTORIAL_TUTORIALINFORMATION_H
#define KTUTORIAL_TUTORIALINFORMATION_H

#include <QtCore/QObject>

#include "ktutorial_export.h"

namespace ktutorial {

/**
 * Information about a Tutorial.
 * Contains the identifier, name and description of a Tutorial.
 *
 * The identifier must be unique. Also, it must be unstranslated, so it is the
 * same no matter what languague is being used. For example, "moveToolbar".
 *
 * The name is the human readable and localized for each language name of the
 * Tutorial. For example, "How to move the toolbar".
 */
class KTUTORIAL_EXPORT TutorialInformation: public QObject {
Q_OBJECT
public:

    /**
     * Creates a new TutorialInformation with the specified identifier.
     *
     * @param id The unique, untranslated identifier of the tutorial.
     */
    explicit TutorialInformation(const QString& id): QObject(),
        mId(id) {
    }

    /**
     * Returns the identifier of the tutorial.
     *
     * @return The identifier of the tutorial.
     */
    const QString& id() const {
        return mId;
    }

    /**
     * Returns the name of the tutorial.
     *
     * @return The name of the tutorial.
     */
    const QString& name() const {
        return mName;
    }

    /**
     * Returns the description of the tutorial.
     *
     * @return The description of the tutorial.
     */
    const QString& description() const {
        return mDescription;
    }

    /**
     * Sets the name of the tutorial.
     * This method can be invoked from a script.
     *
     * @param name The name to set.
     */
    Q_INVOKABLE void setName(const QString& name) {
        mName = name;
    }

    /**
     * Sets the description of the tutorial.
     * This method can be invoked from a script.
     *
     * @param description The description to set.
     */
    Q_INVOKABLE void setDescription(const QString& description) {
        mDescription = description;
    }

private:

    /**
     * The unique, unstranslated identifier of the Tutorial.
     */
    QString mId;

    /**
     * The human readable and localized name of the Tutorial.
     */
    QString mName;

    /**
     * The description of the Tutorial.
     */
    QString mDescription;

};

}

#endif
