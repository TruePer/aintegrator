####################################################################################################
### A-Integrator
### (c) 2016 - Denis Fatkulin - denis.fatkulin@gmail.com
####################################################################################################

include(${PROJECT_SOURCE_DIR}/Cmake/PrepareProject.cmake)

include_directories(
	${SOURCES_DIR}
	${SOURCES_DIR}/Ai_Kernel
	${SOURCES_DIR}/Ai_Kernel/Protocol
	${SOURCES_DIR}/Ai_Kernel/Protocol/code/proto
)

set(LIBRARIES
	Ai_Kernel
)

add_my_executable()
qt5_use_modules(${TARGET_NAME} Widgets)

