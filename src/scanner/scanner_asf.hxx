/*!
 * \file scanner_asf.hxx
 * \brief ASF scanner class
 *
 * Copyright (c) 2013 Falko Schmidt (kaethorn@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include <asffile.h>

#include "scanner.hxx"

/*! \class ASFScanner
 * ASF file scanner
 */
class ASFScanner: public Scanner {

  public:

    /*!
     * Constructs a scanner for ASF files and populates \a report with
     * scan results.
     *
     * \see Scanner::Scanner
     *
     * \param [in,out] report A pointer to the report for scan results.
     * \param [in,out] report A pointer to the meta data.
     */
    ASFScanner(ReportMap* report, MetaDataMap* metaData)
      : Scanner(report, metaData) {};

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
     * \param [in] fileTag A pointer to the ASF file tag object
     */
    void checkASFTags(TagLib::ASF::File *fileTag);
};
