require 'taglib'
require_relative 'scanner'

class MP3Scanner < Scanner

  def scan
    TagLib::MPEG::File.open(@file) do |file_tag|
      # Find tracks with id3v1 tags
      report_id3v1_tag file_tag

      # Find tracks with missing album art
      report_missing_art file_tag

      # Find tracks with more than one album art
      report_multiple_art file_tag

      # Find tracks containing id3v2 tags with versions lower than
      # id3v2.4
      report_obsolete_id3v2_version file_tag
    end
  end
 
  private

  def report_id3v1_tag(file_tag)
    id3v1_tag = file_tag.id3v1_tag
    unless id3v1_tag.empty?
      add_to_report id3v1_tag.artist, id3v1_tag.album, File.dirname(@file), :id3v1
    end
  end

  def report_missing_art(file_tag)
    id3v2_tag = file_tag.id3v2_tag
    if id3v2_tag.frame_list('APIC').length < 1 
      add_to_report id3v2_tag.artist, id3v2_tag.album, File.dirname(@file), :missing_art
    end 
  end 

  def report_multiple_art(file_tag)
    id3v2_tag = file_tag.id3v2_tag
    if id3v2_tag.frame_list('APIC').length > 1 
      add_to_report id3v2_tag.artist, id3v2_tag.album, File.dirname(@file), :multiple_art
    end 
  end 

  def report_obsolete_id3v2_version(file_tag)
    id3v2_tag = file_tag.id3v2_tag
    if id3v2_tag.header.major_version < 4 
      add_to_report id3v2_tag.artist, id3v2_tag.album, File.dirname(@file), :obsolete_id3v2_version
    end
  end   
end
