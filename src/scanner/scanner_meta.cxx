#include "scanner_meta.hxx"

void MetaScanner::scan() {
  checkReport();
}

void MetaScanner::checkReport() {
  if (metaData->empty()) {
    return;
  }

  // Find artists that belong to different genres
  for (MetaDataMap::iterator metaArtist=metaData->begin();
      metaArtist!=metaData->end(); ++metaArtist) {
    if (metaArtist->second.size() > 1) {
      string artist(metaArtist->first);
      string genre((*metaData)[artist].begin()->first);
      string album((*metaData)[artist][genre].begin()->first);
      string directory((*metaData)[artist][genre][album].begin()->first);
      string albumArtist((*metaData)[artist][genre][album][directory].begin()->albumArtist);
      if (albumArtist != "Various Artists") {
        addToReport(artist, genre, album, directory, "multiple_artist_genres");
      }
    }
  }

  // Find MP3 track titles that have potentially been truncated during
  // conversion from ID3v1 to ID3v2
  if (this->showWarnings) {
    for (MetaDataMap::iterator artist=metaData->begin();
        artist!=metaData->end(); ++artist) {
      Scanner::Genres genres(artist->second);
      for (Scanner::Genres::iterator genre=genres.begin();
          genre!=genres.end(); ++genre) {
        Scanner::Albums albums(genre->second);
        for (Scanner::Albums::iterator album=albums.begin();
            album!=albums.end(); ++album) {
          Scanner::Directories directories(album->second);
          for (Scanner::Directories::iterator directory=directories.begin();
              directory!=directories.end(); ++directory) {
            Scanner::Songs songs(directory->second);
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
              addToReport(artist->first, genre->first, album->first,
                  directory->first, "truncation_warning");
            }
          }
        }
      }
    }
  }
}
