add_custom_target(addons)

set(CPACK_DEB_COMPONENT_INSTALL ON PARENT_SCOPE)
set(CPACK_RPM_COMPONENT_INSTALL ON PARENT_SCOPE)

macro(configure_cpack_component PACKAGE_NAME COMPONENT_NAME DISPLAY_NAME)
	set(CPACK_COMPONENT_${COMPONENT_NAME}_DISPLAY_NAME "${DISPLAY_NAME}" PARENT_SCOPE)
	set(CPACK_COMPONENT_${COMPONENT_NAME}_DESCRIPTION "${DISPLAY_NAME}" PARENT_SCOPE)
	if(${CPACK_GENERATOR} STREQUAL "DEB")
		unset(CPACK_PACKAGE_DESCRIPTION_SUMMARY PARENT_SCOPE)
		if(NOT ${PACKAGE_NAME} STREQUAL "veyon-addons")
			set(CPACK_DEBIAN_ENABLE_COMPONENT_DEPENDS ON PARENT_SCOPE)
			set(CPACK_COMPONENT_${COMPONENT_NAME}_DEPENDS "Unspecified" PARENT_SCOPE)
		endif()
		set(CPACK_DEBIAN_${COMPONENT_NAME}_PACKAGE_NAME "${PACKAGE_NAME}" PARENT_SCOPE)
	endif()
	if(${CPACK_GENERATOR} STREQUAL "RPM")
		set(CPACK_RPM_${COMPONENT_NAME}_PACKAGE_NAME "${PACKAGE_NAME}" PARENT_SCOPE)
		set(CPACK_RPM_${COMPONENT_NAME}_PACKAGE_SUMMARY "${DISPLAY_NAME}" PARENT_SCOPE)
		if(NOT ${PACKAGE_NAME} STREQUAL "veyon-addons")
			set(CPACK_RPM_${COMPONENT_NAME}_PACKAGE_REQUIRES "veyon-addons" PARENT_SCOPE)
		endif()
	endif()
endmacro()

configure_cpack_component("veyon-addons" "UNSPECIFIED" "Veyon Add-ons")
configure_cpack_component("veyon-addons-screenrecorder" "SCREENRECORDER" "Veyon ScreenRecorder Add-on")
configure_cpack_component("veyon-addons-webtabs" "WEBTABS" "Veyon WebTabs Add-on")

add_subdirectory(translations)
add_subdirectory(chat)
add_subdirectory(internet-access-control)
add_subdirectory(licensing)
add_subdirectory(ldap-pro)
add_subdirectory(network-discovery)
add_subdirectory(screenrecorder)
add_subdirectory(webtabs)

set(NSI "veyon-addons.nsi")

configure_file(nsis/${NSI}.in nsis/${NSI} @ONLY)

set(ADDONS_BINARIES "${CMAKE_BINARY_DIR}/veyon-addons-${MINGW_PLATFORM}-${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}.${VERSION_BUILD}")

set(DLLDIR "${MINGW_PREFIX}/bin")
set(LIBDIR "${MINGW_PREFIX}/lib")

add_custom_target(addons-binaries
	COMMAND rm -rf ${ADDONS_BINARIES}
	COMMAND mkdir -p ${ADDONS_BINARIES}/plugins
	COMMAND find -name '*.dll' -exec cp '{}' ${ADDONS_BINARIES}/plugins/ '\;'
	COMMAND rm ${ADDONS_BINARIES}/plugins/lib*.dll
	COMMAND mkdir -p ${ADDONS_BINARIES}/translations
	COMMAND cp translations/*qm ${ADDONS_BINARIES}/translations/
	COMMAND cp -r ${CMAKE_SOURCE_DIR}/COPYING ${ADDONS_BINARIES}
	COMMAND cp -r ${CMAKE_SOURCE_DIR}/nsis ${ADDONS_BINARIES}
	COMMAND cp nsis/${NSI} ${ADDONS_BINARIES}
	COMMAND cp ${DLLDIR}/avcodec*.dll ${DLLDIR}/avformat*.dll ${DLLDIR}/avutil*.dll ${DLLDIR}/swscale*.dll ${DLLDIR}/libx264*.dll ${ADDONS_BINARIES}
	COMMAND cp ${DLLDIR}/libxml2-2.dll ${ADDONS_BINARIES}
	COMMAND cp ${DLLDIR}/icu*6*.dll ${ADDONS_BINARIES}
	COMMAND cp ${DLLDIR}/libsqlite3-0.dll ${ADDONS_BINARIES}
	COMMAND cp ${DLLDIR}/Qt5WebKit.dll ${DLLDIR}/Qt5WebKitWidgets.dll ${ADDONS_BINARIES}
	COMMAND cp ${LIBDIR}/WebView2Loader.dll ${ADDONS_BINARIES}
	COMMAND find ${ADDONS_BINARIES} -name "*.dll" -exec ${STRIP} '{}' '\;'
	DEPENDS
	addons
)

add_custom_target(addons-nsi
	COMMAND makensis ${ADDONS_BINARIES}/${NSI}
	COMMAND mv ${ADDONS_BINARIES}/veyon-*setup.exe ${CMAKE_BINARY_DIR}
	DEPENDS addons-binaries
)

