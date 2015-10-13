# .rst:
# FindMimetic
# ----------
#
# Try to find the Mimetic library (vmime)
#
#
#
# Once done this will define
#
# ::
#
#   MIMETIC_FOUND - System has vmime
#   MIMETIC_INCLUDE_DIR - The vmime include directory
#   MIMETIC_LIBRARIES - The libraries needed to use vmime
#   MIMETIC_DEFINITIONS - Compiler switches required for using vmime

#=============================================================================
# Copyright 2009 Kitware, Inc.
# Copyright 2009 Philip Lowman <philip@yhbt.com>
# Copyright 2009 Brad Hards <bradh@kde.org>
# Copyright 2006 Alexander Neundorf <neundorf@kde.org>
#
# Distributed under the OSI-approved BSD License (the "License");
# see accompanying file Copyright.txt for details.
#
# This software is distributed WITHOUT ANY WARRANTY; without even the
# implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
# See the License for more information.
#=============================================================================
# (To distribute this file outside of CMake, substitute the full
#  License text for the above reference.)



if (MIMETIC_INCLUDE_DIR AND MIMETIC_LIBRARY)
   # in cache already
   set(mimetic_FIND_QUIETLY TRUE)
endif ()

if (NOT WIN32)
   # try using pkg-config to get the directories and then use these values
   # in the find_path() and find_library() calls
   # also fills in MIMETIC_DEFINITIONS, although that isn't normally useful
   find_package(PkgConfig QUIET)
   PKG_CHECK_MODULES(PC_MIMETIC QUIET mimetic)
   set(MIMETIC_DEFINITIONS ${PC_MIMETIC_CFLAGS_OTHER})
   set(MIMETIC_VERSION_STRING ${PC_MIMETIC_VERSION})
endif ()

find_path(MIMETIC_INCLUDE_DIR mimetic.h
   HINTS
   ${PC_MIMETIC_INCLUDEDIR}
   ${PC_MIMETIC_INCLUDE_DIRS}
   ${MIMETIC_ROOT}/mimetic/
   )

find_library(MIMETIC_LIBRARY NAMES libmimetic_mt
   HINTS
   ${PC_MIMETIC_LIBDIR}
   ${PC_MIMETIC_LIBRARY_DIRS}
   ${MIMETIC_ROOT}/win32/
   )

mark_as_advanced(MIMETIC_INCLUDE_DIR MIMETIC_LIBRARY)

# handle the QUIETLY and REQUIRED arguments and set Mimetic_FOUND to TRUE if
# all listed variables are TRUE
include(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(Mimetic
                                  REQUIRED_VARS MIMETIC_LIBRARY MIMETIC_INCLUDE_DIR
                                  VERSION_VAR MIMETIC_VERSION_STRING)

if(Mimetic_FOUND)
    set(MIMETIC_LIBRARIES    ${MIMETIC_LIBRARY})
    set(MIMETIC_INCLUDE_DIRS ${MIMETIC_INCLUDE_DIR})
endif()