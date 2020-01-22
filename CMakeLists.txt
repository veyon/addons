ADD_SUBDIRECTORY(translations)
ADD_SUBDIRECTORY(internet-access-control)
ADD_SUBDIRECTORY(licensing)
ADD_SUBDIRECTORY(ldap-pro)
ADD_SUBDIRECTORY(network-discovery)

SET(TMP "${CMAKE_BINARY_DIR}/veyon-addons")
SET(NSI "veyon-addons.nsi")

CONFIGURE_FILE(nsis/${NSI}.in nsis/${NSI} @ONLY)

ADD_CUSTOM_TARGET(addons-nsi

   COMMAND rm -rf ${TMP}
   COMMAND mkdir -p ${TMP}/plugins
   COMMAND find -name '*.dll' -exec cp '{}' ${TMP}/plugins/ '\;'
   COMMAND ${STRIP} ${TMP}/plugins/*.dll
   COMMAND cp nsis/${NSI} ${TMP}
   COMMAND makensis ${TMP}/${NSI}
   COMMAND mv ${TMP}/veyon-*setup.exe ${CMAKE_BINARY_DIR}
   #COMMAND rm -rf ${TMP}
  )

add_dependencies(addons-nsi
	internetaccesscontrol
	internetaccesscontrol-routing
	internetaccesscontrol-firewall
	licensing
	ldap-pro
	network-discovery
)

