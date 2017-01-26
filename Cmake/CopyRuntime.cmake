# Qt runtime libraries for Windows
####################################################################################################
if(WIN32)
get_target_property(QtCore_location Qt5::Core LOCATION)
set(QT_BINARY_DIR "${QtCore_location}/..")

#  Qt5 libraries
set(QT_DLL_NAMES
	Core
	Gui
	Network
	Qml
	Quick
	Widgets
	Svg
)

foreach(dll_file ${QT_DLL_NAMES})
	list(APPEND  QT_RELEASE_DLLS "${QT_BINARY_DIR}/Qt5${dll_file}.dll")
endforeach()

foreach(dll_file ${QT_DLL_NAMES})
	list(APPEND  QT_DEBUG_DLLS "${QT_BINARY_DIR}/Qt5${dll_file}d.dll")
endforeach()

list(APPEND debug_files ${QT_DEBUG_DLLS})
list(APPEND release_files ${QT_RELEASE_DLLS})

# OpenGL library
list(APPEND debug_and_release_files "${QT_BINARY_DIR}/opengl32sw.dll")

# QML
set(QML_MODULES
	QtGraphicalEffects
	Qt
	QtQml
	QtQuick
	QtQuick.2
)

foreach(module ${QML_MODULES})
	install(DIRECTORY "${QT_BINARY_DIR}/../qml/${module}" DESTINATION ${INSTALATION_DIR})
endforeach()

# Windows plugin
install(FILES ${QT_BINARY_DIR}/../plugins/platforms/qwindows.dll  DESTINATION ${INSTALATION_DIR}/platforms CONFIGURATIONS Release)
install(FILES ${QT_BINARY_DIR}/../plugins/platforms/qwindowsd.dll DESTINATION ${INSTALATION_DIR}/platforms CONFIGURATIONS Debug)

# Images plugins
install(FILES ${QT_BINARY_DIR}/../plugins/imageformats/qsvg.dll  DESTINATION ${INSTALATION_DIR}/imageformats CONFIGURATIONS Release)
install(FILES ${QT_BINARY_DIR}/../plugins/imageformats/qsvgd.dll DESTINATION ${INSTALATION_DIR}/imageformats CONFIGURATIONS Debug)

endif(WIN32)

# mingw libraries
####################################################################################################
if(MINGW)
	get_filename_component(CXX_PATH ${CMAKE_CXX_COMPILER} DIRECTORY)

	if(EXISTS ${CXX_PATH}/libgcc_s_dw2-1.dll)
		list(APPEND debug_and_release_files ${CXX_PATH}/libgcc_s_dw2-1.dll)
	endif()

	if(EXISTS ${CXX_PATH}/libstdc++-6.dll)
		list(APPEND debug_and_release_files ${CXX_PATH}/libstdc++-6.dll)
	endif()

	if(EXISTS ${CXX_PATH}/mingwm10.dll)
		list(APPEND debug_and_release_files ${CXX_PATH}/mingwm10.dll)
	endif()

	if(EXISTS ${CXX_PATH}/libwinpthread-1.dll)
		list(APPEND debug_and_release_files ${CXX_PATH}/libwinpthread-1.dll)
	endif()

endif(MINGW)

#################################

#file(COPY ${release_files} ${debug_files} ${debug_and_release_files} DESTINATION ${BUILD_DIR}/Bin)

install(FILES ${release_files} ${debug_and_release_files} DESTINATION ${INSTALATION_DIR} CONFIGURATIONS Release)
install(FILES ${debug_files}   ${debug_and_release_files} DESTINATION ${INSTALATION_DIR} CONFIGURATIONS Debug)

install(FILES ${PHONON_DLL} DESTINATION ${INSTALATION_DIR})
