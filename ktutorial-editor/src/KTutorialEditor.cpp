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

#include "KTutorialEditor.h"

#include <QDockWidget>

#include <KAction>
#include <KActionCollection>
#include <KApplication>
#include <KConfigGroup>
#include <KLocalizedString>

#include <ktutorial/KTutorial.h>

#include "EditActions.h"
#include "FileActions.h"
#include "data/Tutorial.h"
#include "view/ActionListWidget.h"
#include "view/AutoExpandableTreeView.h"
#include "view/TreeModel.h"
#include "view/TutorialTreeItem.h"
#include "view/TutorialTreeSelectionManager.h"

#ifdef QT_QTDBUS_FOUND
#include "TestTutorialActions.h"
#include "targetapplication/TargetApplication.h"
#endif

//public:

KTutorialEditor::KTutorialEditor(): KXmlGuiWindow(0),
    mTutorial(0) {

    mTreeView = new AutoExpandableTreeView();
    mTreeView->setObjectName("centralTreeView");
    mTreeView->setWordWrap(true);
    setCentralWidget(mTreeView);

    setupDocks();

    setupActions();

    connect(mEditActions, SIGNAL(cleanChanged(bool)),
            this, SIGNAL(cleanChanged(bool)));
    connect(this, SIGNAL(cleanChanged(bool)),
            this, SLOT(handleUndoStackCleanChanged(bool)));

    //The actions can not be added in setupDocks because setupActions() needs
    //the docks to be created (to get their toggleAction), so it can be called
    //before setupDocks().
    setupActionListWidgets();

    mFileActions->newTutorial();

    ktutorial::KTutorial::self()->setup(this);

    setupGUI();
}

KTutorialEditor::~KTutorialEditor() {
#ifdef QT_QTDBUS_FOUND
    delete TargetApplication::self();
#endif
}

Tutorial* KTutorialEditor::tutorial() {
    return mTutorial;
}

void KTutorialEditor::setClean() {
    mEditActions->setClean();

    //Force clean state, as setting an empty stack as clean would not emit
    //cleanChanged()
    handleUndoStackCleanChanged(true);
    mFileActions->handleUndoStackCleanChanged(true);
}

void KTutorialEditor::loadTutorialFromUrl(const KUrl& url) {
    mFileActions->loadTutorialFromUrl(url);
}

//protected:

bool KTutorialEditor::queryClose() {
    return mFileActions->queryCloseTutorial();
}

void KTutorialEditor::readProperties(const KConfigGroup& configGroup) {
    KUrl url = configGroup.readEntry("TutorialUrl");
    if (!url.isEmpty()) {
        mFileActions->loadTutorialFromUrl(url);
    }
}

void KTutorialEditor::saveProperties(KConfigGroup& configGroup) {
    configGroup.writeEntry("TutorialUrl", mFileActions->tutorialUrl());
}

//private:

void KTutorialEditor::setTutorialToBeEdited(Tutorial* tutorial) {
    if (!tutorial) {
        tutorial = new Tutorial(this);
    }

    //Clear the selection model to ensure that the actions are disabled as
    //needed when the selection model is replaced
    if (mTreeView->selectionModel()) {
        mTreeView->selectionModel()->clear();
    }

    TutorialTreeItem* tutorialTreeItem = new TutorialTreeItem(tutorial);
    TreeModel* model = new TreeModel(tutorialTreeItem, mTreeView);

    //Neither the old model nor the old selection model are deleted by the
    //QTreeView when a new model is set (which creates a new selection model),
    //so it must be done explicitly
    QItemSelectionModel* oldSelectionModel = mTreeView->selectionModel();
    QAbstractItemModel* oldModel = mTreeView->model();
    mTreeView->setModel(model);
    delete oldSelectionModel;
    delete oldModel;

    //Parent object is set to the selection model, so the manager is also
    //deleted when the selection model it watches is deleted
    TutorialTreeSelectionManager* selectionManager =
            new TutorialTreeSelectionManager(mTreeView->selectionModel(),
                                             mTreeView->selectionModel());
    connect(selectionManager, SIGNAL(stepSelected(Step*)),
            mEditActions, SLOT(selectStep(Step*)));
    connect(selectionManager, SIGNAL(reactionSelected(Reaction*)),
            mEditActions, SLOT(selectReaction(Reaction*)));
#ifdef QT_QTDBUS_FOUND
    connect(selectionManager, SIGNAL(stepSelected(Step*)),
            mTestTutorialActions, SLOT(selectStep(Step*)));
#endif

    mEditActions->clearCommands();

    delete mTutorial;
    mTutorial = tutorial;

#ifdef QT_QTDBUS_FOUND
    TargetApplication::self()->setTargetApplicationFilePath("");
#endif
}

