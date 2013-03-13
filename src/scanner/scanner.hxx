/*!
 * \file scanner.hxx
 * \brief Scanner base class
 *
 * Copyright (c) 2013 Falko Schmidt (kaethorn@gmail.com)
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

/*! \class Scanner
 * Scanner base class
 */
class Scanner {

  public:

    /*!
     * A set of error strings.
     */
    typedef set<string> Errors;

    /*!
     * A map between directory names and an error set.
     */
    typedef map<string, Errors> Directories;

    /*!
     * A map between album names and directory maps.
     */
    typedef map<string, Directories> Albums;

    /*!
     * A map between genre names and album maps.
     */
    typedef map<string, Albums> Genres;

    /*!
     * A map between artist names and genre maps.
     */
    typedef map<string, Genres> ReportMap;

    /*!
     * Construct a scanner for \a file. It will populate \a report
     * with scan results.
     *
     * \param [in]     file   The input file path.
     * \param [in,out] report A pointer to the report for scan results.
     */
    Scanner(const string file, ReportMap* report);

    /*!
     * Construct a scanner for \a report. It will add scan results to
     * \a report.
     *
     * \note This can be used implement meta scanners that scan the report
     * after it has been populated.
     *
     * \param [in,out] report A pointer to the report for scan results.
     */
    Scanner(ReportMap* report);


  protected:

    /*!
     * Adds \a error to the error list in this report if it doesn't exist
     * yet.
     *
     * \see Scanner::ReportMap
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
     * \see Scanner::ReportMap
     */
    ReportMap* report;

    /*!
     * A string that holds the directory name of this file.
     */
    string directory;
};

#endif
