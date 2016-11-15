# Application runs with the required parameters
add_test (MP3Report_runs ${PROJECT_BINARY_DIR}/src/MP3Report .)

# Macro to facilitate running MP3Report with different sample data
macro (run_sample test_name result)
  set(params "")
  foreach(param ${ARGN})
    list(APPEND params ${param})
  endforeach()
  add_test (MP3Report_${test_name}
    ${PROJECT_BINARY_DIR}/src/MP3Report -q -r csv ${params}
    ${PROJECT_BINARY_DIR}/test/testcases/${test_name})
  set_tests_properties (MP3Report_${test_name}
    PROPERTIES PASS_REGULAR_EXPRESSION ${result})
endmacro (run_sample)


### MP3 samples
#
# MP3 without an album artist
run_sample (mp3_missing_album_artist
  "\"mp3\",\"somealbum\",\".+testcases/mp3_missing_album_artist\",missing_album_artist")

# MP3 with an ID3v1 tag
run_sample (mp3_id3v1
  "\"mp3\",\"somealbum\",\".+testcases/mp3_id3v1\",id3v1")

# MP3 with an obsolete ID3v2 version
run_sample (mp3_id3v2_obsolete
  "\"mp3\",\"somealbum\",\".+testcases/mp3_id3v2_obsolete\",invalid_encoding|obsolete_id3v2_version")

# MP3 with an invalid text frame encoding
run_sample (mp3_invalid_encoding
  "\"mp3\",\"somealbum\",\".+testcases/mp3_invalid_encoding\",invalid_encoding")

# MP3 with an invalid album art type
run_sample (mp3_invalid_art_type
  "\"mp3\",\"somealbum\",\".+testcases/mp3_invalid_art_type\",invalid_art")

# MP3 with an invalid album art size
run_sample (mp3_invalid_art_size
  "\"mp3\",\"somealbum\",\".+testcases/mp3_invalid_art_size\",invalid_art")

# MP3 without an album
run_sample (mp3_missing_album
  "\"mp3\",\"\",\".+testcases/mp3_missing_album\",missing_album")

# MP3 without album art
run_sample (mp3_missing_art
  "\"mp3\",\"somealbum\",\".+testcases/mp3_missing_art\",missing_art")

# MP3 without an artist
run_sample (mp3_missing_artist
  "\"\",\"somealbum\",\".+testcases/mp3_missing_artist\",missing_artist")

# MP3 without a genre
run_sample (mp3_missing_genre
  "\"mp3\",\"somealbum\",\".+testcases/mp3_missing_genre\",missing_genre")

# MP3 without a title
run_sample (mp3_missing_title
  "\"mp3\",\"somealbum\",\".+testcases/mp3_missing_title\",missing_title")

# MP3 without a year
run_sample (mp3_missing_year
  "\"mp3\",\"somealbum\",\".+testcases/mp3_missing_year\",missing_year")

# MP3 without a track number
run_sample (mp3_missing_track
  "\"mp3\",\"somealbum\",\".+testcases/mp3_missing_track\",missing_track")

# MP3 with an invalid track number format
run_sample (mp3_invalid_track
  "\"mp3\",\"somealbum\",\".+testcases/mp3_invalid_track\",invalid_track")

# MP3 with an invalid track number format
run_sample (mp3_invalid_disc
  "\"mp3\",\"somealbum\",\".+testcases/mp3_invalid_disc\",invalid_disc")

# MP3 with an invalid track number format
run_sample (mp3_valid_disc
  "^Artist,Album,Directory,Error\\\\(s\\\\)\\n\\n$")

# MP3 with triple digit track numbers
run_sample (mp3_valid_track
  "\"mp3\",\"somealbum\",\".+testcases/mp3_valid_track\",incomplete_album")

# MP3 with multiple album art
run_sample (mp3_multiple_art
  "\"mp3\",\"somealbum\",\".+testcases/mp3_multiple_art\",multiple_art")

