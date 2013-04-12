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
     * Constructs a scanner for ogg files and populates \a report with
     * scan results.
     *
     * \see Scanner::Scanner
     *
     * \param [in,out] report A pointer to the report for scan results.
     * \param [in,out] metaData A pointer to the meta data.
     */
    OggVorbisScanner(ReportMap* report, MetaDataMap* metaData)
      : Scanner(report, metaData) {};

    /*!
     * Invokes private method scanner on the tags \a file.
     *
     * \param [in] file The file to scan.
     */
    void scan(boost::filesystem::path file);


  private:

    /*!
     * Scans \a fileTag for tag inconsistencies.
     *
     * \param [in] fileTag A pointer to the ogg file tag object
     */
    void checkOggVorbisTags(TagLib::Ogg::Vorbis::File *fileTag);
};
