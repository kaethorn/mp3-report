#ifndef REPORTER_H
#define REPORTER_H

#include <fstream>

#include "scanner.hxx"

class Reporter {
  public:
  Reporter(const string*, const string*, const string*);
  ~Reporter();
  void run();

  private:
  const string* directory;
  const string* report_type;
  const string* output_path;
  report_map_type report;
  ostream* output;
  ofstream output_file;

  void generate();
  void iterate_directory();
  const string get_file_type(const string);
  void generate_plain();
  void generate_csv();
  void generate_html_list();
  void generate_html_collapsible();
};

#endif