# MP3 with an album frame containing invalid white spaces
run_sample (mp3_spaces_leading_album
  "\"mp3\",\"an album\\ \",\".+testcases/mp3_spaces_leading_album\",spaces_album")
run_sample (mp3_spaces_trailing_album
  "\"mp3\",\"\\ another album\",\".+testcases/mp3_spaces_trailing_album\",spaces_album")
run_sample (mp3_spaces_multiple_album
  "\"mp3\",\"spacious\\ \\ album\",\".+testcases/mp3_spaces_multiple_album\",spaces_album")
run_sample (mp3_spaces_invalid_album
  "\"mp3\",\"tabbed\\talbum\",\".+testcases/mp3_spaces_invalid_album\",spaces_album")

# MP3 with an artist frame containing invalid white spaces
run_sample (mp3_spaces_leading_artist
  "\"\\ some\\ artist\",\"album1\",\".+testcases/mp3_spaces_leading_artist\",spaces_artist")
run_sample (mp3_spaces_trailing_artist
  "\"some\\ artist\\ \",\"album2\",\".+testcases/mp3_spaces_trailing_artist\",spaces_artist")
run_sample (mp3_spaces_multiple_artist
  "\"some\\ \\ artist\",\"album3\",\".+testcases/mp3_spaces_multiple_artist\",spaces_artist")
run_sample (mp3_spaces_invalid_artist
  "\"some\\tartist\",\"album4\",\".+testcases/mp3_spaces_invalid_artist\",spaces_artist")

# MP3 with an album artist frame containing invalid white spaces
run_sample (mp3_spaces_leading_albumartist
  "\"mp3\",\"album1\",\".+testcases/mp3_spaces_leading_albumartist\",spaces_albumartist")
run_sample (mp3_spaces_trailing_albumartist
  "\"mp3\",\"album2\",\".+testcases/mp3_spaces_trailing_albumartist\",spaces_albumartist")
run_sample (mp3_spaces_multiple_albumartist
  "\"mp3\",\"album3\",\".+testcases/mp3_spaces_multiple_albumartist\",spaces_albumartist")
run_sample (mp3_spaces_invalid_albumartist
  "\"mp3\",\"album4\",\".+testcases/mp3_spaces_invalid_albumartist\",spaces_albumartist")

# MP3 with a title frame containing invalid white spaces
run_sample (mp3_spaces_leading_title
  "\"mp3\",\"album1\",\".+testcases/mp3_spaces_leading_title\",spaces_title")
run_sample (mp3_spaces_trailing_title
  "\"mp3\",\"album2\",\".+testcases/mp3_spaces_trailing_title\",spaces_title")
run_sample (mp3_spaces_multiple_title
  "\"mp3\",\"album3\",\".+testcases/mp3_spaces_multiple_title\",spaces_title")
run_sample (mp3_spaces_invalid_title
  "\"mp3\",\"album4\",\".+testcases/mp3_spaces_invalid_title\",spaces_title")

# MP3 without any flaws
run_sample (mp3_perfect
  "^Artist,Album,Directory,Error\\\\(s\\\\)\\n\\n$")


### Meta information samples
#
# Song with truncated title
run_sample (meta_truncated
  "\"somealbumartist\",\"somealbum\",\".+testcases/meta_truncated\",truncation_warning" "-w")

# Song with non-truncated titles
run_sample (meta_non_truncated
  "^Artist,Album,Directory,Error\\\\(s\\\\)\\n\\n$" "-w")

# Artists with multiple genres
run_sample (meta_multiple_artist_genres
  "\"somealbumartist\",\"somealbum\",\".+testcases/meta_multiple_artist_genres\",multiple_artist_genres")

# Various artists with multiple genres
run_sample (meta_various_artist_genres
  "^Artist,Album,Directory,Error\\\\(s\\\\)\\n\\n$")

# Various artists albums should be valid
run_sample (meta_valid_various_artists
  "^Artist,Album,Directory,Error\\\\(s\\\\)\\n\\n$")

# Album names can appear twice for different artists
run_sample (meta_valid_same_album_name
  "^Artist,Album,Directory,Error\\\\(s\\\\)\\n\\n$")

