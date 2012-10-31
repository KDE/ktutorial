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

#include "WaitForSignalTreeItem.h"

#include <KLocalizedString>

#include "../data/WaitForSignal.h"

//public:

WaitForSignalTreeItem::WaitForSignalTreeItem(WaitForSignal* waitForSignal,
                                             TreeItem* parent):
        WaitForTreeItem(waitForSignal, parent) {
    mEmitterName = waitForSignal->emitterName();
    mSignalName = waitForSignal->signalName();

    connect(waitForSignal, SIGNAL(dataChanged(WaitFor*)),
            this, SLOT(update(WaitFor*)));
}

QString WaitForSignalTreeItem::text() const {
    QString emitterName;
    if (mEmitterName.isEmpty()) {
        emitterName = i18nc("@item:intext", "(object not set)");
    } else {
        emitterName = "\"" + mEmitterName + "\"";
    }

    QString signalName;
    if (mSignalName.isEmpty()) {
        signalName = i18nc("@item:intext", "(signal not set)");
    } else {
        signalName = "\"" + mSignalName + "\"";
    }

    return i18nc("@item", "When the signal %1 is emitted by object %2",
                 signalName, emitterName);
}

//private:

void WaitForSignalTreeItem::update(WaitFor* waitFor) {
    WaitForSignal* waitForSignal = static_cast<WaitForSignal*>(waitFor);
    mEmitterName = waitForSignal->emitterName();
    mSignalName = waitForSignal->signalName();

    emit dataChanged(this);
}
