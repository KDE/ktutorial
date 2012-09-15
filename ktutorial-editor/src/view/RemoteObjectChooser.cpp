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

#include "RemoteObjectChooser.h"
#include "ui_RemoteObjectChooser.h"

#include <QPushButton>

#include <KDebug>
#include <KDialogButtonBox>
#include <KMessageBox>

#include "RemoteObjectChooserFilterModel.h"
#include "RemoteObjectTreeItem.h"
#include "RemoteObjectTreeItemUpdater.h"
#include "RemoteObjectTreeSelectionManager.h"
#include "TreeModel.h"
#include "TargetApplicationView.h"
#include "../targetapplication/RemoteEditorSupport.h"
#include "../targetapplication/RemoteEventSpy.h"
#include "../targetapplication/RemoteObject.h"
#include "../targetapplication/TargetApplication.h"

//public:

RemoteObjectChooser::RemoteObjectChooser(QWidget* parent):
        QWidget(parent),
    mCurrentRemoteObject(0),
    mSuccessfullyStarted(false) {
    setWindowFlags(Qt::Window);
    Q_ASSERT(parent);

    ui = new Ui::RemoteObjectChooser();
    ui->setupUi(this);

    QPushButton* button = ui->dialogButtonBox->button(QDialogButtonBox::Ok);
    button->setObjectName("okButton");
    button->setEnabled(false);
    connect(button, SIGNAL(clicked(bool)), this, SLOT(accept()));

    button = ui->dialogButtonBox->button(QDialogButtonBox::Cancel);
    button->setObjectName("cancelButton");
    connect(button, SIGNAL(clicked(bool)), this, SLOT(cancel()));

    connect(TargetApplication::self(), SIGNAL(started()),
            this, SLOT(handleTargetApplicationStarted()));
    connect(TargetApplication::self(),
            SIGNAL(startFailed(TargetApplication::Error)),
            this, SLOT(cancel()));
    connect(TargetApplication::self(), SIGNAL(finished()),
            this, SLOT(handleTargetApplicationFinished()));

    if (TargetApplication::self()->remoteEditorSupport()) {
        handleTargetApplicationStarted();
    } else {
        TargetApplicationView* targetApplicationView =
                    new TargetApplicationView(TargetApplication::self(), this);
        targetApplicationView->start();
    }
}

RemoteObjectChooser::~RemoteObjectChooser() {
    if (mCurrentRemoteObject &&
            TargetApplication::self()->remoteEditorSupport()) {
        TargetApplication::self()->remoteEditorSupport()->
                                        stopHighlighting(mCurrentRemoteObject);
    }

    delete ui;
}

//protected:

void RemoteObjectChooser::closeEvent(QCloseEvent* event) {
    QWidget::closeEvent(event);
    showParentWindows(this);
    deleteLater();
}

//private:

void RemoteObjectChooser::hideParentWindows(QWidget* widget) {
    if (!widget || !widget->parentWidget()) {
        return;
    }

    if (widget->parentWidget()->windowFlags() & (Qt::Window | Qt::Dialog)) {
        widget->parentWidget()->hide();
    }

    hideParentWindows(widget->parentWidget());
}

void RemoteObjectChooser::showParentWindows(QWidget* widget) {
    if (!widget || !widget->parentWidget()) {
        return;
    }

    //Show them in inverse order, because showing first a dialog and then its
    //parent window would create an entry for the dialog and another entry for
    //the window in the task bar
    showParentWindows(widget->parentWidget());

    if (widget->parentWidget()->windowFlags() & (Qt::Window | Qt::Dialog)) {
        widget->parentWidget()->show();
    }
}

void RemoteObjectChooser::warnAboutFinishedTargetApplicationBeforeClosing() {
    QString text = i18nc("@label", "The target application has been closed, "
"but it must be running to be able to choose the objects.");
    QString caption = i18nc("@title:window", "Target application closed");
    KMessageBox::sorry(this, text, caption);

    close();
}

//private slots:

