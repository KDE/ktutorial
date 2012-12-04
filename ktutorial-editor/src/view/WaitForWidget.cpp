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

#include "WaitForWidget.h"

#include "ui_WaitForWidget.h"
#include "DialogRunner.h"
#include "EditionDialog.h"
#include "NewWaitForWidget.h"
#include "TextTreeItem.h"
#include "TreeModel.h"
#include "WaitForEventWidget.h"
#include "WaitForPropertyWidget.h"
#include "WaitForSignalWidget.h"
#include "WaitForWindowWidget.h"
#include "WaitForTreeItem.h"
#include "../data/WaitForComposed.h"
#include "../data/WaitForEvent.h"
#include "../data/WaitForNot.h"
#include "../data/WaitForProperty.h"
#include "../data/WaitForSignal.h"
#include "../data/WaitForStepActivation.h"
#include "../data/WaitForWindow.h"

//public:

WaitForWidget::WaitForWidget(WaitFor* waitFor, QWidget* parent):
        QWidget(parent),
    mWaitFor(waitFor),
    mCurrentWaitFor(0) {

    ui = new Ui::WaitForWidget();
    ui->setupUi(this);

    ui->addButton->setIcon(KIcon("list-add"));
    ui->editButton->setIcon(KIcon("document-edit"));
    ui->removeButton->setIcon(KIcon("list-remove"));

    setupTreeView(waitFor);

    updateWaitForSelection(0);

    connect(ui->addButton, SIGNAL(clicked(bool)), this, SLOT(addWaitFor()));
    connect(ui->editButton, SIGNAL(clicked(bool)), this, SLOT(editWaitFor()));
    connect(ui->removeButton, SIGNAL(clicked(bool)),
            this, SLOT(removeWaitFor()));
}

WaitForWidget::~WaitForWidget() {
    delete ui;
}

WaitFor* WaitForWidget::waitFor() const {
    return mWaitFor;
}

//private:

void WaitForWidget::setupTreeView(WaitFor* waitFor) {
    mRootItem = new TextTreeItem();
    if (waitFor) {
        TreeItem* item = WaitForTreeItem::treeItemForWaitFor(waitFor,
                                                             mRootItem);
        mRootItem->appendChild(item);
    }

    QTreeView* treeView = ui->waitForTreeView;
    TreeModel* model = new TreeModel(mRootItem, treeView);

    QAbstractItemModel* oldModel = treeView->model();
    treeView->setModel(model);
    delete oldModel;

    connect(treeView->selectionModel(),
            SIGNAL(selectionChanged(QItemSelection,QItemSelection)),
            this,
            SLOT(handleSelectionChanged(QItemSelection,QItemSelection)));
}

void WaitForWidget::updateWaitForSelection(WaitFor* selectedWaitFor) {
    mCurrentWaitFor = selectedWaitFor;

    if (!selectedWaitFor) {
        if (mWaitFor) {
            ui->addButton->setEnabled(false);
        } else {
            ui->addButton->setEnabled(true);
        }
        ui->editButton->setEnabled(false);
        ui->removeButton->setEnabled(false);

        return;
    }

    if (qobject_cast<WaitForComposed*>(selectedWaitFor)) {
        ui->addButton->setEnabled(true);
        ui->editButton->setEnabled(false);
        ui->removeButton->setEnabled(true);

        return;
    }

    if (qobject_cast<WaitForEvent*>(selectedWaitFor)) {
        ui->addButton->setEnabled(false);
        ui->editButton->setEnabled(true);
        ui->removeButton->setEnabled(true);

        return;
    }

    if (qobject_cast<WaitForNot*>(selectedWaitFor)) {
        if (static_cast<WaitForNot*>(selectedWaitFor)->negatedWaitFor()) {
            ui->addButton->setEnabled(false);
        } else {
            ui->addButton->setEnabled(true);
        }
        ui->editButton->setEnabled(false);
        ui->removeButton->setEnabled(true);

        return;
    }

    if (qobject_cast<WaitForProperty*>(selectedWaitFor)) {
        ui->addButton->setEnabled(false);
        ui->editButton->setEnabled(true);
        ui->removeButton->setEnabled(true);

        return;
    }

    if (qobject_cast<WaitForSignal*>(selectedWaitFor)) {
        ui->addButton->setEnabled(false);
        ui->editButton->setEnabled(true);
        ui->removeButton->setEnabled(true);

        return;
    }

    if (qobject_cast<WaitForStepActivation*>(selectedWaitFor)) {
        ui->addButton->setEnabled(false);
        ui->editButton->setEnabled(false);
        ui->removeButton->setEnabled(true);

        return;
    }

    if (qobject_cast<WaitForWindow*>(selectedWaitFor)) {
        ui->addButton->setEnabled(false);
        ui->editButton->setEnabled(true);
        ui->removeButton->setEnabled(true);

        return;
    }
}

//private slots:

