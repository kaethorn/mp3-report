#include <boost/regex.hpp>
#include "scanner_meta.hxx"

void MetaScanner::scan() {
  checkReport();
}

bool MetaScanner::isIncomplete(vector<string>* items) {
  std::sort(items->begin(), items->end());

  int next = 1;
  for (std::vector<string>::iterator item = items->begin(); item != items->end(); ++item) {
    string token(item->substr(0, item->find("/")));
    int parsedItem = atoi(token.c_str());

    // Invalid tracks are already reported separately.
    if (parsedItem == 0) return false;

    // The next item in the vector has an unexpected value indicating an inconsistency
    if (parsedItem != next) return true;

    ++next;
  }
  return false;
}

void MetaScanner::reportMultipleArtistGenres(MetaDataMap::iterator item) {
  Scanner::Genres genres(item->second);
  if (genres.size() > 1) {
    string artist(item->first);
    string genre((*metaData)[artist].begin()->first);
    string album((*metaData)[artist][genre].begin()->first);
    string directory((*metaData)[artist][genre][album].begin()->first);
    string albumArtist((*metaData)[artist][genre][album][directory].begin()->albumArtist);
    if (albumArtist != "Various Artists") {
      addToReport(artist, genre, album, directory, "multiple_artist_genres");
    }
  }
}

void MetaScanner::reportIndexInconsistencies(AlbumMetaDataMap::iterator album) {
  // Collect track- and disc numbers
  vector<string> tracks;
  vector<string> discs;
  for (Scanner::Songs::iterator song=album->second.begin();
      song!=album->second.end(); ++song) {
    tracks.push_back(song->track);

    // Not all albums are part of a collection.
    if (song->disc.size())
      discs.push_back(song->disc);
  }

  // Discs span multiple tracks so duplicates can be removed.
  std::sort(discs.begin(), discs.end());
  discs.erase(std::unique(discs.begin(), discs.end()), discs.end());

  Song song = *album->second.begin();

  if (isIncomplete(&tracks)) {
    addToReport(song.artist, song.genre, song.album, song.directory, "incomplete_album");
  }

  if (isIncomplete(&discs)) {
    addToReport(song.artist, song.genre, song.album, song.directory, "incomplete_collection");
  }
}

void MetaScanner::reportWhiteSpaces(AlbumMetaDataMap::iterator album) {
  static const boost::regex leadingWhiteSpace("^\\s.*");
  static const boost::regex trailingWhiteSpace(".*\\s$");
  static const boost::regex multipleSpaces(".*\\ {2}.*");
  static const boost::regex invalidWhiteSpace(".*[^\\S\\ ].*");

  static const char *framesToCheck[4] = {"album", "title", "artist", "albumartist"};

  for (Scanner::Songs::iterator song=album->second.begin();
      song!=album->second.end(); ++song) {
    for (int i = 0; i < sizeof(framesToCheck); i++) {
      string frame;
      if (framesToCheck[i] == "album")       { frame = song->album; }
      if (framesToCheck[i] == "title")       { frame = song->title; }
      if (framesToCheck[i] == "artist")      { frame = song->artist; }
      if (framesToCheck[i] == "albumartist") { frame = song->albumArtist; }

      if (boost::regex_match(frame, leadingWhiteSpace)  ||
          boost::regex_match(frame, trailingWhiteSpace) ||
          boost::regex_match(frame, multipleSpaces)     ||
          boost::regex_match(frame, invalidWhiteSpace)) {
        addToReport(song->artist, song->genre, song->album, song->directory, "spaces_" + string(framesToCheck[i]));
      }
    }
  }
}

void MetaScanner::reportFileTypeInconsistencies(AlbumMetaDataMap::iterator album) {
  if (album->second.empty())
    return;

  FileType firstType = album->second.begin()->fileType;
  for (Scanner::Songs::iterator song=album->second.begin();
      song!=album->second.end(); ++song) {
    if (song->fileType != firstType) {
      addToReport(song->artist, song->genre, song->album, song->directory, "invalid_file_types");
      return;
    }
  }
}

void MetaScanner::reportTitleTruncation(MetaDataMap::iterator item,
        Scanner::Genres::iterator genre, Scanner::Albums::iterator album,
        Scanner::Directories::iterator directory, Scanner::Songs* songs) {
  if (this->showWarnings) {
    uint maxTitleLength = 0;
    for (Scanner::Songs::iterator song=songs->begin();
        song!=songs->end(); ++song) {
      if (song->title.size() > maxTitleLength) {
        if (song->fileType == MP3) {
          maxTitleLength = song->title.size();
        }
      }
    }
    if (maxTitleLength == 30) {
      addToReport(item->first, genre->first, album->first,
          directory->first, "truncation_warning");
    }
  }
}

void MetaScanner::checkReport() {

  if (albumMetaData->empty() || metaData->empty()) {
    return;
  }

  for (AlbumMetaDataMap::iterator album=albumMetaData->begin();
       album != albumMetaData->end(); ++album) {
    reportIndexInconsistencies(album);
    reportWhiteSpaces(album);
    reportFileTypeInconsistencies(album);
  }

  for (MetaDataMap::iterator item=metaData->begin();
       item != metaData->end(); ++item) {

    reportMultipleArtistGenres(item);

    for (Scanner::Genres::iterator genre=item->second.begin();
        genre!=item->second.end(); ++genre) {
      Scanner::Albums albums(genre->second);
      for (Scanner::Albums::iterator album=albums.begin();
          album!=albums.end(); ++album) {
        Scanner::Directories directories(album->second);
        for (Scanner::Directories::iterator directory=directories.begin();
            directory!=directories.end(); ++directory) {
          Scanner::Songs songs(directory->second);
          reportTitleTruncation(item, genre, album, directory, &songs);
        }
      }
    }
  }
}
