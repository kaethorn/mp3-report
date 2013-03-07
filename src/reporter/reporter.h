#ifndef REPORTER_H
#define REPORTER_H

#include "scanner.h"

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
  void generate_plain();
  void generate_csv();
  void generate_html_list();
  void generate_html_collapsible();
};

#endif
