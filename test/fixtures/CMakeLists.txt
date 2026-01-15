add_library(foo SHARED lib.c)

set_target_properties(
  foo
  PROPERTIES
  OUTPUT_NAME foo.1.2.3
)

add_library(bar SHARED lib.c)

set_target_properties(
  bar
  PROPERTIES
  OUTPUT_NAME bar.2.1.4
)

install(
  TARGETS foo bar
  DESTINATION "${CMAKE_CURRENT_SOURCE_DIR}/lib"
  COMPONENT fixtures
)
