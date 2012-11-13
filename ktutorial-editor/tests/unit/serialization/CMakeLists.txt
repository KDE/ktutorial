# Used by kde4_add_unit_test to set the full path to test executables
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_BINARY_DIR})

include_directories(${ktutorial-editor_SOURCE_DIR}/src/serialization ${KDE4_INCLUDES})

MACRO(UNIT_TESTS)
    FOREACH(_className ${ARGN})
        set(_testName ${_className}Test)
        kde4_add_unit_test(${_testName} TESTNAME ktutorial-editor-unit-${_testName} ${_testName}.cpp)
        target_link_libraries(${_testName} ktutorial_editor_serialization ${QT_QTTEST_LIBRARY})
    ENDFOREACH(_className)
ENDMACRO(UNIT_TESTS)

unit_tests(
    DeserializationException
    IOException
    JavaScriptExporter
    Serialization
    TutorialReader
    TutorialWriter
)

MACRO(MEM_TESTS)
    FOREACH(_testname ${ARGN})
        add_test(ktutorial-editor-unit-mem-${_testname} ${CMAKE_CURRENT_SOURCE_DIR}/../runMemcheck.py ${CMAKE_CURRENT_BINARY_DIR}/${_testname}Test ${CMAKE_CURRENT_BINARY_DIR})
    ENDFOREACH(_testname)
ENDMACRO(MEM_TESTS)

mem_tests(
    DeserializationException
    IOException
    JavaScriptExporter
    Serialization
    TutorialReader
    TutorialWriter
)
