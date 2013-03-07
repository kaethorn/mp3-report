#include "reporter.h"
#include "boost/filesystem.hpp"
#include <magic.h>
#include "scanner_mp3.h"
#include "scanner_ogg_vorbis.h"
#include "scanner_flac.h"
#include "scanner_mp4.h"

namespace fs = boost::filesystem;

Reporter::Reporter(string dir, string r_type) {
  directory = dir;
  report_type = r_type;
  report = report_map_type();
}

void Reporter::run() {
  iterate_directory();
  generate();
}

void Reporter::generate() {
  if (report_type == "plain") {
    generate_plain();
  } else if (report_type == "csv") {
    generate_csv();
  } else if (report_type == "html_list") {
    generate_html_list();
  } else if (report_type == "html_collapsible") {
    generate_html_collapsible();
  }
}

void Reporter::generate_plain() {
  for (report_map_type::iterator artist=report.begin();
      artist!=report.end(); ++artist) {
    cout << "Artist: " << artist->first << endl;
    albums_type albums(artist->second);

    for (albums_type::iterator album=albums.begin();
        album!=albums.end(); ++album) {
      cout << "  Albums: " << album->first << endl;
      directories_type directories(album->second);

      for (directories_type::iterator directory=directories.begin();
          directory!=directories.end(); ++directory) {
        cout << "    Directory '" << directory->first << "': " << endl;
        errors_type errors(directory->second);

        for (errors_type::iterator error=errors.begin();
            error!=errors.end(); ++error) {
          cout << "      * " << *error << endl;
        }
      }
    }
  }
}

void Reporter::generate_csv() {
  cout << "Report type '" << report_type << "' is not supported yet." << endl;
}

void Reporter::generate_html_list() {
  cout << "Report type '" << report_type << "' is not supported yet." << endl;
}

void Reporter::generate_html_collapsible() {
  cout << "Report type '" << report_type << "' is not supported yet." << endl;
}

const string Reporter::get_file_type(const string file) {
  const char *contentType; 
  string type;

  magic_t magic = magic_open(MAGIC_MIME_TYPE|MAGIC_ERROR); 
  if (magic == NULL) { 
    cerr << file << ": Error initializing libmagic" << endl;
    return "unknown";
  } 
  if (magic_load(magic, NULL) != 0) {
    cerr << file << ": Error loading libmagic database" << endl;
    magic_close(magic);
    return "unknown";
  }
  contentType = magic_file(magic, file.c_str()); 
  if (contentType == NULL) { 
    cerr << file << ": Unknown content type" << endl; 
    magic_close(magic);
    return "unknown";
  } 
  type = string(contentType);
  magic_close(magic);
  return type;
}

void Reporter::iterate_directory() {
  for (fs::recursive_directory_iterator end, file(directory); 
      file != end; ++file) {
    if (is_directory(file->status())) {
      continue;
    }
    string file_type = get_file_type(file->path().string());
    // Only consider audio files
    if (file_type == "audio/mpeg") {
      MP3Scanner mp3_scanner(file->path().string(),&report);
      mp3_scanner.scan();
    } else if (file_type == "application/ogg") {
      OggVorbisScanner ogg_vorbis_scanner(file->path().string(),&report);
      ogg_vorbis_scanner.scan();
    } else if (file_type == "audio/x-flac") {
      FLACScanner flac_scanner(file->path().string(),&report);
      flac_scanner.scan();
    } else if (file_type == "audio/mp4") {
      MP4Scanner mp4_scanner(file->path().string(),&report);
      mp4_scanner.scan();
    }
  }
}
