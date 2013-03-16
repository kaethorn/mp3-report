include_directories("${PROJECT_BINARY_DIR}")
include_directories("${PROJECT_SOURCE_DIR}/src/reporter")
include_directories("${PROJECT_SOURCE_DIR}/src/scanner")

add_executable(MP3Report report.cxx)

add_subdirectory(reporter)

target_link_libraries(MP3Report reporter)

add_subdirectory(xdgmime)

target_link_libraries(MP3Report xdgmime)
target_link_libraries(MP3Report xdgmimeint)
target_link_libraries(MP3Report xdgmimeglob)
target_link_libraries(MP3Report xdgmimemagic)
target_link_libraries(MP3Report xdgmimealias)
target_link_libraries(MP3Report xdgmimeicon)
target_link_libraries(MP3Report xdgmimeparent)
target_link_libraries(MP3Report xdgmimecache)

add_subdirectory(scanner)

target_link_libraries(MP3Report scanner)
target_link_libraries(MP3Report scanner_mp3)
target_link_libraries(MP3Report scanner_ogg_vorbis)
target_link_libraries(MP3Report scanner_flac)
target_link_libraries(MP3Report scanner_asf)
target_link_libraries(MP3Report scanner_mp4)
target_link_libraries(MP3Report scanner_meta)
target_link_libraries(MP3Report scanner_file)

target_link_libraries(MP3Report ${TAGLIB_LIBRARIES})
target_link_libraries(MP3Report ${Boost_LIBRARIES})
target_link_libraries(MP3Report ${LibMagic_LIBRARY})
