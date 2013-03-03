require 'taglib'
require_relative 'scanner'

class FLACScanner < Scanner

  def scan
    TagLib::FLAC::File.open(@file) do |file_tag|
      # Find tracks with missing album art
      report_missing_art file_tag

      # Find tracks with more than one album art
      report_multiple_art file_tag
    end
  end
 
  private

  def report_missing_art(file_tag)
    flac_tag = file_tag.xiph_comment
    if file_tag.picture_list.length < 1
      add_to_report flac_tag.artist, flac_tag.album, File.dirname(@file), :missing_art
    end 
  end

  def report_multiple_art(file_tag)
    flac_tag = file_tag.xiph_comment
    if file_tag.picture_list.length > 1
      add_to_report flac_tag.artist, flac_tag.album, File.dirname(@file), :multiple_art
    end 
  end 
end
