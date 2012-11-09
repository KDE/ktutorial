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

#include <QMetaEnum>

#include <KDebug>

namespace ktutorial {

//public:

WaitForEvent::WaitForEvent():
    mObject(0),
    mEventType(QEvent::None),
    mConditionMet(false) {
}

WaitForEvent::WaitForEvent(QObject* object, QEvent::Type type):
    mObject(object),
    mEventType(type),
    mConditionMet(false) {

    if (!object) {
        kWarning() << "The object that receives the event to wait for is null!";
        return;
    }

    object->installEventFilter(this);
}

void WaitForEvent::setEvent(QObject* object, const QString& typeName) {
    if (!object) {
        kWarning() << "The object that receives the event to wait for is null!";
        return;
    }

    int index = QEvent::staticMetaObject.indexOfEnumerator("Type");
    QMetaEnum eventTypeEnumerator = QEvent::staticMetaObject.enumerator(index);

    int eventTypeValue = eventTypeEnumerator.keyToValue(qPrintable(typeName));
    if (eventTypeValue == -1) {
        kWarning() << "QEvent::Type named \"" << typeName << "\" is unknown";
        return;
    }

    mObject = object;
    mEventType = static_cast<QEvent::Type>(eventTypeValue);

    mObject->installEventFilter(this);
}

bool WaitForEvent::eventFilter(QObject* object, QEvent* event) {
    if (!isActive()) {
        return false;
    }

    if (object == mObject && event->type() == mEventType) {
        handleEvent(event);
    }

    return false;
}

bool WaitForEvent::conditionMet() const {
    return mConditionMet;
}

void WaitForEvent::setActive(bool active) {
    WaitFor::setActive(active);

    if (active) {
        mConditionMet = false;
    }
}

//protected:

void WaitForEvent::handleEvent(QEvent* event) {
    Q_UNUSED(event);

    mConditionMet = true;
    emit waitEnded(this);
}

}
