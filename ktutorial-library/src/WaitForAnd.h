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

#ifndef KTUTORIAL_WAITFORAND_H
#define KTUTORIAL_WAITFORAND_H

#include "ktutorial_export.h"

#include "WaitForComposed.h"

namespace ktutorial {

/**
 * Composed WaitFor that performs an AND between its children.
 */
class KTUTORIAL_EXPORT WaitForAnd: public WaitForComposed {
Q_OBJECT
public:

    /**
     * Creates a new WaitForAnd.
     */
    Q_INVOKABLE WaitForAnd(): WaitForComposed() {
    }

    /**
     * Returns true if all of its children met its condition, false otherwise.
     * If there are no children, false is returned.
     *
     * @return True if all of its children met its condition, false otherwise.
     */
    virtual bool conditionMet() const;

};

}

#endif
