include_directories(${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR} ${KDE4_INCLUDES})

set(ktutorial_view_SRCS
    StepTextWidget.cpp
    StepWidget.cpp
    TutorialListModel.cpp
    TutorialManagerDialog.cpp
    WindowOnTopEnforcer.cpp
)

kde4_add_ui_files(ktutorial_view_SRCS
    StepWidget.ui
    TutorialManagerDialog.ui
)

kde4_add_library(ktutorial_view ${ktutorial_view_SRCS})

target_link_libraries(ktutorial_view
                      ktutorial_common
                      ktutorial_extendedinformation
                      ${KDE4_KDEUI_LIBS}
)
