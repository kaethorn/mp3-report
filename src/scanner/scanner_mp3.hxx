/*!
 * \file scanner_mp3.hxx
 * \brief MP3 scanner class
 *
 * Copyright (c) 2013 Falko Schmidt (kaethorn@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <mpegfile.h>

#include "scanner.hxx"

/*! \class MP3Scanner
 * MP3 file scanner
 */
class MP3Scanner: public Scanner {

  public:

    /*!
     * Constructs a scanner for MP3 files and populates \a report with
     * scan results.
     *
     * \see Scanner::Scanner
     *
     * \param [in,out] report A pointer to the report for scan results.
     * \param [in,out] report A pointer to the meta data.
     */
    MP3Scanner(ReportMap* report, MetaDataMap* metaData);

    /*!
     * Invokes private method scanners on the tags of \a file.
     *
     * \param [in] file The file to scan.
     */
    void scan(boost::filesystem::path file);


  private:

    /*!
     * Scans \a fileTag for ID3v1 inconsistencies.
     *
     * \param [in] fileTag A pointer to the MPEG file tag object
     */
    void checkID3v1Tags(TagLib::MPEG::File *fileTag);

    /*!
     * Scans \a fileTag for ID3v2 inconsistencies.
     *
     * \param [in] fileTag A pointer to the MPEG file tag object
     */
    void checkID3v2Tags(TagLib::MPEG::File *fileTag);

    /*!
     * Holds ID3v2 frame names to verify by this scanner.
     */
    vector<string> framesToCheck;
};
