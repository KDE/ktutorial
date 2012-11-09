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

#include <kdebug.h>

#include "Option.h"
#include "WaitFor.h"
#include "WaitForSignal.h"

namespace ktutorial {

//public:

Step::Step(const QString& id): QObject(),
    mId(id),
    mActive(false),
    mDeleteAddedObjectsInTearDown(false) {
}

QString Step::id() const {
    return mId;
}

QList<Option*> Step::options() const {
    return mOptions;
}

QString Step::text() const {
    return mText;
}

void Step::setText(const QString& text) {
    mText = text;
}

bool Step::isActive() const {
    return mActive;
}

void Step::setActive(bool active) {
    if (active) {
        setupWrapper();
    } else {
        tearDownWrapper();
    }

    mActive = active;

    QListIterator<WaitFor*> it(mWaitsFor);
    while (it.hasNext()) {
        it.next()->setActive(active);
    }
}

void Step::addOption(Option* option, QObject* receiver, const QString& slot) {
    if (!addOption(option)) {
        return;
    }

    bool deleteAddedObjectsInTearDownValue = mDeleteAddedObjectsInTearDown;
    mDeleteAddedObjectsInTearDown = false;

    WaitForSignal* waitFor = new WaitForSignal(option, SIGNAL(selected()));
    addWaitFor(waitFor, receiver, slot);

    mOptionsWaitsFor.append(waitFor);

    mDeleteAddedObjectsInTearDown = deleteAddedObjectsInTearDownValue;
}

void Step::addOption(Option* option, const QString& nextStepId) {
    if (!addOption(option)) {
        return;
    }

    bool deleteAddedObjectsInTearDownValue = mDeleteAddedObjectsInTearDown;
    mDeleteAddedObjectsInTearDown = false;

    WaitForSignal* waitFor = new WaitForSignal(option, SIGNAL(selected()));
    addWaitFor(waitFor, nextStepId);

    mOptionsWaitsFor.append(waitFor);

    mDeleteAddedObjectsInTearDown = deleteAddedObjectsInTearDownValue;
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

    mNextStepForWaitFor.insert(waitFor, nextStepId);

    connect(waitFor, SIGNAL(waitEnded(WaitFor*)),
            this, SLOT(requestNextStepForWaitFor(WaitFor*)));
}

void Step::removeOption(Option* option) {
    if (!mOptions.contains(option)) {
        kWarning() << "Tried to remove an Option not added in step " << mId;
        return;
    }

    option->setParent(0);

    int index = mOptions.indexOf(option);
    mOptions.removeAt(index);

    WaitFor* waitFor = mOptionsWaitsFor[index];
    removeWaitFor(waitFor);

    mOptionsWaitsFor.removeAt(index);
    delete waitFor;
}

void Step::removeWaitFor(WaitFor* waitFor) {
    if (!mWaitsFor.contains(waitFor)) {
        kWarning() << "Tried to remove a WaitFor not added in step " << mId;
        return;
    }

    waitFor->setActive(false);

    waitFor->setParent(0);

    mWaitsFor.removeAt(mWaitsFor.indexOf(waitFor));
    disconnectWaitFor(waitFor);

    if (mNextStepForWaitFor.contains(waitFor)) {
        mNextStepForWaitFor.remove(waitFor);
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
    mDeleteAddedObjectsInTearDown = true;
    setup();
    mDeleteAddedObjectsInTearDown = false;
}

void Step::tearDownWrapper() {
    tearDown();

    foreach (Option* option, mOptionsToBeDeletedInTearDown) {
        removeOption(option);
        delete option;
    }
    mOptionsToBeDeletedInTearDown.clear();

    foreach (WaitFor* waitFor, mWaitsForToBeDeletedInTearDown) {
        removeWaitFor(waitFor);
        delete waitFor;
    }
    mWaitsForToBeDeletedInTearDown.clear();
}

bool Step::addOption(Option* option) {
    if (mOptions.contains(option)) {
        kWarning() << "Option " << option->name() << " already added in step " << mId;
        return false;
    }

    QListIterator<Option*> it(mOptions);
    while (it.hasNext()) {
        if (it.next()->name() == option->name()) {
            kWarning() << "Option named " << option->name() << " already added in step " << mId;
            return false;
        }
    }

    option->setParent(this);

    mOptions.append(option);

    if (mDeleteAddedObjectsInTearDown) {
        mOptionsToBeDeletedInTearDown.append(option);
    }

    return true;
}

bool Step::addWaitFor(WaitFor* waitFor) {
    if (mWaitsFor.contains(waitFor)) {
        kWarning() << "Same WaitFor already added in step " << mId;
        return false;
    }

    waitFor->setActive(false);

    waitFor->setParent(this);

    mWaitsFor.append(waitFor);

    if (mDeleteAddedObjectsInTearDown) {
        mWaitsForToBeDeletedInTearDown.append(waitFor);
    }

    return true;
}

//private slots:

void Step::requestNextStepForWaitFor(WaitFor* waitFor) {
    emit nextStepRequested(mNextStepForWaitFor.value(waitFor));
}

}

#include "Step.moc"
