#include <sstream>
#include <fstream>
#include <boost/filesystem.hpp>
#include <boost/algorithm/string/join.hpp>
#include <magic.h>

#include "reporter.hxx"
#include "scanner_mp3.hxx"
#include "scanner_ogg_vorbis.hxx"
#include "scanner_flac.hxx"
#include "scanner_mp4.hxx"

namespace fs = boost::filesystem;

Reporter::Reporter(const string* dir, const string* r_type, const string* o_path) {
  // Assign private attributes
  directory = dir;
  report_type = r_type;
  output_path = o_path;
  report = report_map_type();

  // Determine where the output will be written (file or stdout).
  if (o_path->size() == 0) {
    output = &std::cout;
  } else {
    output_file.open(o_path->c_str(), ios::out);
    output = &output_file;
  }
}

Reporter::~Reporter() {
  output_file.close();
}

void Reporter::run() {
  iterate_directory();
  generate();
}

void Reporter::generate() {
  if (*report_type == "plain") {
    generate_plain();
  } else if (*report_type == "csv") {
    generate_csv();
  } else if (*report_type == "html_list") {
    generate_html_list();
  } else if (*report_type == "html_collapsible") {
    generate_html_collapsible();
  }
}

void Reporter::generate_plain() {
  for (report_map_type::iterator artist=report.begin();
      artist!=report.end(); ++artist) {
    *output << "Artist: " << artist->first << endl;
    albums_type albums(artist->second);

    for (albums_type::iterator album=albums.begin();
        album!=albums.end(); ++album) {
      *output << "  Albums: " << album->first << endl;
      directories_type directories(album->second);

      for (directories_type::iterator directory=directories.begin();
          directory!=directories.end(); ++directory) {
        *output << "    Directory '" << directory->first << "': " << endl;
        errors_type errors(directory->second);

        for (errors_type::iterator error=errors.begin();
            error!=errors.end(); ++error) {
          *output << "      * " << *error << endl;
        }
      }
    }
  }
}

void Reporter::generate_csv() {
  std::ostringstream ss;
  ss << "Artist,Album,Directory,Error(s)" << endl;
  for (report_map_type::iterator artist=report.begin();
      artist!=report.end(); ++artist) {
    albums_type albums(artist->second);
    for (albums_type::iterator album=albums.begin();
        album!=albums.end(); ++album) {
      directories_type directories(album->second);
      for (directories_type::iterator directory=directories.begin();
          directory!=directories.end(); ++directory) {
        ss << "\"" << artist->first << "\",";
        ss << "\"" << album->first << "\",";
        ss << "\"" << directory->first << "\",";
        errors_type errors(directory->second);
        ss << boost::algorithm::join(errors, "|");
        ss << endl;
      }
    }
  }
  *output << ss.str() << endl;
}

void Reporter::generate_html_list() {
  cout << "Report type '" << *report_type << "' is not supported yet." << endl;
}

void Reporter::generate_html_collapsible() {
  cout << "Report type '" << *report_type << "' is not supported yet." << endl;
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
  for (fs::recursive_directory_iterator end, file(*directory);
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
    } else if (file_type.find("audio/") != string::npos) {
      cout << "Missing support for type " << file_type << endl;
    }
  }
}
