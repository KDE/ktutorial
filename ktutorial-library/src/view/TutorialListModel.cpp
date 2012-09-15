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

#include <klocalizedstring.h>

#include "TutorialListModel.h"
#include "../TutorialInformation.h"
#include "../TutorialManager.h"

namespace ktutorial {
namespace view {

//public:

int TutorialListModel::rowCount(const QModelIndex& /*parent = QModelIndex()*/) const {
    return mTutorialManager->tutorialInformations().count();
}

QVariant TutorialListModel::data(const QModelIndex& index, int role) const {
    if (role != Qt::DisplayRole) {
        return QVariant();
    }

    const TutorialInformation* tutorialInformation =
                                        getTutorialInformationForIndex(index);
    if (tutorialInformation == 0) {
        return QVariant();
    }

    return tutorialInformation->name();
}

QVariant TutorialListModel::headerData(int section, Qt::Orientation /*orientation*/,
                                       int role /*= Qt::DisplayRole*/) const {
    if (role != Qt::DisplayRole) {
        return QVariant();
    }

    if (section != 0) {
        return QVariant();
    }

    return i18nc("@title:column", "Tutorials name");
}

const TutorialInformation* TutorialListModel::getTutorialInformationForIndex(
                                            const QModelIndex& index) const {
    if (!index.isValid()) {
        return 0;
    }

    if (index.row() >= mTutorialManager->tutorialInformations().count()) {
        return 0;
    }

    return mTutorialManager->tutorialInformations()[index.row()];
}

}
}
