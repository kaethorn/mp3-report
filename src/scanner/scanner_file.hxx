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
     * Constructs a generic scanner for files and populates \a report with
     * scan results.
     *
     * \see Scanner::Scanner
     *
     * \param [in,out] report A pointer to the report to scan and report to.
     */
    FileScanner(ReportMap* report) : Scanner(report) {};

    /*!
     * Invokes private method scanners on the tags of \a file.
     *
     * \param [in] file The file to scan.
     */
    void scan(boost::filesystem::path file);


  private:

    /*!
     * Scans this file for magic type/file extension inconsistencies.
     *
     * \param [in] file The file to check.
     */
    void checkFile(const boost::filesystem::path file);
};
