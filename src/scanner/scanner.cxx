#include <boost/filesystem.hpp>
#include <iostream>
using namespace std;

#include "scanner.hxx"

Scanner::Scanner(ReportMap* report, MetaDataMap* metaData, AlbumMetaDataMap* albumMetaData) {
  this->report        = report;
  this->metaData      = metaData;
  this->albumMetaData = albumMetaData;

  this->trackExpression = boost::regex("^\\d{2}/\\d{2}|\\d{3}/\\d{3}$");
  this->discExpression  = boost::regex("^[1-9]/[2-9]|\\d{2}/[1-9]\\d$");
}

Scanner::Scanner(ReportMap* report) {
  this->report = report;
}

void Scanner::addToReport(const string artist, const string genre,
    const string album, const string directory, const string error) {
  (*report)[artist][genre][album][directory].insert(error);
}

void Scanner::addToMetaData(const string artist, const string genre,
    const string album, const string directory, const FileType fileType,
    const string title, const string albumArtist, const string track,
    const string disc) {
  Song song;
  song.title       = title;
  song.fileType    = fileType;
  song.albumArtist = albumArtist;
  song.track       = track;
  song.disc        = disc;
  song.directory   = directory;
  song.album       = album;
  song.genre       = genre;
  song.artist      = artist;
  (*metaData)[albumArtist][genre][album][directory].push_back(song);
  (*albumMetaData)[pair<string, string>(album, directory)].push_back(song);
}

const string Scanner::dirname(const boost::filesystem::path file) {
  return file.parent_path().string();
}
