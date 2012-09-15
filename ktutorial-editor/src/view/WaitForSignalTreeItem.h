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

#ifndef WAITFORSIGNALTREEITEM_H
#define WAITFORSIGNALTREEITEM_H

#include "WaitForTreeItem.h"

class WaitForSignal;

/**
 * A TreeItem that represents a WaitForSignal.
 * The tree representation of a WaitForSignal is a plain text:
 * When the signal "signal name" is emitted by the object "object name"
 *
 * If the signal or the emitter name aren't set yet, a placeholder is put
 * instead. Signal placeholder is "(signal not set)", and the emitter name
 * placeholder is "(object name not set)" (without quotes, but with
 * parenthesis).
 *
 * Whenever the WaitForSignal data changes, the WaitForSignalTreeItem text is
 * updated as needed.
 */
class WaitForSignalTreeItem: public WaitForTreeItem {
Q_OBJECT
public:

    /**
     * Creates a new WaitForSignalTreeItem for the given WaitForSignal and with
     * the given parent.
     *
     * @param waitForSignal The WaitForSignal to represent.
     * @param parent The parent TreeItem.
     */
    explicit WaitForSignalTreeItem(WaitForSignal* waitForSignal,
                                   TreeItem* parent = 0);

    /**
     * Returns the description of the WaitForSignal.
     *
     * @return The text for this TreeItem.
     */
    virtual QString text() const;

private:

    /**
     * The emitter name of the WaitForSignal.
     */
    QString mEmitterName;

    /**
     * The signal name of the WaitForSignal.
     */
    QString mSignalName;

private Q_SLOTS:

    /**
     * Updates this WaitForSignalTreeItem when the data of its WaitForSignal
     * changed.
     *
     * @param waitFor The WaitForSignal.
     */
    void update(WaitFor* waitFor);

};

#endif