void RemoteObjectChooser::handleTargetApplicationStarted() {
    if (isHidden()) {
        //In Qt 4.7 a window seems to need to be shown to get a winId. If the
        //parent of a KMessageBox::information does not have a winId, the
        //message box gets no parent.
        show();
    }

    QString text = i18nc("@label", "<para>The "
"<application>KTutorial editor</application> windows will be hidden and only "
"the list with the objects in the target application will be shown. It is made "
"to better view when a widget is highlighted in the target application.</para>"
"<para>The <application>KTutorial editor</application> windows will be shown "
"again once the selection list is closed.</para>");
    QString caption = i18nc("@title:window", "<application>KTutorial "
"editor</application> windows will be hidden");
    KMessageBox::information(this, text, caption,
                             "KTutorialEditorWindowsWillBeHidden");

    RemoteEditorSupport* remoteEditorSupport =
                            TargetApplication::self()->remoteEditorSupport();

    //The target application may have been closed while the message box was
    //shown.
    if (!remoteEditorSupport) {
        warnAboutFinishedTargetApplicationBeforeClosing();
        return;
    }

    hideParentWindows(this);

    RemoteObject* mainWindow = remoteEditorSupport->mainWindow();
    RemoteObjectTreeItem* rootItem = new RemoteObjectTreeItem(mainWindow);
    TreeModel* treeModel = new TreeModel(rootItem, this);

    RemoteObjectChooserFilterModel* filterModel =
                                    new RemoteObjectChooserFilterModel(this);
    filterModel->setSourceModel(treeModel);
    ui->remoteObjectsTreeView->setModel(filterModel);

    connect(ui->showOnlyNamedObjectsCheckBox, SIGNAL(toggled(bool)),
            filterModel, SLOT(setNamedObjectFilterEnabled(bool)));
    connect(ui->showOnlyWidgetsCheckBox, SIGNAL(toggled(bool)),
            filterModel, SLOT(setWidgetFilterEnabled(bool)));

    RemoteObjectTreeItemUpdater* updater =
                                        new RemoteObjectTreeItemUpdater(this);
    rootItem->setUpdater(updater);

    try {
        updater->setRemoteEventSpy(remoteEditorSupport->enableEventSpy());
    } catch (DBusException e) {
        kWarning() << "Remote event spy could not be enabled in the target "
                   << "application. Children objects will not be added and "
                   << "removed automatically in the list to reflect the "
                   << "changes in the remote objects (" << e.message() << ").";
    }

    //Parent object is set to the selection model, so the manager is also
    //deleted when the selection model it watches is deleted
    QItemSelectionModel* selectionModel =
                                    ui->remoteObjectsTreeView->selectionModel();
    RemoteObjectTreeSelectionManager* selectionManager =
        new RemoteObjectTreeSelectionManager(selectionModel, selectionModel);
    connect(selectionManager, SIGNAL(remoteObjectSelected(RemoteObject*)),
            this, SLOT(setCurrentRemoteObject(RemoteObject*)));

    mSuccessfullyStarted = true;
}

void RemoteObjectChooser::handleTargetApplicationFinished() {
    //If the application is running but it was not successfully started (the
    //application does not have the KTutorial editor support module)
    //this RemoteObjectChooser is just closed. The TargetApplicationView takes
    //care of notifying the user about the problem.
    if (!mSuccessfullyStarted) {
        return;
    }

    warnAboutFinishedTargetApplicationBeforeClosing();
}

void RemoteObjectChooser::setCurrentRemoteObject(RemoteObject* remoteObject) {
    if (mCurrentRemoteObject && mCurrentRemoteObject != remoteObject) {
        TargetApplication::self()->remoteEditorSupport()->
                                        stopHighlighting(mCurrentRemoteObject);
    }

    mCurrentRemoteObject = remoteObject;

    if (mCurrentRemoteObject) {
        TargetApplication::self()->remoteEditorSupport()->
                                                highlight(mCurrentRemoteObject);
        ui->dialogButtonBox->button(QDialogButtonBox::Ok)->setEnabled(true);
    } else {
        ui->dialogButtonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
    }
}

void RemoteObjectChooser::accept() {
    Q_ASSERT(mCurrentRemoteObject);

    emit remoteObjectChosen(mCurrentRemoteObject);
    close();
}

void RemoteObjectChooser::cancel() {
    close();
}
