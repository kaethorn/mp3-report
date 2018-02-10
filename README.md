mp3-report
==========

[![CircleCI](https://circleci.com/gh/kaethorn/mp3-report.svg?style=shield&circle-token=:circle-token)](https://circleci.com/gh/kaethorn/mp3-report)
Highly opinionated audio file tag consistency reporter.

## About
Finds out if there's something wrong with albums in your music collection. It's ready-only and produces a report.

### Features
* Groups results by artist, album and directory.
* Supports MP3, MP4, FLAC, WMA, MPC, APE and Ogg Vorbis audio formats.
* Displays scan progress.
* File type detection by name or magic type.
* Various checks:
  * Tags with missing album, artist, album artist, genre, title, year or track number frames
  * Album art that is missing, redudant or has an invalid type or size
  * MP3 files with ID3 tags version lower than 2.4
  * MP3 file Frames with an encoding other than UTF-8
  * Artists with multiple genres
  * Track index frames that are not in the format num/total
  * Disc index frames that are not in the format num/total, unless empty
  * MP3 title frames that got truncated by a conversion from ID3v1 to ID3v2
  * Albums with mixed file types
  * Trailing or multiple spaces
* Gracefully handles `Various Aritst` album artists.
* Provides various report formats, see below.

## Setup

On Debian/Ubuntu:

`apt-get install cmake libtag1-dev libmagic-dev libboost-filesystem-dev libboost-program-options-dev libboost-system-dev libboost-regex-dev`

## Compilation

`cmake .`

`make`

## Usage

`./src/MP3Report -h`

## Report types

The following report types are currently available:
* stdout (default)
* plain text
* CSV

## Tests

Make sure to have ctest installed. Then run

`make test`

to execute tests on various provided sample audio files and their tags.
