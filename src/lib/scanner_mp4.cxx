#include "scanner_mp4.h"

void MP4Scanner::scan() {
  TagLib::MP4::File file_tag(file.c_str());

  // Find tracks with missing album art
  report_missing_art(&file_tag);

  // Find tracks with more than one album art
  report_multiple_art(&file_tag);
}

void MP4Scanner::report_missing_art(TagLib::MP4::File *file_tag) {
  TagLib::MP4::Tag *mp4_tag = file_tag->tag();
  if (!mp4_tag->itemListMap().contains("covr")) {
    add_to_report(mp4_tag->artist().to8Bit(true), mp4_tag->album().to8Bit(true), dirname(file), "missing_art");
  }
}

void MP4Scanner::report_multiple_art(TagLib::MP4::File *file_tag) {
  TagLib::MP4::Tag *mp4_tag = file_tag->tag();
  if (mp4_tag->itemListMap().contains("covr")) {
    TagLib::MP4::CoverArtList cover_art_list(mp4_tag->itemListMap()["covr"].toCoverArtList());
    if (cover_art_list.size() > 1) {
      add_to_report(mp4_tag->artist().to8Bit(true), mp4_tag->album().to8Bit(true), dirname(file), "multiple_art");
    }
  }
}
