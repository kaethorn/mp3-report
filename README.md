mp3-report
==========

Audio file tag consistency reporter

## About
Finds out if there's something wrong with albums in your music collection.

### Features
* groups results by artist, album and directory
* finds tracks with missing album, artist, genre, title or track number frames
* finds tracks without album art
* finds tracks with ID3v1 tags
* finds tracks with more than one album art
* finds tracks with ID3v2 version < ID3v2.4 tags (requires patched taglib-ruby)
* finds tracks that have a text format other than UTF-8
* finds artists with multiple genres
* finds ID3v2 tracks with track frames that are not in the format num/total
* supports MP3, MP4, FLAC, WMA, MPC, APE and Ogg Vorbis audio formats

## Setup

On Debian/Ubuntu:

`apt-get install cmake libtag1-dev libmagic-dev libboost-filesystem-dev libboost-program-options-dev libboost-system-dev libboost-regex-dev`

## Compilation

`cmake .`

`make`

## Usage

`./src/MP3Report -h`

## Report type

The following report types are available:
* plain (default)

## Tests

Make sure to have ctest installed. Then run

`make test`

to execute tests on various provided sample audio files and their tags.

## TODO
* HTML reports
* find directories that have differing album names
* find track titles or album names with invalid case
