/*!
 * \file scanner_mpc.hxx
 * \brief MPC scanner class
 *
 * Copyright (c) 2013 Falko Schmidt (kaethorn@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <taglib/mpcfile.h>
#include <taglib/apetag.h>

#include "scanner.hxx"

/*! \class MPCScanner
 * MPC file scanner
 */
class MPCScanner: public Scanner {

  public:

    /*!
     * Constructs a scanner for MPC files and populates \a report with
     * scan results.
     *
     * \see Scanner::Scanner
     *
     * \param [in,out] report A pointer to the report for scan results.
     * \param [in,out] metaData A pointer to the meta data.
     * \param [in,out] albumMetaData A pointer to the album meta data.
     */
    MPCScanner(ReportMap* report, MetaDataMap* metaData, AlbumMetaDataMap* albumMetaData)
      : Scanner(report, metaData, albumMetaData) {};

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
     * \param [in] fileTag A pointer to the MPC file tag object
     */
    void checkMPCTags(TagLib::MPC::File *fileTag);

    /*!
     * Determines size effective size of the first picture in \a tag.
     *
     * \param [in] tag A pointer to the APE tag object.
     * \return The size of the first picture.
     */
    uint getPictureSize(TagLib::APE::Tag* tag);
};
