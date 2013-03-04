include(CheckIncludeFile)
include(CheckIncludeFiles)
include(CheckSymbolExists)
include(CheckFunctionExists)
include(CheckLibraryExists)
include(CheckTypeSize)
include(CheckCXXSourceCompiles)

set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules)
find_package(TagLib REQUIRED)
if(NOT TAGLIB_FOUND)
  message(FATAL_ERROR "Taglib could not be found")
endif()

# find_package( Boost 1.36.0 COMPONENTS date_time filesystem system ... )
find_package(Boost 1.49.0 COMPONENTS program_options system filesystem)
if (Boost_FOUND)
  include_directories(${Boost_INCLUDE_DIRS})
  target_link_libraries(MP3Report ${Boost_LIBRARIES})
else()
  message(FATAL_ERROR "boost libraries could not be found (program_options, filesystem)")
endif()
