#ifndef SCANNER_H
#define SCANNER_H

#include "reporter.h"

class Scanner {
  public:
  Scanner(string, report_map_type*);
  string file;
  report_map_type* report;

  protected:
  void add_to_report(const string artist, const string album,
      const string directory, const string error);
  const string dirname(const string);
};

#endif
