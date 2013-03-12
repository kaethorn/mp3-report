#include <flacfile.h>

#include "scanner.hxx"

/*! \class FLACScanner
 * FLAC file scanner
 */
class FLACScanner: public Scanner {

  public:

    /*!
     * Constructs a scanner for \a file in FLAC format. It will
     * populate \a report with scan results.
     *
     * \see Scanner::Scanner
     *
     * \param [in]     file   The input file path.
     * \param [in,out] report A pointer to the report for scan results.
     */
    FLACScanner(string file, report_map_type* report)
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
     * \param [in] fileTag A pointer to the FLAC file tag object
     */
    void checkFLACTags(TagLib::FLAC::File *fileTag);
};