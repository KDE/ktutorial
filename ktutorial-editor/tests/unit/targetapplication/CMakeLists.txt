# Used by kde4_add_unit_test to set the full path to test executables
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_BINARY_DIR})

include_directories(${ktutorial-editor_SOURCE_DIR}/src/targetapplication ${KDE4_INCLUDES})

# Remote class stubs are QObjects, so moc files have to be generated for them
qt4_wrap_cpp(dbus_interface_stubs_MOC RemoteClassStubs.h)

# Dummy application to be executed in TargetApplication test
add_executable(DummyApplication DummyApplication.cpp)
target_link_libraries(DummyApplication ${KDE4_KDEUI_LIBS} ${QT_QTDBUS_LIBRARY})

# Target application stub to be executed in TargetApplication test
add_executable(TargetApplicationStub TargetApplicationStub.cpp ${dbus_interface_stubs_MOC})
target_link_libraries(TargetApplicationStub ${KDE4_KDEUI_LIBS} ${QT_QTDBUS_LIBRARY})

# Target application stub with delayed register to be executed in TargetApplication test
set(TargetApplicationStubWithDelayedRegister_MOC ${CMAKE_CURRENT_BINARY_DIR}/TargetApplicationStubWithDelayedRegister.moc)
qt4_generate_moc(TargetApplicationStubWithDelayedRegister.cpp ${TargetApplicationStubWithDelayedRegister_MOC})
add_executable(TargetApplicationStubWithDelayedRegister TargetApplicationStubWithDelayedRegister.cpp ${TargetApplicationStubWithDelayedRegister_MOC} ${dbus_interface_stubs_MOC})
target_link_libraries(TargetApplicationStubWithDelayedRegister ${KDE4_KDEUI_LIBS} ${QT_QTDBUS_LIBRARY})

MACRO(UNIT_TESTS)
    FOREACH(_className ${ARGN})
        set(_testName ${_className}Test)
        kde4_add_unit_test(${_testName} TESTNAME ktutorial-editor-unit-${_testName} ${_testName}.cpp ${dbus_interface_stubs_MOC})
        target_link_libraries(${_testName} ktutorial_editor_targetapplication ${QT_QTTEST_LIBRARY})
    ENDFOREACH(_className)
ENDMACRO(UNIT_TESTS)

unit_tests(
    DBusException
    RemoteClass
    RemoteEditorSupport
    RemoteEventSpy
    RemoteObject
    RemoteObjectMapper
    TargetApplication
)

MACRO(MEM_TESTS)
    FOREACH(_testname ${ARGN})
        add_test(ktutorial-editor-unit-mem-${_testname} ${CMAKE_CURRENT_SOURCE_DIR}/../runMemcheck.py ${CMAKE_CURRENT_BINARY_DIR}/${_testname}Test ${CMAKE_CURRENT_BINARY_DIR})
    ENDFOREACH(_testname)
ENDMACRO(MEM_TESTS)

mem_tests(
    DBusException
    RemoteClass
    RemoteEditorSupport
    RemoteEventSpy
    RemoteObject
    RemoteObjectMapper
    TargetApplication
)
