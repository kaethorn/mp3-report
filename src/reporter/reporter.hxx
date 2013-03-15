/*!
 * \file reporter.hxx
 * \brief Reporter class
 *
 * Copyright (c) 2013 Falko Schmidt (kaethorn@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#ifndef REPORTER_H
#define REPORTER_H

#include <fstream>

#include "scanner.hxx"
#include "scanner_mp3.hxx"
#include "scanner_ogg_vorbis.hxx"
#include "scanner_flac.hxx"
#include "scanner_mp4.hxx"
#include "scanner_meta.hxx"
#include "scanner_file.hxx"


/*! \class Reporter
 * Reports for the given directory
 */
class Reporter {

  public:

    /*!
     * Constructs a reporter for \a directory that writes results
     * in a \a reportType.
     *
     * \param [in] directory   The directory in which to search for audio files
     * \param [in] reportType  The report type to use
     * \param [in] outputPath  The output file to write to (\a cout if empty)
     * \param [in] strictMagic Flag indicationg whether to use strict checking
     */
    Reporter(const string* directory, const string* reportType,
        const string* outputPath, bool strictMagic);

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
     * Flag indicating whether to use strict magic type checking.
     */
    bool strictMagic;

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
     * Determines the file type by its magic byte and invokes the
     * corresponding scanner.
     *
     * \param [in] file File to scan.
     */
    void scanByMagicByte(boost::filesystem::path file);

    /*!
     * Determines the file type by its extension and invokes the
     * corresponding scanner.
     *
     * \param [in] file File to scan.
     */
    void scanByExtension(boost::filesystem::path file);

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

    /*!
     * Instance of an MP3 scanner.
     */
    MP3Scanner *mp3Scanner;

    /*!
     * Instance of an MP4 scanner.
     */
    MP4Scanner *mp4Scanner;

    /*!
     * Instance of an ogg vorbis scanner.
     */
    OggVorbisScanner *oggVorbisScanner;

    /*!
     * Instance of a FLAC scanner.
     */
    FLACScanner *flacScanner;

    /*!
     * Instance of a file scanner.
     */
    FileScanner *fileScanner;

    /*!
     * Instance of a file scanner.
     */
    MetaScanner *metaScanner;
};

#endif
