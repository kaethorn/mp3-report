mp3-report
==========

Auio file ID3 tag consistency reporter

## About
Finds out if there's something wrong with albums in your music collection.

### Features
* group results by album
* find albums without album art
* find albums with id3v1 tags
* find albums with more than one album art
* find albums with id3v2 version < id3v2.4 tags (requires patched taglib-ruby)
* supports MP3, MP4, FLAC and Ogg Vorbis

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

### Features
* find albums with inconsistent album artist
* find tracks that have a text format other than UTF-8
* group by artist and find differing genres

### Misc
* HTML reports (simple, collapsible)
* add more tests (e.g. one per reporter per file format)
