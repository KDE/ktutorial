set(ktutorial_editor_view_SRCS
    ActionListWidget.cpp
    AutoExpandableTreeView.cpp
    CommandWidget.cpp
    DialogRunner.cpp
    EditionDialog.cpp
    EditionWidget.cpp
    LicenseWidget.cpp
    NewWaitForWidget.cpp
    ReactionTreeItem.cpp
    ReactionWidget.cpp
    SemanticMarkupEdition.cpp
    SemanticMarkupLinkWidget.cpp
    SemanticMarkupParser.cpp
    StepCustomCodeWidget.cpp
    StepDataWidget.cpp
    StepTreeItem.cpp
    TextTreeItem.cpp
    TreeItem.cpp
    TreeItemUtil.cpp
    TreeModel.cpp
    TutorialCustomCodeWidget.cpp
    TutorialInformationWidget.cpp
    TutorialTreeItem.cpp
    TutorialTreeSelectionManager.cpp
    WaitForComposedTreeItem.cpp
    WaitForEventTreeItem.cpp
    WaitForEventWidget.cpp
    WaitForNotTreeItem.cpp
    WaitForPropertyTreeItem.cpp
    WaitForPropertyWidget.cpp
    WaitForSignalTreeItem.cpp
    WaitForSignalWidget.cpp
    WaitForStepActivationTreeItem.cpp
    WaitForTreeItem.cpp
    WaitForWidget.cpp
    WaitForWindowTreeItem.cpp
    WaitForWindowWidget.cpp
)

if (QT_QTDBUS_FOUND)
    add_definitions(-DQT_QTDBUS_FOUND)
    set(ktutorial_editor_view_SRCS
        ${ktutorial_editor_view_SRCS}
        RemoteObjectChooser.cpp
        RemoteObjectChooserFilterModel.cpp
        RemoteObjectNameRegister.cpp
        RemoteObjectNameWidget.cpp
        RemoteObjectTreeItem.cpp
        RemoteObjectTreeItemUpdater.cpp
        RemoteObjectTreeSelectionManager.cpp
        TargetApplicationView.cpp
    )
endif (QT_QTDBUS_FOUND)

kde4_add_ui_files(ktutorial_editor_view_SRCS
    CustomCodeWidget.ui
    LicenseWidget.ui
    NewWaitForWidget.ui
    ReactionWidget.ui
    RemoteObjectNameWidget.ui
    SemanticMarkupLinkWidget.ui
    StepDataWidget.ui
    TutorialInformationWidget.ui
    WaitForEventWidget.ui
    WaitForPropertyWidget.ui
    WaitForSignalWidget.ui
    WaitForWidget.ui
    WaitForWindowWidget.ui
)

if (QT_QTDBUS_FOUND)
    kde4_add_ui_files(ktutorial_editor_view_SRCS
        RemoteObjectChooser.ui
    )
endif (QT_QTDBUS_FOUND)

kde4_add_library(ktutorial_editor_view ${ktutorial_editor_view_SRCS})

target_link_libraries(ktutorial_editor_view
                      ktutorial_editor_commands
                      ktutorial_editor_data
                      ${KDE4_KDEUI_LIBS}
                      ${KDE4_KIO_LIBS}
)

if (QT_QTDBUS_FOUND)
    target_link_libraries(ktutorial_editor_view
                          ktutorial_editor_targetapplication
    )
endif (QT_QTDBUS_FOUND)
