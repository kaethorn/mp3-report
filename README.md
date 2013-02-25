mp3-report
==========

Album tag consistency reporter

## About
Finds out if there's something wrong with albums in your
music collection.

It should
* group results by album
* find albums without album art
* find albums with more than one album art
* find albums with id3v1 tags
* find albums with id3v2 version < id3v2.4 tags
* find albums with inconsistent album artist
* find tracks that have a text format other than UTF-8
* group by artist and find differing genres

## Requirements

Ruby 1.9 is required.

`gem install taglib-ruby mimemagic`

or

`bundle`

## Usage

`ruby report.rb <directory>`

## TODO
* Support all audio formats (ogg, mp4, aac, flac, ...)
