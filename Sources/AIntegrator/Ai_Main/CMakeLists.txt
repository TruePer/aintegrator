####################################################################################################
### A-Integrator
### (c) 2016 - Denis Fatkulin - denis.fatkulin@gmail.com
####################################################################################################

include(${PROJECT_SOURCE_DIR}/Cmake/PrepareProject.cmake)
set(TARGET_NAME AIntegrator)

list(APPEND RESOURCES
	${EXTERNAL_DIR}/QmlMaterial/QmlMaterial.qrc
)

include_directories(
	${SOURCES_DIR}
	${SOURCES_DIR}/Ai_Kernel
	${SOURCES_DIR}/Ai_Kernel/Protocol
	${SOURCES_DIR}/Ai_Kernel/Protocol/code/proto
	${EXTERNAL_DIR}/Logger
)

set(LIBRARIES
	Ai_Kernel
	Logger
	QmlMaterial
)

add_my_executable()
qt5_use_modules(${TARGET_NAME} Core Quick Widgets)

