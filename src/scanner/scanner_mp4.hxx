/*!
 * \file scanner_mp4.hxx
 * \brief MP4 scanner class
 *
 * Copyright (c) 2013 Falko Schmidt (kaethorn@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <mp4file.h>

#include "scanner.hxx"

/*! \class MP4Scanner
 * MP4 file scanner
 */
class MP4Scanner: public Scanner {

  public:

    /*!
     * Constructs a scanner for MP4 files and populates \a report with
     * scan results.
     *
     * \see Scanner::Scanner
     *
     * \param [in,out] report A pointer to the report for scan results.
     */
    MP4Scanner(ReportMap* report) : Scanner(report) {};

    /*!
     * Invokes private method scanners on the tags of \a file.
     *
     * \param [in] file The file to scan.
     */
    void scan(boost::filesystem::path file);


  private:

    /*!
     * Scans \a fileTag for tag inconsistencies.
     *
     * \param [in] fileTag A pointer to the MP4 file tag object
     */
    void checkMP4Tags(TagLib::MP4::File *fileTag);
};
