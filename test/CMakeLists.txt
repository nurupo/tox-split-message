set(TARGET toxsplitmessage-test)

aux_source_directory(. SRC_LIST)

add_executable(${TARGET} ${SRC_LIST})
target_link_libraries(${TARGET} toxsplitmessage)

INSTALL(TARGETS ${TARGET}
  RUNTIME DESTINATION bin
)
