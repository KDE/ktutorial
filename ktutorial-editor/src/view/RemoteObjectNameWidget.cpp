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

#include "RemoteObjectNameWidget.h"
#include "ui_RemoteObjectNameWidget.h"

#include <KDebug>
#include <KMessageBox>

#include "RemoteObjectChooser.h"
#include "RemoteObjectNameRegister.h"
#include "../targetapplication/RemoteObject.h"
#include "../targetapplication/TargetApplication.h"

/**
 * Completion for remote object names.
 * The items added for completion should be the unqualified remote object names,
 * that is, just the name of the remote objects, without any ancestor. When a
 * match is found, and it represents more than one remote object, the match is
 * replaced by the best names of its remote objects. Note that an object without
 * a unique name will have the same best name as another object. Showing two or
 * more objects with the same best name in the completion list will hopefully
 * help the user to identify problems with the names in the target application.
 */
class RemoteObjectNameCompletion: public KCompletion {
public:

    /**
     * Creates a new RemoteObjectNameCompletion.
     *
     * @param nameRegister The register to query about the remote object names.
     */
    RemoteObjectNameCompletion(RemoteObjectNameRegister* nameRegister):
            KCompletion() {
        mNameRegister = nameRegister;
    }

protected:

    /**
     * Default parent implementation.
     * It is implemented just to avoid a compilation warning that complains that
     * the method in the parent class is hidden by
     * postProcessMatches(QStringList*).
     *
     * @param matches The matches to process.
     */
    virtual void postProcessMatches(KCompletionMatches* matches) const {
        KCompletion::postProcessMatches(matches);
    }

    /**
     * Processes the matches to provide unique names.
     * When a match is a name that represents several remote objects that match
     * is replaced by the best names of the remote objects it represents.
     *
     * @param matches The matches to process.
     */
    virtual void postProcessMatches(QStringList* matches) const {
        QMutableListIterator<QString> it(*matches);
        while (it.hasNext()) {
            QStringList bestNames;

            try {
                bestNames = mNameRegister->bestNames(it.next());
            } catch (DBusException e) {
                kWarning() << "The unique names for the remote objects named "
                           << it.value() << "could not be added to the "
                           << "completion matches (" << e.message() << ")";
            }

            if (bestNames.count() > 1) {
                it.remove();

                qSort(bestNames);

                foreach (const QString& bestName, bestNames) {
                    it.insert(bestName);
                }
            }
        }
    }

private:

    /**
     * The RemoteObjectNameRegister to query about the remote object names.
     */
    RemoteObjectNameRegister* mNameRegister;

};

//public:

RemoteObjectNameWidget::RemoteObjectNameWidget(QWidget* parent):
        QWidget(parent),
    mIsRemoteObjectChosenPending(false),
    mIsNameChangedPending(false),
    mIsCompletionPending(false),
    mIsSubstringCompletionPending(false) {

    mRemoteObjectNameRegister = new RemoteObjectNameRegister(this);

    connect(mRemoteObjectNameRegister, SIGNAL(nameUpdateStarted()),
            this, SLOT(startNameUpdate()));
    connect(mRemoteObjectNameRegister, SIGNAL(nameUpdateFinished()),
            this, SLOT(finishNameUpdate()));

    ui = new Ui::RemoteObjectNameWidget();
    ui->setupUi(this);

    setFocusPolicy(Qt::StrongFocus);
    setFocusProxy(ui->objectNameLineEdit);

    connect(ui->objectNameLineEdit, SIGNAL(textChanged(QString)),
            this, SLOT(handleNameChanged(QString)));

    connect(ui->objectNamePushButton, SIGNAL(clicked(bool)),
            this, SLOT(showRemoteObjectChooser()));

    //It will probably be IBeamCursor, but just in case
    mObjectNameLineEditCursorShape = ui->objectNameLineEdit->cursor().shape();

    KCompletion* completion =
                    new RemoteObjectNameCompletion(mRemoteObjectNameRegister);
    completion->setOrder(KCompletion::Sorted);
    completion->setIgnoreCase(true);
    ui->objectNameLineEdit->setCompletionObject(completion);
    ui->objectNameLineEdit->setAutoDeleteCompletionObject(true);

    connect(ui->objectNameLineEdit, SIGNAL(completion(QString)),
            this, SLOT(handleCompletion()));
    connect(ui->objectNameLineEdit, SIGNAL(substringCompletion(QString)),
            this, SLOT(handleSubstringCompletion()));

    connect(mRemoteObjectNameRegister, SIGNAL(nameAdded(QString)),
            completion, SLOT(addItem(QString)));
    connect(mRemoteObjectNameRegister, SIGNAL(nameRemoved(QString)),
            completion, SLOT(removeItem(QString)));
    connect(TargetApplication::self(), SIGNAL(finished()),
            completion, SLOT(clear()));

    //If the target application already exists when the register is
    //created the nameUpdateStarted is emitted before being able to connect to
    //it.
    if (mRemoteObjectNameRegister->isBeingUpdated()) {
        startNameUpdate();
    }
}

