/***************************************************************************
 *   Copyright (C) 2010 by Daniel Calviño Sánchez <danxuliu@gmail.com>     *
 *   Copyright (C) 2011 by Daniel Calviño Sánchez <danxuliu@gmail.com>     *
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

#include "ObjectRegisterAdaptor.h"
#include "ObjectRegister.h"

namespace ktutorial {
namespace editorsupport {

//public:

ObjectRegisterAdaptor::ObjectRegisterAdaptor(ObjectRegister* objectRegister):
        QDBusAbstractAdaptor(objectRegister),
    mObjectRegister(objectRegister) {
}

//public slots:

QString ObjectRegisterAdaptor::objectName(int objectId) const {
    QObject* object = mObjectRegister->objectForId(objectId);
    if (!object) {
        return "";
    }

    return object->objectName();
}

QString ObjectRegisterAdaptor::className(int objectId) const {
    QObject* object = mObjectRegister->objectForId(objectId);
    if (!object) {
        return "";
    }

    //Ensure that the meta object is registered. If a widget is registered while
    //it is being constructed (if the event spy is enabled, it will register the
    //object when it sets its parent, as it sends a ParentChanged event) its
    //real class will not be registered, only QWidget, as at that moment the
    //object will not be fully constructed and the pointer to the meta object
    //will not be set to the real class yet.
    //This does not seem to happen with plain objects; only widgets send that
    //ParentChanged event in their constructor.
    mObjectRegister->registerMetaObject(object->metaObject());

    return object->metaObject()->className();
}

QList<int> ObjectRegisterAdaptor::childObjectIds(int objectId) const {
    QObject* object = mObjectRegister->objectForId(objectId);
    if (!object) {
        return QList<int>();
    }

    QList<int> ids;
    foreach (QObject* childObject, object->children()) {
        ids.append(mObjectRegister->idForObject(childObject));
    }

    return ids;
}

void ObjectRegisterAdaptor::clear() {
    mObjectRegister->clear();
}

}
}
