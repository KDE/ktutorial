include_directories(${CMAKE_CURRENT_BINARY_DIR} ${KDE4_INCLUDES})

set(ktutorial_customization_SRCS
    DefaultKdeCustomization.cpp
)

kde4_add_library(ktutorial_customization ${ktutorial_customization_SRCS})

target_link_libraries(ktutorial_customization ktutorial_tutorials ktutorial_view)
