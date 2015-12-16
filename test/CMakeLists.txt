set(TARGET toxsplitmessage-test)

aux_source_directory(. SRC_LIST)

add_executable(${TARGET} ${SRC_LIST})
target_link_libraries(${TARGET} toxsplitmessage)

set(TOXCORE_INCLUDE_DIR_PATH "NONE" CACHE PATH "Path to toxcore include directory.")
if (${TOXCORE_INCLUDE_DIR_PATH} STREQUAL "NONE")
  find_path(FIND_TOXCORE_INCLUDE_DIR_PATH tox.h PATHS /usr/include/ /usr/local/include PATH_SUFFIXES tox)
  if (FIND_TOXCORE_INCLUDE_DIR_PATH)
    target_include_directories(${TARGET} PRIVATE ${FIND_TOXCORE_INCLUDE_DIR_PATH})
  else()
    message(FATAL_ERROR "Couldn't find include directory of tox.h file. Please specify it using TOXCORE_INCLUDE_DIR_PATH option.")
  endif()
else()
  target_include_directories(${TARGET} PRIVATE ${TOXCORE_INCLUDE_DIR_PATH})
endif()

INSTALL(TARGETS ${TARGET}
  RUNTIME DESTINATION bin
)
