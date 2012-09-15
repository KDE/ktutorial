set(ktutorial_editor_commands_SRCS
    ReactionCommands.cpp
    StepCommands.cpp
    TutorialCommands.cpp
)

kde4_add_library(ktutorial_editor_commands ${ktutorial_editor_commands_SRCS})

target_link_libraries(ktutorial_editor_commands ktutorial_editor_data ${KDE4_KDEUI_LIBS})
