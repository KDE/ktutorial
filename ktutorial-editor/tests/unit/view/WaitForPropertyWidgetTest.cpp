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

#define protected public
#define private public
#include "WaitForPropertyWidget.h"
#undef private
#undef protected

#include <KLineEdit>

#include "../data/WaitForProperty.h"

#ifdef QT_QTDBUS_FOUND
#include "RemoteObjectNameWidget.h"
#define protected public
#define private public
#include "../targetapplication/TargetApplication.h"
#undef private
#undef protected
#endif

class WaitForPropertyWidgetTest: public QObject {
Q_OBJECT

private slots:

    void init();
    void cleanup();

    void testConstructor();

    void testSaveChanges();

    void testPropertyNameCompletion();

private:

    QString mTargetApplicationStubPath;

    KLineEdit* objectNameLineEdit(WaitForPropertyWidget* widget) const;
    KLineEdit* propertyNameLineEdit(WaitForPropertyWidget* widget) const;
    KLineEdit* valueLineEdit(WaitForPropertyWidget* widget) const;

    bool waitForTargetApplicationToStart(int timeout) const;

    bool waitForSignalCount(const QSignalSpy* spy, int count,
                            int timeout) const;

};

void WaitForPropertyWidgetTest::init() {
#ifdef QT_QTDBUS_FOUND
    mTargetApplicationStubPath = QApplication::applicationDirPath() +
                                "/../targetapplication/TargetApplicationStub";

    //Avoid signals from previous tests to be delivered to the next ones
    //setting a new TargetApplication
    delete TargetApplication::sSelf;
    TargetApplication::sSelf = new TargetApplication();
#endif
}

void WaitForPropertyWidgetTest::cleanup() {
#ifdef QT_QTDBUS_FOUND
    delete TargetApplication::sSelf;
    TargetApplication::sSelf = 0;
#endif
}

void WaitForPropertyWidgetTest::testConstructor() {
    WaitForProperty waitFor;
    waitFor.setObjectName("The object name");
    waitFor.setPropertyName("The property name");
    waitFor.setValue("The value");

    QWidget parent;
    WaitForPropertyWidget* widget =
                                new WaitForPropertyWidget(&waitFor, &parent);

    QCOMPARE(widget->parentWidget(), &parent);
    QCOMPARE(objectNameLineEdit(widget)->text(), QString("The object name"));
    QCOMPARE(propertyNameLineEdit(widget)->text(),
             QString("The property name"));
    QCOMPARE(valueLineEdit(widget)->text(), QString("The value"));
}

void WaitForPropertyWidgetTest::testSaveChanges() {
    WaitForProperty waitFor;
    waitFor.setObjectName("The object name");
    waitFor.setPropertyName("The property name");
    waitFor.setValue("The value");

    WaitForPropertyWidget widget(&waitFor);
    objectNameLineEdit(&widget)->setText("The new object name");
    propertyNameLineEdit(&widget)->setText("The new property name");
    valueLineEdit(&widget)->setText("The new value");

    widget.saveChanges();

    QCOMPARE(waitFor.objectName(), QString("The new object name"));
    QCOMPARE(waitFor.propertyName(), QString("The new property name"));
    QCOMPARE(waitFor.value(), QString("The new value"));
}

#ifdef QT_QTDBUS_FOUND
//RemoteObject* must be declared as a metatype to be used in qvariant_cast
Q_DECLARE_METATYPE(RemoteObject*);
#endif

void WaitForPropertyWidgetTest::testPropertyNameCompletion() {
#ifdef QT_QTDBUS_FOUND
    TargetApplication::self()->setTargetApplicationFilePath(
                                                    mTargetApplicationStubPath);
    TargetApplication::self()->start();

    QVERIFY(waitForTargetApplicationToStart(10000));

    WaitForProperty waitFor;
    WaitForPropertyWidget widget(&waitFor);

    //RemoteObject* must be registered in order to be used with QSignalSpy
    qRegisterMetaType<RemoteObject*>("RemoteObject*");
    QSignalSpy remoteObjectChosenSpy(widget.mRemoteObjectNameWidget,
                                     SIGNAL(remoteObjectChosen(RemoteObject*)));

    objectNameLineEdit(&widget)->setText("The object name 830");

    //Wait until the RemoteObjectNameWidget emits the remoteObjectChosen signal
    //(as it can take some time until the names of the remote objects are
    //registered), and ensure that the chosen remote object is not null
    QVERIFY(waitForSignalCount(&remoteObjectChosenSpy, 1, 10000));
    QVariant argument = remoteObjectChosenSpy.at(0).at(0);
    QVERIFY(qvariant_cast<RemoteObject*>(argument));

    KCompletion* completion = propertyNameLineEdit(&widget)->completionObject();
    QStringList items = completion->items();
    QCOMPARE(items.count(), 6);
    QCOMPARE(items[0], QString("ChildQWidgetProperty0"));
    QCOMPARE(items[1], QString("ChildQWidgetProperty1"));
    QCOMPARE(items[2], QString("ChildQWidgetProperty2"));
    QCOMPARE(items[3], QString("QWidgetProperty0"));
    QCOMPARE(items[4], QString("QWidgetProperty1"));
    QCOMPARE(items[5], QString("QWidgetProperty2"));
#else
    QSKIP("Property name completion is only available when KTutorial editor is "
          "compiled with DBus support", SkipAll);
#endif
}

/////////////////////////////////// Helpers ////////////////////////////////////

KLineEdit* WaitForPropertyWidgetTest::objectNameLineEdit(
                                        WaitForPropertyWidget* widget) const {
    return widget->findChild<KLineEdit*>("objectNameLineEdit");
}

KLineEdit* WaitForPropertyWidgetTest::propertyNameLineEdit(
                                        WaitForPropertyWidget* widget) const {
    return widget->findChild<KLineEdit*>("propertyNameLineEdit");
}

KLineEdit* WaitForPropertyWidgetTest::valueLineEdit(
                                        WaitForPropertyWidget* widget) const {
    return widget->findChild<KLineEdit*>("valueLineEdit");
}

bool waitFor(bool (*condition)(), int timeout) {
    QElapsedTimer timer;
    timer.start();
    do {
        QTest::qWait(100);
    } while (!condition() && timer.elapsed() < timeout);

    if (timer.elapsed() >= timeout) {
        return false;
    }

    return true;    
}

bool isTargetApplicationStarted() {
    return TargetApplication::self()->remoteEditorSupport();
}

bool WaitForPropertyWidgetTest::waitForTargetApplicationToStart(
                                                            int timeout) const {
    return waitFor(&isTargetApplicationStarted, timeout);
}

template <typename Class>
bool waitFor(Class &object, bool (Class::*condition)() const, int timeout) {
    QElapsedTimer timer;
    timer.start();
    do {
        QTest::qWait(100);
    } while (!(object.*condition)() && timer.elapsed() < timeout);

    if (timer.elapsed() >= timeout) {
        return false;
    }

    return true;    
}

class SignalCountCondition {
public:
    const QSignalSpy* mSpy;
    int mCount;

    bool condition() const {
        return mSpy->count() == mCount;
    }
};

bool WaitForPropertyWidgetTest::waitForSignalCount(const QSignalSpy* spy,
                                                   int count,
                                                   int timeout) const {
    SignalCountCondition helper;
    helper.mSpy = spy;
    helper.mCount = count;
    return waitFor(helper, &SignalCountCondition::condition, timeout);
}

QTEST_MAIN(WaitForPropertyWidgetTest)

#include "WaitForPropertyWidgetTest.moc"
