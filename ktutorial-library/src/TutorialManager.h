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

#ifndef KTUTORIAL_TUTORIALMANAGER_H
#define KTUTORIAL_TUTORIALMANAGER_H

#include <QtCore/QMap>
#include <QtCore/QObject>
#include <QtCore/QString>

#include "ktutorial_export.h"

namespace ktutorial {
class Tutorial;
class TutorialInformation;
}

namespace ktutorial {

/**
 * Manager for tutorials.
 * A TutorialManager contains several tutorials and is responsible for start the
 * desired tutorial. Also, it emits a finished() signal when the Tutorial ends.
 *
 * There is only one TutorialManager for each application, created and used
 * internally in KTutorial class. To register a Tutorial, use
 * KTutorial::register(Tutorial*).
 */
class KTUTORIAL_EXPORT TutorialManager: public QObject {
Q_OBJECT
public:

    /**
     * Creates a new TutorialManager.
     */
    TutorialManager();

    /**
     * Destroys this TutorialManager.
     */
    virtual ~TutorialManager();

    /**
     * Registers a Tutorial with this TutorialManager.
     * Only Tutorials with a identifier not added yet can be added. If the
     * Tutorial couldn't be added, false is returned.
     *
     * The Tutorial is reparented to this TutorialManager, and thus deleted when
     * this manager is deleted.
     *
     * @param tutorial The tutorial to register.
     * @return True if the tutorial was registered, false otherwise.
     */
    bool registerTutorial(Tutorial* tutorial);

    /**
     * Returns a list with the information of all the registered tutorials.
     *
     * @return A list with the information of all the registered tutorials.
     */
    QList<const TutorialInformation*> tutorialInformations() const;

    /**
     * Starts a tutorial identified by its id.
     * When the tutorial finishes, the signal finished() is emitted. It is also
     * emitted if there is no tutorial with the specified id. In this case, no
     * tutorial is started.
     *
     * If a KTutorial customization is used, this method must be called when the
     * tutorial has to be started.
     *
     * @param id The id of the tutorial.
     */
    void start(const QString& id);

Q_SIGNALS:

    /**
     * This signal is emitted when the given tutorial is about to be started.
     */
    void started(Tutorial* tutorial);

    /**
     * This signal is emitted when the started tutorial finishes, or if no
     * tutorial could be started.
     */
    void finished();

private:

    class TutorialManagerPrivate* d;

private Q_SLOTS:

    /**
     * Finishes a tutorial.
     * Disconnects this TutorialManager from finished signal of the current
     * Tutorial. It also emits the finished signal of TutorialManager.
     */
    void finish();

};

}

#endif
