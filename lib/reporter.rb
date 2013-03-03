# MP3 consistency reporter
#
# Author: Falko Schmidt <kaethorn@gmail.com>

require 'taglib'
require 'find'
require 'mimemagic'
require 'haml'

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

  # Creates the following structure
  #
  # { :artist =>
  #   { :album =>
  #     { :directory =>
  #       [ :<error>, ... ]
  #     }
  #   }
  # }
  #
  # where <error> is a symbol describing the error such as 
  # :missing_art or :id3v1
  def do_report(artist, album, directory, error)
    @report[artist] ||= {}
    @report[artist][album] ||= {}
    @report[artist][album][directory] ||= []
    @report[artist][album][directory] |= [error]
  end

  def report_id3v1_tag(file_tag, file)
    id3v1_tag = file_tag.id3v1_tag
    unless id3v1_tag.empty?
      do_report id3v1_tag.artist, id3v1_tag.album, File.dirname(file), :id3v1
    end
  end

  def report_missing_art(file_tag, file)
    id3v2_tag = file_tag.id3v2_tag
    if id3v2_tag.frame_list('APIC').length < 1
      do_report id3v2_tag.artist, id3v2_tag.album, File.dirname(file), :missing_art
    end
  end

  def report_multiple_art(file_tag, file)
    id3v2_tag = file_tag.id3v2_tag
    if id3v2_tag.frame_list('APIC').length > 1
      do_report id3v2_tag.artist, id3v2_tag.album, File.dirname(file), :multiple_art
    end
  end

  def report_obsolete_id3v2_version(file_tag, file)
    id3v2_tag = file_tag.id3v2_tag
    if id3v2_tag.header.major_version < 4
      do_report id3v2_tag.artist, id3v2_tag.album, File.dirname(file), :obsolete_id3v2_version
    end
  end

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
          TagLib::MPEG::File.open(file) do |file_tag|

            # Find tracks with id3v1 tags
            report_id3v1_tag file_tag, file

            # Find tracks with missing album art
            report_missing_art file_tag, file

            # Find tracks with more than one album art
            report_multiple_art file_tag, file

            # Fine tracks containing id3v2 tags with versions lower than
            # id3v2.4
            report_obsolete_id3v2_version file_tag, file
          end
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
