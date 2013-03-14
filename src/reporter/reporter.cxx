#include <sstream>
#include <fstream>
#include <boost/algorithm/string/replace.hpp>
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
  report = Scanner::ReportMap();

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
  for (Scanner::ReportMap::iterator artist=report.begin();
      artist!=report.end(); ++artist) {
    Scanner::Genres genres(artist->second);
    *output << "Artist: " << artist->first << endl;

    for (Scanner::Genres::iterator genre=genres.begin();
        genre!=genres.end(); ++genre) {
      Scanner::Albums albums(genre->second);

      for (Scanner::Albums::iterator album=albums.begin();
          album!=albums.end(); ++album) {
        *output << "  Album: " << album->first << endl;
        Scanner::Directories directories(album->second);

        for (Scanner::Directories::iterator directory=directories.begin();
            directory!=directories.end(); ++directory) {
          *output << "    Directory '" << directory->first << "': " << endl;
          Scanner::Errors errors(directory->second);

          for (Scanner::Errors::iterator error=errors.begin();
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
  for (Scanner::ReportMap::iterator artist=report.begin();
      artist!=report.end(); ++artist) {
    Scanner::Genres genres(artist->second);
    for (Scanner::Genres::iterator genre=genres.begin();
        genre!=genres.end(); ++genre) {
      Scanner::Albums albums(genre->second);
      for (Scanner::Albums::iterator album=albums.begin();
          album!=albums.end(); ++album) {
        Scanner::Directories directories(album->second);
        for (Scanner::Directories::iterator directory=directories.begin();
            directory!=directories.end(); ++directory) {
          string escapedArtist(artist->first);
          boost::replace_all(escapedArtist, "\"", "\"\"");
          string escapedAlbum(album->first);
          boost::replace_all(escapedAlbum, "\"", "\"\"");
          string escapedDirectory(directory->first);
          boost::replace_all(escapedDirectory, "\"", "\"\"");
          ss << "\"" << escapedArtist << "\",";
          ss << "\"" << escapedAlbum << "\",";
          ss << "\"" << escapedDirectory << "\",";
          Scanner::Errors errors(directory->second);
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
    string fileType = getFileType(file->path().string());
    if (fileType == "audio/mpeg") {
      MP3Scanner MP3Scanner(file->path().string(),&report);
      MP3Scanner.scan();
    } else if (fileType == "application/ogg") {
      OggVorbisScanner oggVorbisScanner(file->path().string(),&report);
      oggVorbisScanner.scan();
    } else if (fileType == "audio/x-flac") {
      FLACScanner flacScanner(file->path().string(),&report);
      flacScanner.scan();
    } else if (fileType == "audio/mp4") {
      MP4Scanner MP4Scanner(file->path().string(),&report);
      MP4Scanner.scan();
    } else if (fileType.find("audio/") != string::npos) {
      cerr << "Missing support for type " << fileType << endl;
    }
  }
  MetaScanner metaScanner(&report);
  metaScanner.scan();
}
