# Application runs with the required parameters
add_test (MP3Report_runs ${PROJECT_BINARY_DIR}/src/MP3Report .)

# Macro to facilitate running MP3Report with different sample data
macro (run_sample test_name result)
  add_test (MP3Report_${test_name}
    ${PROJECT_BINARY_DIR}/src/MP3Report -r csv
    ${PROJECT_BINARY_DIR}/test/testcases/${test_name})
  set_tests_properties (MP3Report_${test_name}
    PROPERTIES PASS_REGULAR_EXPRESSION ${result})
endmacro (run_sample)

# MP3 with an album artist
run_sample (mp3_album_artist 
  "\"mp3\",\"somealbum\",\".+testcases/mp3_album_artist\",album_artist")

# mp3 with an ID3v1 tag
run_sample (mp3_id3v1 
  "\"mp3\",\"somealbum\",\".+testcases/mp3_id3v1\",id3v1")

# MP3 with an obsolete ID3v2 version
run_sample (mp3_id3v2_obsolete
  "\"mp3\",\"somealbum\",\".+testcases/mp3_id3v2_obsolete\",obsolete_id3v2_version")

# MP3 with an invalid text frame encoding
run_sample (mp3_invalid_encoding
  "\"mp3\",\"somealbum\",\".+testcases/mp3_invalid_encoding\",invalid_encoding")

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

# MP3 with multiple album art
run_sample (mp3_multiple_art
  "\"mp3\",\"somealbum\",\".+testcases/mp3_multiple_art\",multiple_art")

# MP3 without any flaws
run_sample (mp3_perfect
  "^Artist,Album,Directory,Error\\\\(s\\\\)\\n\\n$")


# MPC without any flaws
run_sample (mpc_perfect
  "^Artist,Album,Directory,Error\\\\(s\\\\)\\n\\n$")

# MPC without album art
run_sample (mpc_missing_art
  "\"mpc\",\"somealbum\",\".+testcases/mpc_missing_art\",missing_art")


# APE without any flaws
run_sample (ape_perfect
  "^Artist,Album,Directory,Error\\\\(s\\\\)\\n\\n$")

# APE without album art
run_sample (ape_missing_art
  "\"ape\",\"somealbum\",\".+testcases/ape_missing_art\",missing_art")
