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

  // Find tracks without an artist tag
  if (artist.size() == 0) {
    addToReport(artist, genre, album, directory, "missing_artist");
  }

  // Find tracks without an album tag
  if (album.size() == 0) {
    addToReport(artist, genre, album, directory, "missing_album");
  }

  // Find tracks without a title tag
  if (oggVorbisTag->title().to8Bit(true).size() == 0) {
    addToReport(artist, genre, album, directory, "missing_title");
  }

  // Find tracks without a year tag
  if (oggVorbisTag->year() == 0) {
    addToReport(artist, genre, album, directory, "missing_year");
  }

  // Find tracks without a track number tag
  if (oggVorbisTag->track() == 0) {
    addToReport(artist, genre, album, directory, "missing_track");
  }

  // Find tracks without a genre tag
  if (genre.size() == 0) {
    addToReport(artist, genre, album, directory, "missing_genre");
  }

  // Find tracks with missing album art
  if (oggVorbisTag->fieldListMap()["METADATA_BLOCK_PICTURE"].isEmpty()) {
    addToReport(artist, genre, album, directory, "missing_art");
  } else {
  // Find tracks with more than one album art
    if (oggVorbisTag->fieldListMap()["METADATA_BLOCK_PICTURE"].size() > 1) {
      addToReport(artist, genre, album, directory, "multiple_art");
    }
  }

  // Find tracks containing album artist tags
  if (!oggVorbisTag->fieldListMap()["ALBUMARTIST"].isEmpty()) {
    addToReport(artist, genre, album, directory, "album_artist");
  }
}
