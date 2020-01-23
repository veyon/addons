add_subdirectory(translations)
add_subdirectory(internet-access-control)
add_subdirectory(licensing)
add_subdirectory(ldap-pro)
add_subdirectory(network-discovery)

set(TMP "${CMAKE_BINARY_DIR}/veyon-addons")
set(NSI "veyon-addons.nsi")

configure_file(nsis/${NSI}.in nsis/${NSI} @ONLY)

add_custom_target(addons-nsi

	COMMAND rm -rf ${TMP}
	COMMAND mkdir -p ${TMP}/plugins
	COMMAND find -name '*.dll' -exec cp '{}' ${TMP}/plugins/ '\;'
	COMMAND ${STRIP} ${TMP}/plugins/*.dll
	COMMAND mkdir -p ${TMP}/translations
	COMMAND cp translations/*qm ${TMP}/translations/
	COMMAND cp nsis/${NSI} ${TMP}
	COMMAND makensis ${TMP}/${NSI}
	COMMAND mv ${TMP}/veyon-*setup.exe ${CMAKE_BINARY_DIR}
)

add_dependencies(addons-nsi
	addons-translations
	internetaccesscontrol
	internetaccesscontrol-routing
	internetaccesscontrol-firewall
	licensing
	ldap-pro
	network-discovery
)

