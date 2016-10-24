#include <boost/regex.hpp>
#include <vorbisfile.h>

#include "scanner_flac.hxx"

void FLACScanner::scan(boost::filesystem::path file) {
  string fileName(file.string());
  directory = dirname(file);
  TagLib::FLAC::File fileTag(fileName.c_str());

  checkFLACTags(&fileTag);
}

void FLACScanner::checkFLACTags(TagLib::FLAC::File *fileTag) {
  // Retrieve the generic tag
  TagLib::Tag *tag = fileTag->tag();
  TagLib::Ogg::XiphComment *oggVorbisTag = fileTag->xiphComment();

  // Get common frames
  string artist(tag->artist().to8Bit(true));
  string genre(tag->genre().to8Bit(true));
  string album(tag->album().to8Bit(true));
  string title(tag->title().to8Bit(true));
  string albumArtist(oggVorbisTag->fieldListMap()["ALBUMARTIST"].isEmpty() ?
    "" : oggVorbisTag->fieldListMap()["ALBUMARTIST"].front().to8Bit(true)
  );

  // Store meta data
  addToMetaData(artist, genre, album, directory, MP3, title, albumArtist);

  // Find tracks without an artist tag
  if (artist.size() == 0) {
    addToReport(artist, genre, album, directory, "missing_artist");
  }

  // Find tracks without an album tag
  if (album.size() == 0) {
    addToReport(artist, genre, album, directory, "missing_album");
  }

  // Find tracks without a title tag
  if (title.size() == 0) {
    addToReport(artist, genre, album, directory, "missing_title");
  }

  // Find tracks without a year tag
  if (tag->year() == 0) {
    addToReport(artist, genre, album, directory, "missing_year");
  }

  // Find tracks without a track number tag
  if (tag->track() == 0) {
    addToReport(artist, genre, album, directory, "missing_track");
  }

  // Find tracks without a genre tag
  if (genre.size() == 0) {
    addToReport(artist, genre, album, directory, "missing_genre");
  }

  // Find tracks without an album artist tag
  if (oggVorbisTag->fieldListMap()["ALBUMARTIST"].isEmpty()) {
    addToReport(artist, genre, album, directory, "missing_album_artist");
  }

  // Find tracks with missing album art
  if (fileTag->pictureList().size() < 1) {
    addToReport(artist, genre, album, directory, "missing_art");
  }

  // Find tracks with more than one album art
  if (fileTag->pictureList().size() > 1) {
    addToReport(artist, genre, album, directory, "multiple_art");
  }

  // Find tracks containing track numbers that are not formatted as <num>/<total>
  if (!oggVorbisTag->fieldListMap()["TRACKNUMBER"].isEmpty()) {
    static const boost::regex e("\\d{2}/\\d{2}");
    string track = oggVorbisTag->fieldListMap()["TRACKNUMBER"].front().to8Bit(true);
    if (!boost::regex_match(track, e)) {
      addToReport(artist, genre, album, directory, "invalid_track");
    }
  }
}
