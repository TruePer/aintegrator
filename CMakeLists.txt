####################################################################################################
### A-Integrator
### (c) 2016 - Denis Fatkulin - denis.fatkulin@gmail.com
####################################################################################################

cmake_minimum_required(VERSION 2.8.9)

project(AIntegrator)

#set(CMAKE_VERBOSE_MAKEFILE ON)
set(NO_SONAME True)

# Укажем общие переменные
##################################################
set(SOURCES_DIR      ${PROJECT_SOURCE_DIR}/Sources/AIntegrator)
set(EXTERNAL_DIR     ${PROJECT_SOURCE_DIR}/External)
set(INSTALATION_DIR  ${PROJECT_SOURCE_DIR}/Bin)
set(BUILD_DIR        ${PROJECT_SOURCE_DIR}/Build)

# Подключаем Qt5
##################################################
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON) # включаем автоматическую генерацию moc-файлов qt

find_package(Qt5Core)
find_package(Qt5Quick)
find_package(Qt5Widgets)

include_directories (
	${Qt5Widgets_INCLUDE_DIRS}
	${Qt5Quick_INCLUDE_DIRS}
)

# Подключаем общие заголовочные файлы проекта
##################################################
include(${PROJECT_SOURCE_DIR}/Cmake/Tools.cmake)
include(${PROJECT_SOURCE_DIR}/Cmake/CopyRuntime.cmake)

# Версия программы
##################################################
hg_version(${PROJECT_SOURCE_DIR})
include(${PROJECT_SOURCE_DIR}/Cmake/Nsis.cmake)

# Зададим куда складывать бинарники
##################################################
set(PROJECT_DESTDIR ${PROJECT_SOURCE_DIR}/Bin)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${BUILD_DIR}/Bin)      # для исполняемых
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${BUILD_DIR}/Bin)      # для *.so
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${BUILD_DIR}/Lib)      # для статических библиотек

file(MAKE_DIRECTORY ${PROJECT_SOURCE_DIR}/Bin)
file(MAKE_DIRECTORY ${BUILD_DIR}/Bin)
file(MAKE_DIRECTORY ${BUILD_DIR}/Lib)

# Зададим пути, по которым линковщик будет искать библиотеки для линковки
##################################################
link_directories(
	${BUILD_DIR}/Bin
	${BUILD_DIR}/Lib
)

# Зададим опции компиляции
##################################################
if(MSVC)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP")
	add_definitions(-D_SCL_SECURE_NO_WARNINGS)
elseif(UNIX)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC -std=c++11")
endif()

# Специальные макросы
##################################################
add_definitions(
	-DPROTOBUF_USE_DLLS                               # Сборка протобуфа с использованием DLL
)

if(UNIX)
	add_definitions(
		-DQ_ATOMIC_POINTER_TEST_AND_SET_IS_ALWAYS_NATIVE  # Сборка логгера с поддержкой многопоточности
	)
endif()

# Собрем протокол взаимодействия с СУО
##################################################
if(NOT EXISTS "${SOURCES_DIR}/Ai_Kernel/Protocol/code/proto" )
	if(UNIX)
		execute_process(
			COMMAND "./makeproto.sh"
			WORKING_DIRECTORY "${SOURCES_DIR}/Ai_Kernel/Protocol/utils"
		)
	elseif(WIN32)
		execute_process(
			COMMAND "makeproto.cmd"
			WORKING_DIRECTORY "${SOURCES_DIR}/Ai_Kernel/Protocol/utils"
		)
	endif()
endif()

# Распечатаем информацию о сборке
##################################################
message("---------------------------------
Configuration  = ${CMAKE_BUILD_TYPE}
---------------------------------
Product        = ${PROJECT_NAME}
Version        = ${PROGRAM_VERSION}
Qt version     = ${Qt5Core_VERSION}
Build path     = ${CMAKE_BINARY_DIR}
Compiler       = ${CMAKE_CXX_COMPILER_ID}
Compiler path  = ${CMAKE_CXX_COMPILER}
Compiler flags = ${CMAKE_CXX_FLAGS}
Linker flags   = ${CMAKE_EXE_LINKER_FLAGS}
---------------------------------
")


# Добавим в сборку необходимые подпроекты
##################################################
add_subdirectory(${EXTERNAL_DIR}/QmlMaterial)
add_subdirectory(${EXTERNAL_DIR}/Logger)
add_subdirectory(${SOURCES_DIR}/Ai_Kernel)
add_subdirectory(${SOURCES_DIR}/Ai_Main)

add_subdirectory(${PROJECT_SOURCE_DIR}/Sources/TestTcpClient)