void WaitForWidget::handleSelectionChanged(const QItemSelection& selected,
                                           const QItemSelection& deselected) {
    Q_UNUSED(deselected);

    //Only single selections are supported
    Q_ASSERT(selected.count() <= 1);
    Q_ASSERT(deselected.count() <= 1);

    WaitFor* selectedWaitFor = 0;

    if (selected.count() == 1) {
        Q_ASSERT(selected.at(0).indexes().count() == 1);

        QModelIndex index = selected.at(0).indexes().at(0);
        WaitForTreeItem* item =
                    static_cast<WaitForTreeItem*>(index.internalPointer());

        selectedWaitFor = item->waitFor();
    }

    updateWaitForSelection(selectedWaitFor);
}

void WaitForWidget::addWaitFor() {
    WaitFor* newWaitFor = 0;

    KDialog* dialog = new KDialog(this);
    NewWaitForWidget* widget = new NewWaitForWidget(dialog);
    dialog->setMainWidget(widget);
    dialog->setWindowTitle(widget->windowTitle());
    dialog->setObjectName("addWaitForDialog");
    dialog->button(KDialog::Ok)->setObjectName("okButton");
    dialog->button(KDialog::Cancel)->setObjectName("cancelButton");
    if (DialogRunner(dialog).exec() == //krazy:exclude=crashy
            QDialog::Accepted) {
        newWaitFor = widget->waitFor();
    }

    if (!newWaitFor) {
        return;
    }

    if (!mWaitFor) {
        ui->addButton->setEnabled(false);

        TreeItem* item = WaitForTreeItem::treeItemForWaitFor(newWaitFor,
                                                             mRootItem);
        mRootItem->appendChild(item);

        mWaitFor = newWaitFor;

        return;
    }

    if (qobject_cast<WaitForComposed*>(mCurrentWaitFor)) {
        WaitForComposed* waitForComposed =
                                static_cast<WaitForComposed*>(mCurrentWaitFor);

        waitForComposed->addWaitFor(newWaitFor);
        return;
    }

    if (qobject_cast<WaitForNot*>(mCurrentWaitFor)) {
        ui->addButton->setEnabled(false);

        WaitForNot* waitForNot = static_cast<WaitForNot*>(mCurrentWaitFor);

        waitForNot->setNegatedWaitFor(newWaitFor);
        return;
    }
}

void WaitForWidget::editWaitFor() {
    EditionWidget* editionWidget = 0;
    if (qobject_cast<WaitForEvent*>(mCurrentWaitFor)) {
        WaitForEvent* waitForEvent =
                                static_cast<WaitForEvent*>(mCurrentWaitFor);
        editionWidget = new WaitForEventWidget(waitForEvent, this);
    }

    if (qobject_cast<WaitForProperty*>(mCurrentWaitFor)) {
        WaitForProperty* waitForProperty =
                                static_cast<WaitForProperty*>(mCurrentWaitFor);
        editionWidget = new WaitForPropertyWidget(waitForProperty, this);
    }

    if (qobject_cast<WaitForSignal*>(mCurrentWaitFor)) {
        WaitForSignal* waitForSignal =
                                static_cast<WaitForSignal*>(mCurrentWaitFor);
        editionWidget = new WaitForSignalWidget(waitForSignal, this);
    }

    if (qobject_cast<WaitForWindow*>(mCurrentWaitFor)) {
        WaitForWindow* waitForWindow =
                                static_cast<WaitForWindow*>(mCurrentWaitFor);
        editionWidget = new WaitForWindowWidget(waitForWindow, this);
    }

    Q_ASSERT(editionWidget);

    QString dialogName = "edit" + editionWidget->objectName();
    dialogName.replace("Widget", "Dialog");

    EditionDialog* dialog = new EditionDialog(editionWidget, this);
    dialog->setObjectName(dialogName);
    DialogRunner(dialog).exec(); //krazy:exclude=crashy
}

void WaitForWidget::removeWaitFor() {
    QModelIndex index =
                    ui->waitForTreeView->selectionModel()->selectedIndexes()[0];
    QModelIndex parent = index.parent();

    if (!parent.isValid()) {
        delete mWaitFor;
        mWaitFor = 0;

        TreeItem* item = static_cast<TreeItem*>(index.internalPointer());
        mRootItem->removeChild(item);
        delete item;

        return;
    }

    WaitFor* parentWaitFor =
            static_cast<WaitForTreeItem*>(parent.internalPointer())->waitFor();

    if (qobject_cast<WaitForComposed*>(parentWaitFor)) {
        WaitForComposed* waitForComposed =
                                static_cast<WaitForComposed*>(parentWaitFor);

        //When the WaitFor is removed, mCurrentWaitFor changes because the
        //selected item in the tree view changes. The WaitFor to be removed must
        //be stored to properly delete it.
        WaitFor* waitForToRemove = mCurrentWaitFor;
        waitForComposed->removeWaitFor(waitForToRemove);
        delete waitForToRemove;

        return;
    }

    if (qobject_cast<WaitForNot*>(parentWaitFor)) {
        WaitForNot* waitForNot = static_cast<WaitForNot*>(parentWaitFor);

        //When the WaitFor is removed, mCurrentWaitFor changes because the
        //selected item in the tree view changes. The WaitFor to be removed must
        //be stored to properly delete it.
        WaitFor* waitForToRemove = mCurrentWaitFor;
        waitForNot->setNegatedWaitFor(0);
        delete waitForToRemove;

        return;
    }
}
