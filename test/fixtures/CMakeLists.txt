add_library(foo SHARED lib.c)

set_target_properties(
  foo
  PROPERTIES
  VERSION 1.2.3
)

add_library(bar SHARED lib.c)

set_target_properties(
  bar
  PROPERTIES
  VERSION 2.1.4
)

install(
  TARGETS foo bar
  LIBRARY
    DESTINATION "${CMAKE_CURRENT_SOURCE_DIR}/lib"
    NAMELINK_SKIP
  COMPONENT fixtures
)
