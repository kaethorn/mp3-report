//Link to Boost
#define BOOST_TEST_DYN_LINK

//Define our Module name (prints at testing)
#define BOOST_TEST_MODULE "ScannerMetaSpec"

//VERY IMPORTANT - include this last
#include <boost/test/unit_test.hpp>

#include "../../src/scanner/scanner.hxx"
#include "../../src/scanner/scanner_meta.hxx"

BOOST_AUTO_TEST_CASE( validSingleDigitDiscCollection ) {
  Scanner::ReportMap report = Scanner::ReportMap();
  Scanner::MetaDataMap metaData = Scanner::MetaDataMap();
  Scanner::AlbumMetaDataMap albumMetaData = Scanner::AlbumMetaDataMap();
  static MetaScanner MetaScanner(&report, &metaData, &albumMetaData, true);
  BOOST_CHECK_EQUAL(0, report.size());
  Song song;
  song.disc = "1/4";
  albumMetaData[pair<string, string>("TestAlbum", "Album Folder")].push_back(song);
  MetaScanner.scan();

  BOOST_CHECK_EQUAL(0, report.size());
}
