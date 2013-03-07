#include "scanner.h"
#include <boost/filesystem.hpp>
#include <iostream>
using namespace std;

Scanner::Scanner(string f, report_type* r) {
  file = f;
  report = r;
}

void Scanner::add_to_report(const string artist, const string album,
    const string directory, const string error) {
  (*report)[artist][album][directory].push_back(error);
}
const string Scanner::dirname(const string path) {
  return boost::filesystem::path(path).parent_path().string();
}
