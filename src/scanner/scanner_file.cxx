#include <boost/filesystem.hpp>

#include "scanner_file.hxx"

void FileScanner::scan(boost::filesystem::path file) {
  string fileName(file.string());
  directory = dirname(fileName);
  checkFile(file);
}

void FileScanner::checkFile(const boost::filesystem::path file) {
  // Find files that have an extension that differs from its magic type
  string fileExtension = file.extension().string();
  transform(fileExtension.begin(), fileExtension.end(), fileExtension.begin(), ::tolower);

  if ((fileExtension == ".mp3") || (fileExtension == ".mp4") || (fileExtension == ".aac") || (fileExtension == ".flac") || (fileExtension == ".ogg")) {
    addToReport("", "", "", directory, "magic_error");
  }
}
