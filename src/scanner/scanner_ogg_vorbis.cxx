#include <boost/regex.hpp>
#include <taglib/flacfile.h>
#include "scanner_ogg_vorbis.hxx"

void OggVorbisScanner::scan(boost::filesystem::path file) {
  string fileName(file.string());
  directory = dirname(file);
  TagLib::Ogg::Vorbis::File fileTag(fileName.c_str());

  checkOggVorbisTags(&fileTag);
}

TagLib::ByteVector OggVorbisScanner::decodeCover(const TagLib::Ogg::XiphComment* oggVorbisTag) {

  // FIXME upgrade to taglib 1.11.1 in order to use internal base64 decoder
  //return TagLib::ByteVector(TagLib::ByteVector::fromBase64(
  //  oggVorbisTag->fieldListMap()["METADATA_BLOCK_PICTURE"].front()
  //));

  string cover = oggVorbisTag->fieldListMap()["METADATA_BLOCK_PICTURE"].front().to8Bit();
  std::stringstream os;

  typedef boost::archive::iterators::transform_width<
    boost::archive::iterators::binary_from_base64<const char *>, 8, 6
  > base64_dec;

  unsigned int size = cover.size();

  if (size && cover[size - 1] == '=') {
    --size;
    if (size && cover[size - 1] == '=') --size;
  }
  if (size == 0) return TagLib::ByteVector();

  std::copy(base64_dec(cover.data()), base64_dec(cover.data() + size),
            std::ostream_iterator<char>(os));

  return TagLib::ByteVector(os.str().c_str());
}

void OggVorbisScanner::checkOggVorbisTags(TagLib::Ogg::Vorbis::File *fileTag) {
  // Retrieve the Ogg Vorbis tag
  TagLib::Ogg::XiphComment *oggVorbisTag = fileTag->tag();

  // Get common frames
  string artist(oggVorbisTag->artist().to8Bit(true));
  string genre(oggVorbisTag->genre().to8Bit(true));
  string album(oggVorbisTag->album().to8Bit(true));
  string title(oggVorbisTag->title().to8Bit(true));
  string albumArtist(oggVorbisTag->fieldListMap()["ALBUMARTIST"].isEmpty() ?
    "" : oggVorbisTag->fieldListMap()["ALBUMARTIST"].toString().to8Bit(true)
  );
  string track(oggVorbisTag->fieldListMap()["TRACKNUMBER"].isEmpty() ?
    "" : oggVorbisTag->fieldListMap()["TRACKNUMBER"].front().to8Bit(true)
  );
  string disc(oggVorbisTag->fieldListMap()["DISCNUMBER"].isEmpty() ?
    "" : oggVorbisTag->fieldListMap()["DISCNUMBER"].front().to8Bit(true)
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
  if (oggVorbisTag->year() == 0) {
    addToReport(artist, genre, album, directory, "missing_year");
  }

  // Find tracks without a track number tag
  if (oggVorbisTag->track() == 0) {
    addToReport(artist, genre, album, directory, "missing_track");
  }

  // Find tracks without a genre tag
  if (genre.size() == 0) {
    addToReport(artist, genre, album, directory, "missing_genre");
  }

  // Find tracks without an album artist tag
  if (oggVorbisTag->fieldListMap()["ALBUMARTIST"].isEmpty()) {
    addToReport(artist, genre, album, directory, "missing_album_artist");
  }

  // Find tracks with missing album art
  if (oggVorbisTag->fieldListMap()["METADATA_BLOCK_PICTURE"].isEmpty()) {
    addToReport(artist, genre, album, directory, "missing_art");

  // Find tracks with more than one album art
  } else if (oggVorbisTag->fieldListMap()["METADATA_BLOCK_PICTURE"].size() > 1) {
    addToReport(artist, genre, album, directory, "multiple_art");

  // Find tracks with invalid album art types
  } else {
    TagLib::FLAC::Picture* albumArt = new TagLib::FLAC::Picture(decodeCover(oggVorbisTag));
    if (albumArt->type() != TagLib::FLAC::Picture::FrontCover) {
      addToReport(artist, genre, album, directory, "invalid_art");
    }
  }

  // Find tracks containing track numbers that are not formatted as <num>/<total>
  if (!oggVorbisTag->fieldListMap()["TRACKNUMBER"].isEmpty()) {
    static const boost::regex expression("\\d{2}/\\d{2}|\\d{3}/\\d{3}");
    if (!boost::regex_match(track, expression)) {
      addToReport(artist, genre, album, directory, "invalid_track");
    }
  }

  // Find tracks containing disc numbers that are not formatted as <num>/<total>
  if (!oggVorbisTag->fieldListMap()["DISCNUMBER"].isEmpty()) {
    static const boost::regex expression("\\d/\\d|\\d{2}/\\d{2}");
    if (!boost::regex_match(disc, expression)) {
      addToReport(artist, genre, album, directory, "invalid_disc");
    }
  }
}
