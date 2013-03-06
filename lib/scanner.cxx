#include "scanner.h"

Scanner::Scanner(string f,report_type* r) {
  file = f;
  report = report;
}

//require 'taglib'
//
//class Scanner
//  def initialize(file, report)
//    @file = file
//    @report = report
//  end
//  
//  protected
//
//  # Creates the following structure if it doesn't already exist
//  #
//  # { :artist =>
//  #   { :album =>
//  #     { :directory =>
//  #       [ :<error>, ... ]
//  #     }
//  #   }
//  # }
//  #
//  # where <error> is a symbol describing the error such as 
//  # :missing_art or :id3v1
//  def add_to_report(artist, album, directory, error)
//    @report[artist] ||= {}
//    @report[artist][album] ||= {}
//    @report[artist][album][directory] ||= []
//    @report[artist][album][directory] |= [error]
//  end
//end
