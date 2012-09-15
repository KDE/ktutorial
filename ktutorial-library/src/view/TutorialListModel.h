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

#ifndef KTUTORIAL_VIEW_TUTORIALLISTMODEL_H
#define KTUTORIAL_VIEW_TUTORIALLISTMODEL_H

#include <QAbstractListModel>

namespace ktutorial {
class TutorialInformation;
class TutorialManager;
}

namespace ktutorial {
namespace view {

/**
 * A ListModel that fetchs its data from a TutorialManager.
 * This is a very basic read-only model. It just shows the TutorialInformations
 * registered in a TutorialManager as a list where each element is the name of
 * a TutorialInformation.
 *
 * To know the TutorialInformation associated with an index, use
 * getTutorialInformationForIndex(const QModelIndex&).
 */
class TutorialListModel: public QAbstractListModel {
Q_OBJECT
public:

    /**
     * Creates a new TutoriaListModel.
     * It gets the data from the specified TutorialManager.
     *
     * @param tutorialManager The TutorialManager to get the data from.
     * @param parent The parent QObject, defaults to null.
     */
    explicit TutorialListModel(const TutorialManager* tutorialManager,
                               QObject* parent = 0):
        QAbstractListModel(parent),
        mTutorialManager(tutorialManager) {
    }

    /**
     * Returns the number of available tutorials.
     *
     * @param parent Ignored.
     * @return The number of available tutorials.
     */
    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;

    /**
     * Returns the name of the tutorial with the specified index.
     * If the index isn't valid or the role isn't DisplayRole, an empty message
     * is returned.
     *
     * @param index The index to get its data.
     * @param role The role of the data.
     * @return The name of the tutorial with the specified index.
     */
    virtual QVariant data(const QModelIndex &index, int role) const;

    /**
     * Returns the data for the given role and section in the header.
     * "Tutorials name" is returned for first section of DisplayRole, and an 
     * empty message for other sections and roles.
     *
     * @param section The section of the header data.
     * @param orientation Ignored.
     * @param role The role of the header data.
     * @return The data for the given role and section in the header.
     */
    virtual QVariant headerData(int section, Qt::Orientation orientation,
                                int role = Qt::DisplayRole) const;

    /**
     * Returns the TutorialInformation associated with the specified index.
     * If the index is invalid, a null pointer is returned.
     *
     * @param index The index to get its associatted TutorialInformation.
     * @return The TutorialInformation associated with the specified index.
     */
    const TutorialInformation* getTutorialInformationForIndex(
                                                const QModelIndex& index) const;

private:

    /**
     * The TutorialManager to get the data from.
     */
    const TutorialManager* mTutorialManager;

};

}
}

#endif
