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

#include "Step.h"
#include "Step_p.h"

#include <KDebug>

#include "Option.h"
#include "WaitFor.h"
#include "WaitForSignal.h"

namespace ktutorial {
extern int debugArea();
}

namespace ktutorial {

//public:

Step::Step(const QString& id): QObject(),
    d(new StepPrivate()) {
    d->mId = id;
    d->mActive = false;
    d->mDeleteAddedObjectsInTearDown = false;
}

Step::~Step() {
    delete d;
}

QString Step::id() const {
    return d->mId;
}

QList<Option*> Step::options() const {
    return d->mOptions;
}

QString Step::text() const {
    return d->mText;
}

void Step::setText(const QString& text) {
    d->mText = text;
}

bool Step::isActive() const {
    return d->mActive;
}

void Step::setActive(bool active) {
    if (active) {
        setupWrapper();
    } else {
        tearDownWrapper();
    }

    d->mActive = active;

    QListIterator<WaitFor*> it(d->mWaitsFor);
    while (it.hasNext()) {
        it.next()->setActive(active);
    }
}

void Step::addOption(Option* option, QObject* receiver, const QString& slot) {
    if (!addOption(option)) {
        return;
    }

    bool deleteAddedObjectsInTearDownValue = d->mDeleteAddedObjectsInTearDown;
    d->mDeleteAddedObjectsInTearDown = false;

    WaitForSignal* waitFor = new WaitForSignal(option, SIGNAL(selected()));
    addWaitFor(waitFor, receiver, slot);

    d->mOptionsWaitsFor.append(waitFor);

    d->mDeleteAddedObjectsInTearDown = deleteAddedObjectsInTearDownValue;
}

void Step::addOption(Option* option, const QString& nextStepId) {
    if (!addOption(option)) {
        return;
    }

    bool deleteAddedObjectsInTearDownValue = d->mDeleteAddedObjectsInTearDown;
    d->mDeleteAddedObjectsInTearDown = false;

    WaitForSignal* waitFor = new WaitForSignal(option, SIGNAL(selected()));
    addWaitFor(waitFor, nextStepId);

    d->mOptionsWaitsFor.append(waitFor);

    d->mDeleteAddedObjectsInTearDown = deleteAddedObjectsInTearDownValue;
}

void Step::addWaitFor(WaitFor* waitFor, QObject* receiver, const QString& slot) {
    if (!addWaitFor(waitFor)) {
        return;
    }

    connectWaitFor(waitFor, receiver, slot);
}

void Step::addWaitFor(WaitFor* waitFor, const QString& nextStepId) {
    if (!addWaitFor(waitFor)) {
        return;
    }

    d->mNextStepForWaitFor.insert(waitFor, nextStepId);

    connect(waitFor, SIGNAL(waitEnded(WaitFor*)),
            this, SLOT(requestNextStepForWaitFor(WaitFor*)));
}

void Step::removeOption(Option* option) {
    if (!d->mOptions.contains(option)) {
        kWarning(debugArea()) << "Tried to remove an Option not added in step"
                              << d->mId;
        return;
    }

    option->setParent(0);

    int index = d->mOptions.indexOf(option);
    d->mOptions.removeAt(index);

    WaitFor* waitFor = d->mOptionsWaitsFor[index];
    removeWaitFor(waitFor);

    d->mOptionsWaitsFor.removeAt(index);
    delete waitFor;
}

void Step::removeWaitFor(WaitFor* waitFor) {
    if (!d->mWaitsFor.contains(waitFor)) {
        kWarning(debugArea()) << "Tried to remove a WaitFor not added in step"
                              << d->mId;
        return;
    }

    waitFor->setActive(false);

    waitFor->setParent(0);

    d->mWaitsFor.removeAt(d->mWaitsFor.indexOf(waitFor));
    disconnectWaitFor(waitFor);

    if (d->mNextStepForWaitFor.contains(waitFor)) {
        d->mNextStepForWaitFor.remove(waitFor);
        disconnect(waitFor, SIGNAL(waitEnded(WaitFor*)),
                   this, SLOT(requestNextStepForWaitFor(WaitFor*)));
    }
}

//protected:

void Step::setup() {
}

void Step::tearDown() {
}

void Step::connectWaitFor(WaitFor* waitFor, QObject* receiver,
                          const QString& slot) {
    QString slotName = slot;
    if (!slotName.startsWith('1')) {
        slotName = QString("1%1").arg(slot);
    }

    connect(waitFor, SIGNAL(waitEnded(WaitFor*)),
            receiver, slotName.toLatin1());
}

void Step::disconnectWaitFor(WaitFor* waitFor) {
    disconnect(waitFor, SIGNAL(waitEnded(WaitFor*)), 0, 0);
}

//private:

void Step::setupWrapper() {
    d->mDeleteAddedObjectsInTearDown = true;
    setup();
    d->mDeleteAddedObjectsInTearDown = false;
}

void Step::tearDownWrapper() {
    tearDown();

    foreach (Option* option, d->mOptionsToBeDeletedInTearDown) {
        removeOption(option);
        delete option;
    }
    d->mOptionsToBeDeletedInTearDown.clear();

    foreach (WaitFor* waitFor, d->mWaitsForToBeDeletedInTearDown) {
        removeWaitFor(waitFor);
        delete waitFor;
    }
    d->mWaitsForToBeDeletedInTearDown.clear();
}

bool Step::addOption(Option* option) {
    if (d->mOptions.contains(option)) {
        kWarning(debugArea()) << "Option" << option->name()
                              << "already added in step" << d->mId;
        return false;
    }

    QListIterator<Option*> it(d->mOptions);
    while (it.hasNext()) {
        if (it.next()->name() == option->name()) {
            kWarning(debugArea()) << "Option named" << option->name()
                                  << "already added in step" << d->mId;
            return false;
        }
    }

    option->setParent(this);

    d->mOptions.append(option);

    if (d->mDeleteAddedObjectsInTearDown) {
        d->mOptionsToBeDeletedInTearDown.append(option);
    }

    return true;
}

bool Step::addWaitFor(WaitFor* waitFor) {
    if (d->mWaitsFor.contains(waitFor)) {
        kWarning(debugArea()) << "Same WaitFor already added in step" << d->mId;
        return false;
    }

    waitFor->setActive(false);

    waitFor->setParent(this);

    d->mWaitsFor.append(waitFor);

    if (d->mDeleteAddedObjectsInTearDown) {
        d->mWaitsForToBeDeletedInTearDown.append(waitFor);
    }

    return true;
}

//private slots:

void Step::requestNextStepForWaitFor(WaitFor* waitFor) {
    emit nextStepRequested(d->mNextStepForWaitFor.value(waitFor));
}

}
