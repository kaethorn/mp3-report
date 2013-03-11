#include <mpegfile.h>

#include "scanner.hxx"

class MP3Scanner: public Scanner {

  public:
  MP3Scanner(string, report_map_type*);
  void scan();

  private:
  void check_id3v1_tags(TagLib::MPEG::File *file_tag);
  void check_id3v2_tags(TagLib::MPEG::File *file_tag);
  vector<string> tags_to_check;
};
