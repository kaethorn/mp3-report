#include "scanner_flac.hxx"

void FLACScanner::scan(boost::filesystem::path file) {
  string fileName(file.string());
  directory = dirname(file);
  TagLib::FLAC::File fileTag(fileName.c_str());

  checkFLACTags(&fileTag);
}

void FLACScanner::checkFLACTags(TagLib::FLAC::File *fileTag) {
  // Retrieve the FLAC tag
  TagLib::Tag *FLACTag = fileTag->tag();

  // Get common frames
  string artist(FLACTag->artist().to8Bit(true));
  string genre(FLACTag->genre().to8Bit(true));
  string album(FLACTag->album().to8Bit(true));

  // Find tracks without an artist tag
  if (artist.size() == 0) {
    addToReport(artist, genre, album, directory, "missing_artist");
  }

  // Find tracks without an album tag
  if (album.size() == 0) {
    addToReport(artist, genre, album, directory, "missing_album");
  }

  // Find tracks without a title tag
  if (FLACTag->title().to8Bit(true).size() == 0) {
    addToReport(artist, genre, album, directory, "missing_title");
  }

  // Find tracks without a year tag
  if (FLACTag->year() == 0) {
    addToReport(artist, genre, album, directory, "missing_year");
  }

  // Find tracks without a track number tag
  if (FLACTag->track() == 0) {
    addToReport(artist, genre, album, directory, "missing_track");
  }

  // Find tracks without a genre tag
  if (artist.size() == 0) {
    addToReport(artist, genre, album, directory, "missing_artist");
  }

  // Find tracks with missing album art
  if (fileTag->pictureList().size() < 1) {
    addToReport(artist, genre, album, directory, "missing_art");
  }

  // Find tracks with more than one album art
  if (fileTag->pictureList().size() > 1) {
    addToReport(artist, genre, album, directory, "multiple_art");
  }
}
