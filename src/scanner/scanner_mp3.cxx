#include <boost/regex.hpp>
#include <taglib/id3v2tag.h>
#include <taglib/id3v1tag.h>
#include <taglib/id3v2frame.h>
#include <taglib/id3v2header.h>
#include <taglib/attachedpictureframe.h>
#include <taglib/textidentificationframe.h>

#include "scanner_mp3.hxx"

MP3Scanner::MP3Scanner(ReportMap* report, MetaDataMap* metaData, AlbumMetaDataMap* albumMetaData)
  : Scanner(report, metaData, albumMetaData) {
  framesToCheck.push_back("TIT2"); // artist
  framesToCheck.push_back("TPE1"); // title
  framesToCheck.push_back("TALB"); // album
  framesToCheck.push_back("TCON"); // genre
  framesToCheck.push_back("TPE2"); // album artist
};

void MP3Scanner::scan(boost::filesystem::path file) {
  string fileName(file.string());
  directory = dirname(file);
  TagLib::MPEG::File fileTag(fileName.c_str());

  checkID3v1Tags(&fileTag);
  checkID3v2Tags(&fileTag);
}

void MP3Scanner::checkID3v1Tags(TagLib::MPEG::File *fileTag) {
  // Retrieve the ID3v1 tag
  TagLib::ID3v1::Tag *ID3v1Tag = fileTag->ID3v1Tag();

  // Get common frames
  string artist(ID3v1Tag->artist().to8Bit(true));
  string genre(ID3v1Tag->genre().to8Bit(true));
  string album(ID3v1Tag->album().to8Bit(true));

  // Find tracks with id3v1 tags
  if (!ID3v1Tag->isEmpty()) {
    addToReport(artist, genre, album, directory, "id3v1");
  }
}

void MP3Scanner::checkID3v2Tags(TagLib::MPEG::File *fileTag) {
  // Retrieve the ID3v2 tag
  TagLib::ID3v2::Tag *ID3v2Tag = fileTag->ID3v2Tag();

  // Get common frames
  string artist(ID3v2Tag->artist().to8Bit(true));
  string genre(ID3v2Tag->genre().to8Bit(true));
  string album(ID3v2Tag->album().to8Bit(true));
  string title(ID3v2Tag->title().to8Bit(true));
  string albumArtist(ID3v2Tag->frameListMap()["TPE2"].isEmpty() ?
    "" : ID3v2Tag->frameListMap()["TPE2"].front()->toString().to8Bit(true)
  );
  string track(ID3v2Tag->frameListMap()["TRCK"].isEmpty() ?
    "" : ID3v2Tag->frameListMap()["TRCK"].front()->toString().to8Bit(true)
  );
  string disc(ID3v2Tag->frameListMap()["TPOS"].isEmpty() ?
    "" : ID3v2Tag->frameListMap()["TPOS"].front()->toString().to8Bit(true)
  );

  // Store meta data
  addToMetaData(artist, genre, album, directory, MP3, title, albumArtist, track, disc);

  // Find tracks without an artist tag
  if (artist.size() == 0) {
    addToReport(artist, genre, album, directory, "missing_artist");
  }

  // Find tracks without an album tag
  if (album.size() == 0) {
    addToReport(artist, genre, album, directory, "missing_album");
  }

  // Find tracks without a title tag
  if (title.size() == 0) {
    addToReport(artist, genre, album, directory, "missing_title");
  }

  // Find tracks without a year tag
  if (ID3v2Tag->year() == 0) {
    addToReport(artist, genre, album, directory, "missing_year");
  }

  // Find tracks without a track number tag
  if (ID3v2Tag->track() == 0) {
    addToReport(artist, genre, album, directory, "missing_track");
  }

  // Find tracks without a genre tag
  if (genre.size() == 0) {
    addToReport(artist, genre, album, directory, "missing_genre");
  }

  // Find tracks without an album artist tag
  if (!ID3v2Tag->frameListMap().contains("TPE2") ||
      ID3v2Tag->frameListMap()["TPE2"].isEmpty()) {
    addToReport(artist, genre, album, directory, "missing_album_artist");
  }

  // Find tracks with missing album art
  const TagLib::ID3v2::FrameList& pictures = ID3v2Tag->frameListMap()["APIC"];
  if (pictures.isEmpty()) {
    addToReport(artist, genre, album, directory, "missing_art");

  // Find tracks with more than one album art
  } else if (pictures.size() > 1) {
    addToReport(artist, genre, album, directory, "multiple_art");

  // Find tracks with invalid album art types or sizes
  } else {
    const TagLib::ID3v2::AttachedPictureFrame* albumArt =
      dynamic_cast<const TagLib::ID3v2::AttachedPictureFrame*>(pictures.front());

    if (albumArt->type() != TagLib::ID3v2::AttachedPictureFrame::FrontCover ||
        albumArt->picture().size() == 0) {
      addToReport(artist, genre, album, directory, "invalid_art");
    }
  }

  // Find tracks containing id3v2 tags with versions lower than
  // id3v2.4
  if (ID3v2Tag->header()->majorVersion() < 4) {
    addToReport(artist, genre, album, directory, "obsolete_id3v2_version");
  }

  // Find tracks containing track numbers that are not formatted as <num>/<total>
  if (!ID3v2Tag->frameListMap()["TRCK"].isEmpty()) {
    static const boost::regex expression("\\d{2}/\\d{2}|\\d{3}/\\d{3}");
    if (!boost::regex_match(track, expression)) {
      addToReport(artist, genre, album, directory, "invalid_track");
    }
  }

  // Find tracks containing disc numbers that are not formatted as <num>/<total>
  if (!ID3v2Tag->frameListMap()["TPOS"].isEmpty()) {
    static const boost::regex expression("\\d/\\d|\\d{2}/\\d{2}");
    if (!boost::regex_match(disc, expression)) {
      addToReport(artist, genre, album, directory, "invalid_disc");
    }
  }

  // Make sure the title, artist, album, genre and album artist tags are UTF-8 encoded
  for (vector<string>::iterator it = framesToCheck.begin();
      it != framesToCheck.end(); ++it) {
    TagLib::ID3v2::FrameList artist_tag = ID3v2Tag->frameListMap()[it->c_str()];
    if (!artist_tag.isEmpty()) {
      TagLib::ID3v2::TextIdentificationFrame *frame =
        dynamic_cast<TagLib::ID3v2::TextIdentificationFrame *>(artist_tag.front());
      if (frame) {
        TagLib::String::Type encoding = frame->textEncoding();
        if(encoding != TagLib::String::UTF8) {
          addToReport(artist, genre, album, directory, "invalid_encoding");
        }
      }
    }
  }
}
