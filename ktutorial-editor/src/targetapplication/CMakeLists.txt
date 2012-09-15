set(ktutorial_editor_targetapplication_SRCS
    DBusException.cpp
    RemoteClass.cpp
    RemoteEditorSupport.cpp
    RemoteEventSpy.cpp
    RemoteObject.cpp
    RemoteObjectMapper.cpp
    TargetApplication.cpp
)

kde4_add_library(ktutorial_editor_targetapplication ${ktutorial_editor_targetapplication_SRCS})

target_link_libraries(ktutorial_editor_targetapplication
                      ktutorial_editor_util
                      ${QT_QTDBUS_LIBRARY}
                      ${KDE4_KDECORE_LIBS}
)
