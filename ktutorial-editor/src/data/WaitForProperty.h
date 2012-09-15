/***************************************************************************
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

#ifndef WAITFORPROPERTY_H
#define WAITFORPROPERTY_H

#include "WaitFor.h"

/**
 * Container for conditions that wait for a property to have some value data.
 * It stores the data used in KTutorial WaitForProperty, but it has nothing to
 * do with it (they don't even know each other). Its purpose is store the data
 * needed to generate the code to initialize a true KTutorial::WaitForProperty
 * object.
 *
 * When any attribute is modified, dataChanged(WaitFor*) signal is emitted.
 */
class WaitForProperty: public WaitFor {
Q_OBJECT
public:

    /**
     * Creates a new WaitForProperty.
     *
     * @param parent The parent QObject.
     */
    WaitForProperty(QObject* parent = 0);

    virtual WaitFor* clone() const;
    virtual bool equals(const WaitFor& waitFor) const;

    QString objectName() const;
    void setObjectName(const QString& objectName);

    QString propertyName() const;
    void setPropertyName(const QString& propertyName);

    QString value() const;
    void setValue(const QString& value);

private:

    QString mObjectName;
    QString mPropertyName;
    QString mValue;

};

#endif
