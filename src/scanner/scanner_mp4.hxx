#include <mp4file.h>

#include "scanner.hxx"

class MP4Scanner: public Scanner {

  public:
  MP4Scanner(string f, report_map_type* r) : Scanner(f, r) {};
  void scan();

  private:
  void report_missing_art(TagLib::MP4::File *file_tag);
  void report_multiple_art(TagLib::MP4::File *file_tag);
};
