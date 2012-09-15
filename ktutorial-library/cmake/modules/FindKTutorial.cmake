# - Try to find KTutorial library
# Once done, this will define
#
#  KTUTORIAL_FOUND - system has KTutorial
#  KTUTORIAL_INCLUDE_DIRS - the KTutorial include directories
#  KTUTORIAL_LIBRARIES - link these to use KTutorial
#  KTUTORIAL_VERSION - the KTutorial version (string value)
#
#  KTUTORIAL_INCLUDE_DIR - where to find ktutorial/KTutorial.h, etc
#  KTUTORIAL_LIBRARY - the KTutorial library
#
# Copyright (C) 2010 Daniel Calviño Sánchez <danxuliu@gmail.com>
# Copyright (C) 2012 Daniel Calviño Sánchez <danxuliu@gmail.com>
#
# Redistribution and use is allowed according to the terms of the BSD license.
# For details see the accompanying COPYING-CMAKE-SCRIPTS file.

find_path(KTUTORIAL_INCLUDE_DIR 
    NAMES ktutorial/ktutorial_export.h
    PATHS ${KDE4_INCLUDE_DIR} ${INCLUDE_INSTALL_DIR}
)

find_library(KTUTORIAL_LIBRARY
    NAMES ktutorial
    PATHS ${KDE4_LIB_DIR} ${LIB_INSTALL_DIR}
)

if(KTUTORIAL_INCLUDE_DIR)
    file(READ ${KTUTORIAL_INCLUDE_DIR}/ktutorial/version.h KTUTORIAL_VERSION_CONTENT)
    string(REGEX MATCH "KTUTORIAL_VERSION_STRING \"(.*)\"\n" KTUTORIAL_VERSION_MATCH "${KTUTORIAL_VERSION_CONTENT}")
    if(KTUTORIAL_VERSION_MATCH)
        set(KTUTORIAL_VERSION "${CMAKE_MATCH_1}")
    endif(KTUTORIAL_VERSION_MATCH)
endif(KTUTORIAL_INCLUDE_DIR)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(KTutorial REQUIRED_VARS KTUTORIAL_LIBRARY KTUTORIAL_INCLUDE_DIR
                                            VERSION_VAR KTUTORIAL_VERSION
)

if(KTUTORIAL_FOUND)
    set(KTUTORIAL_INCLUDE_DIRS ${KTUTORIAL_INCLUDE_DIR})
    set(KTUTORIAL_LIBRARIES ${KTUTORIAL_LIBRARY})
endif(KTUTORIAL_FOUND)

mark_as_advanced(KTUTORIAL_INCLUDE_DIR KTUTORIAL_LIBRARY)
