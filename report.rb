# MP3 consistency reporter
#
# Finds out if there's something wrong with albums in your
# collection.
# It should
#  * find tracks without album art
#  * find tracks with more than one album art
#  * find tracks with id3v1 tags
#  * find tracks with < id3v2.4 tags
#  * find tracks with album artist set
#  * find tracks that have a text format other than UTF-8
#  * group by artist and find differing genres
#  * group results by album
#
# TODO
# 1. Support all audio formats (ogg, mp4, aac, flac, ...)
# 2. Ignore albums that
#    - have no id3v1 tag in any of their tracks
#    - have a picture in all their tracks

require 'taglib'
require 'find'
require 'mimemagic'

dir = ARGV[0]
unless dir
  puts 'Usage: report.rb DIRECTORY'
  exit
end

Find.find(dir) do |file|
  unless FileTest.directory?(file)
    raw_file = File.open file
    fileType = MimeMagic.by_path(file)
    magicType = MimeMagic.by_magic(raw_file)
    raw_file.close

    if fileType == 'audio/mpeg' and magicType == 'audio/mpeg'
      TagLib::MPEG::File.open(file) do |f|
        id3v1_tag = f.id3v1_tag
        id3v2_tag = f.id3v2_tag
        puts "#{id3v1_tag.artist}: #{file}" if id3v1_tag
        #puts "id3v2: #{file}" if id3v2_tag
      end
    end
  end
end
