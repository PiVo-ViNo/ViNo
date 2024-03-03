# ---- Проверяем, установлен ли Doxygen ----
find_package(Doxygen)

if(DOXYGEN_FOUND)
	# ---- Объявяем цель для создания документации ----
	set(DOXYGEN_IN ${PROJECT_SOURCE_DIR}/docs/Doxyfile.in)
	set(DOXYGEN_OUT ${PROJECT_SOURCE_DIR}/docs/Doxyfile)

	add_custom_target(
		docs
		configure_file(${DOXYGEN_IN} ${DOXYGEN_OUT} @ONLY)
		COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_OUT}
		COMMAND rm ${DOXYGEN_OUT}
		COMMENT "Building documentation using Doxygen."
		WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}/docs")

else(DOXYGEN_FOUND)
	message("Doxygen need to be installed to generate the doxygen documentation")
endif(DOXYGEN_FOUND)
