#include "scanner_meta.hxx"

void MetaScanner::scan() {
  checkReport();
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
      if (albumArtist != "[Various Artists]") {
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

          // Collect album names, track numbers and disc numbers
          vector<uint>   tracks;
          vector<uint>   discs;
          vector<string> albums;
          for (Scanner::Songs::iterator song=songs.begin();
              song!=songs.end(); ++song) {
            tracks.push_back(song->track);
            discs.push_back(song->disc);
            FIXME albums.push_back(song->album);
          }
          std::sort(tracks.begin(), tracks.end());
          std::sort(discs.begin(), discs.end());

          // Find inconsitencies in directories, such as album title mismatches
          // or incomplete disc- and track numbers.
          if (tracks.back() < tracks.size()) {
            addToReport(item->first, genre->first, album->first,
                directory->first, "missing_song");
          }
          if (discs.size() > 0 && discs.back() < discs.size()) {
            addToReport(item->first, genre->first, album->first,
                directory->first, "invalid_disc");
          }

          // Collect album names
          for
          // Find inconsitencies in directories, such as album title mismatches
          // or incomplete disc- and track numbers.

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
