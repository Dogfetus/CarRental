# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles/project_autogen.dir/AutogenUsed.txt"
  "CMakeFiles/project_autogen.dir/ParseCache.txt"
  "CMakeFiles/sqlite3_autogen.dir/AutogenUsed.txt"
  "CMakeFiles/sqlite3_autogen.dir/ParseCache.txt"
  "project_autogen"
  "sqlite3_autogen"
  )
endif()
