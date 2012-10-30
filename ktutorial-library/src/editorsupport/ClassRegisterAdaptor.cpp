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

#include "ClassRegisterAdaptor.h"

#include <QMetaClassInfo>

#include "ObjectRegister.h"

namespace ktutorial {
namespace editorsupport {

//public:

ClassRegisterAdaptor::ClassRegisterAdaptor(ObjectRegister* objectRegister):
        QDBusAbstractAdaptor(objectRegister),
    mObjectRegister(objectRegister) {
}

//public slots:

QString ClassRegisterAdaptor::superClass(const QString& className) const {
    const QMetaObject* metaObject =
                            mObjectRegister->metaObjectForClassName(className);
    if (!metaObject) {
        return "";
    }

    if (!metaObject->superClass()) {
        return "";
    }

    return metaObject->superClass()->className();
}

QStringList ClassRegisterAdaptor::propertyList(const QString& className) const {
    const QMetaObject* metaObject =
                            mObjectRegister->metaObjectForClassName(className);
    if (!metaObject) {
        return QStringList();
    }

    QStringList propertyList;
    for (int i=0; i<metaObject->propertyCount(); ++i) {
        QMetaProperty property = metaObject->property(i);
        if (isPropertyDefinedInClass(property, metaObject)) {
            propertyList.append(property.name());
        }
    }

    return propertyList;
}

QStringList ClassRegisterAdaptor::signalList(const QString& className) const {
    const QMetaObject* metaObject =
                            mObjectRegister->metaObjectForClassName(className);
    if (!metaObject) {
        return QStringList();
    }

    QStringList signalList;
    for (int i=0; i<metaObject->methodCount(); ++i) {
        QMetaMethod method = metaObject->method(i);
        if (isSignalDefinedInClass(method, metaObject)) {
            signalList.append(method.signature());
        }
    }

    return signalList;
}

//private:

bool ClassRegisterAdaptor::isPropertyDefinedInClass(
                                        const QMetaProperty& metaProperty,
                                        const QMetaObject* metaObject) const {
    const QMetaObject* superClass = metaObject;
    while ((superClass = superClass->superClass())) {
        if (superClass->indexOfProperty(metaProperty.name()) != -1) {
            return false;
        }
    }

    return true;
}

bool ClassRegisterAdaptor::isSignalDefinedInClass(const QMetaMethod& metaMethod,
                                          const QMetaObject* metaObject) const {
    if (metaMethod.methodType() != QMetaMethod::Signal) {
        return false;
    }

    const QMetaObject* superClass = metaObject;
    while ((superClass = superClass->superClass())) {
        if (superClass->indexOfSignal(metaMethod.signature()) != -1) {
            return false;
        }
    }

    return true;
}

}
}
