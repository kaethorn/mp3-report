#include "reporter.h"
#include "boost/filesystem.hpp"
#include <magic.h>
#include "scanner_mp3.h"

namespace fs = boost::filesystem;

Reporter::Reporter(string dir, string r_type) {
  directory = dir;
  report_type = r_type;
  report = map< string, map< string, map< string, vector<string> > > >();
}

void Reporter::run() {
  iterate_directory();
  generate();
}

void Reporter::generate() {
  /*
    template = File.read("report.#{@report_type}.haml")
    haml_engine = Haml::Engine.new(template)
    output = File.new 'report.html', 'w'
    output.write haml_engine.render(Object.new, :report => @report)
    output.close
  */
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
    } else if (file_type == "audio/x-flac") {
    } else if (file_type == "audio/mp4") {
    }
  }
}
