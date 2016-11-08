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
  string title(MP4Tag->title().to8Bit(true));
  string albumArtist(MP4Tag->itemListMap()["aART"].toStringList().size() == 0 ?
    "" : MP4Tag->itemListMap()["aART"].toStringList().toString().to8Bit(true)
  );
  string track(MP4Tag->itemListMap()["trkn"].toStringList().size() == 0 ?
    "" : MP4Tag->itemListMap()["trkn"].toStringList().toString().to8Bit(true)
  );
  string disc(MP4Tag->itemListMap()["disc"].toStringList().size() == 0 ?
    "" : MP4Tag->itemListMap()["disc"].toStringList().toString().to8Bit(true)
  );

  // Store meta data
  addToMetaData(artist, genre, album, directory, MP3, title, albumArtist, track, disc);

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
  if (MP4Tag->year() == 0) {
    addToReport(artist, genre, album, directory, "missing_year");
  }

  // Find tracks without a track number tag
  if (MP4Tag->track() == 0) {
    addToReport(artist, genre, album, directory, "missing_track");
  }

  // Find tracks without a genre tag
  if (genre.size() == 0) {
    addToReport(artist, genre, album, directory, "missing_genre");
  }

  // Find tracks without an album artist tag
  if (!MP4Tag->itemListMap().contains("aART") ||
      MP4Tag->itemListMap()["aART"].toStringList().size() == 0) {
    addToReport(artist, genre, album, directory, "missing_album_artist");
  }

  // Find tracks with missing album art
  if (!MP4Tag->itemListMap().contains("covr")) {
    addToReport(artist, genre, album, directory, "missing_art");

  // Find tracks with more than one album art
  } else if (MP4Tag->itemListMap()["covr"].toCoverArtList().size() > 1) {
    addToReport(artist, genre, album, directory, "multiple_art");
  }
}
