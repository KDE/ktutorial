# In x86_64, linking fails when a static library is added to a shared library.
# In order to work, they must be compiled using -fPIC
add_definitions("-fPIC")

add_subdirectory(common)
add_subdirectory(customization)
add_subdirectory(extendedinformation)
add_subdirectory(scripting)
add_subdirectory(tutorials)
add_subdirectory(view)

if (QT_QTDBUS_FOUND)
    add_definitions(-DQT_QTDBUS_FOUND)
    add_subdirectory(editorsupport)
endif (QT_QTDBUS_FOUND)

# version.h is used by FindKTutorial.cmake
configure_file(version.h.cmake ${CMAKE_CURRENT_BINARY_DIR}/version.h)

include_directories(${CMAKE_CURRENT_BINARY_DIR} ${KDE4_INCLUDES})

set(ktutorial_LIB_SRCS
    KTutorial.cpp
    KTutorialCustomization.cpp
    ObjectFinder.cpp
    Option.cpp
    Step.cpp
    Tutorial.cpp
    TutorialInformation.cpp
    TutorialManager.cpp
    WaitFor.cpp
    WaitForAnd.cpp
    WaitForComposed.cpp
    WaitForEvent.cpp
    WaitForNot.cpp
    WaitForOr.cpp
    WaitForProperty.cpp
    WaitForSignal.cpp
    WaitForStepActivation.cpp
    WaitForWindow.cpp
)

kde4_add_library(ktutorial SHARED ${ktutorial_LIB_SRCS})

target_link_libraries(ktutorial ktutorial_customization ktutorial_scripting)

if (QT_QTDBUS_FOUND)
    target_link_libraries(ktutorial ktutorial_editorsupport)
endif (QT_QTDBUS_FOUND)

####### Install the library #######

install(TARGETS ktutorial DESTINATION ${LIB_INSTALL_DIR})

set(ktutorial_LIB_HEADERS
    ktutorial_export.h
    KTutorial.h
    KTutorialCustomization.h
    ObjectFinder.h
    Option.h
    Step.h
    Tutorial.h
    TutorialInformation.h
    TutorialManager.h
    WaitFor.h
    WaitForAnd.h
    WaitForComposed.h
    WaitForEvent.h
    WaitForNot.h
    WaitForOr.h
    WaitForProperty.h
    WaitForSignal.h
    WaitForStepActivation.h
    WaitForWindow.h
    ${CMAKE_CURRENT_BINARY_DIR}/version.h
)

# Hack to make headers available to other ktutorial modules (like
# ktutorial-test-app) when the library is built but not installed yet.
foreach(header ${ktutorial_LIB_HEADERS})
    get_filename_component(headerName ${header} NAME)
    configure_file(${header} ${ktutorial-library_BINARY_DIR}/includes/ktutorial/${headerName} COPYONLY)
endforeach(header)

install(FILES ${ktutorial_LIB_HEADERS} DESTINATION ${INCLUDE_INSTALL_DIR}/ktutorial)

install(FILES ktutorialui.rc DESTINATION ${DATA_INSTALL_DIR}/ktutorial)
