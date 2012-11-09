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

#ifndef WAITFORNOT_H
#define WAITFORNOT_H

#include "WaitFor.h"

/**
 * Container for negated wait for data.
 * It stores the data used in KTutorial WaitForNot, but it has nothing to do
 * with it (they don't even know each other). Its purpose is store the data
 * needed to generate the code to initialize a true KTutorial::WaitForNot
 * object.
 *
 * When the negated wait for is modified, dataChanged(WaitFor*) signal is
 * emitted.
 */
class WaitForNot: public WaitFor {
Q_OBJECT
public:

    /**
     * Creates a new WaitForNot.
     *
     * @param parent The parent QObject.
     */
    explicit WaitForNot(QObject* parent = 0);
    virtual ~WaitForNot();

    virtual WaitFor* clone() const;
    virtual bool equals(const WaitFor& waitFor) const;

    WaitFor* negatedWaitFor() const;

    /**
     * Sets the negated WaitFor.
     * The WaitFor will be destroyed when this WaitForNot is destroyed. However,
     * if a WaitFor is set over a previous one, the previous one must be deleted
     * explicitly.
     *
     * @param waitFor The WaitFor to set.
     */
    void setNegatedWaitFor(WaitFor* waitFor);

private:

    WaitFor* mNegatedWaitFor;

};

#endif
