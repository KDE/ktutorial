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

#include <QtTest>

#include <QApplication>
#include <QtDBus/QtDBus>

#include <KTemporaryFile>

#define protected public
#define private public
#include "EditorSupport.h"
#include "../Tutorial.h"
#undef private
#undef protected

#include "ClassRegisterAdaptor.h"
#include "EventSpy.h"
#include "ObjectRegister.h"
#include "ObjectRegisterAdaptor.h"
#include "../ObjectFinder.h"
#include "../Step.h"
#include "../TutorialInformation.h"
#include "../extendedinformation/WidgetHighlighter.h"

using ktutorial::extendedinformation::WidgetHighlighter;

//Tutorial* must be declared as a metatype to be used in qvariant_cast
Q_DECLARE_METATYPE(ktutorial::Tutorial*);

class SubClassWidget: public QWidget {
Q_OBJECT
public:
    SubClassWidget(QWidget* parent = 0): QWidget(parent) {
    }
};

class SubSubClassWidget: public SubClassWidget {
Q_OBJECT
public:
    SubSubClassWidget(QWidget* parent = 0): SubClassWidget(parent) {
    }
};

namespace ktutorial {
namespace editorsupport {

class EditorSupportTest: public QObject {
Q_OBJECT

public slots:

    void handleEventReceived(int objectId, const QString& eventType) {
        Q_UNUSED(objectId);
        mEventTypes.append(eventType);
    }

private slots:

    void init();

    void testConstructor();

    void testSetObjectFinder();

    void testSetup();

    void testMainWindowObjectId();

    void testFindObject();
    void testFindObjectWithEmptyName();
    void testFindObjectWithUnknownName();

    void testHighlight();

    void testStopHighlighting();

    void testEnableEventSpy();

    void testDisableEventSpy();

    void testTestScriptedTutorial();
    void testTestScriptedTutorialWithStepId();
    void testTestScriptedTutorialWithInvalidTutorial();

    void testObjectRegisterWhenEventSpyNotifiesEventBeforeEndingConstructor();

private:

    QStringList mEventTypes;

};

void EditorSupportTest::init() {
    mEventTypes.clear();
}

void EditorSupportTest::testConstructor() {
    QObject parent;
    EditorSupport* editorSupport = new EditorSupport(&parent);

    QCOMPARE(editorSupport->parent(), &parent);
}

void EditorSupportTest::testSetObjectFinder() {
    EditorSupport editorSupport;
    ObjectFinder objectFinder;
    
    editorSupport.setObjectFinder(&objectFinder);
    
    QCOMPARE(editorSupport.mObjectFinder, &objectFinder);
}

void EditorSupportTest::testSetup() {
    QDBusConnection bus = QDBusConnection::sessionBus();
    QVERIFY(bus.isConnected());

    EditorSupport editorSupport;
    QObject mainObject;

    editorSupport.setup(&mainObject);

    QObject* ktutorialObject = bus.objectRegisteredAt("/ktutorial");
    QVERIFY(ktutorialObject);
    QVERIFY(qobject_cast<EditorSupport*>(ktutorialObject));

    QObject* objectRegisterObject =
                            bus.objectRegisteredAt("/ktutorial/ObjectRegister");
    QVERIFY(objectRegisterObject);
    ObjectRegister* objectRegister =
                            qobject_cast<ObjectRegister*>(objectRegisterObject);
    QVERIFY(objectRegister);
    QVERIFY(objectRegister->findChild<ClassRegisterAdaptor*>(""));
    QVERIFY(objectRegister->findChild<ObjectRegisterAdaptor*>(""));

    QVERIFY(!bus.objectRegisteredAt("/ktutorial/EventSpy"));
}

void EditorSupportTest::testMainWindowObjectId() {
    EditorSupport editorSupport;
    QWidget window;
    editorSupport.setup(&window);

    int mainWindowObjectId = editorSupport.mainWindowObjectId();

    QCOMPARE(mainWindowObjectId,
             editorSupport.mObjectRegister->idForObject(&window));
}

void EditorSupportTest::testFindObject() {
    EditorSupport editorSupport;
    ObjectFinder objectFinder;
    editorSupport.setObjectFinder(&objectFinder);

    QWidget window;
    QObject* object = new QObject(&window);
    object->setObjectName("Object");
    QObject* childObject = new QObject(object);
    childObject->setObjectName("Child object");
    editorSupport.setup(&window);

    QCOMPARE(editorSupport.findObject("Child object"),
             editorSupport.mObjectRegister->idForObject(childObject));
}

void EditorSupportTest::testFindObjectWithEmptyName() {
    EditorSupport editorSupport;
    ObjectFinder objectFinder;
    editorSupport.setObjectFinder(&objectFinder);

    QWidget window;
    QObject* object = new QObject(&window);
    object->setObjectName("Object");
    QObject* childObject = new QObject(object);
    childObject->setObjectName("Child object");
    editorSupport.setup(&window);

    QCOMPARE(editorSupport.findObject(""),
             editorSupport.mObjectRegister->idForObject(0));
}

void EditorSupportTest::testFindObjectWithUnknownName() {
    EditorSupport editorSupport;
    ObjectFinder objectFinder;
    editorSupport.setObjectFinder(&objectFinder);

    QWidget window;
    QObject* object = new QObject(&window);
    object->setObjectName("Object");
    QObject* childObject = new QObject(object);
    childObject->setObjectName("Child object");
    editorSupport.setup(&window);

    QCOMPARE(editorSupport.findObject("Unknown object"),
             editorSupport.mObjectRegister->idForObject(0));
}

void EditorSupportTest::testHighlight() {
    EditorSupport editorSupport;
    QWidget window;
    editorSupport.setup(&window);

    QWidget* widget = new QWidget(&window);

    int widgetId = editorSupport.mObjectRegister->idForObject(widget);
    editorSupport.highlight(widgetId);

    QCOMPARE(widget->findChildren<WidgetHighlighter*>().count(), 1);
    QVERIFY(widget->findChild<WidgetHighlighter*>());
}

void EditorSupportTest::testStopHighlighting() {
    EditorSupport editorSupport;
    QWidget window;
    editorSupport.setup(&window);

    QWidget* widget = new QWidget(&window);

    int widgetId = editorSupport.mObjectRegister->idForObject(widget);
    editorSupport.highlight(widgetId);
    editorSupport.stopHighlighting(widgetId);

    QCOMPARE(widget->findChildren<WidgetHighlighter*>().count(), 0);
}

void EditorSupportTest::testEnableEventSpy() {
    QDBusConnection bus = QDBusConnection::sessionBus();
    QVERIFY(bus.isConnected());

    EditorSupport editorSupport;
    QObject mainObject;
    editorSupport.setup(&mainObject);
    editorSupport.enableEventSpy();

    QDBusInterface* iface = new QDBusInterface(bus.baseService(),
                                               "/ktutorial/EventSpy", "",
                                               bus, this);
    QVERIFY(iface->isValid());

    connect(iface, SIGNAL(eventReceived(int,QString)),
            this, SLOT(handleEventReceived(int,QString)));

    QObject* eventSpyObject =  bus.objectRegisteredAt("/ktutorial/EventSpy");
    QVERIFY(eventSpyObject);
    QVERIFY(qobject_cast<EventSpy*>(eventSpyObject));

    //Send an event not managed by QObject to avoid messing up its internal
    //state
    QEvent event(QEvent::Show);
    QApplication::sendEvent(&mainObject, &event);

    //Give D-Bus time to deliver the signal
    QTest::qWait(100);

    QCOMPARE(mEventTypes.count(), 1);
    QCOMPARE(mEventTypes.at(0), QString("Show"));
}

void EditorSupportTest::testDisableEventSpy() {
    QDBusConnection bus = QDBusConnection::sessionBus();
    QVERIFY(bus.isConnected());

    EditorSupport editorSupport;
    QObject mainObject;
    editorSupport.setup(&mainObject);
    editorSupport.enableEventSpy();

    QDBusInterface* iface = new QDBusInterface(bus.baseService(),
                                               "/ktutorial/EventSpy", "",
                                               bus, this);
    QVERIFY(iface->isValid());

    connect(iface, SIGNAL(eventReceived(int,QString)),
            this, SLOT(handleEventReceived(int,QString)));

    editorSupport.disableEventSpy();

    QVERIFY(!bus.objectRegisteredAt("/ktutorial/EventSpy"));

    //Send an event not managed by QObject to avoid messing up its internal
    //state
    QEvent event(QEvent::Show);
    QApplication::sendEvent(&mainObject, &event);

    //Give D-Bus time to deliver the signal (although nothing should be
    //delivered)
    QTest::qWait(100);

    QCOMPARE(mEventTypes.count(), 0);
}

void EditorSupportTest::testTestScriptedTutorial() {
    KTemporaryFile temporaryFile;
    temporaryFile.setSuffix(".js");
    temporaryFile.open();

    QTextStream out(&temporaryFile);
    out << "tutorial.tutorialInformationAsObject().setName(\
\"Test tutorial\");\n";
    out << "tutorial.addStep(ktutorial.newStep(\"start\"))";
    out.flush();

    EditorSupport editorSupport;

    //Tutorial* must be registered in order to be used with QSignalSpy
    int tutorialStarType = qRegisterMetaType<Tutorial*>("Tutorial*");
    QSignalSpy startedSpy(&editorSupport, SIGNAL(started(Tutorial*)));

    editorSupport.testScriptedTutorial(temporaryFile.fileName());

    QCOMPARE(startedSpy.count(), 1);
    QVariant argument = startedSpy.at(0).at(0);
    QCOMPARE(argument.userType(), tutorialStarType);

    Tutorial* tutorial = qvariant_cast<Tutorial*>(argument);
    QVERIFY(tutorial->tutorialInformation());
    QCOMPARE(tutorial->tutorialInformation()->id(),
             temporaryFile.fileName());
    QCOMPARE(tutorial->tutorialInformation()->name(), QString("Test tutorial"));

    QVERIFY(tutorial->mCurrentStep);
    QCOMPARE(tutorial->mCurrentStep->id(), QString("start"));

    QSignalSpy destroyedSpy(tutorial, SIGNAL(destroyed()));

    //Process deleteLater()
    QCoreApplication::sendPostedEvents(tutorial, QEvent::DeferredDelete);

    //Ensure that the tutorial is not deleted before explicitly calling finish
    //(for example, if the test tutorial written in the text stream does not
    //have a start step)
    QCOMPARE(destroyedSpy.count(), 0);

    tutorial->finish();

    //Process deleteLater()
    QCoreApplication::sendPostedEvents(tutorial, QEvent::DeferredDelete);

    QCOMPARE(destroyedSpy.count(), 1);
}

void EditorSupportTest::testTestScriptedTutorialWithStepId() {
    KTemporaryFile temporaryFile;
    temporaryFile.setSuffix(".js");
    temporaryFile.open();

    QTextStream out(&temporaryFile);
    out << "tutorial.tutorialInformationAsObject().setName(\
\"Test tutorial\");\n";
    out << "tutorial.addStep(ktutorial.newStep(\"start\"));\n";
    out << "tutorial.addStep(ktutorial.newStep(\"second step\"));\n";
    out << "tutorial.addStep(ktutorial.newStep(\"third step\"));\n";
    out.flush();

    EditorSupport editorSupport;

    //Tutorial* must be registered in order to be used with QSignalSpy
    int tutorialStarType = qRegisterMetaType<Tutorial*>("Tutorial*");
    QSignalSpy startedSpy(&editorSupport, SIGNAL(started(Tutorial*)));

    editorSupport.testScriptedTutorial(temporaryFile.fileName(), "third step");

    QCOMPARE(startedSpy.count(), 1);
    QVariant argument = startedSpy.at(0).at(0);
    QCOMPARE(argument.userType(), tutorialStarType);

    Tutorial* tutorial = qvariant_cast<Tutorial*>(argument);
    QVERIFY(tutorial->tutorialInformation());
    QCOMPARE(tutorial->tutorialInformation()->id(),
             temporaryFile.fileName());
    QCOMPARE(tutorial->tutorialInformation()->name(), QString("Test tutorial"));

    QVERIFY(tutorial->mCurrentStep);
    QCOMPARE(tutorial->mCurrentStep->id(), QString("third step"));

    QSignalSpy destroyedSpy(tutorial, SIGNAL(destroyed()));

    //Process deleteLater()
    QCoreApplication::sendPostedEvents(tutorial, QEvent::DeferredDelete);

    //Ensure that the tutorial is not deleted before explicitly calling finish
    //(for example, if the test tutorial written in the text stream does not
    //have a start step)
    QCOMPARE(destroyedSpy.count(), 0);

    tutorial->finish();

    //Process deleteLater()
    QCoreApplication::sendPostedEvents(tutorial, QEvent::DeferredDelete);

    QCOMPARE(destroyedSpy.count(), 1);
}

void EditorSupportTest::testTestScriptedTutorialWithInvalidTutorial() {
    KTemporaryFile temporaryFile;
    temporaryFile.setSuffix(".js");
    temporaryFile.open();

    QTextStream out(&temporaryFile);
    out << "tutorial.tutorialInformationAsObject().setName(\
\"Test tutorial\");\n";
    out << "Just a bunch of text to make the script invalid\n";
    out.flush();

    EditorSupport editorSupport;

    //Tutorial* must be registered in order to be used with QSignalSpy
    qRegisterMetaType<Tutorial*>("Tutorial*");
    QSignalSpy startedSpy(&editorSupport, SIGNAL(started(Tutorial*)));

    editorSupport.testScriptedTutorial(temporaryFile.fileName());

    QCOMPARE(startedSpy.count(), 0);
}

//Not really a unit test, but sort of an integration test for a strange bug.
void EditorSupportTest::
        testObjectRegisterWhenEventSpyNotifiesEventBeforeEndingConstructor() {
    QDBusConnection bus = QDBusConnection::sessionBus();
    QVERIFY(bus.isConnected());

    EditorSupport editorSupport;
    QWidget mainWidget;

    editorSupport.setup(&mainWidget);
    editorSupport.enableEventSpy();

    SubSubClassWidget* childWidget = new SubSubClassWidget(&mainWidget);

    QCOMPARE(childWidget->metaObject()->className(),
             "SubSubClassWidget");
    QCOMPARE(childWidget->metaObject()->superClass()->className(),
             "SubClassWidget");
    QCOMPARE(childWidget->metaObject()->superClass()->superClass()->className(),
             "QWidget");

    //childWidget gets registered by a ParentChange event. However, that event
    //is sent by QWidget constructor itself. The object is not fully
    //constructed, so SubClassWidget and SubSubClassWidget are not registered.
    ObjectRegister* objectRegister = editorSupport.mObjectRegister;
    QCOMPARE(objectRegister->objectForId(1), &mainWidget);
    QCOMPARE(objectRegister->objectForId(2), childWidget);
    QVERIFY(objectRegister->metaObjectForClassName("QWidget"));
    QVERIFY(!objectRegister->metaObjectForClassName("SubClassWidget"));
    QVERIFY(!objectRegister->metaObjectForClassName("SubSubClassWidget"));

    //From the outside (the caller DBus client), the only way to get an object
    //class is asking the ObjectRegister adaptor about it. So if a class is
    //registered after its name was returned by the ObjectRegister adaptor, when
    //the client asks for information about that class the register will be able
    //to provide it.
    ObjectRegisterAdaptor* objectRegisterAdaptor =
                            objectRegister->findChild<ObjectRegisterAdaptor*>();

    QCOMPARE(objectRegisterAdaptor->className(2), QString("SubSubClassWidget"));
    QVERIFY(objectRegister->metaObjectForClassName("SubClassWidget"));
    QVERIFY(objectRegister->metaObjectForClassName("SubSubClassWidget"));
}

}
}

QTEST_MAIN(ktutorial::editorsupport::EditorSupportTest)

#include "EditorSupportTest.moc"