# Inconsistent track sequences
run_sample (meta_incomplete_album
  "\"mp3\",\"somealbum\",\".+testcases/meta_incomplete_album\",incomplete_album")

# Inconsistent disc sequences
run_sample (meta_incomplete_collection
  "\"mp3\",\"somealbum\",\".+testcases/meta_incomplete_collection\",incomplete_collection")


### Ogg samples
#
# Ogg without an album artist
run_sample (ogg_missing_album_artist
  "\"ogg\",\"somealbum\",\".+testcases/ogg_missing_album_artist\",missing_album_artist")

# Ogg without an album
run_sample (ogg_missing_album
  "\"ogg\",\"\",\".+testcases/ogg_missing_album\",missing_album")

# Ogg without album art
run_sample (ogg_missing_art
  "\"ogg\",\"somealbum\",\".+testcases/ogg_missing_art\",missing_art")

# Ogg without an artist
run_sample (ogg_missing_artist
  "\"\",\"somealbum\",\".+testcases/ogg_missing_artist\",missing_artist")

# Ogg without a genre
run_sample (ogg_missing_genre
  "\"ogg\",\"somealbum\",\".+testcases/ogg_missing_genre\",missing_genre")

# Ogg without a title
run_sample (ogg_missing_title
  "\"ogg\",\"somealbum\",\".+testcases/ogg_missing_title\",missing_title")

# Ogg without a year
run_sample (ogg_missing_year
  "\"ogg\",\"somealbum\",\".+testcases/ogg_missing_year\",missing_year")

# Ogg without a track number
run_sample (ogg_missing_track
  "\"ogg\",\"somealbum\",\".+testcases/ogg_missing_track\",missing_track")

# Ogg with an invalid track number format
run_sample (ogg_invalid_track
  "\"ogg\",\"somealbum\",\".+testcases/ogg_invalid_track\",invalid_track")

# Ogg with invalid album art
run_sample (ogg_invalid_art_type
  "\"ogg\",\"somealbum\",\".+testcases/ogg_invalid_art_type\",invalid_art")

# Ogg with multiple album art
run_sample (ogg_multiple_art
  "\"ogg\",\"somealbum\",\".+testcases/ogg_multiple_art\",multiple_art")

# Ogg with an invalid album art size
run_sample (ape_invalid_art_size
  "\"ogg\",\"somealbum\",\".+testcases/ogg_invalid_art_size\",invalid_art")

# Ogg without any flaws
run_sample (ogg_perfect
  "^Artist,Album,Directory,Error\\\\(s\\\\)\\n\\n$")


### MP4 samples
#
# MP4 without an album artist
run_sample (mp4_missing_album_artist
  "\"mp4\",\"somealbum\",\".+testcases/mp4_missing_album_artist\",missing_album_artist")

# MP4 without an album
run_sample (mp4_missing_album
  "\"mp4\",\"\",\".+testcases/mp4_missing_album\",missing_album")

# MP4 without album art
run_sample (mp4_missing_art
  "\"mp4\",\"somealbum\",\".+testcases/mp4_missing_art\",missing_art")

# MP4 without an artist
run_sample (mp4_missing_artist
  "\"\",\"somealbum\",\".+testcases/mp4_missing_artist\",missing_artist")

# MP4 without a genre
run_sample (mp4_missing_genre
  "\"mp4\",\"somealbum\",\".+testcases/mp4_missing_genre\",missing_genre")

# MP4 without a title
run_sample (mp4_missing_title
  "\"mp4\",\"somealbum\",\".+testcases/mp4_missing_title\",missing_title")

# MP4 without a year
run_sample (mp4_missing_year
  "\"mp4\",\"somealbum\",\".+testcases/mp4_missing_year\",missing_year")

# MP4 without a track number
run_sample (mp4_missing_track
  "\"mp4\",\"somealbum\",\".+testcases/mp4_missing_track\",missing_track")

