# MP3 consistency reporter
#
# Author: Falko Schmidt <kaethorn@gmail.com>

require 'taglib'
require 'find'
require 'mimemagic'
require 'pp'

def report_id3v1_tag(store, file_tag, file)
  id3v1_tag = file_tag.id3v1_tag
  unless id3v1_tag.empty?
    store[id3v1_tag.artist] ||= {}
    store[id3v1_tag.artist][id3v1_tag.album] = File.dirname file
  end
end

def report_missing_art(store, file_tag, file)
  id3v2_tag = file_tag.id3v2_tag
  if id3v2_tag.frame_list('APIC').length < 1
    store[id3v2_tag.artist] ||= {}
    store[id3v2_tag.artist][id3v2_tag.album] = File.dirname file
  end
end

def main(directory)
  id3v1_albums = {}
  missing_art_albums = {}

  Find.find(directory) do |file|
    # Only consider files
    unless FileTest.directory?(file)
      raw_file = File.open file
      fileType = MimeMagic.by_path(file)
      magicType = MimeMagic.by_magic(raw_file)
      raw_file.close

      # Only consider audio files
      if fileType == 'audio/mpeg' and magicType == 'audio/mpeg'
        TagLib::MPEG::File.open(file) do |file_tag|

          # Find tracks with id3v1 tags
          report_id3v1_tag id3v1_albums, file_tag, file

          # Find tracks with missing album art
          report_missing_art missing_art_albums, file_tag, file
        end
      end
    end
  end

  pp id3v1_albums
  pp missing_art_albums
end

dir = ARGV[0]
unless dir
  puts 'Usage: report.rb DIRECTORY'
  exit
end

main dir
