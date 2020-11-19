add_subdirectory(translations)
add_subdirectory(internet-access-control)
add_subdirectory(licensing)
add_subdirectory(ldap-pro)
add_subdirectory(network-discovery)
add_subdirectory(webtabs)

set(NSI "veyon-addons.nsi")

configure_file(nsis/${NSI}.in nsis/${NSI} @ONLY)

find_package(LibVNCServer 0.9.8)
if(NOT LibVNCServer_FOUND)
	include(LibVNCServerIntegration)
	set(_RFB_RFBCONFIG_H TRUE)
	set(LIBVNCSERVER_HAVE_LIBJPEG TRUE)
	set(LIBVNCSERVER_HAVE_LZO TRUE)
	set(LIBVNCSERVER_HAVE_LIBPNG TRUE)
	set(LIBVNCSERVER_HAVE_LIBPTHREAD TRUE)
	set(LIBVNCSERVER_HAVE_LIBZ TRUE)
	set(LIBVNCSERVER_HAVE_LIBSSL TRUE)
	set(LIBVNCSERVER_ALLOW24BPP TRUE)
	set(LIBVNCSERVER_IPv6 TRUE)
	file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/${VEYON_CORE_INCLUDE_DIR}/rfb)
	configure_file(${CMAKE_SOURCE_DIR}/3rdparty/libvncserver/rfb/rfbconfig.h.cmakein ${CMAKE_BINARY_DIR}/${VEYON_CORE_INCLUDE_DIR}/rfb/rfbconfig.h @ONLY)
endif()

set(ADDONS_BINARIES "${CMAKE_BINARY_DIR}/veyon-addons-${MINGW_PLATFORM}-${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}.${VERSION_BUILD}")

set(DLLDIR "${MINGW_PREFIX}/bin")
add_custom_target(addons
	DEPENDS
	addons-translations
	internetaccesscontrol
	internetaccesscontrol-routing
	internetaccesscontrol-firewall
	licensing
	ldap-pro
	network-discovery
	webtabs
)

add_custom_target(addons-binaries
	COMMAND rm -rf ${ADDONS_BINARIES}
	COMMAND mkdir -p ${ADDONS_BINARIES}/plugins
	COMMAND find -name '*.dll' -exec cp '{}' ${ADDONS_BINARIES}/plugins/ '\;'
	COMMAND mkdir -p ${ADDONS_BINARIES}/translations
	COMMAND cp translations/*qm ${ADDONS_BINARIES}/translations/
	COMMAND cp -r ${CMAKE_SOURCE_DIR}/COPYING ${ADDONS_BINARIES}
	COMMAND cp -r ${CMAKE_SOURCE_DIR}/nsis ${ADDONS_BINARIES}
	COMMAND cp nsis/${NSI} ${ADDONS_BINARIES}
	COMMAND cp ${DLLDIR}/libxml2-2.dll ${ADDONS_BINARIES}
	COMMAND cp ${DLLDIR}/icu*6*.dll ${ADDONS_BINARIES}
	COMMAND cp ${DLLDIR}/Qt5WebKit.dll ${DLLDIR}/Qt5WebKitWidgets.dll ${ADDONS_BINARIES}
	COMMAND find ${ADDONS_BINARIES} -name "*.dll" -exec ${STRIP} '{}' '\;'
	DEPENDS
	addons
)

add_custom_target(addons-nsi
	COMMAND makensis ${ADDONS_BINARIES}/${NSI}
	COMMAND mv ${ADDONS_BINARIES}/veyon-*setup.exe ${CMAKE_BINARY_DIR}
	DEPENDS addons-binaries
)

