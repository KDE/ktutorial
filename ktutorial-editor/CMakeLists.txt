project(ktutorial-editor)

set(QT_MIN_VERSION "4.6")
find_package(KDE4 REQUIRED)

include(KDE4Defaults)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${KDE4_ENABLE_EXCEPTIONS}")

add_subdirectory(doc)
add_subdirectory(po)
add_subdirectory(src)
add_subdirectory(tests)
