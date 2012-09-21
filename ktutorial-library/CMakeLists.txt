project(ktutorial-library)

set(QT_MIN_VERSION "4.6")
find_package(KDE4 REQUIRED)

####### KTutorial version #######
set(CMAKE_KTUTORIAL_VERSION_MAJOR 0)
set(CMAKE_KTUTORIAL_VERSION_MINOR 5)
set(CMAKE_KTUTORIAL_VERSION_RELEASE 0)
set(CMAKE_KTUTORIAL_VERSION_STRING "${CMAKE_KTUTORIAL_VERSION_MAJOR}.${CMAKE_KTUTORIAL_VERSION_MINOR}.${CMAKE_KTUTORIAL_VERSION_RELEASE}")

enable_testing()

add_subdirectory(cmake)
add_subdirectory(src)
add_subdirectory(tests)
