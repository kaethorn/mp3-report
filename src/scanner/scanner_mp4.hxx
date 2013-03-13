/*!
 * \file scanner_mp4.hxx
 * \brief MP4 scanner class
 *
 * Copyright (c) 2013 Falko Schmidt <kaethorn@gmail.com>
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
     * Constructs a scanner for \a file in MP4 format. It will
     * populate \a report with scan results.
     *
     * \see Scanner::Scanner
     *
     * \param [in]     file   The input file path.
     * \param [in,out] report A pointer to the report for scan results.
     */
    MP4Scanner(string file, report_map_type* report)
      : Scanner(file, report) {};

    /*!
     * Invokes private method scanner on the tags of the file defined in the
     * instance's file attribute.
     */
    void scan();


  private:

    /*!
     * Scans \a fileTag for tag inconsistencies.
     *
     * \param [in] fileTag A pointer to the MP4 file tag object
     */
    void checkMP4Tags(TagLib::MP4::File *fileTag);
};
