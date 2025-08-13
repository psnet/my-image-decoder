cmake_minimum_required(VERSION 3.4.1)

add_library(
  myimagedecoder
  SHARED
  src/main/cpp/myimagedecoder.cpp
)

find_library(
  log-lib
  log
)

target_link_libraries(
  myimagedecoder
  ${log-lib}
)
