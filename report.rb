# MP3 consistency reporter
#
# Author: Falko Schmidt <kaethorn@gmail.com>

require 'taglib'
require 'find'
require 'mimemagic'
require 'haml'
require 'optparse'
require 'ostruct'

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

def generate_report(report, report_type)
  template = File.read("report.#{report_type}.haml")
  haml_engine = Haml::Engine.new(template)
  output = File.new 'report.html', 'w'
  output.write haml_engine.render(Object.new, :report => report)
  output.close
end

def main(directory, report_type)
  report = {}
  iterate_directory(directory, report)
  generate_report report, report_type
end

def parseCommandLine
  options = OpenStruct.new
  options.report_type = 'list'
  opts = OptionParser.new do |opts|
    opts.banner = "Usage: report.rb [options] [directory]"
    opts.on("-r", "--report-type [REPORT_TYPE]", [:list, :collapsible], "Report type to use (list*, collapsible)") do |report_type|
      options.report_type = report_type
    end
  end
  opts.parse!

  dir = ARGV[0]

  if ARGV.length != 1
    puts opts
    exit
  end

  [dir,options]
end

dir,options = parseCommandLine
main dir, options.report_type
