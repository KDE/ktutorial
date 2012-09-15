include_directories(${CMAKE_CURRENT_BINARY_DIR} ${KDE4_INCLUDES})

set(ktutorial_scripting_SRCS
    ScriptedStep.cpp
    ScriptedTutorial.cpp
    ScriptingModule.cpp
    ScriptManager.cpp
)

kde4_add_library(ktutorial_scripting ${ktutorial_scripting_SRCS})

target_link_libraries(ktutorial_scripting ${KDE4_KDECORE_LIBS} ${KDE4_KROSSCORE_LIBS})
