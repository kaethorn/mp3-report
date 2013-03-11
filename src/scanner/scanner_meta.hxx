#include "scanner.hxx"

class MetaScanner: public Scanner {

  public:
  MetaScanner(report_map_type* r) : Scanner(r) {};
  void scan();

  private:
  void report_multiple_artist_genre();
};