void KTutorialEditor::setupDocks() {
    mTutorialActionDock = new QDockWidget(i18nc("@title", "Edit tutorial"),
                                          this);
    mTutorialActionDock->setObjectName("editTutorialDock");
    addDockWidget(Qt::RightDockWidgetArea, mTutorialActionDock);

    mStepActionDock = new QDockWidget(i18nc("@title", "Edit step"), this);
    mStepActionDock->setObjectName("editStepDock");
    addDockWidget(Qt::RightDockWidgetArea, mStepActionDock);

    mReactionActionDock = new QDockWidget(i18nc("@title", "Edit reaction"),
                                          this);
    mReactionActionDock->setObjectName("editReactionDock");
    addDockWidget(Qt::RightDockWidgetArea, mReactionActionDock);
}

void KTutorialEditor::setupActions() {
    mFileActions = new FileActions(this);

    KStandardAction::quit(this, SLOT(close()), actionCollection());

    mEditActions = new EditActions(this);

#ifdef QT_QTDBUS_FOUND
    mTestTutorialActions = new TestTutorialActions(this);
#endif

    actionCollection()->addAction("showEditTutorialDock",
                                  mTutorialActionDock->toggleViewAction());
    actionCollection()->addAction("showEditStepDock",
                                  mStepActionDock->toggleViewAction());
    actionCollection()->addAction("showEditReactionDock",
                                  mReactionActionDock->toggleViewAction());
}

void KTutorialEditor::setupActionListWidgets() {
    ActionListWidget* actionListWidget =
                                    new ActionListWidget(mTutorialActionDock);

    QAction* action = actionCollection()->action("setTutorialInformation");
    actionListWidget->addAction(action);

    action = actionCollection()->action("setTutorialLicense");
    actionListWidget->addAction(action);

    action = actionCollection()->action("setTutorialSetup");
    actionListWidget->addAction(action);

    action = actionCollection()->action("setTutorialTearDown");
    actionListWidget->addAction(action);

    mTutorialActionDock->setWidget(actionListWidget);
    actionListWidget = new ActionListWidget(mStepActionDock);

    action = actionCollection()->action("addStep");
    actionListWidget->addAction(action);

    action = actionCollection()->action("setStepData");
    actionListWidget->addAction(action);

    action = actionCollection()->action("setStepSetup");
    actionListWidget->addAction(action);

    action = actionCollection()->action("setStepTearDown");
    actionListWidget->addAction(action);

    action =  actionCollection()->action("removeStep");
    actionListWidget->addAction(action);

    mStepActionDock->setWidget(actionListWidget);
    actionListWidget = new ActionListWidget(mReactionActionDock);

    action = actionCollection()->action("addReaction");
    actionListWidget->addAction(action);

    action = actionCollection()->action("setReactionData");
    actionListWidget->addAction(action);

    action = actionCollection()->action("removeReaction");
    actionListWidget->addAction(action);

    mReactionActionDock->setWidget(actionListWidget);
}

QString KTutorialEditor::captionFromTutorialUrl() {
    if (mFileActions->tutorialUrl().isEmpty()) {
        return i18nc("@title:window Window title for KTutorial editor when the \
tutorial has no associated URL", "New file");
    }

    QString caption = mFileActions->tutorialUrl().prettyUrl();
    if (caption.length() > 64) {
        caption = "..." + caption.right(64);
    }

    return caption;
}

//private slots:

void KTutorialEditor::handleUndoStackCleanChanged(bool clean) {
    QString caption = captionFromTutorialUrl();
    if (clean) {
        setCaption(caption);
    } else {
        setCaption(i18nc("@title:window Wrapper for the window title when the \
tutorial was modified but not saved yet", "%1 [not saved]", caption));
    }
}
