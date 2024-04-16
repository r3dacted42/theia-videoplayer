# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles\\Theia_VideoPlayer_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\Theia_VideoPlayer_autogen.dir\\ParseCache.txt"
  "Theia_VideoPlayer_autogen"
  )
endif()
