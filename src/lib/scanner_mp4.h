#include "scanner.h"
#include <mp4file.h>

class MP4Scanner: public Scanner {

  public:
  MP4Scanner(string f, report_map_type* r) : Scanner(f, r) {};
  void scan();

  private:
  void report_missing_art(TagLib::MP4::File *file_tag);
  void report_multiple_art(TagLib::MP4::File *file_tag);
};
