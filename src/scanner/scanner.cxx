#include <boost/filesystem.hpp>
#include <iostream>
using namespace std;

#include "scanner.hxx"

Scanner::Scanner(string file, report_map_type* report) {
  file = file;
  report = report;
  directory = dirname(file);
}

Scanner::Scanner(report_map_type* report) {
  report = report;
}

void Scanner::addToReport(const string artist, const string genre,
    const string album, const string directory, const string error) {
  (*report)[artist][genre][album][directory].insert(error);
}

const string Scanner::dirname(const string path) {
  return boost::filesystem::path(path).parent_path().string();
}
