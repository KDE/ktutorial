/***************************************************************************
 *   Copyright (C) 2008 by Daniel Calviño Sánchez <danxuliu@gmail.com>     *
 *   Copyright (C) 2009 by Daniel Calviño Sánchez <danxuliu@gmail.com>     *
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

#ifndef KTUTORIAL_KTUTORIAL_H
#define KTUTORIAL_KTUTORIAL_H

#include <QtGui/QWidget>

#include "ktutorial_export.h"

#include "ObjectFinder.h"
#include "TutorialManager.h"

class KXmlGuiWindow;

namespace ktutorial {
class KTutorialCustomization;
}

namespace ktutorial {

/**
 * Main class of KTutorial.
 * This class is used to setup KTutorial, register WaitFor classes to be created
 * from scripts, register new Tutorials embedded in the application and find
 * objects in the Tutorials. It uses a Singleton design pattern.
 *
 * When using KTutorial and no customization is needed,
 * KTutorial::setup(KxmlGuiWindow*) must be called to set up everything needed
 * for KTutorial to work. It needs to know the main window of the application.
 *
 * If a KTutorial customization is needed,
 * KTutorial::setup(KTutorialCustomization*) must be called instead providing
 * the desired customization.
 * 
 * The setup method will be the first one to be called, but when custom WaitFor
 * objects are used in scripts. In that case, all the needed WaitFor classes
 * must be registered using registerWaitForMetaObject(const QMetaObject&) before
 * calling setup(KXmlGuiWindow*), as that method creates and registers all the
 * valid scripted tutorials.
 *
 * Once KTutorial is set up, Tutorials embedded in the application can be added.
 * Just use KTutorial::registerTutorial(Tutorial*) so it is registered in the
 * system. Only registered tutorial are seen by the user.
 *
 * Note, however, that when a customization is used, the WaitFor classes and the
 * tutorials embedded in the application can be registered during the
 * customization setup itself.
 *
 * When no customization is used, the TutorialManagerDialog is shown as modal
 * through the "tutorials" action in the "Help" menu. This action will be
 * automatically disabled whenever a tutorial is being executed.
 *
 * Finally, no matter whether a customization is used or not, any of the
 * children objects of the main window of the application (which will usually be
 * at least all widgets) can be got with KTutorial::findObject(const QString&),
 * provided the looked for object has its object name set.
 */
class KTUTORIAL_EXPORT KTutorial: public QObject {
Q_OBJECT
public:

    /**
     * Returns the only instance of this class.
     *
     * @return The only instance of this class.
     */
    static KTutorial* self();

    /**
     * Registers a WaitFor type to create instances of it in scripted tutorials.
     * Any WaitFor class has to be registered before calling
     * setup(KXmlGuiWindow*).
     *
     * @param waitForMetaObject The QMetaObject to create instances of.
     * @param typeName A specific name to be used for the QMetaObject.
     * @return True if the type was successfully registered, false otherwise.
     * @see ScriptingModule::registerWaitForMetaObject(const QMetaObject&)
     */
    bool registerWaitForMetaObject(const QMetaObject& waitForMetaObject,
                                   const QString& typeName = QString());

    /**
     * Sets up everything for KTutorial to work with the default customization.
     * If no customization is needed, this is usually the first method that must
     * be called when using KTutorial in an application. However, if any WaitFor
     * class has to be registered, it must be done before calling this method.
     *
     * It adds the Tutorials action and the menu entry for it in Help menu and
     * loads the scripted tutorials from the application standard directories.
     *
     * @param window The main window of the application.
     */
    void setup(KXmlGuiWindow* window);

    /**
     * Sets up everything for KTutorial to work with the given customization.
     * After setting up the customization, it loads the scripted tutorials from
     * the application standard directories.
     * 
     * @param ktutorialCustomization The customization to use.
     */
    void setup(KTutorialCustomization* ktutorialCustomization);

    /**
     * Registers the Tutorial.
     * Only Tutorials with a identifier not added yet can be added. If the
     * Tutorial couldn't be added, false is returned.
     *
     * The Tutorial is reparented and thus deleted when needed.
     *
     * @param tutorial The Tutorial to register.
     * @return True if the Tutorial was registered, false otherwise.
     */
    bool registerTutorial(Tutorial* tutorial);

    /**
     * Returns the main window of the application.
     *
     * @return The main window of the application.
     */
    QWidget* mainApplicationWindow() const;

    /**
     * Returns the object with the specified name, if any.
     * Objects are searched in the children of the main window of the
     * application.
     * 
     * When the name of the desired object is not unique the name of some
     * ancestor must be included. Ancestor names are separated using a "/". That
     * is, "Ancestor name/The object with a repeated name". As many ancestor
     * names as desired can be included, not just one.
     * The name of the ancestors does not need to be unique either; what has to
     * be unique is the full path to the object to find. For example, "Ancestor
     * name not unique/The object to find" is valid if, among all the objects
     * called "Ancestor name not unique", there is only one that has a
     * descendant called "The object to find".
     * 
     * In some cases it may be possible that a unique name can not be provided,
     * even using ancestor names. In those cases there are some rules to resolve
     * the ambiguity. Between two or more objects with the same name, the
     * selected one is:
     * -A direct child of the base object.
     * -A nested child without named ancestors between it and the base object.
     *   -If there is more than one, the one nearer to the base object (that is,
     *    with the lesser number of intermediate ancestors).
     * -A nested child with named or unnamed ancestors between it and the base
     * object.
     *   -If there is more than one, the one nearer to the base object (that is,
     *    with the lesser number of intermediate ancestors).
     * 
     * If the ambiguous name contains ancestor names the rules are applied for
     * each component of the name. Each component is searched for only from the
     * already selected previous components. That is, if the ambiguous name is
     * "Ancestor name/The object", the rules are applied to find the objects
     * named "Ancestor name" using the main window as base object. Then, the
     * rules are applied to find "The object" using the "Ancestor name" objects
     * as base, so "The object" is searched only in the children of the
     * "Ancestor name" objects previously found. Note that, even with the
     * ambiguity resolving rules, it may be searched for from several "Ancestor
     * name" if, for example, there are several "Ancestor name" child of the
     * main window. However, if there were other "Ancestor name" objects that
     * were not direct children of the main window they will not be used in the
     * search, as the direct children would have taken precedence.
     * 
     * If after applying the rules there is still more than one object that
     * matches, the first one is selected.
     *
     * @param name The name of the object to find.
     * @return The object with the specified name, or null if there is none.
     * @todo Is it needed to look in other parent objects apart of the main
     *       window?
     */
    template <typename T>
    T findObject(const QString& name) const {
        return mObjectFinder->findObject<T>(name, mainApplicationWindow());
    }

private:

    /**
     * The instance of this class.
     */
    static KTutorial* sSelf;

    /**
     * The manager for the tutorials.
     */
    TutorialManager* mTutorialmanager;

    /**
     * The helper used to find objects.
     */
    ObjectFinder* mObjectFinder;

    /**
     * The KTutorialCustomization used.
     */
    KTutorialCustomization* mCustomization;

    /**
     * Creates a new KTutorial.
     * Private to avoid classes other than self to create instances.
     */
    KTutorial():
        mTutorialmanager(new TutorialManager()) {
        mTutorialmanager->setParent(this);
        mObjectFinder = new ObjectFinder(this);
        mCustomization = 0;
    }

};

}

#endif
