#include <mpegfile.h>

#include "scanner.hxx"

class MP3Scanner: public Scanner {

  public:
  MP3Scanner(string f, report_map_type* r) : Scanner(f, r) {};
  void scan();

  private:
  void check_id3v1_tags(TagLib::MPEG::File *file_tag);
  void check_id3v2_tags(TagLib::MPEG::File *file_tag);
};
