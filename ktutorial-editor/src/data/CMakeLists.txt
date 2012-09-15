set(ktutorial_editor_data_SRCS
    Reaction.cpp
    Step.cpp
    Tutorial.cpp
    WaitFor.cpp
    WaitForComposed.cpp
    WaitForEvent.cpp
    WaitForNot.cpp
    WaitForProperty.cpp
    WaitForSignal.cpp
    WaitForStepActivation.cpp
    WaitForWindow.cpp
)

kde4_add_library(ktutorial_editor_data ${ktutorial_editor_data_SRCS})
