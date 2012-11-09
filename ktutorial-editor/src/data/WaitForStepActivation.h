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

#ifndef WAITFORSTEPACTIVATION_H
#define WAITFORSTEPACTIVATION_H

#include "WaitFor.h"

/**
 * Container for conditions that wait for their step to be activated data.
 * It stores the data used in KTutorial WaitForStepActivation, but it has
 * nothing to do with it (they don't even know each other). Its purpose is store
 * the data needed to generate the code to initialize a true
 * KTutorial::WaitForStepActivation object.
 *
 * However, it contains no attributes as WaitForStepActivation only makes sense
 * to be used with the step and tutorial that contains it, and those are set
 * when the tutorial is exported without needing a explicit attribute.
 */
class WaitForStepActivation: public WaitFor {
Q_OBJECT
public:

    /**
     * Creates a new WaitForStepActivation.
     *
     * @param parent The parent QObject.
     */
    explicit WaitForStepActivation(QObject* parent = 0);

    virtual WaitFor* clone() const;
    virtual bool equals(const WaitFor& waitFor) const;

};

#endif
