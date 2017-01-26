cmake_minimum_required(VERSION 2.8.9)
 
project(PwLoggerCustom)

set(TARGET_NAME Logger)

file(GLOB_RECURSE SOURCES *.cpp)
file(GLOB_RECURSE HEADERS *.h)

include_directories(
	${CMAKE_CURRENT_SOURCE_DIR}
	${CMAKE_CURRENT_SOURCE_DIR}/log4qt/helpers
	${CMAKE_CURRENT_BINARY_DIR}
	${CMAKE_CURRENT_SOURCE_DIR}/../../Sources/AIntegrator/Ai_Kernel
)

add_definitions(-DPWLOGGERLIB_BUILD)

# Подключаем Qt5
####################################################################################################
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON) # включаем автоматическую генерацию moc-файлов qt

find_package(Qt5Widgets)

# Определяем, куда складывать собранное
####################################################################################################
if(NOT DEFINED BUILD_DIR)
	set(BUILD_DIR ${CMAKE_CURRENT_SOURCE_DIR})
endif()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${BUILD_DIR}/Bin)      # для исполняемых
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${BUILD_DIR}/Bin)      # для *.so
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${BUILD_DIR}/Lib)      # для статический библиотек

add_library(${TARGET_NAME} SHARED ${SOURCES} ${HEADERS})

qt5_use_modules(${TARGET_NAME} Widgets)

# Скроем варнинги, нам они не интересны
####################################################################################################
if(MSVC)
	set(CMAKE_CXX_FLAGS "/DWIN32 /D_WINDOWS /w /GR /EHsc /MP")
elseif(UNIX)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -w")
endif()

set_target_properties(${TARGET_NAME}
	PROPERTIES
	VERSION   0.0.1
	DEBUG_OUTPUT_NAME ${TARGET_NAME}
	RELEASE_OUTPUT_NAME ${TARGET_NAME}
	)

if(DEFINED INSTALATION_DIR)
	INSTALL(TARGETS ${TARGET_NAME}
		RUNTIME DESTINATION ${INSTALATION_DIR}
		LIBRARY DESTINATION ${INSTALATION_DIR}
	)
endif()
