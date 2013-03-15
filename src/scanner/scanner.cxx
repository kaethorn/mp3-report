#include <boost/filesystem.hpp>
#include <iostream>
using namespace std;

#include "scanner.hxx"

Scanner::Scanner(ReportMap* report) {
  this->report = report;
}

void Scanner::addToReport(const string artist, const string genre,
    const string album, const string directory, const string error) {
  (*report)[artist][genre][album][directory].insert(error);
}

const string Scanner::dirname(const boost::filesystem::path file) {
  return file.parent_path().string();
}
