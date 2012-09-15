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

#ifndef TARGETAPPLICATION_H
#define TARGETAPPLICATION_H

#include <QObject>
#include <QProcess>
#include <QTimer>

class KProcess;
class RemoteEditorSupport;
class RemoteObjectMapper;

/**
 * Class to execute a target application and communicate with it through D-Bus.
 * The target application is the application that the tutorial will be part of.
 * Using TargetApplication class a new instance of a target application can be
 * executed and, through D-Bus, KTutorial editor can communicate with it to
 * introspect its structure. The target application must have KTutorial enabled
 * in order to use the KTutorial editor support module. Applications that do not
 * use KTutorial can not be introspected.
 *
 * Knowing the structure of the target application, KTutorial editor can provide
 * valuable information to the tutorial author like the name of an object or its
 * class, so the tutorial author can design the tutorial more easily.
 *
 * The TargetApplication can be started using method start(). The method returns
 * immediately, and when the application is really running the started() signal
 * is emitted (if it uses KTutorial). If the application could not be executed,
 * or it was executed but other problem happened (for example, if the target
 * application does not use KTutorial), startFailed(Error) signal is emitted
 * instead.
 *
 * Note that startFailed(Error) signal is also emitted if the target application
 * is closed too soon to check if it uses KTutorial.
 *
 * Once the target application has been started, remoteEditorSupport
 * returns a RemoteEditorSupport connected to the remote
 * "org.kde.ktutorial.EditorSupport" interface exposed by the target
 * application.
 *
 * The target application will be killed when the TargetApplication is
 * destroyed, or when the start method is called again after setting a different
 * application file path. In any of those cases, or if the target application
 * was closed externally (by the user), finished() signal is emitted (but only
 * if, preiously, it was successfully started).
 */
class TargetApplication: public QObject {
Q_OBJECT
public:

    /**
     * The type of error that made starting the target application fail.
     */
    enum Error {

        /**
         * The application was started, but it does not use KTutorial.
         */
        InvalidApplication,

        /**
         * The application was not even started.
         */
        InvalidPath,

        /**
         * The application was started, but there is no D-Bus connection to
         * communicate with it.
         */
        NoDBusConnection
    };

    /**
     * Returns the only instance of TargetApplication class.
     *
     * @return The TargetApplication instance.
     */
    static TargetApplication* self();

    /**
     * Destroys this TargetApplication.
     * If the target application is running, it is killed.
     */
    virtual ~TargetApplication();

    /**
     * Returns the executable for the target application.
     *
     * @return The target application file path.
     */
    QString targetApplicationFilePath() const;

    /**
     * Sets the executable for the target application.
     * The executable must be an absolute path. A relative path will be set as
     * an empty string. If the path is absolute but contains "." and ".."
     * components they are resolved and the clean path is set.
     *
     * @param targetApplicationFilePath The target application file path.
     */
    void setTargetApplicationFilePath(const QString& targetApplicationFilePath);

    /**
     * Returns the RemoteEditorSupport for the target application.
     * If the application is not running a null pointer is returned.
     *
     * @return The RemoteEditorSupport for the target application.
     */
    RemoteEditorSupport* remoteEditorSupport();

    /**
     * Starts a new TargetApplication.
     * When the target application is running, started() signal is emitted. If
     * there is no application file path, the application file path is not valid
     * or the application does not support KTutorial editor (or it was closed
     * before being able to determine it), startFailed(Error) signal is emitted
     * instead.
     *
     * If the target application was already started nothing is done (even
     * started() signal is not emitted).
     */
    void start();

Q_SIGNALS:

    /**
     * Emitted when the target application was started, it uses KTutorial and
     * there is a D-Bus connection to it.
     */
    void started();

    /**
     * Emitted when the target application could not be started successfully.
     *
     * @param error The type of error that happened.
     */
    void startFailed(TargetApplication::Error error);

    /**
     * Emitted when the target application was finished after being successfully
     * started.
     * Note that this signal is not emitted, for example, when a target
     * application without support for KTutorial is closed.
     */
    void finished();

private:

    /**
     * The only instance of TargetApplication.
     */
    static TargetApplication* sSelf;

    /**
     * The executable for the target application.
     */
    QString mTargetApplicationFilePath;

    /**
     * The process executing the target application, if any.
     */
    KProcess* mProcess;

    /**
     * Timer to give time to "/ktutorial" object to appear in the service of
     * the target application.
     * If the timer ends and the object was not found, the target application
     * seems to not to use KTutorial editor.
     */
    QTimer mKTutorialSupportModuleDiscoveryTimer;

    /**
     * The D-Bus service name provided by the target application, if any.
     */
    QString mServiceName;

    /**
     * The mapper that associates RemoteObjects with their object id.
     */
    RemoteObjectMapper* mMapper;

    /**
     * The RemoteEditorSupport for the target application, if any.
     */
    RemoteEditorSupport* mRemoteEditorSupport;

    /**
     * Creats a new TargetApplication.
     * Hidden to avoid classes other than TargetApplication itself to create
     * objects of it.
     */
    TargetApplication();

private Q_SLOTS:

    /**
     * Checks if the new service found in the session bus is the one from the
     * started target application.
     * If the service is provided by the target application, it is checked if it
     * contains the support module from KTutorial.
     *
     * @param service The D-Bus service to check.
     */
    void checkNewService(const QString& service);

    /**
     * Checks if the service of the target application provides the "/ktutorial"
     * object.
     * If the object is not found, the service will be checked again later.
     * When the object is found, the target application has started from
     * KTutorial editor point of view. Signal started() is emitted in that case.
     */
    void checkKTutorialSupportModule();

    /**
     * Checks if the process could not be started.
     * Signal startFailed(Error), with InvalidPath, is emitted in that case.
     *
     * @param error The process error.
     */
    void handleProcessError(QProcess::ProcessError error);

    /**
     * Called when the time to find the "/ktutorial" object expired.
     * The target application is killed (which causes signal startFailed(Error),
     * with InvalidApplication, to be emitted).
     */
    void handleTargetApplicationDoesNotSupportKTutorial();

    /**
     * Called when the process has finished.
     * The process, mapper and remote editor support are deleted.
     * startFailed(Error) and finished() signals are emitted, depending on the
     * case.
     */
    void clean();

};

#endif
