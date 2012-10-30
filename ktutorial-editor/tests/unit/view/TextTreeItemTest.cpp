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

#include <QTest>

#include "TextTreeItem.h"

#include <QSignalSpy>

class TextTreeItemTest: public QObject {
Q_OBJECT

private slots:

    void testConstructor();

    void testSetText();

};

void TextTreeItemTest::testConstructor() {
    TextTreeItem parent;
    TextTreeItem treeItem(&parent);

    QCOMPARE(parent.parent(), (TreeItem*)0);
    QCOMPARE(treeItem.parent(), &parent);
}

//TreeItem* must be declared as a metatype to be used in qvariant_cast
Q_DECLARE_METATYPE(TreeItem*);

void TextTreeItemTest::testSetText() {
    TextTreeItem item;

    //TreeItem* must be registered in order to be used with QSignalSpy
    int treeItemStarType = qRegisterMetaType<TreeItem*>("TreeItem*");
    QSignalSpy dataChangedSpy(&item, SIGNAL(dataChanged(TreeItem*)));

    item.setText("Hello world");

    QCOMPARE(item.text(), QString("Hello world"));
    QCOMPARE(dataChangedSpy.count(), 1);
    QVariant argument = dataChangedSpy.at(0).at(0);
    QCOMPARE(argument.userType(), treeItemStarType);
    QCOMPARE(qvariant_cast<TreeItem*>(argument), &item);
}

QTEST_MAIN(TextTreeItemTest)

#include "TextTreeItemTest.moc"
