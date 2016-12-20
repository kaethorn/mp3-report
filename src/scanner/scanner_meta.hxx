/*!
 * \file scanner_meta.hxx
 * \brief Meta scanner class
 *
 * Copyright (c) 2013 Falko Schmidt (kaethorn@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "scanner.hxx"

/*! \class MetaScanner
 * Scanner for \a report structure
 *
 * \see Scanner::ReportMap
 */
class MetaScanner: public Scanner {

  public:

    /*!
     * Constructs a scanner for \a report structure. It will
     * populate \a report with scan results.
     *
     * \see Scanner::Scanner
     * \see Scanner::ReportMap
     *
     * \param [in,out] report A pointer to the report to scan and report to.
     * \param [in,out] metaData A pointer to the meta data.
     * \param [in,out] albumMetaData A pointer to the album meta data.
     * \param [in] showWarnings Flag indicating whether to show warnings as well as errors in the report.
     */
    MetaScanner(ReportMap* report, MetaDataMap* metaData, AlbumMetaDataMap* albumMetaData, bool showWarnings)
      : Scanner(report, metaData, albumMetaData) {
        this->showWarnings = showWarnings;
      };

    /*!
     * Invokes private method scanner on this report structure.
     */
    void scan();


  private:

    /*!
     * Scans this report structure for inconsistencies.
     */
    void checkReport();

    /*!
     * Flag indicating whether to show warnings as well as errors in the report.
     */
    bool showWarnings;

    /*!
     * Determines whether the given vector's values are discontinous.
     *
     * The given vector will be sorted.
     *
     * \param [in,out] items A pointer to vector containing track or disc numbers.
     * \return true if the given vector is inconsistent, false otherwise.
     */
    bool isIncomplete(vector<string>* items);

    /*!
     * Find artists that belong to different genres.
     *
     * \param [in] directory A pointer to the directory level of meta data.
     */
    void reportMultipleArtistGenres(MetaDataMap::iterator item);

    /*!
     * Find inconsistencies in track- or disc number sequences.
     *
     * \param [in] item An iterator of albums.
     */
    void reportIndexInconsistencies(AlbumMetaDataMap::iterator album);

    /*!
     * Find frames with invalid white spaces.
     *
     * \param [in] item An iterator of albums.
     */
    void reportWhiteSpaces(AlbumMetaDataMap::iterator album);

    /*!
     * Find albums with more than one file type.
     *
     * \param [in] item An iterator of albums.
     */
    void reportFileTypeInconsistencies(AlbumMetaDataMap::iterator album);

    /*!
     * Find MP3 track titles that have potentially been truncated during
     * conversion from ID3v1 to ID3v2.
     *
     * \param [in] item An iterator of meta items.
     * \param [in] genre An iterator of genres.
     * \param [in] album An iterator of albums.
     * \param [in] directory An iterator of directories.
     * \param [in] songs A pointer to the list of song structures.
     */
    void reportTitleTruncation(MetaDataMap::iterator item,
        Scanner::Genres::iterator genre, Scanner::Albums::iterator album,
        Scanner::Directories::iterator directory, Scanner::Songs* songs);
};
