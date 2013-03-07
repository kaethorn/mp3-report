#ifndef REPORTER_H
#define REPORTER_H

#include <iostream>
#include <map>
#include <set>
using namespace std;

typedef map< string, map< string, map< string, set<string> > > > report_map_type;

class Reporter {
  string directory;
  string report_type;
  report_map_type report;

  public:
  Reporter(string, string);
  void run();

  private:
  void generate();
  void iterate_directory();
  const string get_file_type(const string);
};

#endif
