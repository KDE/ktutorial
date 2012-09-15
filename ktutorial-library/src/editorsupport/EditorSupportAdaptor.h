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

#ifndef KTUTORIAL_EDITORSUPPORT_EDITORSUPPORTADAPTOR_H
#define KTUTORIAL_EDITORSUPPORT_EDITORSUPPORTADAPTOR_H

#include <QDBusAbstractAdaptor>

namespace ktutorial {
namespace editorsupport {
class EditorSupport;
class ObjectRegister;
}
}

namespace ktutorial {
namespace editorsupport {

/**
 * Adaptor to expose an EditorSupport through DBus.
 *
 * @see EditorSupport
 */
class EditorSupportAdaptor: public QDBusAbstractAdaptor {
Q_OBJECT
Q_CLASSINFO("D-Bus Interface", "org.kde.ktutorial.EditorSupport")
public:

    /**
     * Creates a new EditorSupportAdaptor for the given EditorSupport.
     *
     * @param editorSupport The EditorSupport to adapt.
     */
    explicit EditorSupportAdaptor(EditorSupport* editorSupport);

public Q_SLOTS:

    /**
     * Returns the object id of the application main window.
     *
     * @return The object id of the application main window.
     */
    int mainWindowObjectId() const;

    /**
     * Returns the id of the object with the given name.
     * The name can include ancestor names.
     * 
     * @param name The name of the object to find.
     * @return The id of the object with the given name, or 0 if there is none.
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

private:

    /**
     * The EditorSupport to adapt.
     */
    EditorSupport* mEditorSupport;

};

}
}

#endif
