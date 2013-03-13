/*!
 * \file scanner_meta.hxx
 * \brief Meta scanner class
 *
 * Copyright (c) 2013 Falko Schmidt <kaethorn@gmail.com>
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "scanner.hxx"

/*! \class MetaScanner
 * Scanner for \a report structure
 *
 * \see Report::report
 */
class MetaScanner: public Scanner {

  public:

    /*!
     * Constructs a scanner for \a report structure. It will
     * populate \a report with scan results.
     *
     * \see Scanner::Scanner
     * \see Report::report
     *
     * \param [in,out] report A pointer to the report to scan and report to.
     */
    MetaScanner(report_map_type* report)
      : Scanner(report) {};

    /*!
     * Invokes private method scanner on this report structure.
     */
    void scan();


  private:

    /*!
     * Scans this report structure for inconsistencies.
     */
    void checkReport();
};
