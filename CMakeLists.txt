project(ktutorial)

find_package(KDE4 REQUIRED)

enable_testing()

macro_optional_add_subdirectory(ktutorial-library)
macro_optional_add_subdirectory(ktutorial-editor)
macro_optional_add_subdirectory(ktutorial-test-app)
