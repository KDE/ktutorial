set(ktutorial_editor_serialization_SRCS
    DeserializationException.cpp
    IOException.cpp
    JavaScriptExporter.cpp
    Serialization.cpp
    TutorialReader.cpp
    TutorialWriter.cpp
)

kde4_add_library(ktutorial_editor_serialization ${ktutorial_editor_serialization_SRCS})

target_link_libraries(ktutorial_editor_serialization
                      ktutorial_editor_data
                      ktutorial_editor_util
                      ${KDE4_KIO_LIBS}
)
