#include <id3v2tag.h>
#include <id3v1tag.h>
#include <id3v2frame.h>
#include <id3v2header.h>
#include <textidentificationframe.h>

#include "scanner_mp3.hxx"

MP3Scanner::MP3Scanner(string f, report_map_type* r) : Scanner(f, r) {
  tags_to_check.push_back("TIT2"); // artist
  tags_to_check.push_back("TPE1"); // title
  tags_to_check.push_back("TALB"); // album
  tags_to_check.push_back("TCON"); // genre
};

void MP3Scanner::scan() {
  TagLib::MPEG::File file_tag(file.c_str());

  check_id3v1_tags(&file_tag);
  check_id3v2_tags(&file_tag);
}

void MP3Scanner::check_id3v1_tags(TagLib::MPEG::File *file_tag) {
  TagLib::ID3v1::Tag *id3v1_tag = file_tag->ID3v1Tag();

  // Find tracks with id3v1 tags
  if (!id3v1_tag->isEmpty()) {
    add_to_report(id3v1_tag->artist().to8Bit(true), id3v1_tag->album().to8Bit(true), dirname(file), "id3v1");
  }
}

void MP3Scanner::check_id3v2_tags(TagLib::MPEG::File *file_tag) {
  TagLib::ID3v2::Tag *id3v2_tag = file_tag->ID3v2Tag();
  string artist(id3v2_tag->artist().to8Bit(true));
  string album(id3v2_tag->album().to8Bit(true));
  string directory(dirname(file));

  // Find tracks without an artist tag
  if (artist.size() == 0) {
    add_to_report(artist, album, directory, "missing_artist");
  }

  // Find tracks without an album tag
  if (album.size() == 0) {
    add_to_report(artist, album, directory, "missing_album");
  }

  // Find tracks without a title tag
  if (id3v2_tag->title().to8Bit(true).size() == 0) {
    add_to_report(artist, album, directory, "missing_title");
  }

  // Find tracks without a year tag
  if (id3v2_tag->year() == 0) {
    add_to_report(artist, album, directory, "missing_year");
  }

  // Find tracks without a track number tag
  if (id3v2_tag->track() == 0) {
    add_to_report(artist, album, directory, "missing_track");
  }

  // Find tracks without a genre tag
  if (artist.size() == 0) {
    add_to_report(artist, album, directory, "missing_artist");
  }

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
    add_to_report(artist, album, directory, "album_artist");
  }

  // Make sure the title, artist, album and genre tags are UTF-8 encoded
  for (vector<string>::iterator it = tags_to_check.begin();
      it != tags_to_check.end(); ++it) {
    TagLib::ID3v2::FrameList artist_tag = id3v2_tag->frameListMap()[it->c_str()];
    if (!artist_tag.isEmpty()) {
      TagLib::ID3v2::TextIdentificationFrame *frame =
        dynamic_cast<TagLib::ID3v2::TextIdentificationFrame *>(artist_tag.front());
      if (frame) {
        TagLib::String::Type encoding = frame->textEncoding();
        if(encoding != TagLib::String::UTF8) {
          add_to_report(artist, album, directory, "invalid_encoding");
        }
      }
    }
  }
}
