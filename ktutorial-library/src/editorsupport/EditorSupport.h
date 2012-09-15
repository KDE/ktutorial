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

#ifndef KTUTORIAL_EDITORSUPPORT_EDITORSUPPORT_H
#define KTUTORIAL_EDITORSUPPORT_EDITORSUPPORT_H

#include <QObject>

namespace ktutorial {
class ObjectFinder;
class Tutorial;
}

namespace ktutorial {
namespace editorsupport {
class EventSpy;
class EventSupportAdaptor;
class ObjectRegister;
}
}

namespace ktutorial {
namespace editorsupport {

/**
 * Support module for KTutorial editor to be accessed through D-Bus.
 * EditorSupport provides a way to introspect a running application from
 * KTutorial editor to help the author to design a tutorial.
 *
 * EditorSupport is composed by three elements: the main object (this class),
 * an object register and an event spy.
 *
 * The main object sets up the D-Bus objects and provides a way to enable and
 * disable the EventSpy (as notifying all the events sent by an application
 * through D-Bus is very costly, the EventSpy should be enabled only when
 * needed), highlight and stop the highlighting of widgets, test a scripted
 * tutorial (starting the tutorial stored in the given filename and, optionally,
 * from the given step id), and find objects.
 *
 * The object register assigns an id to QObjects to be identified by the remote
 * KTutorial editor. Using that id, KTutorial editor can request further
 * information about an object to the ObjectRegister (for example, the object
 * name or the class name of an object). Moreover, the object register also
 * provides information about the classes of the registered objects, like the
 * super class or the list of signals defined in each class.
 *
 * The event spy filters all the events received by the main window and,
 * recursively, all its children objects. It is used in the remote KTutorial
 * editor to know, for example, that some widget got the focus, or that the
 * mouse entered in some widget.
 *
 * The main object is registered at "/ktutorial" path, and provides the
 * "org.kde.ktutorial.EditorSupport" interface. The object register is
 * registered at "/ktutorial/ObjectRegister" path and provides the
 * "org.kde.ktutorial.ObjectRegister" and "org.kde.ktutorial.ClassRegister"
 * interfaces. Finally, when it is enabled, the EventSpy is registered at
 * "/ktutorial/EVentSpy" path and provides the "org.kde.ktutorial.EventSpy"
 * interface.
 */
class EditorSupport: public QObject {
Q_OBJECT
public:

    /**
     * Creates a new EditorSupport with the given parent.
     *
     * @param parent The parent QObject.
     */
    explicit EditorSupport(QObject* parent = 0);

    /**
     * Sets the helper used to find objects.
     * 
     * @param objectFinder The helper to set.
     */
    void setObjectFinder(ObjectFinder* objectFinder);

    /**
     * Exposes the editor support interfaces through DBus.
     * The window and all its children will be spied when EventSpy is enabled.
     *
     * @param window The main window in the application.
     */
    void setup(QObject* window);

    /**
     * Returns the object id of the application main window.
     *
     * @return The object id of the application main window.
     */
    int mainWindowObjectId();

    /**
     * Returns the id of the object with the given name.
     * The name can include ancestor names.
     * 
     * @param name The name of the object to find.
     * @return The id of the object with the given name.
     */
    int findObject(const QString& name);

    /**
     * Starts the highlighting animation for the widget associated to the given
     * id.
     *
     * @param objectId The id of the widget to highlight.
     */
    void highlight(int objectId);

    /**
     * Stops the highlighting animation for the widget associated to the given
     * id.
     *
     * @param objectId The id of the widget to stop highlighting.
     */
    void stopHighlighting(int objectId);

    /**
     * Enables the EventSpy.
     */
    void enableEventSpy();

    /**
     * Disables the EventSpy.
     */
    void disableEventSpy();

    /**
     * Starts the scripted tutorial stored in the given filename.
     * If a step id is given, the tutorial is changed to that step after
     * starting.
     *
     * @param filename The name of the file to read the scripted tutorial from.
     * @param stepId The id of the step to change to, if any.
     */
    void testScriptedTutorial(const QString& filename,
                              const QString& stepId = QString());

Q_SIGNALS:

    /**
     * This signals is emitted when the given tutorial is about to be started.
     *
     * @param tutorial The tutorial that is going to be started.
     */
    void started(Tutorial* tutorial);

private:

    /**
     * The object register.
     */
    ObjectRegister* mObjectRegister;

    /**
     * The event spy.
     */
    EventSpy* mEventSpy;

    /**
     * The object to spy its events and the events of its children.
     */
    QObject* mWindow;

    /**
     * The helper used to find objects.
     */
    ObjectFinder* mObjectFinder;

private Q_SLOTS:

    /**
     * Deletes the test tutorial when it is finished.
     *
     * @param tutorial The tutorial to delete.
     */
    void deleteFinishedTestTutorial(Tutorial* tutorial);

};

}
}

#endif
