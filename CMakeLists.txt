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

include_directories("${PROJECT_SOURCE_DIR}/lib")
add_subdirectory(lib)

add_executable(MP3Report report.cxx)
target_link_libraries(MP3Report reporter)
target_link_libraries(MP3Report scanner)

include(ConfigureChecks.cmake)
