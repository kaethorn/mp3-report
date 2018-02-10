find_package(Boost COMPONENTS system filesystem regex unit_test_framework REQUIRED)
include_directories (${Boost_INCLUDE_DIRS})
add_executable(scannerMetaSpec scanner_meta_spec.cxx)
target_link_libraries(scannerMetaSpec
  ${Boost_LIBRARIES}
  scanner
  scanner_meta
)
add_test(scannerMetaSpec scannerMetaSpec)
