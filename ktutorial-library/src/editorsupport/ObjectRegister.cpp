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

#include "ObjectRegister.h"

namespace ktutorial {
namespace editorsupport {

//public:

ObjectRegister::ObjectRegister(QObject* parent /*= 0*/): QObject(parent),
    mNextId(1) {
}

int ObjectRegister::idForObject(QObject* object) {
    if (!object) {
        return 0;
    }

    int id = mRegisteredIds.value(object);

    if (!id) {
        id = mNextId;
        mRegisteredObjects.insert(id, object);
        mRegisteredIds.insert(object, id);
        connect(object, SIGNAL(destroyed(QObject*)),
                this, SLOT(deregister(QObject*)));
        mNextId++;

        registerMetaObject(object->metaObject());
    }

    return id;
}

QObject* ObjectRegister::objectForId(int objectId) {
    return mRegisteredObjects.value(objectId);
}

const QMetaObject* ObjectRegister::metaObjectForClassName(
                                            const QString& className) const {
    return mRegisteredMetaObjects.value(className);
}

void ObjectRegister::clear() {
    mRegisteredIds.clear();
    mRegisteredObjects.clear();
    mRegisteredMetaObjects.clear();
}

void ObjectRegister::registerMetaObject(const QMetaObject* metaObject) {
    if (mRegisteredMetaObjects.contains(metaObject->className())) {
        return;
    }

    mRegisteredMetaObjects.insert(metaObject->className(), metaObject);

    if (metaObject->superClass()) {
        registerMetaObject(metaObject->superClass());
    }
}

//private slots:

void ObjectRegister::deregister(QObject* object) {
    int id = mRegisteredIds.value(object);
    mRegisteredIds.remove(object);
    mRegisteredObjects.remove(id);
}

}
}
