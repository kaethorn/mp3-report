#include <sstream>
#include <fstream>
#include <boost/algorithm/string/replace.hpp>
#include <boost/filesystem.hpp>
#include <boost/algorithm/string/join.hpp>
#include <magic.h>
#include "xdgmime.h"

#include "reporter.hxx"

namespace fs = boost::filesystem;

Reporter::Reporter(const string* directory, const string* reportType,
    const string* outputPath, bool noMagicType, bool useLibMagic) {
  this->directory = directory;
  this->reportType = reportType;
  this->outputPath = outputPath;
  this->noMagicType = noMagicType;
  this->useLibMagic = useLibMagic;
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

const string Reporter::getFileTypeByXdgMIME(const string file) {
  const char *type;
  type = xdg_mime_get_mime_type_for_file(file.c_str(), NULL);
  return string(type);
}

const string Reporter::getFileTypeLibMagic(const string file) {
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

void Reporter::scanByMagicByte(boost::filesystem::path file) {
  // Determine the file's magic type
  string fileType;
  if (useLibMagic) {
    fileType = getFileTypeLibMagic(file.string());
  } else  {
    fileType = getFileTypeByXdgMIME(file.string());
  }

  if (fileType == "audio/mpeg") {
    mp3Scanner->scan(file);
  } else if (fileType == "application/ogg") {
    oggVorbisScanner->scan(file);
  } else if (fileType == "audio/x-vorbis+ogg") {
    oggVorbisScanner->scan(file);
  } else if (fileType == "audio/x-flac") {
    flacScanner->scan(file);
  } else if (fileType == "audio/flac") {
    flacScanner->scan(file);
  } else if (fileType == "audio/x-ape") {
    apeScanner->scan(file);
  } else if (fileType == "audio/x-musepack") {
    mpcScanner->scan(file);
  } else if (fileType == "audio/x-ms-wma") {
    asfScanner->scan(file);
  } else if (fileType == "audio/mp4") {
    mp4Scanner->scan(file);
  } else if (fileType.find("audio/") != string::npos) {
    cerr << "Missing support for type " << fileType << endl;
  } else {
    fileScanner->scan(file);
  }
}

void Reporter::scanByExtension(boost::filesystem::path file) {
  // Determine the file's extension
  string fileExtension = file.extension().string();
  transform(fileExtension.begin(), fileExtension.end(), fileExtension.begin(), ::tolower);

  if (fileExtension == ".mp3") {
    mp3Scanner->scan(file);
  } else if (fileExtension == ".ogg") {
    oggVorbisScanner->scan(file);
  } else if (fileExtension == ".flac") {
    flacScanner->scan(file);
  } else if (fileExtension == ".mpc") {
    mpcScanner->scan(file);
  } else if (fileExtension == ".ape") {
    apeScanner->scan(file);
  } else if (fileExtension == ".wma") {
    asfScanner->scan(file);
  } else if ((fileExtension == ".mp4") || (fileExtension == ".aac")) {
    mp4Scanner->scan(file);
  }
}

void Reporter::iterateDirectory() {
  // Instantiate scanners
  MP3Scanner       MP3Scanner(&report);
  OggVorbisScanner OggVorbisScanner(&report);
  FLACScanner      FLACScanner(&report);
  MPCScanner       MPCScanner(&report);
  APEScanner       APEScanner(&report);
  ASFScanner       ASFScanner(&report);
  MP4Scanner       MP4Scanner(&report);
  FileScanner      FileScanner(&report);
  MetaScanner      MetaScanner(&report);

  this->mp3Scanner       = &MP3Scanner;
  this->oggVorbisScanner = &OggVorbisScanner;
  this->flacScanner      = &FLACScanner;
  this->mpcScanner       = &MPCScanner;
  this->apeScanner       = &APEScanner;
  this->asfScanner       = &ASFScanner;
  this->mp4Scanner       = &MP4Scanner;
  this->fileScanner      = &FileScanner;
  this->metaScanner      = &MetaScanner;

  for (fs::recursive_directory_iterator end, file(*directory);
      file != end; ++file) {
    if (is_directory(file->status())) {
      continue;
    }

    if (noMagicType)
      scanByExtension(file->path());
    else
      scanByMagicByte(file->path());
  }
  metaScanner->scan();
}
