/***************************************************************************
 *   Copyright (C) 2008 by Daniel Calviño Sánchez <danxuliu@gmail.com>     *
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

#ifndef KTUTORIAL_WAITFOR_H
#define KTUTORIAL_WAITFOR_H

#include <QtCore/QObject>

#include "ktutorial_export.h"

namespace ktutorial {

/**
 * Abstract base class for conditions to be waited for.
 * Classes derived from this one model a condition to be waited for. WaitFor
 * objects are added to Step objects specifying some slot to be called when the
 * wait ends.
 *
 * WaitFor objects can be active or inactive. If the condition is met when they
 * are inactive, wait isn't ended. That is, only active WaitFor objects end
 * their wait (emitting the waitEnded(WaitFor*) signal).
 *
 * WaitFor hierarchy uses a Composite design pattern. You can compose several
 * WaitFor objects with subclasses of WaitForComposed, each having some specific
 * behaviour (like behaving as an "AND" or an "OR" of their child conditions).
 *
 * When subclassing WaitFor, conditionMet() must be implemented. This method
 * is used to know in WaitForComposed subclasses whether a WaitFor object met
 * their condition or not. Also, when subclassing, waitEnded(WaitFor*) must be
 * emitted when needed.
 */
class KTUTORIAL_EXPORT WaitFor: public QObject {
Q_OBJECT
public:

    /**
     * Destroys this WaitFor.
     */
    virtual ~WaitFor();

    /**
     * Returns true if the condition being waited for is met, false otherwise.
     * This method must be defined in subclasses of WaitFor.
     *
     * @return True if the condition being waited for is met, false otherwise.
     */
    virtual bool conditionMet() const = 0;

    /**
     * Returns true if this WaitFor is active, false otherwise.
     *
     * @return True if this WaitFor is active, false otherwise.
     */
    bool isActive() const;

    /**
     * Sets this WaitFor as active or inactive.
     * This method is used internally. Do not call this method yourself.
     *
     * WaitFor subclasses can redefine this method if some special action has to
     * be taken when activating or deactivating it.
     *
     * @param active True to set it active, false otherwise.
     */
    virtual void setActive(bool active);

Q_SIGNALS:

    /**
     * This signal is emitted when the condition being waited for is met, and
     * this WaitFor is active.
     * This WaitFor is passed in the signal so the sender can be identified.
     *
     * @param waitFor This WaitFor.
     */
    void waitEnded(WaitFor* waitFor);

protected:

    /**
     * Creates a new WaitFor.
     * WaitFor are initially inactive.
     *
     * Protected to avoid classes other than subclasses to create them.
     */
    WaitFor();

private:

    class WaitForPrivate* d;

};

}

#endif
