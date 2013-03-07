#include <iostream>
#include <map>
#include <vector>
using namespace std;

typedef map< string, map< string, map< string, vector<string> > > > report_type;

class Scanner {
  public:
  Scanner(string,report_type*);
  string file;
  report_type* report;

  protected:
  void add_to_report(const string artist, const string album,
      const string directory, const string error);
  const string dirname(const string);
};
