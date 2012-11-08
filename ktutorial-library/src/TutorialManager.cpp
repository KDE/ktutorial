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

#include "TutorialManager.h"

#include <kdebug.h>

#include "Tutorial.h"
#include "TutorialInformation.h"

namespace ktutorial {

//public:

TutorialManager::TutorialManager(): QObject() {
}

TutorialManager::~TutorialManager() {
}

bool TutorialManager::registerTutorial(Tutorial* tutorial) {
    if (mTutorialInformations.contains(tutorial->tutorialInformation()->id())) {
        kWarning() << "Tutorial with id " 
                   << tutorial->tutorialInformation()->id() << " already added";
        return false;
    }

    tutorial->setParent(this);

    mTutorialInformations.insert(tutorial->tutorialInformation()->id(),
                                 tutorial->tutorialInformation());
    mTutorials.insert(tutorial->tutorialInformation(), tutorial);

    return true;
}

QList<const TutorialInformation*> TutorialManager::tutorialInformations()
                                                                        const {
    return mTutorialInformations.values();
}

void TutorialManager::start(const QString& id) {
    if (!mTutorialInformations.contains(id)) {
        finish();
        return;
    }

    //TODO remove
    kDebug() << "Started: " << id;

    Tutorial* tutorial = mTutorials.value(mTutorialInformations.value(id));
    connect(tutorial, SIGNAL(finished(Tutorial*)), this, SLOT(finish()));

    emit started(tutorial);

    tutorial->start();
}

//private slots:

void TutorialManager::finish() {
    disconnect(this, SLOT(finish()));
    emit finished();
}

}
