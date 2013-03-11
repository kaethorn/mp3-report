#ifndef SCANNER_H
#define SCANNER_H

#include <iostream>
#include <map>
#include <set>
using namespace std;

typedef set<string> errors_type;
typedef map<string, errors_type> directories_type;
typedef map<string, directories_type> albums_type;
typedef map<string, albums_type> genres_type;
typedef map<string, genres_type> report_map_type;

class Scanner {
  public:
  Scanner(string, report_map_type*);
  Scanner(report_map_type*);
  string file;
  report_map_type* report;

  protected:
  void add_to_report(const string artist, const string genre,
      const string album, const string directory, const string error);
  const string dirname(const string);
};

#endif
