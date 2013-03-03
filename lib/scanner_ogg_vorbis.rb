require 'taglib'
require_relative 'scanner'

class OggVorbisScanner < Scanner

  def scan
    TagLib::Ogg::Vorbis::File.open(@file) do |file_tag|
      # Find tracks with missing album art
      report_missing_art file_tag
    end
  end
 
  private

  def report_missing_art(file_tag)
    ogg_vorbis_tag = file_tag.tag
    unless ogg_vorbis_tag.field_list_map['METADATA_BLOCK_PICTURE']
      add_to_report ogg_vorbis_tag.artist, ogg_vorbis_tag.album, File.dirname(@file), :missing_art
    end 
  end 
end
