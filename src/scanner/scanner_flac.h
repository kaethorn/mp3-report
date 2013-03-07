#include <flacfile.h>

#include "scanner.h"

class FLACScanner: public Scanner {

  public:
  FLACScanner(string f, report_map_type* r) : Scanner(f, r) {};
  void scan();

  private:
  void report_missing_art(TagLib::FLAC::File *file_tag);
  void report_multiple_art(TagLib::FLAC::File *file_tag);
};
