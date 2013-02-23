# MP3 consistency reporter
#
# Author: Falko Schmidt <kaethorn@gmail.com>

require 'taglib'
require 'find'
require 'mimemagic'
require 'pp'

# Creates the following structure
#
# { :artist
#   { :album
#     { :directory
#       [ :<error>, ... ]
#     }
#   }
# }
#
# where <error> is a symbol describing the error such as 
# :missing_art or :id3v1
def report(store, artist, album, directory, error)
  store[artist] ||= {}
  store[artist][album] ||= {}
  store[artist][album][directory] ||= []
  store[artist][album][directory] |= [error]
end

def report_id3v1_tag(store, file_tag, file)
  id3v1_tag = file_tag.id3v1_tag
  unless id3v1_tag.empty?
    report store, id3v1_tag.artist, id3v1_tag.album, File.dirname(file), :id3v1
  end
end

def report_missing_art(store, file_tag, file)
  id3v2_tag = file_tag.id3v2_tag
  if id3v2_tag.frame_list('APIC').length < 1
    report store, id3v2_tag.artist, id3v2_tag.album, File.dirname(file), :missing_art
  end
end

# Generates a report for the given directory
def iterate_directory(directory, report)
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
          report_id3v1_tag report, file_tag, file

          # Find tracks with missing album art
          report_missing_art report, file_tag, file
        end
      end
    end
  end
end

def main(directory)
  report = {}
  iterate_directory(directory, report)
  pp report
end

dir = ARGV[0]
unless dir
  puts 'Usage: report.rb DIRECTORY'
  exit
end

main dir
