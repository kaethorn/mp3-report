# MP3 consistency reporter
#
# Author: Falko Schmidt <kaethorn@gmail.com>

require 'optparse'
require 'ostruct'
require_relative 'lib/reporter'

def parseCommandLine
  options = OpenStruct.new
  options.report_type = 'list'
  opts = OptionParser.new do |opts|
    opts.banner = "Usage: report.rb [options] [directory]"
    opts.on("-r", "--report-type [REPORT_TYPE]", [:list, :collapsible], "Report type to use (list*, collapsible)") do |report_type|
      options.report_type = report_type
    end
  end
  opts.parse!

  directory = ARGV[0]

  if ARGV.length != 1
    puts opts
    exit
  end

  [directory,options]
end

directory,options = parseCommandLine
reporter = Reporter.new directory, options.report_type
reporter.run
