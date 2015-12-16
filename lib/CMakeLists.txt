set(TARGET toxsplitmessage)

option(BUILD_STATIC "Build a static library instead." OFF)
if (BUILD_STATIC)
  set(LIBRARY_TYPE "STATIC")
  message(STATUS "Building static library")
else()
  set(LIBRARY_TYPE "SHARED")
  message(STATUS "Building shared library")
endif()

aux_source_directory(src SRC_LIST)
file(GLOB INCLUDE_LIST include/*.h)

include (GenerateExportHeader)
add_compiler_export_flags()

add_library(${TARGET} ${LIBRARY_TYPE} ${SRC_LIST} ${INCLUDE_LIST})

generate_export_header(${TARGET} BASE_NAME tox_split_message)

target_include_directories(${TARGET} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include/ ${PROJECT_BINARY_DIR}/src/)

set(INCLUDE_LIST ${INCLUDE_LIST} ${PROJECT_BINARY_DIR}/lib/tox_split_message_export.h)

# make public headers globally available
target_include_directories(${TARGET} PUBLIC ${CMAKE_SOURCE_DIR}/include ${PROJECT_BINARY_DIR}/lib)

set_target_properties(${TARGET} PROPERTIES PUBLIC_HEADER "${INCLUDE_LIST}")

INSTALL(TARGETS ${TARGET}
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib
  ARCHIVE DESTINATION lib
  PUBLIC_HEADER DESTINATION include/${TARGET}
)
