#include "scanner_ogg_vorbis.hxx"

void OggVorbisScanner::scan(boost::filesystem::path file) {
  string fileName(file.string());
  directory = dirname(file);
  TagLib::Ogg::Vorbis::File fileTag(fileName.c_str());

  checkOggVorbisTags(&fileTag);
}

uint OggVorbisScanner::getPictureSize(const TagLib::FLAC::Picture* picture) {
  uint size = 0;
  const TagLib::ByteVector nullStringTerminator(1, 0);
  TagLib::ByteVector albumArt = picture->data();
  int pos = albumArt.find(nullStringTerminator);

  if (++pos > 0) {
    const TagLib::ByteVector &bytes = albumArt.mid(pos);
    size = bytes.size();
  }

  return size;
}

void OggVorbisScanner::checkOggVorbisTags(TagLib::Ogg::Vorbis::File *fileTag) {
  // Retrieve the Ogg Vorbis tag
  TagLib::Ogg::XiphComment *oggVorbisTag = fileTag->tag();

  // Get common frames
  string artist(oggVorbisTag->artist().to8Bit(true));
  string genre(oggVorbisTag->genre().to8Bit(true));
  string album(oggVorbisTag->album().to8Bit(true));
  string title(oggVorbisTag->title().to8Bit(true));
  string albumArtist(oggVorbisTag->fieldListMap()["ALBUMARTIST"].isEmpty() ?
    "" : oggVorbisTag->fieldListMap()["ALBUMARTIST"].toString().to8Bit(true)
  );
  string track(oggVorbisTag->fieldListMap()["TRACKNUMBER"].isEmpty() ?
    "" : oggVorbisTag->fieldListMap()["TRACKNUMBER"].front().to8Bit(true)
  );
  string disc(oggVorbisTag->fieldListMap()["DISCNUMBER"].isEmpty() ?
    "" : oggVorbisTag->fieldListMap()["DISCNUMBER"].front().to8Bit(true)
  );

  // Store meta data
  addToMetaData(artist, genre, album, directory, OGG, title, albumArtist, track, disc);

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

  // Find tracks without an album artist tag
  if (oggVorbisTag->fieldListMap()["ALBUMARTIST"].isEmpty()) {
    addToReport(artist, genre, album, directory, "missing_album_artist");
  }

  // Find tracks with missing album art
  if (oggVorbisTag->pictureList().isEmpty()) {
    addToReport(artist, genre, album, directory, "missing_art");

  // Find tracks with more than one album art
  } else if (oggVorbisTag->pictureList().size() > 1) {
    addToReport(artist, genre, album, directory, "multiple_art");

  // Find tracks with invalid album art types
  } else {
    TagLib::FLAC::Picture* albumArt = oggVorbisTag->pictureList().front();
    if (albumArt->type() != TagLib::FLAC::Picture::FrontCover ||
        getPictureSize(albumArt) == 0) {
      addToReport(artist, genre, album, directory, "invalid_art");
    }
  }

  // Find tracks containing track numbers that are not formatted as <num>/<total>
  if (!oggVorbisTag->fieldListMap()["TRACKNUMBER"].isEmpty()) {
    if (!boost::regex_match(track, trackExpression)) {
      addToReport(artist, genre, album, directory, "invalid_track");
    }
  }

  // Find tracks containing disc numbers that are not formatted as <num>/<total>
  if (!oggVorbisTag->fieldListMap()["DISCNUMBER"].isEmpty()) {
    if (!boost::regex_match(disc, discExpression)) {
      addToReport(artist, genre, album, directory, "invalid_disc");
    }
  }
}
