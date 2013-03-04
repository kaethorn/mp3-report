# MP3 consistency reporter
#
# Author: Falko Schmidt <kaethorn@gmail.com>

require 'find'
require 'mimemagic'
require 'haml'
require_relative 'scanner_mp3'
require_relative 'scanner_ogg_vorbis'
require_relative 'scanner_flac'

class Reporter

  def initialize(directory, report_type)
    @directory = directory
    @report_type = report_type
    @report = {}
  end

  def run
    iterate_directory
    generate
  end

  private

  # Generates a report for the specified directory
  def iterate_directory
    Find.find(@directory) do |file|
      # Only consider files
      unless FileTest.directory?(file)
        raw_file = File.open file
        fileType = MimeMagic.by_path(file)
        magicType = MimeMagic.by_magic(raw_file)
        raw_file.close

        # Only consider audio files
        if fileType == 'audio/mpeg' and magicType == 'audio/mpeg'
          mp3_scanner = MP3Scanner.new file, @report
          mp3_scanner.scan
        elsif fileType == 'audio/ogg' and magicType == 'audio/x-vorbis+ogg'
          ogg_vorbis_scanner = OggVorbisScanner.new file, @report
          ogg_vorbis_scanner.scan
        elsif fileType == 'audio/flac' and magicType == 'audio/flac'
          flac_scanner = FLACScanner.new file, @report
          flac_scanner.scan
        elsif fileType == 'audio/mp4' and magicType == 'audio/mp4'
          # MP4 files are not yet supported in taglib-ruby
        end
      end
    end
  end

  def generate
    template = File.read("report.#{@report_type}.haml")
    haml_engine = Haml::Engine.new(template)
    output = File.new 'report.html', 'w'
    output.write haml_engine.render(Object.new, :report => @report)
    output.close
  end
end
