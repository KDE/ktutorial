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

#ifndef WAITFORCOMPOSED_H
#define WAITFORCOMPOSED_H

#include "WaitFor.h"

/**
 * Container for composed conditions to wait for data.
 * It stores the data used in KTutorial WaitForAnd and WaitForOr, but it has
 * nothing to do with them (they don't even know each other). Its purpose is
 * store the data needed to generate the code to initialize a true
 * KTutorial::WaitForComposed subclass object.
 *
 * When the composition type is modified, dataChanged(WaitFor*) signal is
 * emitted. When WaitFors are added or removed, waitForAdded(WaitFor*) and
 * waitForRemoved(WaitFor*) are emitted.
 */
class WaitForComposed: public WaitFor {
Q_OBJECT
public:

    enum CompositionType {
        And,
        Or
    };

    /**
     * Creates a new WaitForComposed.
     *
     * @param parent The parent QObject.
     */
    WaitForComposed(QObject* parent = 0);
    virtual ~WaitForComposed();

    virtual WaitFor* clone() const;
    virtual bool equals(const WaitFor& waitFor) const;

    CompositionType compositionType() const;
    void setCompositionType(CompositionType compositionType);

    /**
     * Adds a new WaitFor to this WaitForComposed.
     * This WaitForComposed gets ownership of the added WaitFor, so it is
     * deleted when this WaitForComposed is deleted.
     *
     * @param waitFor The WaitFor to add.
     */
    void addWaitFor(WaitFor* waitFor);
    QList<WaitFor*> waitFors() const;

    /**
     * Removes a WaitFor from this WaitForComposed.
     * The WaitFor must be deleted explicitly.
     *
     * @param waitFor The WaitFor to remove.
     */
    void removeWaitFor(WaitFor* waitFor);

Q_SIGNALS:

    /**
     * Emitted when the WaitFor is added to this WaitForComposed.
     *
     * @param waitFor The WaitFor added.
     */
    void waitForAdded(WaitFor* waitFor);

    /**
     * Emitted when the WaitFor is removed from this WaitForComposed.
     *
     * @param waitFor The WaitFor removed.
     */
    void waitForRemoved(WaitFor* waitFor);

private:

    CompositionType mCompositionType;
    QList<WaitFor*> mWaitFors;

};

#endif
