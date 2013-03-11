#include "scanner_flac.hxx"

void FLACScanner::scan() {
  TagLib::FLAC::File file_tag(file.c_str());

  // Find tracks with missing album art
  report_missing_art(&file_tag);

  // Find tracks with more than one album art
  report_multiple_art(&file_tag);
}

void FLACScanner::report_missing_art(TagLib::FLAC::File *file_tag) {
  TagLib::Tag *flac_tag = file_tag->tag();
  if (file_tag->pictureList().size() < 1) {
    add_to_report(flac_tag->artist().to8Bit(true), flac_tag->genre().to8Bit(true), flac_tag->album().to8Bit(true), dirname(file), "missing_art");
  }
}

void FLACScanner::report_multiple_art(TagLib::FLAC::File *file_tag) {
  TagLib::Tag *flac_tag = file_tag->tag();
  if (file_tag->pictureList().size() > 1) {
    add_to_report(flac_tag->artist().to8Bit(true), flac_tag->genre().to8Bit(true), flac_tag->album().to8Bit(true), dirname(file), "multiple_art");
  }
}
