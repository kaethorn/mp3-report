#include <iostream>
#include <map>
#include <vector>
using namespace std;

typedef map< string, map< string, map< string, vector<string> > > > report_type;

class Scanner {
  string file;
  //map< string, map< string, map< string, vector<string> > > >* report;
  report_type* report;

  public:
  Scanner(string,report_type*);

  protected:
  void add_to_report();
};
