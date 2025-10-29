# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles\\QuanLyThueSanQt_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\QuanLyThueSanQt_autogen.dir\\ParseCache.txt"
  "QuanLyThueSanQt_autogen"
  )
endif()
