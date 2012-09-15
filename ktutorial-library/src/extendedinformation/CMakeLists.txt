include_directories(${CMAKE_CURRENT_BINARY_DIR} ${KDE4_INCLUDES})

set(ktutorial_extendedinformation_SRCS
    WidgetHighlighter.cpp
    WidgetHighlighterManager.cpp
)

kde4_add_library(ktutorial_extendedinformation ${ktutorial_extendedinformation_SRCS})

target_link_libraries(ktutorial_extendedinformation ${KDE4_KDEUI_LIBS})
