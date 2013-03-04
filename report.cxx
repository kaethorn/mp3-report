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

#include "report_config.h"
#include "lib/reporter.h"

int main (int argc, char *argv[]) {

  string report_type, directory;

  po::options_description desc("Allowed options");
  desc.add_options()
    ("help,h", "produce help message")
    ("version,v", "print version string")
    ("report-type,r", po::value<string>(&report_type)->default_value("list"),
     "set report type")
    ("directory", po::value<string>(&directory), "working directory")
  ;

  po::positional_options_description p;
  p.add("directory", -1);

  po::variables_map vm;

  try {
    po::store(po::command_line_parser(argc, argv).options(desc).positional(p).run(), vm);
    po::notify(vm);
  } catch ( const boost::program_options::error& e ) {
    cerr << e.what() << endl;
    return 1;
  }

  if (vm.count("help")) {
    cout << desc << "\n";
    return 0;
  }
  if (vm.count("version")) {
    cout << argv[0] << " version " << MP3Report_VERSION_MAJOR << "." << MP3Report_VERSION_MINOR;
    return 0;
  }
  if (!vm.count("directory")) {
    cerr << "missing required option directory" << endl;
    return 1;
  }

  cout << "Using report type "
    << vm["report-type"].as<string>() << ".\n";

  Reporter reporter(vm["directory"].as<string>(), vm["report-type"].as<string>());
  reporter.run();
  return 0;
}
