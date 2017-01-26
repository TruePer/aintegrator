####################################################################################################
### A-Integrator
### (c) 2016 - Denis Fatkulin - denis.fatkulin@gmail.com
####################################################################################################

# Имя проекта равно названию папки, в которой лежат файлы проекта
get_filename_component(TARGET_NAME ${CMAKE_CURRENT_SOURCE_DIR} NAME)
string(REPLACE " " "_" TARGET_NAME ${TARGET_NAME})

# Рекурсивно ищем файлы в текущей директории проекта
file(GLOB_RECURSE SOURCES
	${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/*.cc
)

file(GLOB_RECURSE HEADERS
	${CMAKE_CURRENT_SOURCE_DIR}/*.h
	${CMAKE_CURRENT_SOURCE_DIR}/*.hpp
)

file(GLOB_RECURSE RESOURCES     ${CMAKE_CURRENT_SOURCE_DIR}/*.qrc)
file(GLOB_RECURSE FORMS         ${CMAKE_CURRENT_SOURCE_DIR}/*.ui)
file(GLOB_RECURSE QML_FILES     ${CMAKE_CURRENT_SOURCE_DIR}/*.qml)

if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/Jinn AND DEFINED INSTALATION_DIR)
	install (DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/Jinn DESTINATION ${INSTALATION_DIR})
endif()

# Добавим необходимые папки для поиска заголовочных файлов
include_directories(${CMAKE_CURRENT_BINARY_DIR})           # Сюда складываются moc_* и ui_* файлы
if(NOT "${REQURSIVE_INCLUDE_ENABLED}" STREQUAL "no")
	include_recursive_directories(${CMAKE_CURRENT_SOURCE_DIR}) # Рекурсивно добавим все папки проекта
endif()

# Добавление в сборку библиотеки
macro(add_my_library)
	prepare_source_group()

	qt5_add_resources(QRC_SOURCES ${RESOURCES})
	qt5_wrap_ui(UI_SOURCES ${FORMS})

	add_library(${TARGET_NAME} SHARED ${SOURCES} ${HEADERS} ${QRC_SOURCES} ${UI_SOURCES} ${QML_FILES})

	target_link_libraries(${TARGET_NAME} ${LIBRARIES})
	
	set_target_properties(${TARGET_NAME} PROPERTIES
		VERSION   ${HG_FULL_VERSION}
		SOVERSION ${HG_FULL_VERSION}
		DEBUG_OUTPUT_NAME ${TARGET_NAME}d
		RELEASE_OUTPUT_NAME ${TARGET_NAME}
	)

	if(DEFINED INSTALATION_DIR)
		INSTALL(TARGETS ${TARGET_NAME}
			RUNTIME DESTINATION ${INSTALATION_DIR}
			LIBRARY DESTINATION ${INSTALATION_DIR}
			ARCHIVE DESTINATION ${INSTALATION_DIR}/../Lib)
	endif()
endmacro()

# Добавление в сборку екзешника с его установкой
macro(add_my_executable)
	prepare_source_group()

	qt5_add_resources(QRC_SOURCES ${RESOURCES})
	qt5_wrap_ui(UI_SOURCES ${FORMS})
	
	add_executable(${TARGET_NAME} WIN32 ${SOURCES} ${HEADERS} ${QRC_SOURCES} ${UI_SOURCES} ${QML_FILES})
	target_link_libraries(${TARGET_NAME} ${LIBRARIES})

	if(DEFINED INSTALATION_DIR)
		INSTALL(TARGETS ${TARGET_NAME}
			RUNTIME DESTINATION ${INSTALATION_DIR}
			LIBRARY DESTINATION ${INSTALATION_DIR})
	endif()
endmacro()
