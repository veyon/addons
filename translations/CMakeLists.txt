include(CreateTranslations)

file(GLOB addon_translations ${CMAKE_CURRENT_SOURCE_DIR}/*.ts)
file(GLOB_RECURSE addon_sources
	${CMAKE_CURRENT_SOURCE_DIR}/../*.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/../*.h
	${CMAKE_CURRENT_SOURCE_DIR}/../*.ui
	${CMAKE_CURRENT_SOURCE_DIR}/../*.qml
)

string(REGEX REPLACE "${CMAKE_CURRENT_SOURCE_DIR}/../veyon[^;]+;?" "" addon_sources "${addon_sources}")
string(REGEX REPLACE "${CMAKE_CURRENT_SOURCE_DIR}/../internet-access-control/3rdparty[^;]+;?" "" addon_sources "${addon_sources}")

create_translations(addons "${addon_translations}" "${addon_sources}")
