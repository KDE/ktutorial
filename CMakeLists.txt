project(ktutorial)

find_package(KDE4 REQUIRED)

enable_testing()

macro_log_feature(QT_QTDBUS_FOUND
    "QtDBus"
    "Unix-only Qt module used to introspect applications from KTutorial editor"
    "http://qt-project.org/"
    FALSE
)

macro_display_feature_log()

macro_optional_add_subdirectory(ktutorial-library)
macro_optional_add_subdirectory(ktutorial-editor)
macro_optional_add_subdirectory(ktutorial-test-app)
