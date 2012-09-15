# In x86_64, linking fails when a static library is added to a shared library.
# In order to work, they must be compiled using -fPIC
add_definitions("-fPIC")

include_directories(${KDE4_INCLUDES})

add_subdirectory(commands)
add_subdirectory(data)
add_subdirectory(serialization)
add_subdirectory(tutorials)
add_subdirectory(util)
add_subdirectory(view)

# When KTutorial is built, the library isn't installed yet when the editor is
# built. In that case, use the headers and libraries in the build directory
# of the library.
if(ktutorial-library_BINARY_DIR)
    set(KTUTORIAL_FOUND TRUE)
    # Include the headers from the build directory before the general includes
    # to ensure that, even if the KTutorial headers were installed, the ones
    # from the build directory (and, thus, the most updated ones) are used.
    # Not really necessary here, as the editor uses KDE4Defaults, and they set
    # CMAKE_INCLUDE_DIRECTORIES_BEFORE to ON, but just in case this changes.
    include_directories(BEFORE ${ktutorial-library_BINARY_DIR}/includes)
    set(KTUTORIAL_LIBRARIES ktutorial)
endif(ktutorial-library_BINARY_DIR)

# If ktutorial-test-app is built in standalone mode, look for an installed
# KTutorial library.
if(NOT KTUTORIAL_FOUND)
    find_package(KTutorial REQUIRED)
    include_directories(${KTUTORIAL_INCLUDE_DIRS})
endif(NOT KTUTORIAL_FOUND)

if (QT_QTDBUS_FOUND)
    add_definitions(-DQT_QTDBUS_FOUND)
    add_subdirectory(targetapplication)
endif (QT_QTDBUS_FOUND)

set(ktutorial_editor_SRCS
    EditActions.cpp
    FileActions.cpp
    KTutorialEditor.cpp
)

if (QT_QTDBUS_FOUND)
    set(ktutorial_editor_SRCS
        ${ktutorial_editor_SRCS}
        TestTutorialActions.cpp
        TutorialTester.cpp
    )
endif (QT_QTDBUS_FOUND)

# Instead of compiling the executable directly from the sources, the sources are
# compiled to a static library that is linked (and, being static, also embedded)
# in the editor executable.
# As everything but a tiny initialization code is in a library, the build system
# for the tests can be easily set up.
kde4_add_library(ktutorial_editor ${ktutorial_editor_SRCS})
target_link_libraries(ktutorial_editor
                      ktutorial_editor_data
                      ktutorial_editor_serialization
                      ktutorial_editor_util
                      ktutorial_editor_view
                      ${KTUTORIAL_LIBRARIES}
)

if (QT_QTDBUS_FOUND)
    target_link_libraries(ktutorial_editor ktutorial_editor_targetapplication)
endif (QT_QTDBUS_FOUND)


kde4_add_executable(ktutorial-editor main.cpp)
target_link_libraries(ktutorial-editor ktutorial_editor)

####### Install the editor #######

install(TARGETS ktutorial-editor DESTINATION ${BIN_INSTALL_DIR})
# Desktop files should not contain "-" in their name, as it could confuse the
# desktop id system:
# http://standards.freedesktop.org/menu-spec/latest/go01.html#term-desktop-file-id
install(FILES ktutorial-editor.desktop DESTINATION ${XDG_APPS_INSTALL_DIR}
        RENAME ktutorial_editor.desktop
)
install(FILES ktutorial-editorui.rc DESTINATION ${DATA_INSTALL_DIR}/ktutorial-editor)
