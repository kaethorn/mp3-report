#include <iostream>
#include <map>
#include <vector>
using namespace std;

class Reporter {
  string directory;
  string report_type;
  map< string, map< string, map< string, vector<string> > > > report;

  public:
  Reporter(string, string);
  void run();

  private:
  void generate();
  void iterate_directory();
};
