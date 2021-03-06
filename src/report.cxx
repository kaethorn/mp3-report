/*!
 * \file report.cxx
 * \brief Program entry and command line parsing for MP3Report.
 *
 * Copyright (c) 2013 Falko Schmidt (kaethorn@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <boost/program_options.hpp>
#include <boost/filesystem.hpp>
namespace po = boost::program_options;

#include <iostream>
#include <iterator>
#include <map>
using namespace std;

#include "report_config.hxx"
#include "reporter/reporter.hxx"

int main (int argc, char *argv[]) {

  string reportType, directory, outputPath;
  bool noMagicType, useLibMagic, showWarnings, beQuiet;

  po::options_description desc("Allowed options");
  desc.add_options()
    ("help,h", "Produce this help message")
    ("version,v", "Print version string")
    ("no-magic-type,n", po::value<bool>(&noMagicType)->zero_tokens()->default_value(false),
     "Detect file types by their name (instead of magic type which is the default)")
    ("use-libmagic,m", po::value<bool>(&useLibMagic)->zero_tokens()->default_value(false),
     "Use libmagic when detecting file type by magic type (instead of xdgmime which is the default)")
    ("report-type,r", po::value<string>(&reportType)->default_value("plain"),
     "Report type (csv, plain)")
    ("show-warnings,w", po::value<bool>(&showWarnings)->zero_tokens()->default_value(false),
     "Show inconsistencies in the report that are considered warnings. Errors will always be reported.")
    ("output-path,o", po::value<string>(&outputPath),
     "File to write the report to (fallback: stdout)")
    ("directory", po::value<string>(&directory)->required(), "Working directory")
    ("quiet,q", po::value<bool>(&beQuiet)->zero_tokens()->default_value(false),
     "Suppress everything but the report. Will hide progress and warnings.")
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
    cout << boost::filesystem::basename(argv[0]) << " version " <<
      MP3REPORT_VERSION_MAJOR << "." << MP3REPORT_VERSION_MINOR << endl;
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

  Reporter reporter(&directory, &reportType, &outputPath, noMagicType,
      useLibMagic, showWarnings, beQuiet);
  reporter.run();
  return 0;
}
