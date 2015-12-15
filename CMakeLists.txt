project(libtoxsplitmessage)
cmake_minimum_required(VERSION 2.8)

set(CMAKE_BUILD_TYPE Release)

if (("${CMAKE_C_COMPILER_ID}" STREQUAL "GNU") OR ("${CMAKE_C_COMPILER_ID}" STREQUAL "Clang"))
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -pedantic -O2")
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c99")
elseif ("${CMAKE_C_COMPILER_ID}" STREQUAL "MSVC")
  # msvc uses the latest supported C standard by default, so we don't have to set it to C99
  # force to always compile with W4 warning level
  if(CMAKE_C_FLAGS MATCHES "/W[0-4]")
    string(REGEX REPLACE "/W[0-4]" "/W4" CMAKE_C_FLAGS "${CMAKE_C_FLAGS}")
  endif()
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /MP /FS")
endif()

add_subdirectory(lib)

option(BUILD_TEST "Build test application." OFF)
if (BUILD_TEST)
  add_subdirectory(test)
endif()
