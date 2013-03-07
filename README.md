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
* supports MP3, FLAC and Ogg Vorbis

## Requirements

Make sure cmake is installed.

`cmake .`

`make`

`make install`

## Usage

`./MP3Report -h`

## Report type

The following report types are available:
* List (plain, non-interactive) (default)
* Collapsible (bootstrap accordion)

## TODO

### Features
* find albums with inconsistent album artist
* find tracks that have a text format other than UTF-8
* group by artist and find differing genres

### Misc
* Support MP4 audio format
* Collapsible Report: allow expanding multiple elements
* Pass pointers where possible
* Fix project file structure (src, ...)
