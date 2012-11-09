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

#ifndef WAITFOREVENT_H
#define WAITFOREVENT_H

#include "WaitFor.h"

/**
 * Container for conditions that wait for an event to be received data.
 * It stores the data used in KTutorial WaitForEvent, but it has nothing to do
 * with it (they don't even know each other). Its purpose is store the data
 * needed to generate the code to initialize a true KTutorial::WaitForEvent
 * object.
 *
 * When any attribute is modified, dataChanged(WaitFor*) signal is emitted.
 */
class WaitForEvent: public WaitFor {
Q_OBJECT
public:

    /**
     * Creates a new WaitForEvent.
     *
     * @param parent The parent QObject.
     */
    explicit WaitForEvent(QObject* parent = 0);

    virtual WaitFor* clone() const;
    virtual bool equals(const WaitFor& waitFor) const;

    QString receiverName() const;
    void setReceiverName(const QString& receiverName);

    QString eventName() const;
    void setEventName(const QString& eventName);

private:

    QString mReceiverName;
    QString mEventName;

};

#endif
