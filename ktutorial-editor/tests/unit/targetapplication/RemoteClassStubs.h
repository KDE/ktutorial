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

#ifndef REMOTECLASSSTUBS_H
#define REMOTECLASSSTUBS_H

#include <QApplication>
#include <QObject>
#include <QtDBus/QtDBus>

class StubEventSpy: public QObject {
Q_OBJECT
Q_CLASSINFO("D-Bus Interface", "org.kde.ktutorial.EventSpy")
public:

    StubEventSpy(QObject* parent = 0): QObject(parent) {
    }

    void emitEventReceived(int objectId, const QString& eventType) {
        emit eventReceived(objectId, eventType);
    }

signals:

    void eventReceived(int objectId, const QString& eventType);

};

class StubClassRegisterAdaptor: public QDBusAbstractAdaptor {
Q_OBJECT
Q_CLASSINFO("D-Bus Interface", "org.kde.ktutorial.ClassRegister")
public:

    StubClassRegisterAdaptor(QObject* parent): QDBusAbstractAdaptor(parent) {
    }

public slots:

    QString superClass(const QString& className) {
        if (className.startsWith("Child")) {
            return className.mid(QString("Child").count());
        }

        return "";
    }

    QStringList propertyList(const QString& className) {
        QStringList propertyList;
        for (int i=0; i<3; ++i) {
            propertyList.append(className + "Property" + QString::number(i));
        }

        return propertyList;
    }

    QStringList signalList(const QString& className) {
        QStringList signalList;
        for (int i=0; i<3; ++i) {
            signalList.append(className + "Signal" + QString::number(i) + "()");
        }

        return signalList;
    }
};

/**
 * ...
 *  |-42: "The object name 42" 
 *    |-420: "The object name 420"
 *    |-421: "The object name 421"
 *    |-422: "The object name 422"
 *    |-423: "The object name 423"
 *    |-5: "Duplicated grandparent"
 *    | |-50: "The object name 50"
 *    | | |-500: "Duplicated object"
 *    | | |...
 *    | |...
 *    |-6: "Duplicated grandparent"
 *    | |-60: "Duplicated parent"
 *    | | |-600: "Duplicated object"
 *    | | |...
 *    | |...
 *    |-7: "The object name 7"
 *    | |-70: "Duplicated parent"
 *    | | |-700: "Duplicated object"
 *    | | |...
 *    | |...
 *    |-8: "The object name 8"
 *    | |-80: "" (The class name 80)
 *    | | |-800: "Duplicated object"
 *    | | |-801: "The object name 801"
 *    | | |-802: ""
 *    | | |-803: "The object name 803"
 *    | |-81: "" (QWidgetChildChild)
 *    | | |-810: ""
 *    | | |-811: ""
 *    | | |-812: ""
 *    | | |-813: ""
 *    | |-82: "The object name 82" (ChildChildQWidget)
 *    | | |-820: "The object name 820"
 *    | | |...
 *    | |-83: "The object name 83" (The class name 83)
 *    |   |-830: "The object name 830" (ChildQWidget)
 *    |   |...
 *    |-9: "Duplicated grandparent"
 *      |-90: "The object name 90"
 *      | |-900: "Duplicated object"
 *      | |-901: "Duplicated object"
 *      | |...
 *      |-91: "Another duplicated parent"
 *      | |-910: "Duplicated object"
 *      | |...
 *      |-92: "Another duplicated parent"
 *      | |-920: "Duplicated object"
 *      | |...
 *      |-93: ""
 *        |-930: "Duplicated object"
 *        |-931: "Duplicated object"
 *        |...
 */
class StubObjectRegisterAdaptor: public QDBusAbstractAdaptor {
Q_OBJECT
Q_CLASSINFO("D-Bus Interface", "org.kde.ktutorial.ObjectRegister")
public:

    StubObjectRegisterAdaptor(QObject* parent): QDBusAbstractAdaptor(parent) {
    }

public slots:

