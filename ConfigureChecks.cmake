include(CheckIncludeFile)
include(CheckIncludeFiles)
include(CheckSymbolExists)
include(CheckFunctionExists)
include(CheckLibraryExists)
include(CheckTypeSize)
include(CheckCXXSourceCompiles)

set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules)

# Detect Taglib
find_package(TagLib REQUIRED)
if(NOT TAGLIB_FOUND)
  message(FATAL_ERROR "Taglib could not be found.")
else()
  include_directories(${TAGLIB_CFLAGS})
  target_link_libraries(MP3Report ${TAGLIB_LIBRARIES})
endif()

# Detect Boost and its components
find_package(Boost 1.49.0 COMPONENTS program_options system filesystem REQUIRED)
if (Boost_FOUND)
  include_directories(${Boost_INCLUDE_DIRS})
  target_link_libraries(MP3Report ${Boost_LIBRARIES})
else()
  message(FATAL_ERROR "boost libraries could not be found (program_options, filesystem).")
endif()

# Detect LibMagic
find_package(LibMagic REQUIRED)
if(NOT LIBMAGIC_FOUND)
  message(FATAL_ERROR "LibMagic could not be found.")
else()
  include_directories(${LibMagic_INCLUDE_DIR})
  target_link_libraries(MP3Report ${LibMagic_LIBRARY})
endif()
