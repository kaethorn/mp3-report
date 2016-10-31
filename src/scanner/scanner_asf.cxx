#include <boost/regex.hpp>
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
  string title(ASFTag->title().to8Bit(true));
  string albumArtist(ASFTag->attributeListMap()["WM/AlbumArtist"].isEmpty() ?
    "" : ASFTag->attributeListMap()["WM/AlbumArtist"].front().toString().to8Bit(true)
  );
  string track(ASFTag->attributeListMap()["WM/TrackNumber"].isEmpty() ?
    "" : ASFTag->attributeListMap()["WM/TrackNumber"].front().toString().to8Bit(true)
  );
  string disc(ASFTag->attributeListMap()["WM/PartOfSet"].isEmpty() ?
    "" : ASFTag->attributeListMap()["WM/PartOfSet"].front().toString().to8Bit(true)
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
  if (ASFTag->year() == 0) {
    addToReport(artist, genre, album, directory, "missing_year");
  }

  // Find tracks without a track number tag
  if (ASFTag->track() == 0) {
    addToReport(artist, genre, album, directory, "missing_track");
  }

  // Find tracks without a genre tag
  if (genre.size() == 0) {
    addToReport(artist, genre, album, directory, "missing_genre");
  }

  // Find tracks whithout an album artist tag
  if (ASFTag->attributeListMap()["WM/AlbumArtist"].isEmpty()) {
    addToReport(artist, genre, album, directory, "missing_album_artist");
  }

  // Find tracks with missing album art
  if (ASFTag->attributeListMap()["WM/Picture"].size() < 1) {
    addToReport(artist, genre, album, directory, "missing_art");
  }

  // Find tracks with more than one album art
  if (ASFTag->attributeListMap()["WM/Picture"].size() > 1) {
    addToReport(artist, genre, album, directory, "multiple_art");
  }

  // Find tracks containing track numbers that are not formatted as <num>/<total>
  if (!ASFTag->attributeListMap()["WM/TrackNumber"].isEmpty()) {
    static const boost::regex expression("\\d{2}/\\d{2}");
    if (!boost::regex_match(track, expression)) {
      addToReport(artist, genre, album, directory, "invalid_track");
    }
  }

  // Find tracks containing disc numbers that are not formatted as <num>/<total>
  if (!ASFTag->attributeListMap()["WM/PartOfSet"].isEmpty()) {
    static const boost::regex expression("\\d/\\d");
    if (!boost::regex_match(disc, expression)) {
      addToReport(artist, genre, album, directory, "invalid_disc");
    }
  }
}
