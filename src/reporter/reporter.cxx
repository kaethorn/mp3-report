#include <sstream>
#include <fstream>
#include <iomanip>
#include <boost/algorithm/string/replace.hpp>
#include <boost/filesystem.hpp>
#include <boost/algorithm/string/join.hpp>
#include <magic.h>
#include "../xdgmime/xdgmime.h"

#include "reporter.hxx"

namespace fs = boost::filesystem;

Reporter::Reporter(const string* directory, const string* reportType,
    const string* outputPath, bool noMagicType, bool useLibMagic,
    bool showWarnings) {
  this->directory = directory;
  this->reportType = reportType;
  this->outputPath = outputPath;
  this->noMagicType = noMagicType;
  this->useLibMagic = useLibMagic;
  this->showWarnings = showWarnings;
  report = Scanner::ReportMap();
  metaData = Scanner::MetaDataMap();
  albumMetaData = Scanner::AlbumMetaDataMap();

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
  initializeScanners();
  iterateDirectory();
  generate();
}

void Reporter::initializeScanners() {
  // Instantiate scanners
  static MP3Scanner       MP3Scanner(&report, &metaData, &albumMetaData);
  static OggVorbisScanner OggVorbisScanner(&report, &metaData, &albumMetaData);
  static FLACScanner      FLACScanner(&report, &metaData, &albumMetaData);
  static MPCScanner       MPCScanner(&report, &metaData, &albumMetaData);
  static APEScanner       APEScanner(&report, &metaData, &albumMetaData);
  static ASFScanner       ASFScanner(&report, &metaData, &albumMetaData);
  static MP4Scanner       MP4Scanner(&report, &metaData, &albumMetaData);
  static FileScanner      FileScanner(&report);
  static MetaScanner      MetaScanner(&report, &metaData, &albumMetaData, showWarnings);

  // Assign scanner pointers
  this->mp3Scanner       = &MP3Scanner;
  this->oggVorbisScanner = &OggVorbisScanner;
  this->flacScanner      = &FLACScanner;
  this->mpcScanner       = &MPCScanner;
  this->apeScanner       = &APEScanner;
  this->asfScanner       = &ASFScanner;
  this->mp4Scanner       = &MP4Scanner;
  this->fileScanner      = &FileScanner;
  this->metaScanner      = &MetaScanner;
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
    Scanner::ReportGenres genres(artist->second);
    *output << "Artist: " << artist->first << endl;

    for (Scanner::ReportGenres::iterator genre=genres.begin();
        genre!=genres.end(); ++genre) {
      Scanner::ReportAlbums albums(genre->second);

      for (Scanner::ReportAlbums::iterator album=albums.begin();
          album!=albums.end(); ++album) {
        *output << "  Album: " << album->first << endl;
        Scanner::ReportDirectories directories(album->second);

        for (Scanner::ReportDirectories::iterator directory=directories.begin();
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

void Reporter::generateCSV() {
  std::ostringstream ss;
  ss << "Artist,Album,Directory,Error(s)" << endl;
  for (Scanner::ReportMap::iterator artist=report.begin();
      artist!=report.end(); ++artist) {
    Scanner::ReportGenres genres(artist->second);
    for (Scanner::ReportGenres::iterator genre=genres.begin();
        genre!=genres.end(); ++genre) {
      Scanner::ReportAlbums albums(genre->second);
      for (Scanner::ReportAlbums::iterator album=albums.begin();
          album!=albums.end(); ++album) {
        Scanner::ReportDirectories directories(album->second);
        for (Scanner::ReportDirectories::iterator directory=directories.begin();
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
    cerr << "\r" << " ⚠ File type `" << fileType << "` in file `" << file.string() << "` is not supported." << endl;
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

void Reporter::printProgress(string activity, string message, float progress) {
  string progressActivity = activity + " " + getSpinner() + " " + getProgress(progress) + " -";
  printActivity(progressActivity, message);
}

void Reporter::printActivity(string activity, string message) {
  cout << "\r " << activity << " " << left << setw(70) << message.substr(0,70) << flush;
}

string Reporter::getSpinner() {
  static uint spinnerPosition = 0;
  static const string spinnerCharacters[4] = { "◐", "◓", "◑", "◒" };
  spinnerPosition = (spinnerPosition + 1) % 4;
  return spinnerCharacters[spinnerPosition];
}

string Reporter::getProgress(float progress) {
  stringstream percentage;
  percentage << fixed << setprecision(0) << progress << '%';
  return percentage.str();
}

uint Reporter::getFolderCount(const string* directory) {
  printActivity("∞", "Counting files");
  uint totalFolderCount = 0;
  fs::recursive_directory_iterator file(*directory), end;
  while (file != end) {
    if (is_directory(file->status())) {
      if (file->path().filename() == "@eaDir") {
        file.no_push();
      } else {
        totalFolderCount++;
      }
    }
    ++file;
  }

  return totalFolderCount;
}

void Reporter::iterateDirectory() {

  printActivity("∞", "Counting files");
  uint totalFolderCount = getFolderCount(directory);

  uint folderCounter = 0;
  fs::recursive_directory_iterator file(*directory), end;
  while (file != end) {
    if (is_directory(file->status())) {
      if (file->path().filename() == "@eaDir") {
        file.no_push();
      } else {
        folderCounter++;
        printProgress("Scanning", file->path().filename().string(),
          100.0*(float)folderCounter/(float)totalFolderCount);
      }
    } else {
      if (noMagicType)
        scanByExtension(file->path());
      else
        scanByMagicByte(file->path());
    }

    ++file;
  }

  printActivity("Inspecting", "tag information");
  metaScanner->scan();
  printActivity("✓", "Done");
  cout << "\r" << setw(0);
}
