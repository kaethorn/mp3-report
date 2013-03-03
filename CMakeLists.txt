project(MP3Report)

cmake_minimum_required(VERSION 2.6.0 FATAL_ERROR)

set (MP3Report_VERSION_MAJOR 1)
set (MP3Report_VERSION_MINOR 0)

# configure a header file to pass some of the CMake settings
# to the source code
configure_file (
  "${PROJECT_SOURCE_DIR}/report_config.h.in"
  "${PROJECT_BINARY_DIR}/report_config.h"
  )
 
# add the binary tree to the search path for include files
# so that we will find TutorialConfig.h
include_directories("${PROJECT_BINARY_DIR}")

add_executable(MP3Report report.cxx)

include(ConfigureChecks.cmake)