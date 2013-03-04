#include "reporter.h"
#include "boost/filesystem.hpp"

namespace fs = boost::filesystem;

Reporter::Reporter(string dir, string r_type) {
  directory = dir;
  report_type = r_type;
  report = map< string, map< string, map< string, vector<string> > > >();
}

void Reporter::run() {
  iterate_directory();
  generate();
}

void Reporter::generate() {
  /*
    template = File.read("report.#{@report_type}.haml")
    haml_engine = Haml::Engine.new(template)
    output = File.new 'report.html', 'w'
    output.write haml_engine.render(Object.new, :report => @report)
    output.close
  */
}

void Reporter::iterate_directory() {
  for (fs::recursive_directory_iterator end, file(directory); 
      file != end; ++file) {
    if (is_directory(file->status())) {
      continue;
    }
    //cout << *file << std::endl;
    /*
    raw_file = File.open file
    fileType = MimeMagic.by_path(file)
    magicType = MimeMagic.by_magic(raw_file)
    raw_file.close

    # Only consider audio files
    if fileType == 'audio/mpeg' and magicType == 'audio/mpeg'
      mp3_scanner = MP3Scanner.new file, @report
      mp3_scanner.scan
    elsif fileType == 'audio/ogg' and magicType == 'audio/x-vorbis+ogg'
      ogg_vorbis_scanner = OggVorbisScanner.new file, @report
      ogg_vorbis_scanner.scan
    elsif fileType == 'audio/flac' and magicType == 'audio/flac'
      flac_scanner = FLACScanner.new file, @report
      flac_scanner.scan
    elsif fileType == 'audio/mp4' and magicType == 'audio/mp4'
      # MP4 files are not yet supported in taglib-ruby
    end
    */
  }
}
