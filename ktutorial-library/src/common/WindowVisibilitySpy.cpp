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

#include <QEvent>
#include <QWidget>

#include "WindowVisibilitySpy.h"

namespace ktutorial {
namespace common {

//public:

WindowVisibilitySpy::WindowVisibilitySpy(QObject* parent /*= 0*/):
        QObject(parent) {
}

void WindowVisibilitySpy::addWidgetToSpy(QWidget* widget) {
    Q_ASSERT(widget);

    widget->installEventFilter(this);

    foreach (QObject* childObject, widget->children()) {
        if (childObject->isWidgetType()) {
            addWidgetToSpy(static_cast<QWidget*>(childObject));
        }
    }
}

//protected:

bool WindowVisibilitySpy::eventFilter(QObject* object, QEvent* event) {
    if (event->type() == QEvent::ChildAdded) {
        QChildEvent* childEvent = static_cast<QChildEvent*>(event);
        if (childEvent->child()->isWidgetType()) {
            addWidgetToSpy(static_cast<QWidget*>(childEvent->child()));
        }
        return false;
    }

    QWidget* widget = static_cast<QWidget*>(object);

    if (!(widget->windowFlags() & (Qt::Window | Qt::Dialog))) {
        return false;
    }

    if (event->type() == QEvent::Show) {
        emit windowShown(widget);
    } else if (event->type() == QEvent::Hide) {
        emit windowHidden(widget);
    }

    return false;
}

}
}

#include "WindowVisibilitySpy.moc"
