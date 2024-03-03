# ---- Режим разработчика ----

# Режим разработчика включает цели и пути кода в сценариях CMake,
# которые актуальны только для разработчиков этого проекта.
# Цели, необходимые для сборки проекта, должны быть предоставлены безоговорочно,
# чтобы потребители могли легко собрать и упаковать проект.
if(PROJECT_IS_TOP_LEVEL)
	option(ViNo_DEVELOPER_MODE "Enable developer mode" OFF)
endif()

# ---- Проверка на достустимость использования режима разработчика ----

# target_include_directories с модификатором SYSTEM будет требовать от компилятора
# исключить предупреждения из предоставленных путей, если компилятор поддерживает это.
# Это сделано для того, чтобы обеспечить взаимодействие с пользователем, аналогичное
# find_package, когда add_subdirectory или FetchContent используется для этого проекта.
set(warning_guard "")
if(NOT PROJECT_IS_TOP_LEVEL)
	option(ViNo_INCLUDES_WITH_SYSTEM "Use SYSTEM modifier for ViNo's includes, disabling warnings" ON)
	mark_as_advanced(ViNo_INCLUDES_WITH_SYSTEM)
	if(ViNo_INCLUDES_WITH_SYSTEM)
		set(warning_guard SYSTEM)
	endif()
endif()
