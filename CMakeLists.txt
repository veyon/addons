ADD_SUBDIRECTORY(internet-access-control)
ADD_SUBDIRECTORY(licensing)
ADD_SUBDIRECTORY(ldap-pro)
ADD_SUBDIRECTORY(network-discovery)

SET(TMP "${CMAKE_BINARY_DIR}/veyon-extra-plugins")
SET(NSI "veyon-extra-plugins.nsi")

CONFIGURE_FILE(nsis/${NSI}.in nsis/${NSI} @ONLY)

ADD_CUSTOM_TARGET(extra-plugins-nsi

   COMMAND rm -rf ${TMP}
   COMMAND mkdir -p ${TMP}/plugins
   COMMAND find -name '*.dll' -exec cp '{}' ${TMP}/plugins/ '\;'
   COMMAND ${STRIP} ${TMP}/plugins/*.dll
   COMMAND cp nsis/${NSI} ${TMP}
   COMMAND makensis ${TMP}/${NSI}
   COMMAND mv ${TMP}/veyon-*setup.exe ${CMAKE_BINARY_DIR}
   #COMMAND rm -rf ${TMP}
  )

add_dependencies(extra-plugins-nsi licensing ldap-pro network-discovery)


