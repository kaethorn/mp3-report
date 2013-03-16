include_directories("${PROJECT_SOURCE_DIR}/src/xdgmime")

add_definitions(-DHAVE_MMAP)

add_library(xdgmime xdgmime.c)
add_library(xdgmimeint xdgmimeint.c)
add_library(xdgmimeglob xdgmimeglob.c)
add_library(xdgmimemagic xdgmimemagic.c)
add_library(xdgmimealias xdgmimealias.c)
add_library(xdgmimeicon xdgmimeicon.c)
add_library(xdgmimeparent xdgmimeparent.c)
add_library(xdgmimecache xdgmimecache.c)
