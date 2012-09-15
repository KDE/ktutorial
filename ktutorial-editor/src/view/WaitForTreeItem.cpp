/***************************************************************************
 *   Copyright (C) 2010 by Daniel Calviño Sánchez <danxuliu@gmail.com>     *
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

#include "WaitForTreeItem.h"
#include "WaitForComposedTreeItem.h"
#include "WaitForEventTreeItem.h"
#include "WaitForNotTreeItem.h"
#include "WaitForPropertyTreeItem.h"
#include "WaitForSignalTreeItem.h"
#include "WaitForStepActivationTreeItem.h"
#include "WaitForWindowTreeItem.h"
#include "../data/WaitForComposed.h"
#include "../data/WaitForEvent.h"
#include "../data/WaitForNot.h"
#include "../data/WaitForProperty.h"
#include "../data/WaitForSignal.h"
#include "../data/WaitForStepActivation.h"
#include "../data/WaitForWindow.h"

//public:

WaitForTreeItem* WaitForTreeItem::treeItemForWaitFor(WaitFor* waitFor,
                                                     TreeItem* parent) {
    if (qobject_cast<WaitForComposed*>(waitFor)) {
        return new WaitForComposedTreeItem(
                                static_cast<WaitForComposed*>(waitFor), parent);
    }

    if (qobject_cast<WaitForEvent*>(waitFor)) {
        return new WaitForEventTreeItem(static_cast<WaitForEvent*>(waitFor),
                                        parent);
    }

    if (qobject_cast<WaitForNot*>(waitFor)) {
        return new WaitForNotTreeItem(static_cast<WaitForNot*>(waitFor),
                                      parent);
    }

    if (qobject_cast<WaitForProperty*>(waitFor)) {
        return new WaitForPropertyTreeItem(
                                static_cast<WaitForProperty*>(waitFor), parent);
    }

    if (qobject_cast<WaitForSignal*>(waitFor)) {
        return new WaitForSignalTreeItem(static_cast<WaitForSignal*>(waitFor),
                                         parent);
    }

    if (qobject_cast<WaitForStepActivation*>(waitFor)) {
        return new WaitForStepActivationTreeItem(
                        static_cast<WaitForStepActivation*>(waitFor), parent);
    }

    if (qobject_cast<WaitForWindow*>(waitFor)) {
        return new WaitForWindowTreeItem(static_cast<WaitForWindow*>(waitFor),
                                         parent);
    }

    Q_ASSERT(false);
    return 0;
}

WaitForTreeItem::WaitForTreeItem(WaitFor* waitFor, TreeItem* parent):
        TreeItem(parent),
    mWaitFor(waitFor) {
}

WaitFor* WaitForTreeItem::waitFor() const {
    return mWaitFor;
}
