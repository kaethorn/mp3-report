#include "scanner_meta.hxx"

void MetaScanner::scan() {
  checkReport();
}

void MetaScanner::checkReport() {
  // Find artists that belong to different genres
  for (report_map_type::iterator artist=report->begin();
      artist!=report->end(); ++artist) {
    if (artist->second.size() > 1) {
      (*report)[artist->first].begin()   // genre
        ->second.begin()                 // album
        ->second.begin()                 // directory
        ->second.insert("multiple_artist_genres");
    }
  }
}