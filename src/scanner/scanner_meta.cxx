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

void MetaScanner::checkReport() {
  if (metaData->empty()) {
    return;
  }

  for (MetaDataMap::iterator item=metaData->begin();
       item!=metaData->end(); ++item) {
    Scanner::Genres genres(item->second);

    // Find artists that belong to different genres
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

    for (Scanner::Genres::iterator genre=genres.begin();
        genre!=genres.end(); ++genre) {
      Scanner::Albums albums(genre->second);
      for (Scanner::Albums::iterator album=albums.begin();
          album!=albums.end(); ++album) {
        Scanner::Directories directories(album->second);
        for (Scanner::Directories::iterator directory=directories.begin();
            directory!=directories.end(); ++directory) {
          Scanner::Songs songs(directory->second);

          // Collect track- and disc numbers
          vector<string> tracks;
          vector<string> discs;
          for (Scanner::Songs::iterator song=songs.begin();
              song!=songs.end(); ++song) {
            tracks.push_back(song->track);
            if (song->disc.size()) {
              discs.push_back(song->disc);
            }
          }
          if (isIncomplete(&tracks)) {
            addToReport(item->first, genre->first, album->first,
                directory->first, "incomplete_album");
          }

          if (isIncomplete(&discs)) {
            addToReport(item->first, genre->first, album->first,
                directory->first, "incomplete_collection");
          }

          // Find MP3 track titles that have potentially been truncated during
          // conversion from ID3v1 to ID3v2
          if (this->showWarnings) {
            uint maxTitleLength = 0;
            for (Scanner::Songs::iterator song=songs.begin();
                song!=songs.end(); ++song) {
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
      }
    }
  }
}
