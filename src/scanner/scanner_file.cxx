#include <boost/filesystem.hpp>

#include "scanner_file.hxx"

void FileScanner::scan() {
  checkFile();
}

void FileScanner::checkFile() {
  // Find files that have an extension that differs from its magic type
  boost::filesystem::path filePath(file);
  string fileExtension = filePath.extension().string();
  transform(fileExtension.begin(), fileExtension.end(), fileExtension.begin(), ::tolower);

  if ((fileExtension == ".mp3") || (fileExtension == ".mp4") || (fileExtension == ".aac") || (fileExtension == ".flac") || (fileExtension == ".ogg")) {
    addToReport("", "", "", directory, "magic_error");
  }
}
