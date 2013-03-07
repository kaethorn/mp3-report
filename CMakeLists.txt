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
 
add_executable(MP3Report report.cxx)

include(ConfigureChecks.cmake)

add_subdirectory(lib)
target_link_libraries(MP3Report reporter)
target_link_libraries(MP3Report scanner)
target_link_libraries(MP3Report scanner_mp3)
target_link_libraries(MP3Report scanner_ogg_vorbis)

include_directories("${PROJECT_BINARY_DIR}")
include_directories("${PROJECT_SOURCE_DIR}/lib")

target_link_libraries(MP3Report ${TAGLIB_LIBRARIES})
target_link_libraries(MP3Report ${Boost_LIBRARIES})
target_link_libraries(MP3Report ${LibMagic_LIBRARY})

# Include tests
enable_testing ()
add_subdirectory(test)
