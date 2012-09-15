/***************************************************************************
 *   Copyright (C) 2008 by Daniel Calviño Sánchez <danxuliu@gmail.com>     *
 *   Copyright (C) 2009 by Daniel Calviño Sánchez <danxuliu@gmail.com>     *
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

#ifndef KTUTORIAL_WAITFORCOMPOSED_H
#define KTUTORIAL_WAITFORCOMPOSED_H

#include "ktutorial_export.h"

#include "WaitFor.h"

namespace ktutorial {

/**
 * Abstract base class for composed conditions to wait for.
 * Classes derived from this one model a composed condition to be waited for.
 * Child WaitFor objects are added using WaitForComposed::add(WaitFor*).
 *
 * Once a WaitFor is added, it will be automatically activated or deactivated
 * when the parent (the subclass of WaitForComposed) is activated or
 * deactivated.
 *
 * When an added WaitFor ends its wait, it notifies the parent WaitForComposed
 * subclass. It then checks if it is active and its own condition was met, and
 * in that case ends its own wait.
 *
 * When subclassing WaitForComposed, conditionMet() must be implemented.
 */
class KTUTORIAL_EXPORT WaitForComposed: public WaitFor {
Q_OBJECT
public:

    /**
     * Sets this WaitForComposed and all the children as active or inactive.
     *
     * @param active True to set it active, false otherwise.
     */
    virtual void setActive(bool active);

    /**
     * Adds a new WaitFor to this WaitForComposed.
     * If the WaitFor is already added, nothing happens.
     * WaitFor notifies this WaitForComposed when its wait ends through
     * WaitForComposed::childWaitEnd(WaitFor*)
     *
     * The WaitFor is reparented to this WaitForComposed, and thus deleted when
     * this WaitForComposed is deleted.
     *
     * This method can be invoked from a script.
     *
     * @param waitFor The WaitFor to add.
     */
    Q_INVOKABLE void add(WaitFor* waitFor);

public slots:

    /**
     * Notifies that the child WaitFor object has ended its wait.
     * If is active and the condition is met for this WaitForComposed, its own
     * wait ends.
     *
     * This slot is connected automatically to children when they are added.
     *
     * @param waitFor The WaitFor child object that ended its wait.
     */
    void childWaitEnd(WaitFor* waitFor);

protected:

    /**
     * A list containing all the WaitFor objects added.
     */
    QList<WaitFor*> mWaitFors;

    /**
     * Creates a new WaitForComposed.
     *
     * Protected to avoid classes other than subclasses to create them.
     */
    WaitForComposed(): WaitFor() {
    }

};

}

#endif
