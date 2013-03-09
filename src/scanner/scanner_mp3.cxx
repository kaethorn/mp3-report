#include <id3v2tag.h>
#include <id3v1tag.h>
#include <id3v2frame.h>
#include <id3v2header.h>
#include <textidentificationframe.h>

#include "scanner_mp3.hxx"

void MP3Scanner::scan() {
  TagLib::MPEG::File file_tag(file.c_str());

  check_id3v1_tags(&file_tag);
  check_id3v2_tags(&file_tag);
}

void MP3Scanner::check_id3v1_tags(TagLib::MPEG::File *file_tag) {
  TagLib::ID3v1::Tag *id3v1_tag = file_tag->ID3v1Tag();
  if (!id3v1_tag->isEmpty()) {
    add_to_report(id3v1_tag->artist().to8Bit(true), id3v1_tag->album().to8Bit(true), dirname(file), "id3v1");
  }
}

void MP3Scanner::check_id3v2_tags(TagLib::MPEG::File *file_tag) {
  TagLib::ID3v2::Tag *id3v2_tag = file_tag->ID3v2Tag();
  string artist(id3v2_tag->artist().to8Bit(true));
  string album(id3v2_tag->album().to8Bit(true));
  string directory(dirname(file));

  // Find tracks with missing album art
  if (id3v2_tag->frameListMap()["APIC"].isEmpty()) {
    add_to_report(artist, album, directory, "missing_art");
  }

  // Find tracks with more than one album art
  if (id3v2_tag->frameListMap()["APIC"].size() > 1) {
    add_to_report(artist, album, directory, "multiple_art");
  }

  // Find tracks containing id3v2 tags with versions lower than
  // id3v2.4
  if (id3v2_tag->header()->majorVersion() < 4) {
    add_to_report(artist, album, directory, "obsolete_id3v2_version");
  }

  // Find track containing album artist tags
  // FIXME album artists aren't bad per se, but check for albums with differing tags
  if (!id3v2_tag->frameListMap()["TPE2"].isEmpty()) {
    add_to_report(id3v2_tag->artist().to8Bit(true), id3v2_tag->album().to8Bit(true), dirname(file), "album_artist");
  }

  // Example for artist tag (is this enough, or shall we test every tag, just in case?)
  TagLib::ID3v2::FrameList artist_tag = id3v2_tag->frameListMap()["TPE1"];
  if (!artist_tag.isEmpty()) {
    TagLib::ID3v2::TextIdentificationFrame *frame = static_cast<TagLib::ID3v2::TextIdentificationFrame *>(artist_tag.front());
    TagLib::String::Type encoding = frame->textEncoding();
    if(encoding != TagLib::String::UTF8) {
      add_to_report(artist, album, directory, "invalid_encoding");
    }
  }
}
