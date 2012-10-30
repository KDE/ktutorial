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

#include "EventSpyAdaptor.h"

#include <QEvent>
#include <QMetaEnum>

#include "EventSpy.h"
#include "ObjectRegister.h"

namespace ktutorial {
namespace editorsupport {

//public:

EventSpyAdaptor::EventSpyAdaptor(EventSpy* eventSpy,
                                 ObjectRegister* objectRegister):
        QDBusAbstractAdaptor(eventSpy),
    mObjectRegister(objectRegister) {
    connect(eventSpy, SIGNAL(eventReceived(QObject*,QEvent*)),
            this, SLOT(handleEventReceived(QObject*,QEvent*)));
}

//private:

void EventSpyAdaptor::handleEventReceived(QObject* object, QEvent* event) {
    int id = mObjectRegister->idForObject(object);

    int index = QEvent::staticMetaObject.indexOfEnumerator("Type");
    QMetaEnum eventTypeEnumerator = QEvent::staticMetaObject.enumerator(index);
    QString eventType = eventTypeEnumerator.valueToKey(event->type());

    emit eventReceived(id, eventType);
}

}
}
