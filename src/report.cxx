// MP3 consistency reporter
//
// Author: Falko Schmidt <kaethorn@gmail.com>
// Copyright (c) 2013, Falko Schmidt <kaethorn@gmail.com>
//
// Redistribution and use is allowed according to the terms of the BSD license.

#include <boost/program_options.hpp>
namespace po = boost::program_options;

#include <iostream>
#include <iterator>
#include <map>
using namespace std;

#include "report_config.hxx"
#include "reporter.hxx"

int main (int argc, char *argv[]) {

  string report_type, directory, output_path;

  po::options_description desc("Allowed options");
  desc.add_options()
    ("help,h", "Produce this help message")
    ("version,v", "Print version string")
    ("report-type,r", po::value<string>(&report_type)->default_value("plain"),
     "Report type (html_list, html_collapsible, csv, plain)")
    ("output-path,o", po::value<string>(&output_path),
     "File to write the report to (fallback: stdout)")
    ("directory", po::value<string>(&directory)->required(), "Working directory")
  ;

  po::positional_options_description p;
  p.add("directory", -1);

  po::variables_map vm;

  po::store(po::command_line_parser(argc, argv).options(desc).positional(p).run(), vm);

  if (vm.count("help")) {
    cout << desc << endl;
    return 0;
  }
  if (vm.count("version")) {
    cout << argv[0] << " version " << MP3Report_VERSION_MAJOR << "." << MP3Report_VERSION_MINOR << endl;
    return 0;
  }

  try {
    po::notify(vm);
  } catch ( const boost::program_options::error& e ) {
    cerr << e.what() << endl;
    return 1;
  }

  if (vm["report-type"].as<string>() != "html_list" && 
      vm["report-type"].as<string>() != "html_collapsible" &&
      vm["report-type"].as<string>() != "csv" &&
      vm["report-type"].as<string>() != "plain") {
    cerr << "invalid report type (should be one of html_list, html_collapsible, csv or plain)" << endl;
    return 1;
  }

  Reporter reporter(&directory, &report_type, &output_path);
  reporter.run();
  return 0;
}
