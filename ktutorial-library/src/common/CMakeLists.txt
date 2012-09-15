include_directories(${CMAKE_CURRENT_BINARY_DIR} ${KDE4_INCLUDES})

set(ktutorial_common_SRCS
    WindowVisibilitySpy.cpp
)

kde4_add_library(ktutorial_common ${ktutorial_common_SRCS})

target_link_libraries(ktutorial_common ${KDE4_KDEUI_LIBS})
