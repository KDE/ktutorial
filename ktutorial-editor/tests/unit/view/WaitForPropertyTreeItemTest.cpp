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

#include <QTest>

#include "WaitForPropertyTreeItem.h"

#include <QSignalSpy>

#include <KLocalizedString>

#include "../data/WaitForProperty.h"

class WaitForPropertyTreeItemTest: public QObject {
Q_OBJECT

private slots:

    void initTestCase();

    void testConstructor();
    void testConstructorFull();

    void testWaitForPropertySetObjectName();
    void testWaitForPropertySetObjectNameChange();

    void testWaitForPropertySetPropertyName();
    void testWaitForPropertySetPropertyNameChange();

    void testWaitForPropertySetValue();
    void testWaitForPropertySetValueChange();

private:

    int mTreeItemStarType;

    void assertDataChanged(const QSignalSpy& spy, int index,
                           TreeItem* item) const;

};

class StubTreeItem: public TreeItem {
public:
    virtual QString text() const {
        return "";
    }
};

void WaitForPropertyTreeItemTest::initTestCase() {
    //TreeItem* must be registered in order to be used with QSignalSpy
    mTreeItemStarType = qRegisterMetaType<TreeItem*>("TreeItem*");
}

void WaitForPropertyTreeItemTest::testConstructor() {
    WaitForProperty waitForProperty;

    StubTreeItem parent;
    WaitForPropertyTreeItem item(&waitForProperty, &parent);

    QCOMPARE(item.parent(), &parent);
    QCOMPARE(item.waitFor(), &waitForProperty);
    QCOMPARE(item.text(), i18nc("@item", "When the property (property not set) "
                                         "in the object (object name not set) "
                                         "has the value (value not set)"));
}

void WaitForPropertyTreeItemTest::testConstructorFull() {
    WaitForProperty waitForProperty;
    waitForProperty.setObjectName("objectName");
    waitForProperty.setPropertyName("propertyName");
    waitForProperty.setValue("theValue");

    StubTreeItem parent;
    WaitForPropertyTreeItem item(&waitForProperty, &parent);

    QCOMPARE(item.parent(), &parent);
    QCOMPARE(item.waitFor(), &waitForProperty);
    QCOMPARE(item.text(), i18nc("@item", "When the property \"propertyName\" "
                                         "in the object \"objectName\" has the "
                                         "value theValue"));
}

void WaitForPropertyTreeItemTest::testWaitForPropertySetObjectName() {
    WaitForProperty waitForProperty;
    waitForProperty.setObjectName("objectName");

    WaitForPropertyTreeItem item(&waitForProperty);

    QCOMPARE(item.text(), i18nc("@item", "When the property (property not set) "
                                         "in the object \"objectName\" has the "
                                         "value (value not set)"));
}

void WaitForPropertyTreeItemTest::testWaitForPropertySetObjectNameChange() {
    WaitForProperty waitForProperty;
    WaitForPropertyTreeItem item(&waitForProperty);

    waitForProperty.setObjectName("objectName");

    QSignalSpy dataChangedSpy(&item, SIGNAL(dataChanged(TreeItem*)));

    waitForProperty.setObjectName("objectNameChanged");

    QCOMPARE(item.text(), i18nc("@item", "When the property (property not set) "
                                         "in the object \"objectNameChanged\" "
                                         "has the value (value not set)"));
    QCOMPARE(dataChangedSpy.count(), 1);
    assertDataChanged(dataChangedSpy, 0, &item);
}

void WaitForPropertyTreeItemTest::testWaitForPropertySetPropertyName() {
    WaitForProperty waitForProperty;
    waitForProperty.setPropertyName("propertyName");

    WaitForPropertyTreeItem item(&waitForProperty);

    QCOMPARE(item.text(), i18nc("@item", "When the property \"propertyName\" "
                                         "in the object (object name not set) "
                                         "has the value (value not set)"));
}

void WaitForPropertyTreeItemTest::testWaitForPropertySetPropertyNameChange() {
    WaitForProperty waitForProperty;
    WaitForPropertyTreeItem item(&waitForProperty);

    waitForProperty.setPropertyName("propertyName");

    QSignalSpy dataChangedSpy(&item, SIGNAL(dataChanged(TreeItem*)));

    waitForProperty.setPropertyName("propertyNameChanged");

    QCOMPARE(item.text(), i18nc("@item", "When the property "
                                         "\"propertyNameChanged\" in the "
                                         "object (object name not set) has the "
                                         "value (value not set)"));
    QCOMPARE(dataChangedSpy.count(), 1);
    assertDataChanged(dataChangedSpy, 0, &item);
}

void WaitForPropertyTreeItemTest::testWaitForPropertySetValue() {
    WaitForProperty waitForProperty;
    waitForProperty.setValue("theValue");

    WaitForPropertyTreeItem item(&waitForProperty);

    QCOMPARE(item.text(), i18nc("@item", "When the property (property not set) "
                                         "in the object (object name not set) "
                                         "has the value theValue"));
}

void WaitForPropertyTreeItemTest::testWaitForPropertySetValueChange() {
    WaitForProperty waitForProperty;
    WaitForPropertyTreeItem item(&waitForProperty);

    waitForProperty.setValue("valueName");

    QSignalSpy dataChangedSpy(&item, SIGNAL(dataChanged(TreeItem*)));

    waitForProperty.setValue("\"theValueChanged\"");

    QCOMPARE(item.text(), i18nc("@item", "When the property (property not set) "
                                         "in the object (object name not set) "
                                         "has the value \"theValueChanged\""));
    QCOMPARE(dataChangedSpy.count(), 1);
    assertDataChanged(dataChangedSpy, 0, &item);
}

/////////////////////////////////// Helpers ////////////////////////////////////

//TreeItem* must be declared as a metatype to be used in qvariant_cast
Q_DECLARE_METATYPE(TreeItem*);

void WaitForPropertyTreeItemTest::assertDataChanged(const QSignalSpy& spy,
                                                    int index,
                                                    TreeItem* item) const {
    QCOMPARE(spy.at(index).count(), 1);

    QVariant argument = spy.at(index).at(0);
    QCOMPARE(argument.userType(), mTreeItemStarType);
    QCOMPARE(qvariant_cast<TreeItem*>(argument), item);
}

QTEST_MAIN(WaitForPropertyTreeItemTest)

#include "WaitForPropertyTreeItemTest.moc"
