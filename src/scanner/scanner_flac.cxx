#include <boost/regex.hpp>
#include <vorbisfile.h>

#include "scanner_flac.hxx"

void FLACScanner::scan(boost::filesystem::path file) {
  string fileName(file.string());
  directory = dirname(file);
  TagLib::FLAC::File fileTag(fileName.c_str());

  checkFLACTags(&fileTag);
}

void FLACScanner::checkFLACTags(TagLib::FLAC::File *fileTag) {
  // Retrieve the generic tag
  TagLib::Tag *tag = fileTag->tag();

  // Get common frames
  string artist(tag->artist().to8Bit(true));
  string genre(tag->genre().to8Bit(true));
  string album(tag->album().to8Bit(true));

  // Find tracks without an artist tag
  if (artist.size() == 0) {
    addToReport(artist, genre, album, directory, "missing_artist");
  }

  // Find tracks without an album tag
  if (album.size() == 0) {
    addToReport(artist, genre, album, directory, "missing_album");
  }

  // Find tracks without a title tag
  if (tag->title().to8Bit(true).size() == 0) {
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

  // Find tracks with missing album art
  if (fileTag->pictureList().size() < 1) {
    addToReport(artist, genre, album, directory, "missing_art");
  }

  // Find tracks with more than one album art
  if (fileTag->pictureList().size() > 1) {
    addToReport(artist, genre, album, directory, "multiple_art");
  }

  // Find tracks containing album artist tags
  TagLib::Ogg::XiphComment *oggVorbisTag = fileTag->xiphComment();
  if (!oggVorbisTag->fieldListMap()["ALBUMARTIST"].isEmpty()) {
    addToReport(artist, genre, album, directory, "album_artist");
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
