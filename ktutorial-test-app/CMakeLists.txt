project(ktutorial-test-app)

find_package(KDE4 REQUIRED)
include_directories(${KDE4_INCLUDES})

# When KTutorial is built, the library isn't installed yet when the test-app is
# built. In that case, use the headers and libraries in the build directory
# of the library.
if(ktutorial-library_BINARY_DIR)
    set(KTUTORIAL_FOUND TRUE)
    # Include the headers from the build directory before the general includes
    # to ensure that, even if the KTutorial headers were installed, the ones
    # from the build directory (and, thus, the most updated ones) are used.
    include_directories(BEFORE ${ktutorial-library_BINARY_DIR}/includes)
    set(KTUTORIAL_LIBRARIES ktutorial) 
endif(ktutorial-library_BINARY_DIR)

# If ktutorial-test-app is built in standalone mode, look for an installed
# KTutorial library.
if(NOT KTUTORIAL_FOUND)
    find_package(KTutorial REQUIRED)
    include_directories(${KTUTORIAL_INCLUDE_DIRS})
endif(NOT KTUTORIAL_FOUND)

set(ktutorial_test_app_SRCS
    main.cpp
    TestApp.cpp
    TutorialClearText.cpp
    TutorialMoveText.cpp
)

kde4_add_executable(ktutorial-test-app ${ktutorial_test_app_SRCS})

target_link_libraries(ktutorial-test-app ${KTUTORIAL_LIBRARIES} ${KDE4_KDEUI_LIBS} ${KDE4_KIO_LIBS})

####### Install the test application #######

install(TARGETS ktutorial-test-app DESTINATION ${BIN_INSTALL_DIR})

install(FILES ktutorial-test-appui.rc DESTINATION ${DATA_INSTALL_DIR}/ktutorial-test-app)
install(FILES TutorialClearText.js TutorialClearText.py TutorialMoveText.py DESTINATION ${DATA_INSTALL_DIR}/ktutorial-test-app/tutorials)
