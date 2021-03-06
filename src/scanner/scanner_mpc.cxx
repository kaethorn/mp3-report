#include "scanner_mpc.hxx"

void MPCScanner::scan(boost::filesystem::path file) {
  string fileName(file.string());
  directory = dirname(file);
  TagLib::MPC::File fileTag(fileName.c_str());

  checkMPCTags(&fileTag);
}

uint MPCScanner::getPictureSize(TagLib::APE::Tag* tag) {
  uint size = 0;
  const TagLib::ByteVector nullStringTerminator(1, 0);
  TagLib::ByteVector albumArt = tag->itemListMap()["COVER ART (FRONT)"].value();
  int pos = albumArt.find(nullStringTerminator);

  if (++pos > 0) {
    const TagLib::ByteVector &bytes = albumArt.mid(pos);
    size = bytes.size();
  }

  return size;
}

void MPCScanner::checkMPCTags(TagLib::MPC::File *fileTag) {
  // Retrieve the generic and MPC tags
  TagLib::Tag *tag = fileTag->tag();
  TagLib::APE::Tag *APETag = fileTag->APETag();

  // Get common frames
  string artist(tag->artist().to8Bit(true));
  string genre(tag->genre().to8Bit(true));
  string album(tag->album().to8Bit(true));
  string title(tag->title().to8Bit(true));
  string albumArtist(APETag->itemListMap()["ALBUMARTIST"].isEmpty() ?
    "" : APETag->itemListMap()["ALBUMARTIST"].toString().to8Bit(true)
  );
  string track(APETag->itemListMap()["TRACK"].isEmpty() ?
    "" : APETag->itemListMap()["TRACK"].toString().to8Bit(true)
  );
  string disc(APETag->itemListMap()["DISC"].isEmpty() ?
    "" : APETag->itemListMap()["DISC"].toString().to8Bit(true)
  );

  // Store meta data
  addToMetaData(artist, genre, album, directory, MPC, title, albumArtist, track, disc);

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
  if (!APETag->itemListMap().contains("ALBUMARTIST") ||
      APETag->itemListMap()["ALBUMARTIST"].isEmpty()) {
    addToReport(artist, genre, album, directory, "missing_album_artist");
  }

  // Find tracks with missing album art
  if (!APETag->itemListMap().contains("COVER ART (FRONT)")) {
    addToReport(artist, genre, album, directory, "missing_art");

  // Find tracks with invalid album art sizes
  } else {
    if (getPictureSize(APETag) == 0) {
      addToReport(artist, genre, album, directory, "invalid_art");
    }
  }

  // Find tracks containing track numbers that are not formatted as <num>/<total>
  if (!APETag->itemListMap()["TRACK"].isEmpty()) {
    if (!boost::regex_match(track, trackExpression)) {
      addToReport(artist, genre, album, directory, "invalid_track");
    }
  }

  // Find tracks containing disc numbers that are not formatted as <num>/<total>
  if (!APETag->itemListMap()["DISC"].isEmpty()) {
    if (!boost::regex_match(disc, discExpression)) {
      addToReport(artist, genre, album, directory, "invalid_disc");
    }
  }
}
