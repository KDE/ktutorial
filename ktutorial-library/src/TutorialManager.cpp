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
#include "TutorialManager_p.h"

#include <KDebug>

#include "Tutorial.h"
#include "TutorialInformation.h"

namespace ktutorial {
extern int debugArea();
}

namespace ktutorial {

//public:

TutorialManager::TutorialManager(): QObject(),
    d(new TutorialManagerPrivate()) {
}

TutorialManager::~TutorialManager() {
    delete d;
}

bool TutorialManager::registerTutorial(Tutorial* tutorial) {
    if (d->mTutorialInformations.contains(
                                    tutorial->tutorialInformation()->id())) {
        kWarning(debugArea()) << "Tutorial with id" 
                              << tutorial->tutorialInformation()->id()
                              << "already added";
        return false;
    }

    tutorial->setParent(this);

    d->mTutorialInformations.insert(tutorial->tutorialInformation()->id(),
                                    tutorial->tutorialInformation());
    d->mTutorials.insert(tutorial->tutorialInformation(), tutorial);

    return true;
}

QList<const TutorialInformation*> TutorialManager::tutorialInformations()
                                                                        const {
    return d->mTutorialInformations.values();
}

void TutorialManager::start(const QString& id) {
    if (!d->mTutorialInformations.contains(id)) {
        finish();
        return;
    }

    kDebug(debugArea()) << "Started:" << id;

    Tutorial* tutorial = d->mTutorials.value(
                                            d->mTutorialInformations.value(id));
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
