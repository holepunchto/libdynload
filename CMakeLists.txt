cmake_minimum_required(VERSION 4.0)

find_package(cmake-fetch REQUIRED PATHS node_modules/cmake-fetch)

project(dynload C)

fetch_package("github:libuv/libuv@1.51.0")

fetch_package("github:holepunchto/libpath")

add_library(dynload OBJECT)

set_target_properties(
  dynload
  PROPERTIES
  C_STANDARD 99
  POSITION_INDEPENDENT_CODE ON
)

target_sources(
  dynload
  INTERFACE
    include/dynload.h
  PRIVATE
    src/dynload.c
)

target_include_directories(
  dynload
  INTERFACE
    include
  PUBLIC
    $<TARGET_PROPERTY:uv,INTERFACE_INCLUDE_DIRECTORIES>
)

target_link_libraries(
  dynload
  PUBLIC
    path
)

add_library(dynload_shared SHARED)

set_target_properties(
  dynload_shared
  PROPERTIES
  OUTPUT_NAME dynload
  WINDOWS_EXPORT_ALL_SYMBOLS ON
)

target_link_libraries(
  dynload_shared
  PUBLIC
    dynload
    uv
    path_shared
)

add_library(dynload_static STATIC)

set_target_properties(
  dynload_static
  PROPERTIES
  OUTPUT_NAME dynload
  PREFIX lib
)

target_link_libraries(
  dynload_static
  PUBLIC
    dynload
    uv_a
    path_static
)

install(TARGETS dynload_shared dynload_static)

install(FILES include/dynload.h DESTINATION include)

if(PROJECT_IS_TOP_LEVEL)
  enable_testing()

  add_subdirectory(test)
endif()
