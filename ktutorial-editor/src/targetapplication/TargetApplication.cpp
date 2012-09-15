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

#include "TargetApplication.h"

#include <QtDBus/QtDBus>

#include <KProcess>
#include <KUrl>

#include "RemoteEditorSupport.h"
#include "RemoteObjectMapper.h"

//public:

TargetApplication* TargetApplication::self() {
    return sSelf;
}

TargetApplication::~TargetApplication() {
    if (mProcess) {
        mProcess->kill();
        mProcess->waitForFinished(100);
    }
}

QString TargetApplication::targetApplicationFilePath() const {
    return mTargetApplicationFilePath;
}

void TargetApplication::setTargetApplicationFilePath(
                                    const QString& targetApplicationFilePath) {
    KUrl url(targetApplicationFilePath);
    url.cleanPath();

    if (url.isRelative()) {
        mTargetApplicationFilePath = "";
    } else {
        mTargetApplicationFilePath = url.toLocalFile();
    }
}

RemoteEditorSupport* TargetApplication::remoteEditorSupport() {
    return mRemoteEditorSupport;
}

void TargetApplication::start() {
    if (mProcess && mProcess->program()[0] == mTargetApplicationFilePath) {
        return;
    }

    if (!QDBusConnection::sessionBus().isConnected()) {
        emit startFailed(NoDBusConnection);
        return;
    }

    if (mProcess) {
        mProcess->kill();
        //Don't start the new target application until the old one is dead.
        //It may freeze the editor GUI, but it is unlikely that a process gets
        //enough time being killed to be noticed
        mProcess->waitForFinished(-1);
    }

    mProcess = new KProcess(this);
    mProcess->setProgram(mTargetApplicationFilePath);

    mServiceName.clear();

    QDBusConnectionInterface* interface =
                                    QDBusConnection::sessionBus().interface();
    connect(interface, SIGNAL(serviceRegistered(QString)),
            this, SLOT(checkNewService(QString)));
    connect(interface, SIGNAL(serviceOwnerChanged(QString,QString,QString)),
            this, SLOT(checkNewService(QString)));

    connect(mProcess, SIGNAL(error(QProcess::ProcessError)),
            this, SLOT(handleProcessError(QProcess::ProcessError)));

    connect(mProcess, SIGNAL(finished(int)),
            this, SLOT(clean()));

    mKTutorialSupportModuleDiscoveryTimer.setInterval(3000);
    mKTutorialSupportModuleDiscoveryTimer.setSingleShot(true);
    connect(mProcess, SIGNAL(started()),
            &mKTutorialSupportModuleDiscoveryTimer, SLOT(start()));

    mProcess->start();
}

//private:

TargetApplication* TargetApplication::sSelf = new TargetApplication();

TargetApplication::TargetApplication():
    mProcess(0),
    mMapper(0),
    mRemoteEditorSupport(0) {

    connect(&mKTutorialSupportModuleDiscoveryTimer, SIGNAL(timeout()),
            this, SLOT(handleTargetApplicationDoesNotSupportKTutorial()));
}

//private slots:

void TargetApplication::checkNewService(const QString& service) {
    //Do not check new services if there is no target application running. The
    //slot may still be connected if no service was found for the target
    //application (for example, if it failed to start, if it did not use D-Bus,
    //if it was closed before checking the service...)
    if (!mProcess) {
        disconnect(QDBusConnection::sessionBus().interface(), 0, this, 0);
        return;
    }

    QDBusInterface mainDBusInterface("org.freedesktop.DBus", "/",
                                     "org.freedesktop.DBus");
    if (!mainDBusInterface.isValid()) {
        return;
    }

    QDBusReply<uint> pidReply =
                mainDBusInterface.call("GetConnectionUnixProcessID", service);
    if (!pidReply.isValid()) {
        return;
    }

    if (pidReply.value() != (uint)mProcess->pid()) {
        return;
    }

    disconnect(QDBusConnection::sessionBus().interface(), 0, this, 0);

    mServiceName = service;

    checkKTutorialSupportModule();
}

void TargetApplication::checkKTutorialSupportModule() {
    //Prevent pending checks from being executed after the timeout of the
    //discovery timer
    if (!mKTutorialSupportModuleDiscoveryTimer.isActive()) {
        return;
    }

    QDBusInterface interface(mServiceName, "/ktutorial",
                             "org.freedesktop.DBus.Introspectable");
    if (!interface.isValid()) {
        QTimer::singleShot(500, this, SLOT(checkKTutorialSupportModule()));
        return;
    }

    //Just a generic DBus call to check whether /ktutorial is already available
    //or not
    QDBusReply<QString> reply = interface.call("Introspect");
    if (!reply.isValid() ||
        !reply.value().contains("org.kde.ktutorial.EditorSupport")) {
        QTimer::singleShot(500, this, SLOT(checkKTutorialSupportModule()));
        return;
    }

    mKTutorialSupportModuleDiscoveryTimer.stop();

    mMapper = new RemoteObjectMapper(mServiceName);
    mRemoteEditorSupport = new RemoteEditorSupport(mServiceName, mMapper);

    emit started();
}

void TargetApplication::handleProcessError(QProcess::ProcessError error) {
    if (error != QProcess::FailedToStart) {
        return;
    }

    mKTutorialSupportModuleDiscoveryTimer.stop();

    mProcess->deleteLater();
    mProcess = 0;

    emit startFailed(InvalidPath);
}

void TargetApplication::handleTargetApplicationDoesNotSupportKTutorial() {
    //The target application not using KTutorial may have been closed by the
    //user before the timeout expired and this slot was called
    if (!mProcess) {
        return;
    }

    mProcess->kill();
}

void TargetApplication::clean() {
    bool wasStartedSuccessfully = true;
    if (!mRemoteEditorSupport) {
        wasStartedSuccessfully = false;

        emit startFailed(InvalidApplication);
    }

    //Don't delete it directly, as waitForFinished crashes somewhere internally
    //due to an event problem
    mProcess->deleteLater();
    mProcess = 0;
    delete mRemoteEditorSupport;
    mRemoteEditorSupport = 0;
    delete mMapper;
    mMapper = 0;

    if (wasStartedSuccessfully) {
        emit finished();
    }
}
