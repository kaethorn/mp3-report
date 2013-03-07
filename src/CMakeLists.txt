add_executable(MP3Report report.cxx)

add_subdirectory(lib)

target_link_libraries(MP3Report reporter)
target_link_libraries(MP3Report scanner)
target_link_libraries(MP3Report scanner_mp3)
target_link_libraries(MP3Report scanner_ogg_vorbis)
target_link_libraries(MP3Report scanner_flac)
target_link_libraries(MP3Report scanner_mp4)

include_directories("${PROJECT_BINARY_DIR}")
include_directories("${PROJECT_SOURCE_DIR}/lib")

target_link_libraries(MP3Report ${TAGLIB_LIBRARIES})
target_link_libraries(MP3Report ${Boost_LIBRARIES})
target_link_libraries(MP3Report ${LibMagic_LIBRARY})
