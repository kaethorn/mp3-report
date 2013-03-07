#include "scanner_mp3.h"
#include <mpegfile.h>
#include <id3v2tag.h>
#include <id3v1tag.h>
#include <id3v2frame.h>
#include <id3v2header.h>

void MP3Scanner::scan() {
  TagLib::MPEG::File file_tag(file.c_str());

  // Find tracks with id3v1 tags
  report_id3v1_tag(&file_tag);

  // Find tracks with missing album art
  report_missing_art(&file_tag);

  // Find tracks with more than one album art
  report_multiple_art(&file_tag);

  // Find tracks containing id3v2 tags with versions lower than
  // id3v2.4
  report_obsolete_id3v2_version(&file_tag);
}

void MP3Scanner::report_id3v1_tag(TagLib::MPEG::File *file_tag) {
  TagLib::ID3v1::Tag *id3v1_tag = file_tag->ID3v1Tag();
  if (!id3v1_tag->isEmpty()) {
    add_to_report(id3v1_tag->artist().to8Bit(true), id3v1_tag->album().to8Bit(true), dirname(file), "id3v1");
  }
}
void MP3Scanner::report_missing_art(TagLib::MPEG::File *file_tag) {
  TagLib::ID3v2::Tag *id3v2_tag = file_tag->ID3v2Tag();
  if (id3v2_tag->frameListMap()["APIC"].isEmpty()) {
    add_to_report(id3v2_tag->artist().to8Bit(true), id3v2_tag->album().to8Bit(true), dirname(file), "missing_art");
  }
}
void MP3Scanner::report_multiple_art(TagLib::MPEG::File *file_tag) {
  TagLib::ID3v2::Tag *id3v2_tag = file_tag->ID3v2Tag();
  if (id3v2_tag->frameListMap()["APIC"].size() > 1) {
    add_to_report(id3v2_tag->artist().to8Bit(true), id3v2_tag->album().to8Bit(true), dirname(file), "multiple_art");
  }
}
void MP3Scanner::report_obsolete_id3v2_version(TagLib::MPEG::File *file_tag) {
  TagLib::ID3v2::Tag *id3v2_tag = file_tag->ID3v2Tag();
  if (id3v2_tag->header()->majorVersion() < 4) {
    add_to_report(id3v2_tag->artist().to8Bit(true), id3v2_tag->album().to8Bit(true), dirname(file), "obsolete_id3v2_version");
  }
}
