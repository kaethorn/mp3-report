#include <asftag.h>

#include "scanner_asf.hxx"

void ASFScanner::scan(boost::filesystem::path file) {
  string fileName(file.string());
  directory = dirname(file);
  TagLib::ASF::File fileTag(fileName.c_str());

  checkASFTags(&fileTag);
}

void ASFScanner::checkASFTags(TagLib::ASF::File *fileTag) {
  // Retrieve the ASF tag
  TagLib::ASF::Tag *ASFTag = fileTag->tag();

  // Get common frames
  string artist(ASFTag->artist().to8Bit(true));
  string genre(ASFTag->genre().to8Bit(true));
  string album(ASFTag->album().to8Bit(true));

  // Find tracks without an artist tag
  if (artist.size() == 0) {
    addToReport(artist, genre, album, directory, "missing_artist");
  }

  // Find tracks without an album tag
  if (album.size() == 0) {
    addToReport(artist, genre, album, directory, "missing_album");
  }

  // Find tracks without a title tag
  if (ASFTag->title().to8Bit(true).size() == 0) {
    addToReport(artist, genre, album, directory, "missing_title");
  }

  // Find tracks without a year tag
  if (ASFTag->year() == 0) {
    addToReport(artist, genre, album, directory, "missing_year");
  }

  // Find tracks without a track number tag
  if (ASFTag->track() == 0) {
    addToReport(artist, genre, album, directory, "missing_track");
  }

  // Find tracks without a genre tag
  if (genre.size() == 0) {
    addToReport(artist, genre, album, directory, "missing_genre");
  }

  // Find tracks with missing album art
  if (ASFTag->attributeListMap()["WM/Picture"].size() < 1) {
    addToReport(artist, genre, album, directory, "missing_art");
  }

  // Find tracks with more than one album art
  if (ASFTag->attributeListMap()["WM/Picture"].size() > 1) {
    addToReport(artist, genre, album, directory, "multiple_art");
  }
}
