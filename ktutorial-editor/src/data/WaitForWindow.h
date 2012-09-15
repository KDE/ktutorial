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

#ifndef WAITFORWINDOW_H
#define WAITFORWINDOW_H

#include "WaitFor.h"

/**
 * Container for conditions that wait for a window to be shown data.
 * It stores the data used in KTutorial WaitForWindow, but it has nothing to do
 * with it (they don't even know each other). Its purpose is store the data
 * needed to generate the code to initialize a true KTutorial::WaitForWindow
 * object.
 *
 * When any attribute is modified, dataChanged(WaitFor*) signal is emitted.
 */
class WaitForWindow: public WaitFor {
Q_OBJECT
public:

    /**
     * Creates a new WaitForWindow.
     *
     * @param parent The parent QObject.
     */
    WaitForWindow(QObject* parent = 0);

    virtual WaitFor* clone() const;
    virtual bool equals(const WaitFor& waitFor) const;

    QString windowObjectName() const;
    void setWindowObjectName(const QString& windowObjectName);

private:

    QString mWindowObjectName;

};

#endif
