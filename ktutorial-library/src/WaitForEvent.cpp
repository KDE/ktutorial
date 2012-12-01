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

#include "WaitForEvent.h"
#include "WaitForEvent_p.h"

#include <QMetaEnum>

#include <KDebug>

namespace ktutorial {
extern int debugArea();
}

namespace ktutorial {

//public:

WaitForEvent::WaitForEvent():
    d(new WaitForEventPrivate()) {
    d->mObject = 0;
    d->mEventType = QEvent::None;
    d->mConditionMet = false;
}

WaitForEvent::WaitForEvent(QObject* object, QEvent::Type type):
    d(new WaitForEventPrivate()) {
    d->mObject = object;
    d->mEventType = type;
    d->mConditionMet = false;

    if (!object) {
        kWarning(debugArea()) << "The object that receives the event to wait"
                              << "for is null!";
        return;
    }

    object->installEventFilter(this);
}

WaitForEvent::~WaitForEvent() {
    delete d;
}

void WaitForEvent::setEvent(QObject* object, const QString& typeName) {
    if (!object) {
        kWarning(debugArea()) << "The object that receives the event to wait"
                              << "for is null!";
        return;
    }

    int index = QEvent::staticMetaObject.indexOfEnumerator("Type");
    QMetaEnum eventTypeEnumerator = QEvent::staticMetaObject.enumerator(index);

    int eventTypeValue = eventTypeEnumerator.keyToValue(qPrintable(typeName));
    if (eventTypeValue == -1) {
        kWarning(debugArea()) << "QEvent::Type named" << typeName << "is"
                              << "unknown";
        return;
    }

    d->mObject = object;
    d->mEventType = static_cast<QEvent::Type>(eventTypeValue);

    d->mObject->installEventFilter(this);
}

bool WaitForEvent::eventFilter(QObject* object, QEvent* event) {
    if (!isActive()) {
        return false;
    }

    if (object == d->mObject && event->type() == d->mEventType) {
        handleEvent(event);
    }

    return false;
}

bool WaitForEvent::conditionMet() const {
    return d->mConditionMet;
}

void WaitForEvent::setActive(bool active) {
    WaitFor::setActive(active);

    if (active) {
        d->mConditionMet = false;
    }
}

//protected:

void WaitForEvent::handleEvent(QEvent* event) {
    Q_UNUSED(event);

    d->mConditionMet = true;
    emit waitEnded(this);
}

}
