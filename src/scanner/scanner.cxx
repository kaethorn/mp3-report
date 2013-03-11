#include <boost/filesystem.hpp>
#include <iostream>
using namespace std;

#include "scanner.hxx"

Scanner::Scanner(string f, report_map_type* r) {
  file = f;
  report = r;
}

Scanner::Scanner(report_map_type* r) {
  report = r;
}

void Scanner::add_to_report(const string artist, const string genre,
    const string album, const string directory, const string error) {
  (*report)[artist][genre][album][directory].insert(error);
}

const string Scanner::dirname(const string path) {
  return boost::filesystem::path(path).parent_path().string();
}
