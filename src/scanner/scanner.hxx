/*!
 * \file scanner.hxx
 * \brief Scanner base class
 *
 * Copyright (c) 2013 Falko Schmidt <kaethorn@gmail.com>
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#ifndef SCANNER_H
#define SCANNER_H

#include <iostream>
#include <map>
#include <set>
using namespace std;

typedef set<string> errors_type;
typedef map<string, errors_type> directories_type;
typedef map<string, directories_type> albums_type;
typedef map<string, albums_type> genres_type;
typedef map<string, genres_type> report_map_type;

/*! \class Scanner
 * Scanner base class
 */
class Scanner {

  public:

    /*!
     * Construct a scanner for \a file. It will populate \a report
     * with scan results.
     *
     * \param [in]     file   The input file path.
     * \param [in,out] report A pointer to the report for scan results.
     */
    Scanner(const string file, report_map_type* report);

    /*!
     * Construct a scanner for \a report. It will add scan results to
     * \a report.
     *
     * \note This can be used implement meta scanners that scan the report
     * after it has been populated.
     *
     * \param [in,out] report A pointer to the report for scan results.
     */
    Scanner(report_map_type* report);


  protected:

    /*!
     * Adds \a error to the error list in this report if it doesn't exist
     * yet.
     *
     * \see Reporter::report
     *
     * \param [in] artist    The artist for which to report.
     * \param [in] genre     The genre for which to report.
     * \param [in] album     The album for which to report.
     * \param [in] directory The directory for which to report.
     * \param [in] error     The error to report.
     */
    void addToReport(const string artist, const string genre,
        const string album, const string directory, const string error);

    /*!
     * Determines the directory name for \a path.
     *
     * \param [in] path The path for which to return the directory name.
     * \return The directory name for the input path.
     */
    const string dirname(const string path);

    /*!
     * A string that holds the path to the file to scan by this instance.
     */
    string file;

    /*!
     * A pointer to the report structure.
     *
     * \see Reporter::report
     */
    report_map_type* report;

    /*!
     * A string that holds the directory name of this file.
     */
    string directory;
};

#endif
