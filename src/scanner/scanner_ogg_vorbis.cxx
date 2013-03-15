#include "scanner_ogg_vorbis.hxx"

void OggVorbisScanner::scan(boost::filesystem::path file) {
  string fileName(file.string());
  directory = dirname(file);
  TagLib::Ogg::Vorbis::File fileTag(fileName.c_str());

  checkOggVorbisTags(&fileTag);
}

void OggVorbisScanner::checkOggVorbisTags(TagLib::Ogg::Vorbis::File *fileTag) {
  // Retrieve the Ogg Vorbis tag
  TagLib::Ogg::XiphComment *oggVorbisTag = fileTag->tag();

  // Get common frames
  string artist(oggVorbisTag->artist().to8Bit(true));
  string genre(oggVorbisTag->genre().to8Bit(true));
  string album(oggVorbisTag->album().to8Bit(true));

  // Find tracks with missing album art
  if (oggVorbisTag->fieldListMap()["METADATA_BLOCK_PICTURE"].isEmpty()) {
    addToReport(artist, genre, album, directory, "missing_art");
  }
}
