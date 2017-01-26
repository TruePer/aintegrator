####################################################################################################
### A-Integrator
### (c) 2016 - Denis Fatkulin - denis.fatkulin@gmail.com
####################################################################################################

set(REQURSIVE_INCLUDE_ENABLED no)
include(${PROJECT_SOURCE_DIR}/Cmake/PrepareProject.cmake)

include_directories(
	${EXTERNAL_DIR}/Logger
	${CMAKE_CURRENT_SOURCE_DIR}
	${CMAKE_CURRENT_SOURCE_DIR}/Protocol
	${CMAKE_CURRENT_SOURCE_DIR}/Protocol/code/proto
)

add_definitions(
	-DLIBPROTOBUF_EXPORTS
	-DAI_KERNEL_BUILD
)

if(MSVC)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /EHsc /wd4267 /bigobj")
	include_directories(${CMAKE_CURRENT_SOURCE_DIR}/Protocol/vcsprojects)
else()
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-deprecated")
	include_directories(${CMAKE_CURRENT_SOURCE_DIR}/Protocol/gcc)
endif()

set(LIBRARIES Logger)

add_my_library()
qt5_use_modules(${TARGET_NAME} Core Quick)

