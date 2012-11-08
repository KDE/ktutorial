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

#ifndef KTUTORIAL_OPTION_H
#define KTUTORIAL_OPTION_H

#include <QtCore/QObject>

#include "ktutorial_export.h"

namespace ktutorial {

/**
 * An option shown to the user.
 * An option is something a user can choose to do in a Step. For example, in the
 * start step of a tutorial, if the user wants to follow the easy or the hard
 * way. Two options would be used in that case, named "Easy" and "Hard".
 *
 * Several options can be added in each Step of a Tutorial. When an Option is
 * added, it is connected to a slot that is executed when the user selects that
 * Option.
 * \code
 *   Step* startStep = new Step("start");
 *   startStep->setText(i18nc("@info", "Do you want to follow the easy or the hard way of the tutorial?"));
 *
 *   Option* easyWayOption = new Option(i18n("Easy"));
 *   startStep->addOption(easyWayOption, this, SLOT(startEasyOption()));
 *
 *   Option* hardWayOption = new Option(i18n("Hard"));
 *   startStep->addOption(hardWayOption, this, SLOT(startHardOption()));
 * \endcode
 *
 * In order to use an Option, it is only needed to set its properties and
 * specify a slot to be called when the user selects it.
 */
class KTUTORIAL_EXPORT Option: public QObject {
Q_OBJECT
public:

    /**
     * Creates a new Option with the specified name.
     *
     * @param name The name of this Option.
     */
    explicit Option(const QString& name): QObject(),
        mName(name) {
    }

    /**
     * Returns the name of this Option.
     *
     * @return The name of this Option.
     */
    const QString& name() const {
        return mName;
    }

Q_SIGNALS:

    /**
     * This signal is emitted when this Option is selected by the user.
     *
     * If a KTutorial customization is used, you must emit this signal
     * (connecting it to another signal) when the user selects the option in the
     * UI.
     */
    void selected();

private:

    /**
     * The human readable name of this Option shown to the user.
     */
    QString mName;

};

}

#endif
