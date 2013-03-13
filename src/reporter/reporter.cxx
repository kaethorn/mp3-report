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
#include "scanner_meta.hxx"

namespace fs = boost::filesystem;

Reporter::Reporter(const string* directory, const string* reportType,
    const string* outputPath) {
  this->directory = directory;
  this->reportType = reportType;
  this->outputPath = outputPath;
  report = report_map_type();

  // Determine where the output will be written (file or stdout).
  if (outputPath->size() == 0) {
    output = &std::cout;
  } else {
    outputFile.open(outputPath->c_str(), ios::out);
    output = &outputFile;
  }
}

Reporter::~Reporter() {
  outputFile.close();
}

void Reporter::run() {
  iterateDirectory();
  generate();
}

void Reporter::generate() {
  if (*reportType == "plain") {
    generatePlain();
  } else if (*reportType == "csv") {
    generateCSV();
  } else if (*reportType == "html_list") {
    generateHTMLList();
  } else if (*reportType == "html_collapsible") {
    generateHTMLCollapsible();
  }
}

void Reporter::generatePlain() {
  for (report_map_type::iterator artist=report.begin();
      artist!=report.end(); ++artist) {
    genres_type genres(artist->second);
    *output << "Artist: " << artist->first << endl;

    for (genres_type::iterator genre=genres.begin();
        genre!=genres.end(); ++genre) {
      albums_type albums(genre->second);

      for (albums_type::iterator album=albums.begin();
          album!=albums.end(); ++album) {
        *output << "  Album: " << album->first << endl;
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
}

// FIXME escape "
void Reporter::generateCSV() {
  std::ostringstream ss;
  ss << "Artist,Album,Directory,Error(s)" << endl;
  for (report_map_type::iterator artist=report.begin();
      artist!=report.end(); ++artist) {
    genres_type genres(artist->second);
    for (genres_type::iterator genre=genres.begin();
        genre!=genres.end(); ++genre) {
      albums_type albums(genre->second);
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
  }
  *output << ss.str() << endl;
}

void Reporter::generateHTMLList() {
  cout << "Report type '" << *reportType << "' is not supported yet." << endl;
}

void Reporter::generateHTMLCollapsible() {
  cout << "Report type '" << *reportType << "' is not supported yet." << endl;
}

const string Reporter::getFileType(const string file) {
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

void Reporter::iterateDirectory() {
  for (fs::recursive_directory_iterator end, file(*directory);
      file != end; ++file) {
    if (is_directory(file->status())) {
      continue;
    }
    string file_type = getFileType(file->path().string());
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
      cerr << "Missing support for type " << file_type << endl;
    }
  }
  MetaScanner meta_scanner(&report);
  meta_scanner.scan();
}
