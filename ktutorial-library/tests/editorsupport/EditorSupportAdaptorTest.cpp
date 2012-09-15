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

#include <QWidget>
#include <QtDBus/QtDBus>

#include <KTemporaryFile>

#include "EditorSupportAdaptor.h"

#define protected public
#define private public
#include "EditorSupport.h"
#include "../Tutorial.h"
#undef private
#undef protected

#include "EventSpy.h"
#include "ObjectRegister.h"
#include "../ObjectFinder.h"
#include "../Step.h"
#include "../extendedinformation/WidgetHighlighter.h"

using ktutorial::extendedinformation::WidgetHighlighter;

//Tutorial* must be declared as a metatype to be used in qvariant_cast
Q_DECLARE_METATYPE(ktutorial::Tutorial*);

namespace ktutorial {
namespace editorsupport {

class EditorSupportAdaptorTest: public QObject {
Q_OBJECT

private slots:

    void testConstructor();

    void testMainWindowObjectId();

    void testFindObject();

    void testHighlight();

    void testStopHighlighting();

    void testEnableEventSpy();

    void testDisableEventSpy();

    void testTestScriptedTutorial();
    void testTestScriptedTutorialWithStepId();

};

void EditorSupportAdaptorTest::testConstructor() {
    EditorSupport editorSupport;
    EditorSupportAdaptor* adaptor = new EditorSupportAdaptor(&editorSupport);

    QCOMPARE(adaptor->parent(), &editorSupport);
}

void EditorSupportAdaptorTest::testMainWindowObjectId() {
    EditorSupport editorSupport;
    QWidget window;
    editorSupport.setup(&window);
    EditorSupportAdaptor* adaptor = new EditorSupportAdaptor(&editorSupport);

    QCOMPARE(adaptor->mainWindowObjectId(), editorSupport.mainWindowObjectId());
}

void EditorSupportAdaptorTest::testFindObject() {
    EditorSupport editorSupport;
    ObjectFinder objectFinder;
    editorSupport.setObjectFinder(&objectFinder);

    QWidget window;
    QObject* object = new QObject(&window);
    object->setObjectName("Object");
    QObject* childObject = new QObject(object);
    childObject->setObjectName("Child object");
    editorSupport.setup(&window);

    EditorSupportAdaptor* adaptor = new EditorSupportAdaptor(&editorSupport);

    QCOMPARE(adaptor->findObject("Child object"),
             editorSupport.mObjectRegister->idForObject(childObject));
}

void EditorSupportAdaptorTest::testHighlight() {
    EditorSupport editorSupport;
    QWidget window;
    editorSupport.setup(&window);
    EditorSupportAdaptor* adaptor = new EditorSupportAdaptor(&editorSupport);

    adaptor->highlight(adaptor->mainWindowObjectId());

    QCOMPARE(window.findChildren<WidgetHighlighter*>().count(), 1);
    QVERIFY(window.findChild<WidgetHighlighter*>());
}

void EditorSupportAdaptorTest::testStopHighlighting() {
    EditorSupport editorSupport;
    QWidget window;
    editorSupport.setup(&window);
    EditorSupportAdaptor* adaptor = new EditorSupportAdaptor(&editorSupport);

    adaptor->highlight(adaptor->mainWindowObjectId());
    adaptor->stopHighlighting(adaptor->mainWindowObjectId());

    QCOMPARE(window.findChildren<WidgetHighlighter*>().count(), 0);
}

void EditorSupportAdaptorTest::testEnableEventSpy() {
    QDBusConnection bus = QDBusConnection::sessionBus();
    QVERIFY(bus.isConnected());

    EditorSupport editorSupport;
    QWidget window;
    editorSupport.setup(&window);
    EditorSupportAdaptor* adaptor = new EditorSupportAdaptor(&editorSupport);

    adaptor->enableEventSpy();

    QObject* spyObject = bus.objectRegisteredAt("/ktutorial/EventSpy");
    QVERIFY(spyObject);
    QVERIFY(qobject_cast<EventSpy*>(spyObject));
}

void EditorSupportAdaptorTest::testDisableEventSpy() {
    QDBusConnection bus = QDBusConnection::sessionBus();
    QVERIFY(bus.isConnected());

    EditorSupport editorSupport;
    QWidget window;
    editorSupport.setup(&window);
    EditorSupportAdaptor* adaptor = new EditorSupportAdaptor(&editorSupport);

    adaptor->enableEventSpy();
    adaptor->disableEventSpy();

    QVERIFY(!bus.objectRegisteredAt("/ktutorial/EventSpy"));
}

void EditorSupportAdaptorTest::testTestScriptedTutorial() {
    KTemporaryFile temporaryFile;
    temporaryFile.setSuffix(".js");
    temporaryFile.open();

    QTextStream out(&temporaryFile);
    out << "tutorial.tutorialInformationAsObject().setName(\
\"Test tutorial\");\n";
    out.flush();

    EditorSupport editorSupport;
    EditorSupportAdaptor* adaptor = new EditorSupportAdaptor(&editorSupport);

    //Tutorial* must be registered in order to be used with QSignalSpy
    qRegisterMetaType<Tutorial*>("Tutorial*");
    QSignalSpy startedSpy(&editorSupport, SIGNAL(started(Tutorial*)));

    adaptor->testScriptedTutorial(temporaryFile.fileName());

    QCOMPARE(startedSpy.count(), 1);
}

void EditorSupportAdaptorTest::testTestScriptedTutorialWithStepId() {
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
    EditorSupportAdaptor* adaptor = new EditorSupportAdaptor(&editorSupport);

    //Tutorial* must be registered in order to be used with QSignalSpy
    qRegisterMetaType<Tutorial*>("Tutorial*");
    QSignalSpy startedSpy(&editorSupport, SIGNAL(started(Tutorial*)));

    adaptor->testScriptedTutorial(temporaryFile.fileName(), "third step");

    QCOMPARE(startedSpy.count(), 1);
    QVariant argument = startedSpy.at(0).at(0);

    Tutorial* tutorial = qvariant_cast<Tutorial*>(argument);
    QVERIFY(tutorial->mCurrentStep);
    QCOMPARE(tutorial->mCurrentStep->id(), QString("third step"));
}

}
}

QTEST_MAIN(ktutorial::editorsupport::EditorSupportAdaptorTest)

#include "EditorSupportAdaptorTest.moc"
