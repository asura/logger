# Doxygen
find_package(Doxygen)

if (DOXYGEN_FOUND)
  configure_file(
    ${PROJECT_SOURCE_DIR}/docs/Doxyfile.in
    ${PROJECT_BINARY_DIR}/docs/Doxyfile
  )

  # Doxygenがある場合のみ"make doxygen"というmakeのターゲットを用意しておく
  add_custom_target(
    doc
    ${DOXYGEN_EXECUTABLE} ${PROJECT_BINARY_DIR}/docs/Doxyfile
    WORKING_DIRECTORY ${PROJECT_BINARY_DIR}
  )
endif ()