RemoteObjectNameWidget::~RemoteObjectNameWidget() {
    delete ui;
}

QString RemoteObjectNameWidget::name() const {
    return ui->objectNameLineEdit->text();
}

void RemoteObjectNameWidget::setName(const QString& name) {
    ui->objectNameLineEdit->setText(name);
}

//private slots:

void RemoteObjectNameWidget::showRemoteObjectChooser() {
    RemoteObjectChooser* chooser = new RemoteObjectChooser(this);
    connect(chooser, SIGNAL(remoteObjectChosen(RemoteObject*)),
            this, SLOT(setChosenRemoteObject(RemoteObject*)));

    chooser->show();
}

void RemoteObjectNameWidget::setChosenRemoteObject(RemoteObject* remoteObject) {
    if (mRemoteObjectNameRegister->isBeingUpdated()) {
        mIsRemoteObjectChosenPending = true;
        mPendingRemoteObjectChosen = QPointer<RemoteObject>(remoteObject);
        return;
    }

    if (!remoteObject) {
        return;
    }

    QString uniqueName;
    try {
        uniqueName = mRemoteObjectNameRegister->uniqueName(remoteObject);
    } catch (DBusException e) {
        QString text = i18nc("@label", "The object name can not be set, there "
"was a problem getting the name from the target application: %1", e.message());
        QString title = i18nc("@title", "Can not communicate with the target "
"application");
        KMessageBox::sorry(this, text, title);
        return;
    }

    if (uniqueName.isEmpty()) {
        QString text = i18nc("@label", "There is no unique name for the chosen "
"object (at least, in the current state of the target application), so it is "
"not possible to refer to that object in a tutorial. The target application "
"must be modified to provide a unique name for that object.");
        QString title = i18nc("@title", "The chosen object has no unique name");
        KMessageBox::sorry(this, text, title);
        return;
    }

    ui->objectNameLineEdit->setText(uniqueName);
}

void RemoteObjectNameWidget::handleNameChanged(const QString& name) {
    if (mRemoteObjectNameRegister->isBeingUpdated()) {
        mIsNameChangedPending = true;
        return;
    }

    RemoteObject* remoteObject;
    try {
        remoteObject = mRemoteObjectNameRegister->findRemoteObject(name);
    } catch (DBusException e) {
        kWarning() << "There was a problem finding the remote object ("
                   << e.message() << ").";
        return;
    }

    emit remoteObjectChosen(remoteObject);
}

void RemoteObjectNameWidget::handleCompletion() {
    if (!mRemoteObjectNameRegister->isBeingUpdated()) {
        return;
    }

    mIsCompletionPending = true;
}

void RemoteObjectNameWidget::handleSubstringCompletion() {
    if (!mRemoteObjectNameRegister->isBeingUpdated()) {
        return;
    }

    mIsSubstringCompletionPending = true;
}

//This method may be called several times before finishNameUpdate() is called,
//so it should not be assumed that the widget will be in a "not updating" state
void RemoteObjectNameWidget::startNameUpdate() {
    ui->objectNameLineEdit->setCursor(Qt::BusyCursor);
    
    ui->objectNameLineEdit->setHandleSignals(false);
}

void RemoteObjectNameWidget::finishNameUpdate() {
    if (mIsRemoteObjectChosenPending) {
        mIsRemoteObjectChosenPending = false;
        setChosenRemoteObject(mPendingRemoteObjectChosen);
    }
    
    if (mIsNameChangedPending) {
        mIsNameChangedPending = false;
        handleNameChanged(ui->objectNameLineEdit->text());
    }

    ui->objectNameLineEdit->setHandleSignals(true);
    
    if (mIsSubstringCompletionPending) {
        mIsSubstringCompletionPending = false;
        ui->objectNameLineEdit->setCompletedItems(
            ui->objectNameLineEdit->completionObject()->substringCompletion(
                                            ui->objectNameLineEdit->text()));
    }

    if (mIsCompletionPending) {
        mIsCompletionPending = false;
        ui->objectNameLineEdit->doCompletion(ui->objectNameLineEdit->text());
    }

    ui->objectNameLineEdit->setCursor(mObjectNameLineEditCursorShape);
}