    QString objectName(int objectId) {
        if (objectId == 500 || objectId == 600 || objectId == 700 ||
            objectId == 800 || objectId == 900 || objectId == 901 ||
            objectId == 910 || objectId == 920 || objectId == 930 ||
            objectId == 931) {
            return "Duplicated object";
        }

        if (objectId == 60 || objectId == 70) {
            return "Duplicated parent";
        }

        if (objectId == 91 || objectId == 92) {
            return "Another duplicated parent";
        }

        if (objectId == 5 || objectId == 6 || objectId == 9) {
            return "Duplicated grandparent";
        }

        if (objectId == 80 || objectId == 81 || objectId == 802 ||
            objectId == 810 || objectId == 811 || objectId == 812 ||
            objectId == 813 || objectId == 93) {
            return "";
        }

        if (objectId > 1000) {
            return "";
        }

        return "The object name " + QString::number(objectId);
    }

    QString className(int objectId) {
        if (objectId == 81 || objectId == 82) {
            return "ChildChildQWidget";
        }

        if (objectId == 830) {
            return "ChildQWidget";
        }

        if (objectId > 1000) {
            return "";
        }

        return "The class name " + QString::number(objectId);
    }

    QList<int> childObjectIds(int objectId) {
        if (objectId > 99) {
            return QList<int>();
        }

        QList<int> ids;
        for (int i=0; i<4; i++) {
            ids.append(objectId * 10 + i);
        }

        if (objectId == 42) {
            ids.append(5);
            ids.append(6);
            ids.append(7);
            ids.append(8);
            ids.append(9);
        }

        return ids;
    }
};

//Only one Q_CLASSINFO("D-Bus Interface", "whatever") is supported in
//each class, so adaptors have to be used to represent several interfaces
class StubObjectRegister: public QObject {
Q_OBJECT
public:

    StubObjectRegister(QObject* parent = 0): QObject(parent) {
        new StubClassRegisterAdaptor(this);
        new StubObjectRegisterAdaptor(this);
    }

};

class StubEditorSupport: public QObject {
Q_OBJECT
Q_CLASSINFO("D-Bus Interface", "org.kde.ktutorial.EditorSupport")
public:

    StubEventSpy* mEventSpy;
    QList<int> mHighlightRemoteWidgetIds;
    QList<int> mStopHighlightingRemoteWidgetIds;
    int mEnableEventSpyCount;
    int mDisableEventSpyCount;
    QList<QString> mTestScriptedTutorialFilenames;
    QList<QString> mTestScriptedTutorialStepIds;

    StubEditorSupport(QObject* parent = 0): QObject(parent),
        mEventSpy(0),
        mEnableEventSpyCount(0),
        mDisableEventSpyCount(0) {
    }

public slots:

    int mainWindowObjectId() const {
        return 42;
    }

    int findObject(const QString& name) {
        //Used in RemoteEditorSupportTest
        if (name == "The object name 423") {
            return 423;
        }

        //Used in WaitForPropertyWidgetTest
        if (name == "The object name 830") {
            return 830;
        }

        //Used in RemoteObjectNameRegisterTest/RemoteObjectNameWidgetTest
        if (name == "The object name 50/Duplicated object") {
            return 500;
        }

        if (name == "The object name 501") {
            return 501;
        }

        if (name == "Duplicated grandparent/Duplicated parent/"
                    "Duplicated object") {
            return 600;
        }

        if (name == "The object name 7/Duplicated object") {
            return 700;
        }

        if (name == "The object name 90/Duplicated object") {
            return 900;
        }

        if (name == "Duplicated grandparent/Another duplicated parent/"
                    "Duplicated object") {
            return 910;
        }

        if (name == "Duplicated grandparent/Duplicated object") {
            return 930;
        }

        return 0;
    }

    void highlight(int objectId) {
        mHighlightRemoteWidgetIds.append(objectId);
    }

    void stopHighlighting(int objectId) {
        mStopHighlightingRemoteWidgetIds.append(objectId);
    }

    void enableEventSpy() {
        mEnableEventSpyCount++;

        mEventSpy = new StubEventSpy(this);
        QDBusConnection::sessionBus().registerObject("/ktutorial/EventSpy",
                                mEventSpy, QDBusConnection::ExportAllSignals);
    }

    void disableEventSpy() {
        mDisableEventSpyCount++;

        QDBusConnection::sessionBus().unregisterObject("/ktutorial/EventSpy");
    }

    void testScriptedTutorial(const QString& filename,
                              const QString& stepId = QString()) {
        mTestScriptedTutorialFilenames.append(filename);

        if (!stepId.isEmpty()) {
            mTestScriptedTutorialStepIds.append(stepId);
        }
    }

};

#endif
