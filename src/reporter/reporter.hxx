/*!
 * \file reporter.hxx
 * \brief Reporter class
 *
 * Copyright (c) 2013 Falko Schmidt <kaethorn@gmail.com>
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#ifndef REPORTER_H
#define REPORTER_H

#include <fstream>

#include "scanner.hxx"

/*! \class Reporter
 * Reports for the given directory
 */
class Reporter {

  public:

    /*!
     * Constructs a reporter for \a directory that writes results
     * in a \a reportType.
     *
     * \param [in] directory  The directory in which to search for audio files
     * \param [in] reportType The report type to use
     * \param [in] outputPath The output file to write to (\a cout if empty)
     */
    Reporter(const string* directory, const string* reportType,
        const string* outputPath);

    /*!
     * Destructs the reporter and closes the output file.
     */
    ~Reporter();

    /*!
     * Launches necessary tasks.
     */
    void run();


  private:

    /*!
     * The directory  in which to search for audio files.
     */
    const string* directory;

    /*!
     * The report type to use.
     */
    const string* reportType;

    /*!
     * The output file to write to (\a cout if empty).
     */
    const string* outputPath;

    /*!
     * The report structure to populate with scan results.
     */
    Scanner::ReportMap report;

    /*!
     * The output stream associated with either a file or \a cout.
     */
    ostream* output;

    /*!
     * The output file used instead of \a cout.
     */
    ofstream outputFile;

    /*!
     * Generates a report depending on \a reportType and stores the
     * result in \a report.
     */
    void generate();

    /*!
     * Inspects each audio file in \a directory and runs the appropriate
     * scanner.
     */
    void iterateDirectory();

    /*!
     * Determines the magic file type for \a file.
     *
     * \param [in] file File to analyse.
     * \return A string containing the magic type.
     */
    const string getFileType(const string);

    /*!
     * Produces a simple text report.
     *
     * The output format looks like
     * \code
     * Artist: <artist>
     *   Album: <album>
     *     Directory '<directory>': 
     *       * <error>
     *       [* .. ]
     *     [..]
     *   [..]
     * [..]
     * \endcode
     */
    void generatePlain();

    /*!
     * Produces lines with comma separated values.
     *
     * The output format looks like
     * \code
     * "<artist>","<album>","<directory>",<error>[|..]
     * \endcode
     */
    void generateCSV();

    /*!
     * Produces a list in HTML.
     *
     * \note This reporter has not been implemented yet.
     */
    void generateHTMLList();

    /*!
     * Produces an interactive list in HTML.
     *
     * \note This reporter has not been implemented yet.
     */
    void generateHTMLCollapsible();
};

#endif
