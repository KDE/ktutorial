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

#include "EventSpy.h"

#include <QEvent>

namespace ktutorial {
namespace editorsupport {

//public:

EventSpy::EventSpy(QObject* parent /*= 0*/): QObject(parent) {
}

void EventSpy::addObjectToSpy(QObject* object) {
    object->installEventFilter(this);

    foreach (QObject* child, object->children()) {
        addObjectToSpy(child);
    }
}

//protected:

bool EventSpy::eventFilter(QObject* object, QEvent* event) {
    emit eventReceived(object, event);

    if (event->type() == QEvent::ChildAdded) {
        addObjectToSpy(static_cast<QChildEvent*>(event)->child());
    }

    return false;
}

}
}
