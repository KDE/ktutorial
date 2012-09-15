add_subdirectory(common)
add_subdirectory(customization)
add_subdirectory(extendedinformation)
add_subdirectory(scripting)
add_subdirectory(view)

if (QT_QTDBUS_FOUND)
    add_subdirectory(editorsupport)
endif (QT_QTDBUS_FOUND)

# Used by kde4_add_unit_test to set the full path to test executables
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_BINARY_DIR})

include_directories(${CMAKE_CURRENT_BINARY_DIR} ${ktutorial-library_SOURCE_DIR}/src ${KDE4_INCLUDES})

# Since Qt 4.6.0, this definition is needed for GUI testing.
# It is backwards compatible with previous Qt versions, unlike the alternative
# which is to add #include <QTestGui> in the test files.
add_definitions(-DQT_GUI_LIB)

MACRO(UNIT_TESTS)
    FOREACH(_className ${ARGN})
        set(_testName ${_className}Test)
        kde4_add_unit_test(${_testName} TESTNAME ktutorial-${_testName} ${_testName}.cpp)
        target_link_libraries(${_testName} ktutorial ${QT_QTTEST_LIBRARY} ${KDE4_KDEUI_LIBS})
    ENDFOREACH(_className)
ENDMACRO(UNIT_TESTS)

unit_tests(
    ObjectFinder
    Option
    Step
    Tutorial
    TutorialInformation
    TutorialManager
    WaitFor
    WaitForAnd
    WaitForComposed
    WaitForEvent
    WaitForNot
    WaitForOr
    WaitForProperty
    WaitForSignal
    WaitForStepActivation
    WaitForWindow
)

MACRO(MEM_TESTS)
    FOREACH(_testname ${ARGN})
        add_test(ktutorial-mem-${_testname} ${CMAKE_CURRENT_SOURCE_DIR}/runMemcheck.py ${CMAKE_CURRENT_BINARY_DIR}/${_testname}Test ${CMAKE_CURRENT_BINARY_DIR})
    ENDFOREACH(_testname)
ENDMACRO(MEM_TESTS)

mem_tests(
    ObjectFinder
    Option
    Step
    Tutorial
    TutorialInformation
    TutorialManager
    WaitFor
    WaitForAnd
    WaitForComposed
    WaitForEvent
    WaitForNot
    WaitForOr
    WaitForProperty
    WaitForSignal
    WaitForStepActivation
    WaitForWindow
)
