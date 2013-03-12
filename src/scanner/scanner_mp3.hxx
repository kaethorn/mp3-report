#include <mpegfile.h>

#include "scanner.hxx"

/*! \class MP3Scanner
 * MP3 file scanner
 */
class MP3Scanner: public Scanner {

  public:

    /*!
     * Constructs a scanner for \a file in MP3 format. It will
     * populate \a report with scan results.
     *
     * \see Scanner::Scanner
     *
     * \param [in]     file   The input file path.
     * \param [in,out] report A pointer to the report for scan results.
     */
    MP3Scanner(const string file, report_map_type* report);

    /*!
     * Invokes private method scanner on the tags of the file defined in the
     * instance's file attribute.
     */
    void scan();


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