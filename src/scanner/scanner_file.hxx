/*!
 * \file scanner_file.hxx
 * \brief File scanner class
 *
 * Copyright (c) 2013 Falko Schmidt (kaethorn@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "scanner.hxx"

/*! \class FileScanner
 * Scanner for \a report structure
 *
 * \see Scanner::ReportMap
 */
class FileScanner: public Scanner {

  public:

    /*!
     * Constructs a scanner for \a report structure. It will
     * populate \a report with scan results.
     *
     * \see Scanner::Scanner
     * \see Scanner::ReportMap
     *
     * \param [in]     file   The input file path.
     * \param [in,out] report A pointer to the report to scan and report to.
     */
    FileScanner(string file, ReportMap* report)
      : Scanner(file, report) {};

    /*!
     * Invokes private method scanner on this report structure.
     */
    void scan();


  private:

    /*!
     * Scans this file for magic type/file extension inconsistencies.
     */
    void checkFile();
};
