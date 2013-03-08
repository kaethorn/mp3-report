#include "scanner_ogg_vorbis.hxx"

void OggVorbisScanner::scan() {
  TagLib::Ogg::Vorbis::File file_tag(file.c_str());

  // Find tracks with missing album art
  report_missing_art(&file_tag);
}

void OggVorbisScanner::report_missing_art(TagLib::Ogg::Vorbis::File *file_tag) {
  TagLib::Ogg::XiphComment *ogg_vorbis_tag = file_tag->tag();
  if (ogg_vorbis_tag->fieldListMap()["METADATA_BLOCK_PICTURE"].isEmpty()) {
    add_to_report(ogg_vorbis_tag->artist().to8Bit(true), ogg_vorbis_tag->album().to8Bit(true), dirname(file), "missing_art");
  }
}
