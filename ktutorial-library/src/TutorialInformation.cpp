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

#include "TutorialInformation.h"

namespace ktutorial {

//public:

TutorialInformation::TutorialInformation(const QString& id): QObject(),
    mId(id) {
}

const QString& TutorialInformation::id() const {
    return mId;
}

const QString& TutorialInformation::name() const {
    return mName;
}

const QString& TutorialInformation::description() const {
    return mDescription;
}

void TutorialInformation::setName(const QString& name) {
    mName = name;
}

void TutorialInformation::setDescription(const QString& description) {
    mDescription = description;
}

}

#include "TutorialInformation.moc"
