/*!
 * \file scanner.hxx
 * \brief Scanner base class
 *
 * Copyright (c) 2013 Falko Schmidt (kaethorn@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#ifndef SCANNER_H
#define SCANNER_H

#include <iostream>
#include <map>
#include <set>
using namespace std;
#include <boost/filesystem.hpp>

/*! \class Scanner
 * Scanner base class
 */
class Scanner {

  public:

    /*!
     * A set of error strings.
     */
    typedef set<string> Errors;

    /*!
     * A report map between directory names and an error set.
     */
    typedef map<string, Errors> ReportDirectories;

    /*!
     * A report map between album names and directory maps.
     */
    typedef map<string, ReportDirectories> ReportAlbums;

    /*!
     * A report map between genre names and album maps.
     */
    typedef map<string, ReportAlbums> ReportGenres;

    /*!
     * A report map between artist names and genre maps.
     */
    typedef map<string, ReportGenres> ReportMap;

    /*!
     * Enumerator for all supported file types as detected by
     * this application.
     */
    enum FileType { MP3, MP4, ASF, APE, FLAC, OGG, MPC };

    /*!
     * Structure to hold song information.
     */
    struct Song {
      string   title;
      FileType fileType;
      string   albumArtist;
      string   track;
      string   disc;
      string   directory;
      string   album;
      string   genre;
      string   artist;
    };

    /*!
     * A list of song structures.
     */
    typedef list<Song> Songs;

    /*!
     * A map between directory names and an error set.
     */
    typedef map<string, Songs> Directories;

    /*!
     * A map between album names and directory maps.
     */
    typedef map<string, Directories> Albums;

    /*!
     * A map between genre names and album maps.
     */
    typedef map<string, Albums> Genres;

    /*!
     * A map of every song grouped by artist, genre, album and directory
     */
    typedef map<string, Genres> MetaDataMap;

    /*!
     * A map of every song grouped by album
     */
    typedef map<string, Songs> AlbumMetaDataMap;

    /*!
     * Construct a scanner for \a report. It will add scan results to
     * \a report.
     *
     * \note This can be used implement meta scanners that scan the report
     * after it has been populated.
     *
     * \param [in,out] report A pointer to the report for scan results.
     * \param [in,out] metaData A pointer to the meta data.
     * \param [in,out] albumMetaData A pointer to the album meta data.
     */
    Scanner(ReportMap* report, MetaDataMap* metaData, AlbumMetaDataMap* albumMetaData);

    /*!
     * Construct a scanner for \a report. It will add scan results to
     * \a report.
     *
     * \note This can be used implement meta scanners that scan the report
     * after it has been populated.
     *
     * \param [in,out] report A pointer to the report for scan results.
     */
    Scanner(ReportMap* report);


  protected:

    /*!
     * Adds \a error to the error list in this report if it doesn't exist
     * yet.
     *
     * \see Scanner::ReportMap
     *
     * \param [in] artist    The artist for which to report.
     * \param [in] genre     The genre for which to report.
     * \param [in] album     The album for which to report.
     * \param [in] directory The directory for which to report.
     * \param [in] error     The error to report.
     */
    void addToReport(const string artist, const string genre,
        const string album, const string directory, const string error);

    /*!
     * Adds \a title and \a fileType to the meta data map.
     *
     * \see Scanner::MetaDataMap
     *
     * \param [in] artist      The artist
     * \param [in] genre       The genre
     * \param [in] album       The album
     * \param [in] directory   The directory
     * \param [in] fileType    The song file type
     * \param [in] title       The song title
     * \param [in] albumArtist The song album artist
     * \param [in] track       The song track number
     * \param [in] disc        The song disc number
     */
    void addToMetaData(const string artist, const string genre,
        const string album, const string directory, const FileType fileType,
        const string title, const string albumArtist, const string track,
        const string disc);

    /*!
     * Determines the directory name for \a path.
     *
     * \param [in] file The path for which to return the directory name.
     * \return The directory name for the input path.
     */
    const string dirname(const boost::filesystem::path file);

    /*!
     * A pointer to the report structure.
     *
     * \see Scanner::ReportMap
     */
    ReportMap* report;

    /*!
     * A pointer to the meta data structure.
     *
     * \see Scanner::MetaDataMap
     */
    MetaDataMap* metaData;

    /*!
     * A pointer to the album meta data structure.
     *
     * \see Scanner::AlbumMetaDataMap
     */
    AlbumMetaDataMap* albumMetaData;

    /*!
     * A string that holds the directory name of this file.
     */
    string directory;
};

#endif
