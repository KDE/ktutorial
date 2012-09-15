include_directories(${CMAKE_CURRENT_BINARY_DIR} ${KDE4_INCLUDES})

set(ktutorial_tutorials_SRCS
    UsingKTutorial.cpp
)

kde4_add_library(ktutorial_tutorials ${ktutorial_tutorials_SRCS})
