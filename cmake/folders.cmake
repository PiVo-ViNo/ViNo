set_property(GLOBAL PROPERTY USE_FOLDERS YES)

# Вызовите эту функцию в конце области каталога, чтобы назначить папку таргетам, созданным в этом каталоге.
# Таргеты будут назначены в папку UtilityTargets, в противном случае в папку ${name}Targets.
# Если таргету уже назначена папка, то этот таргет будет пропущен.
function(add_folders name)
	get_property(
		targets
		DIRECTORY
		PROPERTY BUILDSYSTEM_TARGETS)
	foreach(target IN LISTS targets)
		get_property(
			folder
			TARGET "${target}"
			PROPERTY FOLDER)
		if(DEFINED folder)
			continue()
		endif()
		set(folder Utility)
		get_property(
			type
			TARGET "${target}"
			PROPERTY TYPE)
		if(NOT type STREQUAL "UTILITY")
			set(folder "${name}")
		endif()
		set_property(TARGET "${target}" PROPERTY FOLDER "${folder}Targets")
	endforeach()
endfunction()
