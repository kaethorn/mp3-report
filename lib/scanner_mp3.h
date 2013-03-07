#include "scanner.h"
#include <mpegfile.h>

class MP3Scanner: public Scanner {

  public:
  MP3Scanner(string f, report_type* r) : Scanner(f, r) {};
  void scan();

  private:
  void report_id3v1_tag(TagLib::MPEG::File *file_tag);
  void report_missing_art(TagLib::MPEG::File *file_tag);
  void report_multiple_art(TagLib::MPEG::File *file_tag);
  void report_obsolete_id3v2_version(TagLib::MPEG::File *file_tag);
};
