# Used by kde4_add_unit_test to set the full path to test executables
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_BINARY_DIR})

include_directories(${ktutorial-editor_SOURCE_DIR}/src/view ${KDE4_INCLUDES})

# Since Qt 4.6.0, this definition is needed for GUI testing.
# It is backwards compatible with previous Qt versions, unlike the alternative
# which is to add #include <QTestGui> in the test files.
add_definitions(-DQT_GUI_LIB)

MACRO(UNIT_TESTS)
    FOREACH(_className ${ARGN})
        set(_testName ${_className}Test)
        kde4_add_unit_test(${_testName} TESTNAME ktutorial-editor-unit-${_testName} ${_testName}.cpp)
        target_link_libraries(${_testName} ktutorial_editor_view ${QT_QTTEST_LIBRARY})
    ENDFOREACH(_className)
ENDMACRO(UNIT_TESTS)

unit_tests(
    ActionListWidget
    AutoExpandableTreeView
    CommandWidget
    DialogRunner
    EditionDialog
    LicenseWidget
    NewWaitForWidget
    ReactionTreeItem
    ReactionWidget
    SemanticMarkupEdition
    SemanticMarkupLinkWidget
    SemanticMarkupParser
    StepCustomCodeWidget
    StepDataWidget
    StepTreeItem
    TextTreeItem
    TreeItem
    TreeItemUtil
    TreeModel
    TutorialCustomCodeWidget
    TutorialInformationWidget
    TutorialTreeItem
    TutorialTreeSelectionManager
    WaitForComposedTreeItem
    WaitForEventTreeItem
    WaitForEventWidget
    WaitForNotTreeItem
    WaitForPropertyTreeItem
    WaitForPropertyWidget
    WaitForSignalTreeItem
    WaitForSignalWidget
    WaitForStepActivationTreeItem
    WaitForTreeItem
    WaitForWidget
    WaitForWindowTreeItem
    WaitForWindowWidget
)

if (QT_QTDBUS_FOUND)
    add_definitions(-DQT_QTDBUS_FOUND)

    unit_tests(
        RemoteObjectChooser
        RemoteObjectChooserFilterModel
        RemoteObjectNameRegister
        RemoteObjectNameWidget
        RemoteObjectTreeItem
        RemoteObjectTreeItemUpdater
        RemoteObjectTreeSelectionManager
    )
endif (QT_QTDBUS_FOUND)

MACRO(MEM_TESTS)
    FOREACH(_testname ${ARGN})
        add_test(ktutorial-editor-unit-mem-${_testname} ${CMAKE_CURRENT_SOURCE_DIR}/../runMemcheck.py ${CMAKE_CURRENT_BINARY_DIR}/${_testname}Test ${CMAKE_CURRENT_BINARY_DIR})
    ENDFOREACH(_testname)
ENDMACRO(MEM_TESTS)

mem_tests(
    ActionListWidget
    AutoExpandableTreeView
    CommandWidget
    DialogRunner
    EditionDialog
    LicenseWidget
    NewWaitForWidget
    ReactionTreeItem
    ReactionWidget
    SemanticMarkupEdition
    SemanticMarkupLinkWidget
    SemanticMarkupParser
    StepCustomCodeWidget
    StepDataWidget
    StepTreeItem
    TextTreeItem
    TreeItem
    TreeItemUtil
    TreeModel
    TutorialCustomCodeWidget
    TutorialInformationWidget
    TutorialTreeItem
    TutorialTreeSelectionManager
    WaitForComposedTreeItem
    WaitForEventTreeItem
    WaitForEventWidget
    WaitForNotTreeItem
    WaitForPropertyTreeItem
    WaitForPropertyWidget
    WaitForSignalTreeItem
    WaitForSignalWidget
    WaitForStepActivationTreeItem
    WaitForTreeItem
    WaitForWidget
    WaitForWindowTreeItem
    WaitForWindowWidget
)

if (QT_QTDBUS_FOUND)
    mem_tests(
        RemoteObjectChooser
        RemoteObjectChooserFilterModel
        RemoteObjectNameRegister
        RemoteObjectNameWidget
        RemoteObjectTreeItem
        RemoteObjectTreeItemUpdater
        RemoteObjectTreeSelectionManager
    )
endif (QT_QTDBUS_FOUND)
