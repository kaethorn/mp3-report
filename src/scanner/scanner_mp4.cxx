#include "scanner_mp4.hxx"

void MP4Scanner::scan(boost::filesystem::path file) {
  string fileName(file.string());
  directory = dirname(file);
  TagLib::MP4::File fileTag(fileName.c_str());

  checkMP4Tags(&fileTag);
}

void MP4Scanner::checkMP4Tags(TagLib::MP4::File *fileTag) {
  // Retrieve the MP4 tag
  TagLib::MP4::Tag *MP4Tag = fileTag->tag();

  // Get common frames
  string artist(MP4Tag->artist().to8Bit(true));
  string genre(MP4Tag->genre().to8Bit(true));
  string album(MP4Tag->album().to8Bit(true));

  // Find tracks with missing album art
  if (!MP4Tag->itemListMap().contains("covr")) {
    addToReport(artist, genre, album, directory, "missing_art");
  } else {
  // Find tracks with more than one album art
    if (MP4Tag->itemListMap()["covr"].toCoverArtList().size() > 1) {
      addToReport(artist, genre, album, directory, "multiple_art");
    }
  }
}
