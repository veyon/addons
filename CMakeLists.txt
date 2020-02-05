add_subdirectory(translations)
add_subdirectory(internet-access-control)
add_subdirectory(licensing)
add_subdirectory(ldap-pro)
add_subdirectory(network-discovery)

set(NSI "veyon-addons.nsi")

configure_file(nsis/${NSI}.in nsis/${NSI} @ONLY)

set(ADDONS_BINARIES "${CMAKE_BINARY_DIR}/veyon-addons-${MINGW_PLATFORM}-${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}.${VERSION_BUILD}")

add_custom_target(addons-binaries
	COMMAND rm -rf ${ADDONS_BINARIES}
	COMMAND mkdir -p ${ADDONS_BINARIES}/plugins
	COMMAND find -name '*.dll' -exec cp '{}' ${ADDONS_BINARIES}/plugins/ '\;'
	COMMAND ${STRIP} ${ADDONS_BINARIES}/plugins/*.dll
	COMMAND mkdir -p ${ADDONS_BINARIES}/translations
	COMMAND cp translations/*qm ${ADDONS_BINARIES}/translations/
	COMMAND cp -r ${CMAKE_SOURCE_DIR}/COPYING ${ADDONS_BINARIES}
	COMMAND cp -r ${CMAKE_SOURCE_DIR}/nsis ${ADDONS_BINARIES}
	COMMAND cp nsis/${NSI} ${ADDONS_BINARIES}
	DEPENDS
	addons-translations
	internetaccesscontrol
	internetaccesscontrol-routing
	internetaccesscontrol-firewall
	licensing
	ldap-pro
	network-discovery
)

add_custom_target(addons-nsi
	COMMAND makensis ${ADDONS_BINARIES}/${NSI}
	COMMAND mv ${ADDONS_BINARIES}/veyon-*setup.exe ${CMAKE_BINARY_DIR}
	DEPENDS addons-binaries
)

