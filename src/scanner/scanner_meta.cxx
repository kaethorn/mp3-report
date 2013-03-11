#include "scanner_meta.hxx"

void MetaScanner::scan() {
  // Find artists that belong to different genres
  report_multiple_artist_genre();
}

void MetaScanner::report_multiple_artist_genre() {
  for (report_map_type::iterator artist=report->begin();
      artist!=report->end(); ++artist) {
    if (artist->second.size() > 1) {
      (*report)[artist->first].begin()->
        second.begin()->
        second.begin()->
        second.insert("multiple_artist_genres");
    }
  }
}