# MP4 with multiple album art
run_sample (mp4_multiple_art
  "\"mp4\",\"somealbum\",\".+testcases/mp4_multiple_art\",multiple_art")

# MP4 with an invalid album art size
run_sample (mp4_invalid_art_size
  "\"mp4\",\"somealbum\",\".+testcases/mp4_invalid_art_size\",invalid_art")

# MP4 without any flaws
run_sample (mp4_perfect
  "^Artist,Album,Directory,Error\\\\(s\\\\)\\n\\n$")


### WMA samples
#
# WMA without an album artist
run_sample (wma_missing_album_artist
  "\"wma\",\"somealbum\",\".+testcases/wma_missing_album_artist\",missing_album_artist")

# WMA without an album
run_sample (wma_missing_album
  "\"wma\",\"\",\".+testcases/wma_missing_album\",missing_album")

# WMA without album art
run_sample (wma_missing_art
  "\"wma\",\"somealbum\",\".+testcases/wma_missing_art\",missing_art")

# WMA without an artist
run_sample (wma_missing_artist
  "\"\",\"somealbum\",\".+testcases/wma_missing_artist\",missing_artist")

# WMA without a genre
run_sample (wma_missing_genre
  "\"wma\",\"somealbum\",\".+testcases/wma_missing_genre\",missing_genre")

# WMA without a title
run_sample (wma_missing_title
  "\"wma\",\"somealbum\",\".+testcases/wma_missing_title\",missing_title")

# WMA without a year
run_sample (wma_missing_year
  "\"wma\",\"somealbum\",\".+testcases/wma_missing_year\",missing_year")

# WMA without a track number
run_sample (wma_missing_track
  "\"wma\",\"somealbum\",\".+testcases/wma_missing_track\",missing_track")

# WMA with an invalid track number format
run_sample (wma_invalid_track
  "\"wma\",\"somealbum\",\".+testcases/wma_invalid_track\",invalid_track")

# WMA with multiple album art
run_sample (wma_multiple_art
  "\"wma\",\"somealbum\",\".+testcases/wma_multiple_art\",multiple_art")

# WMA with an invalid album art size
run_sample (ape_invalid_art_size
  "\"wma\",\"somealbum\",\".+testcases/wma_invalid_art_size\",invalid_art")

# WMA without any flaws
run_sample (wma_perfect
  "^Artist,Album,Directory,Error\\\\(s\\\\)\\n\\n$")


### FLAC samples
#
# FLAC without an album artist
run_sample (flac_missing_album_artist
  "\"flac\",\"somealbum\",\".+testcases/flac_missing_album_artist\",missing_album_artist")

# FLAC without an album
run_sample (flac_missing_album
  "\"flac\",\"\",\".+testcases/flac_missing_album\",missing_album")

# FLAC without album art
run_sample (flac_missing_art
  "\"flac\",\"somealbum\",\".+testcases/flac_missing_art\",missing_art")

# FLAC without an artist
run_sample (flac_missing_artist
  "\"\",\"somealbum\",\".+testcases/flac_missing_artist\",missing_artist")

# FLAC without a genre
run_sample (flac_missing_genre
  "\"flac\",\"somealbum\",\".+testcases/flac_missing_genre\",missing_genre")

# FLAC without a title
run_sample (flac_missing_title
  "\"flac\",\"somealbum\",\".+testcases/flac_missing_title\",missing_title")

# FLAC without a year
run_sample (flac_missing_year
  "\"flac\",\"somealbum\",\".+testcases/flac_missing_year\",missing_year")

# FLAC without a track number
run_sample (flac_missing_track
  "\"flac\",\"somealbum\",\".+testcases/flac_missing_track\",missing_track")

# FLAC with an invalid track number format
run_sample (flac_invalid_track
  "\"flac\",\"somealbum\",\".+testcases/flac_invalid_track\",invalid_track")

# FLAC with invalid album art
run_sample (flac_invalid_art_type
  "\"flac\",\"somealbum\",\".+testcases/flac_invalid_art_type\",invalid_art")

