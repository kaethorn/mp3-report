mp3-report
==========

Auio file ID3 tag consistency reporter

## About
Finds out if there's something wrong with albums in your music collection.

### Features
* groups results by artist, album and directory
* finds tracks with missing tags: album, artist, genre, title or track number
* finds tracks without album art
* finds tracks with id3v1 tags
* finds tracks with more than one album art
* finds tracks with id3v2 version < id3v2.4 tags (requires patched taglib-ruby)
* finds tracks that have a text format other than UTF-8
* finds artists with multiple genres
* supports MP3, MP4, FLAC and Ogg Vorbis audio formats

## Setup

On Debian/Ubuntu:

`apt-get install cmake libtag1-dev libmagic-dev libboost-filesystem-dev libboost-program-options-dev libboost-system-dev`

## Compilation

`cmake .`

`make`

## Usage

`./src/MP3Report -h`

## Report type

The following report types are available:
* plain (default)

## TODO
* add more tests and comments (e.g. one per reporter per file format)
