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

#ifndef WAITFOR_H
#define WAITFOR_H

#include <QObject>

/**
 * Abstract base class for conditions to wait for to be met.
 * Its subclasses store the data used in KTutorial wait for subclasses, but they
 * have nothing to do with them (they don't even know each others). Its purpose
 * is store the data needed to generate the code to initialize a true
 * KTutorial::WaitFor subclass object.
 *
 * Subclass must implement clone() and equals(const WaitFor&) methods, which
 * perform a deep copy and a comparison of the WaitFor. Operators == and != are
 * provided for convenience in this base class and it isn't needed to define
 * them in subclasses.
 *
 * Subclasses must also emit dataChanged(Step*) signal when their data is
 * modified (including the data of child conditions).
 */
class WaitFor: public QObject {
Q_OBJECT
public:

    /**
     * Creates a new WaitFor.
     *
     * @param parent The parent QObject.
     */
    explicit WaitFor(QObject* parent = 0);

    /**
     * Deep copy of this WaitFor.
     * If this WaitFor has child WaitFor, they are also cloned in the returned
     * WaitFor.
     *
     * Subclasses must implement this method.
     *
     * @return A deep copy of this WaitFor.
     */
    virtual WaitFor* clone() const = 0;

    /**
     * Deep comparison of this WaitFor to the given one.
     * Two WaitFor are equal if they contain the same data and, if they contain
     * child WaitFor, if their child WaitFor are equal one to one. The child
     * WaitFor aren't checked for identity, but for equality (that is, it is
     * checked if they are equal, no matter whether they are the same object or
     * not).
     *
     * Subclasses must implement this method.
     *
     * @param waitFor The WaitFor to compare to.
     * @return True if this WaitFor is equal to the given one, false otherwise.
     */
    virtual bool equals(const WaitFor& waitFor) const = 0;

    /**
     * Equality operator.
     *
     * @param waitFor The WaitFor to compare to.
     * @return True if this WaitFor is equal to the given one, false otherwise.
     */
    bool operator==(const WaitFor& waitFor) const;

    /**
     * Inequality operator.
     *
     * @param waitFor The WaitFor to compare to.
     * @return True if this WaitFor is not equal to the given one, false
     *         otherwise.
     */
    bool operator!=(const WaitFor& waitFor) const;

Q_SIGNALS:

    /**
     * Emitted when any data in the WaitFor changed.
     * Subclasses must emit this signal when needed.
     *
     * @param waitFor This WaitFor.
     */
    void dataChanged(WaitFor* waitFor);

};

#endif
