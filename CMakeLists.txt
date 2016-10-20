project(MP3Report)

cmake_minimum_required(VERSION 2.6.0 FATAL_ERROR)

set(MP3REPORT_VERSION_MAJOR 0)
set(MP3REPORT_VERSION_MINOR 1)
set(MP3REPORT_VERSION "${MP3REPORT_VERSION_MAJOR}.${MP3REPORT_VERSION_MINOR}")

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

# Doxygen
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.cmake ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)
add_custom_target(docs doxygen)
