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

  // Find tracks with missing album art
  if (ASFTag->attributeListMap()["WM/Picture"].size() < 1) {
    addToReport(artist, genre, album, directory, "missing_art");
  }

  // Find tracks with more than one album art
  if (ASFTag->attributeListMap()["WM/Picture"].size() > 1) {
    addToReport(artist, genre, album, directory, "multiple_art");
  }
}
