#include "scanner_flac.hxx"

void FLACScanner::scan() {
  TagLib::FLAC::File fileTag(file.c_str());

  checkFLACTags(&fileTag);
}

void FLACScanner::checkFLACTags(TagLib::FLAC::File *fileTag) {
  // Retrieve the FLAC tag
  TagLib::Tag *FLACTag = fileTag->tag();

  // Get common frames
  string artist(FLACTag->artist().to8Bit(true));
  string genre(FLACTag->genre().to8Bit(true));
  string album(FLACTag->album().to8Bit(true));

  // Find tracks with missing album art
  if (fileTag->pictureList().size() < 1) {
    addToReport(artist, genre, album, directory, "missing_art");
  }

  // Find tracks with more than one album art
  if (fileTag->pictureList().size() > 1) {
    addToReport(artist, genre, album, directory, "multiple_art");
  }
}
