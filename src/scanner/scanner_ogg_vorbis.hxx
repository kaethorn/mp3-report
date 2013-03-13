/*!
 * \file scanner_ogg_vorbis.hxx
 * \brief Ogg Vorbis scanner class
 *
 * Copyright (c) 2013 Falko Schmidt (kaethorn@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <vorbisfile.h>

#include "scanner.hxx"

/*! \class OggVorbisScanner
 * Ogg Vorbis file scanner
 */
class OggVorbisScanner: public Scanner {

  public:

    /*!
     * Constructs a scanner for \a file in ogg format. It will
     * populate \a report with scan results.
     *
     * \see Scanner::Scanner
     *
     * \param [in]     file   The input file path.
     * \param [in,out] report A pointer to the report for scan results.
     */
    OggVorbisScanner(string file, ReportMap* report)
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
     * \param [in] fileTag A pointer to the ogg file tag object
     */
    void checkOggVorbisTags(TagLib::Ogg::Vorbis::File *fileTag);
};
