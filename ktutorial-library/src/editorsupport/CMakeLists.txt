include_directories(${CMAKE_CURRENT_BINARY_DIR} ${KDE4_INCLUDES})

set(ktutorial_editorsupport_SRCS
    ClassRegisterAdaptor.cpp
    EditorSupport.cpp
    EditorSupportAdaptor.cpp
    EventSpy.cpp
    EventSpyAdaptor.cpp
    ObjectRegister.cpp
    ObjectRegisterAdaptor.cpp
)

kde4_add_library(ktutorial_editorsupport ${ktutorial_editorsupport_SRCS})

target_link_libraries(ktutorial_editorsupport
                      ktutorial_extendedinformation
                      ktutorial_scripting
                      ${QT_QTDBUS_LIBRARY}
                      ${KDE4_KDECORE_LIBS}
)
