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
    DEPENDS adoc
  )
endif ()

add_custom_target(
  adoc_design
  bundle exec asciidoctor-pdf -D ${PROJECT_BINARY_DIR}/docs/html -a imagesdir=${PROJECT_BINARY_DIR}/docs/images -a chapter-label= -a toc -a pdf-stylesdir=docs/design/style -a pdf-style=style.yml -r asciidoctor-pdf-cjk -r asciidoctor-diagram -a imagesoutdir=${PROJECT_BINARY_DIR}/docs/images docs/design/design.adoc
  WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
)

add_custom_target(
  adoc_test_spec
  bundle exec asciidoctor-pdf -D ${PROJECT_BINARY_DIR}/docs/html -a imagesdir=${PROJECT_BINARY_DIR}/docs/images -a chapter-label= -a toc -a pdf-stylesdir=docs/test_spec/style -a pdf-style=style.yml -r asciidoctor-pdf-cjk -r asciidoctor-diagram -a imagesoutdir=${PROJECT_BINARY_DIR}/docs/images docs/test_spec/test_spec.adoc
  WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
)

add_custom_target(
  adoc
  DEPENDS adoc_design adoc_test_spec
)