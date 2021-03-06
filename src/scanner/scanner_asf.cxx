#include <taglib/asftag.h>
#include "scanner_asf.hxx"

uint ASFScanner::getPictureSize(TagLib::ASF::Tag* tag) {
  uint size = 0;
  const TagLib::ByteVector nullStringTerminator(1, 0);
  TagLib::ByteVector albumArt = tag->attributeListMap()["WM/Picture"].front().toByteVector();
  int pos = albumArt.find(nullStringTerminator);

  if (++pos > 0) {
    const TagLib::ByteVector &bytes = albumArt.mid(pos);
    size = bytes.size();
  }

  return size;
}

void ASFScanner::scan(boost::filesystem::path file) {
  string fileName(file.string());
  directory = dirname(file);
  TagLib::ASF::File fileTag(fileName.c_str());

  checkASFTags(&fileTag);
}

void ASFScanner::checkASFTags(TagLib::ASF::File *fileTag) {
  // Retrieve the ASF tag
  TagLib::ASF::Tag *ASFTag = fileTag->tag();

  // Get common frames
  string artist(ASFTag->artist().to8Bit(true));
  string genre(ASFTag->genre().to8Bit(true));
  string album(ASFTag->album().to8Bit(true));
  string title(ASFTag->title().to8Bit(true));
  string albumArtist(ASFTag->attributeListMap()["WM/AlbumArtist"].isEmpty() ?
    "" : ASFTag->attributeListMap()["WM/AlbumArtist"].front().toString().to8Bit(true)
  );
  string track(ASFTag->track() == 0 ?
    "" : ASFTag->attributeListMap()["WM/TrackNumber"].front().toString().to8Bit(true)
  );
  string disc(ASFTag->attributeListMap()["WM/PartOfSet"].isEmpty() ?
    "" : ASFTag->attributeListMap()["WM/PartOfSet"].front().toString().to8Bit(true)
  );

  // Store meta data
  addToMetaData(artist, genre, album, directory, ASF, title, albumArtist, track, disc);

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
  if (ASFTag->year() == 0) {
    addToReport(artist, genre, album, directory, "missing_year");
  }

  // Find tracks without a track number tag
  if (ASFTag->track() == 0) {
    addToReport(artist, genre, album, directory, "missing_track");
  }

  // Find tracks without a genre tag
  if (genre.size() == 0) {
    addToReport(artist, genre, album, directory, "missing_genre");
  }

  // Find tracks whithout an album artist tag
  if (ASFTag->attributeListMap()["WM/AlbumArtist"].isEmpty()) {
    addToReport(artist, genre, album, directory, "missing_album_artist");
  }

  // Find tracks with missing album art
  const TagLib::ASF::AttributeList& pictures = ASFTag->attributeListMap()["WM/Picture"];
  if (pictures.size() < 1) {
    addToReport(artist, genre, album, directory, "missing_art");

  // Find tracks with more than one album art
  } else if (pictures.size() > 1) {
    addToReport(artist, genre, album, directory, "multiple_art");

  // Find tracks with invalid album art types or sizes
  } else {
    const TagLib::ASF::Picture& albumArt = pictures.front().toPicture();
    if (albumArt.type() != TagLib::ASF::Picture::FrontCover ||
        getPictureSize(ASFTag) == 0 ||
        !albumArt.isValid()) {
      addToReport(artist, genre, album, directory, "invalid_art");
    }
  }

  // Find tracks containing track numbers that are not formatted as <num>/<total>
  if (!ASFTag->attributeListMap()["WM/TrackNumber"].isEmpty()) {
    if (!boost::regex_match(track, trackExpression)) {
      addToReport(artist, genre, album, directory, "invalid_track");
    }
  }

  // Find tracks containing disc numbers that are not formatted as <num>/<total>
  if (!ASFTag->attributeListMap()["WM/PartOfSet"].isEmpty()) {
    if (!boost::regex_match(disc, discExpression)) {
      addToReport(artist, genre, album, directory, "invalid_disc");
    }
  }
}
