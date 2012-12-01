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

#include "WaitForOr.h"
#include "WaitForOr_p.h"

namespace ktutorial {

//public:

WaitForOr::WaitForOr(): WaitForComposed(),
    d(new WaitForOrPrivate()) {
}

WaitForOr::~WaitForOr() {
    delete d;
}

bool WaitForOr::conditionMet() const {
    QListIterator<WaitFor*> it(waitFors());
    while (it.hasNext()) {
        if (it.next()->conditionMet()) {
            return true;
        }
    }

    return false;
}

}
