# ---- Checking if Doxygen is installed ----
find_package(Doxygen)

if(DOXYGEN_FOUND)
	# ---- Declare documentation target ----
	set(DOXYGEN_IN ${PROJECT_SOURCE_DIR}/docs/Doxyfile.in)
	set(DOXYGEN_OUT ${PROJECT_SOURCE_DIR}/docs/Doxyfile)
	configure_file(${DOXYGEN_IN} ${DOXYGEN_OUT} @ONLY)

	add_custom_target(
		docs
		COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_OUT}
		COMMENT "Building documentation using Doxygen."
		WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}/docs")

else(DOXYGEN_FOUND)
	message("Doxygen need to be installed to generate the doxygen documentation")
endif(DOXYGEN_FOUND)
