####################################################################################################
### A-Integrator
### (c) 2016 - Denis Fatkulin - denis.fatkulin@gmail.com
####################################################################################################

####################################################################################################
# MACRO: header_directories
# На вход принимает путь к папке, в которой рекурсвино ищет заголовочные файлы (*.h)
# Возвращает список INCLUDE_RECURSIVE_DIR, содержащий директории для поключения в include
macro(include_recursive_directories include_root_dir)
	file(GLOB_RECURSE new_list ${include_root_dir}/*.h)
	set(INCLUDE_RECURSIVE_DIR "")
	foreach(file_path ${new_list})
		get_filename_component(dir_path ${file_path} PATH)
		set(INCLUDE_RECURSIVE_DIR ${INCLUDE_RECURSIVE_DIR} ${dir_path})
	endforeach()
	list(REMOVE_DUPLICATES INCLUDE_RECURSIVE_DIR)
	set(INCLUDE_RECURSIVE_DIR ${INCLUDE_RECURSIVE_DIR} ${include_root_dir})

	include_directories(${INCLUDE_RECURSIVE_DIR})
endmacro()

####################################################################################################
#MACRO: hg_version
# Узнает версию приложения из тега Mercurial'a
macro(hg_version source_dir)
	# Зададим начальные значения, на случай если не удастся узнать версию
	set(HG_MAJOR_VER "0")
	set(HG_MINOR_VER "1")
	set(HG_REVISION_VER "0")

	# Узнаем номер версии по тэгу hg
	execute_process(
		COMMAND hg log -r . --template {latesttag}
		OUTPUT_VARIABLE HG_FULL_VERSION
		WORKING_DIRECTORY ${source_dir})

	if("${HG_FULL_VERSION}" STREQUAL  "")
		set(HG_FULL_VERSION "0.1.0")
	else()
		string(REPLACE "." ";" HG_VERSION_LIST "${HG_FULL_VERSION}")
	endif()



	list(LENGTH HG_VERSION_LIST len)

	if(len EQUAL 2)
		list(GET HG_VERSION_LIST 0 HG_MAJOR_VER)
		list(GET HG_VERSION_LIST 1 HG_MINOR_VER)
	endif()

	# Узнаем номер текущей ревизии
	execute_process(
		COMMAND hg id -n ${source_dir}
		OUTPUT_VARIABLE HG_REVISION_VER)

    if(NOT HG_REVISION_VER STREQUAL  "")
		string(REPLACE "+" "" HG_REVISION_VER ${HG_REVISION_VER})
		string(REPLACE "\n" "" HG_REVISION_VER ${HG_REVISION_VER})
    else()
        set(HG_REVISION_VER "0")
    endif()

		# Узнаем id текущей ревизии
	execute_process(
		COMMAND hg id -i ${source_dir}
		OUTPUT_VARIABLE HG_REVISION_ID)

	if(NOT HG_REVISION_ID STREQUAL  "")
		string(REPLACE "+" "" HG_REVISION_ID ${HG_REVISION_ID})
		string(REPLACE "\n" "" HG_REVISION_ID ${HG_REVISION_ID})
	else()
		set(HG_REVISION_ID "develop")
	endif()

	add_definitions(
		-DMAJOR_VER=${HG_MAJOR_VER}
		-DMINOR_VER=${HG_MINOR_VER}
		-DREVISION_VER=${HG_REVISION_VER}
		-DREVISION_ID=\"${HG_REVISION_ID}\"
	)

	set(PROGRAM_VERSION "${HG_MAJOR_VER}.${HG_MINOR_VER}.${HG_REVISION_VER} [${HG_REVISION_ID}]")
endmacro()

####################################################################################################
#MACRO: add_subdirectory_ordered
macro(add_subdirectory_ordered subdir_path)
	get_filename_component(subdir ${subdir_path} NAME)
	add_subdirectory(${subdir_path})
	string(REPLACE " " "_" subdir ${subdir})

	if(DEFINED SUBDIR_LIST AND TARGET ${subdir})
		add_dependencies(${subdir} ${SUBDIR_LIST})
	endif()

	list(APPEND SUBDIR_LIST ${subdir})
endmacro()

####################################################################################################
#MACRO: add_subdirectory_depended
macro(add_subdirectory_depended subdir_path depend)
	get_filename_component(subdir ${subdir_path} NAME)
	add_subdirectory(${subdir_path})
	string(REPLACE " " "_" subdir ${subdir})

	if(TARGET ${depend})
		add_dependencies(${subdir} ${depend})
	endif()
endmacro()

####################################################################################################
#MACRO: add_subdirectory_plugin
macro(add_subdirectory_plugin subdir_path)
	get_filename_component(subdir ${subdir_path} NAME)
	add_subdirectory(${subdir_path}) # ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/plugins/widget_plugins)
	string(REPLACE " " "_" subdir ${subdir})


	add_dependencies(${subdir} 3Gpp.Module)
endmacro()

####################################################################################################
#MACRO: add_subdirectory_test
macro(add_subdirectory_test subdir_path)
	get_filename_component(subdir ${subdir_path} NAME)
	add_subdirectory(${subdir_path}) # ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/plugins/widget_plugins)
	string(REPLACE " " "_" subdir ${subdir})


	add_dependencies(${subdir} 3Gpp.Module)
endmacro()

####################################################################################################
#MACRO: prepare_source_group
# Группирует файлы по папочкам для отображения в MSVS
macro(prepare_source_group)

	file(GLOB_RECURSE files_list ${CMAKE_CURRENT_SOURCE_DIR}/*.*)

	# Прочитаем все файлики из списка
	foreach(file ${files_list})
		# Узнаем путь относительно текущего каталога проекта
		file(RELATIVE_PATH rel_path ${CMAKE_CURRENT_SOURCE_DIR} ${file})

		get_filename_component(dir "${rel_path}" PATH)
		get_filename_component(exten "${rel_path}" EXT)

		set(group "Files")

		# Определим, является ли файл заголовочным, или исходником, или формой 
		string(FIND ".h .hpp .pb.h .pb.hpp .hcc .c .cc .cpp .ui" ${exten} is_source)

		if(is_source GREATER -1)
			set(group "Sources")
		endif()

		# Определим, является ли файл заголовочным, или исходником, или формой 
		string(FIND ".rc .qrc" ${exten} is_resource)

		if(is_resource GREATER -1)
			set(group "Resources")
		endif()

		# Поместим файл в найденную группу
		string(REPLACE "/" "\\" file_group "${dir}")
		source_group("${group}\\${file_group}" FILES ${file})

	endforeach()	
endmacro()

####################################################################################################
#MACRO: generate_qmldir
#PARAMETERS:
# module_path - Расположение модуля в файловой системе
# module_name - Название модуля, которое будет указано в заголовке qmldir
# Группирует файлы по папочкам для отображения в MSVS
macro(generate_qmldir module_path module_name)

	set(qmldir_file "${module_path}/qmldir")
	file(GLOB_RECURSE files_list ${module_path}/*.qml ${module_path}/*.js)

	list(APPEND files_list
		${qml_list}
		${js_list}
	)

	file(WRITE ${qmldir_file} "# Autogenerated file. Do not edit!\n")
	file(APPEND ${qmldir_file} "module ${module_name}\n")

	# Прочитаем все файлики из списка
	foreach(file ${files_list})
		# Узнаем путь относительно текущего каталога проекта
		file(RELATIVE_PATH rel_path ${module_path} ${file})

		get_filename_component(name "${rel_path}" NAME_WE)
		get_filename_component(dir "${rel_path}" PATH)

		file(APPEND ${qmldir_file} "${name} 1.0 ${rel_path}\n")

	endforeach()
endmacro()
