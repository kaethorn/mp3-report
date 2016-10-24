#include <boost/regex.hpp>

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
  string title(oggVorbisTag->title().to8Bit(true));
  string albumArtist("");
  if (!oggVorbisTag->fieldListMap()["ALBUMARTIST"].isEmpty()) {
    albumArtist = oggVorbisTag->fieldListMap()["ALBUMARTIST"].toString().to8Bit(true);
  }

  // Store meta data
  addToMetaData(artist, genre, album, directory, MP3, title, albumArtist);

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
  if (oggVorbisTag->fieldListMap()["METADATA_BLOCK_PICTURE"].isEmpty()) {
    addToReport(artist, genre, album, directory, "missing_art");
  } else {
  // Find tracks with more than one album art
    if (oggVorbisTag->fieldListMap()["METADATA_BLOCK_PICTURE"].size() > 1) {
      addToReport(artist, genre, album, directory, "multiple_art");
    }
  }

  // Find tracks containing track numbers that are not formatted as <num>/<total>
  if (!oggVorbisTag->fieldListMap()["TRACKNUMBER"].isEmpty()) {
    static const boost::regex e("\\d{2}/\\d{2}");
    string track = oggVorbisTag->fieldListMap()["TRACKNUMBER"].front().to8Bit(true);
    if (!boost::regex_match(track, e)) {
      addToReport(artist, genre, album, directory, "invalid_track");
    }
  }
}
