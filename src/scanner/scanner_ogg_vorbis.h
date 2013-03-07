#include <vorbisfile.h>

#include "scanner.h"

class OggVorbisScanner: public Scanner {

  public:
  OggVorbisScanner(string f, report_map_type* r) : Scanner(f, r) {};
  void scan();

  private:
  void report_missing_art(TagLib::Ogg::Vorbis::File *file_tag);
};
