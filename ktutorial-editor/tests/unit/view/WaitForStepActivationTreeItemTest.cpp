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

#include <QtTest>

#include "WaitForStepActivationTreeItem.h"

#include <KLocalizedString>

#include "../data/WaitForStepActivation.h"

class WaitForStepActivationTreeItemTest: public QObject {
Q_OBJECT

private slots:

    void testConstructor();

};

class StubTreeItem: public TreeItem {
public:
    virtual QString text() const {
        return "";
    }
};

void WaitForStepActivationTreeItemTest::testConstructor() {
    WaitForStepActivation waitForStepActivation;

    StubTreeItem parent;
    WaitForStepActivationTreeItem item(&waitForStepActivation, &parent);

    QCOMPARE(item.parent(), &parent);
    QCOMPARE(item.waitFor(), &waitForStepActivation);
    QCOMPARE(item.text(), i18nc("@item", "When the step is activated"));
}

QTEST_MAIN(WaitForStepActivationTreeItemTest)

#include "WaitForStepActivationTreeItemTest.moc"
