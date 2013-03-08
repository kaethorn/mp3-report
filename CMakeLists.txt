project(MP3Report)

cmake_minimum_required(VERSION 2.6.0 FATAL_ERROR)

set (MP3Report_VERSION_MAJOR 1)
set (MP3Report_VERSION_MINOR 0)

# configure a header file to pass some of the CMake settings
# to the source code
configure_file (
  "${PROJECT_SOURCE_DIR}/src/report_config.hxx.in"
  "${PROJECT_BINARY_DIR}/src/report_config.hxx"
  )
 
include(ConfigureChecks.cmake)

add_subdirectory(src)

# Include tests
enable_testing ()
add_subdirectory(test)