# FLAC with multiple album art
run_sample (flac_multiple_art
  "\"flac\",\"somealbum\",\".+testcases/flac_multiple_art\",multiple_art")

# FLAC without any flaws
run_sample (flac_perfect
  "^Artist,Album,Directory,Error\\\\(s\\\\)\\n\\n$")


### MPC samples
#
# MPC without an album artist
run_sample (mpc_missing_album_artist
  "\"mpc\",\"somealbum\",\".+testcases/mpc_missing_album_artist\",missing_album_artist")

# MPC without an album
run_sample (mpc_missing_album
  "\"mpc\",\"\",\".+testcases/mpc_missing_album\",missing_album")

# MPC without album art
run_sample (mpc_missing_art
  "\"mpc\",\"somealbum\",\".+testcases/mpc_missing_art\",missing_art")

# MPC without an artist
run_sample (mpc_missing_artist
  "\"\",\"somealbum\",\".+testcases/mpc_missing_artist\",missing_artist")

# MPC without a genre
run_sample (mpc_missing_genre
  "\"mpc\",\"somealbum\",\".+testcases/mpc_missing_genre\",missing_genre")

# MPC without a title
run_sample (mpc_missing_title
  "\"mpc\",\"somealbum\",\".+testcases/mpc_missing_title\",missing_title")

# MPC without a year
run_sample (mpc_missing_year
  "\"mpc\",\"somealbum\",\".+testcases/mpc_missing_year\",missing_year")

# MPC without a track number
run_sample (mpc_missing_track
  "\"mpc\",\"somealbum\",\".+testcases/mpc_missing_track\",missing_track")

# MPC with an invalid track number format
run_sample (mpc_invalid_track
  "\"mpc\",\"somealbum\",\".+testcases/mpc_invalid_track\",invalid_track")

# MPC with multiple album art (not yet implemented)
#run_sample (mpc_multiple_art
#  "\"mpc\",\"somealbum\",\".+testcases/mpc_multiple_art\",multiple_art")

# MPC without any flaws
run_sample (mpc_perfect
  "^Artist,Album,Directory,Error\\\\(s\\\\)\\n\\n$")


### APE samples
#
# APE without an album artist
run_sample (ape_missing_album_artist
  "\"ape\",\"somealbum\",\".+testcases/ape_missing_album_artist\",missing_album_artist")

# APE without an album
run_sample (ape_missing_album
  "\"ape\",\"\",\".+testcases/ape_missing_album\",missing_album")

# APE without album art
run_sample (ape_missing_art
  "\"ape\",\"somealbum\",\".+testcases/ape_missing_art\",missing_art")

# APE without an artist
run_sample (ape_missing_artist
  "\"\",\"somealbum\",\".+testcases/ape_missing_artist\",missing_artist")

# APE without a genre
run_sample (ape_missing_genre
  "\"ape\",\"somealbum\",\".+testcases/ape_missing_genre\",missing_genre")

# APE without a title
run_sample (ape_missing_title
  "\"ape\",\"somealbum\",\".+testcases/ape_missing_title\",missing_title")

# APE without a year
run_sample (ape_missing_year
  "\"ape\",\"somealbum\",\".+testcases/ape_missing_year\",missing_year")

# APE without a track number
run_sample (ape_missing_track
  "\"ape\",\"somealbum\",\".+testcases/ape_missing_track\",missing_track")

# APE with an invalid track number format
run_sample (ape_invalid_track
  "\"ape\",\"somealbum\",\".+testcases/ape_invalid_track\",invalid_track")

# APE with multiple album art (not yet implemented)
#run_sample (ape_multiple_art
#  "\"ape\",\"somealbum\",\".+testcases/ape_multiple_art\",multiple_art")

# APE with an invalid album art size
run_sample (ape_invalid_art_size
  "\"ape\",\"somealbum\",\".+testcases/ape_invalid_art_size\",invalid_art")


# APE without any flaws
run_sample (ape_perfect
  "^Artist,Album,Directory,Error\\\\(s\\\\)\\n\\n$")
