cmake_minimum_required(VERSION 3.16)

set(ISPELL_VERSION 3.4.05)
set(ISPELL_TARGET_NAME ISpell)
project(${ISPELL_TARGET_NAME} VERSION ${ISPELL_VERSION} LANGUAGES C)

include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

execute_process(COMMAND
	sh -c "mkdir -p ${CMAKE_CURRENT_BINARY_DIR}/include && cp *.h ${CMAKE_CURRENT_BINARY_DIR}/include"
	WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)

if (APPLE)
	set(LOCAL_H "local.h.macos")
elseif (UNIX)
	set(LOCAL_H "local.h.linux")
else()
	message(FATAL_ERROR "Only supported on APPLE and UNIX")
endif()

execute_process(COMMAND
	cp ${LOCAL_H} local.h
	WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)

file(READ "${CMAKE_CURRENT_SOURCE_DIR}/local.h" FILE_CONTENT)
string(REPLACE "/usr/local" "${CMAKE_INSTALL_FULL_DATAROOTDIR}/${EXECUTABLE_NAME}" FILE_CONTENT "${FILE_CONTENT}" )
file(WRITE "${CMAKE_CURRENT_SOURCE_DIR}/local.h" "${FILE_CONTENT}")


set(ISPELL_SOURCES
	term.c
	ispell.c
	correct.c
	defmt.c
	dump.c
	exp_table.c
	fields.c
	good.c
	lookup.c
	hash.c
	makedent.c
	tgood.c
	tree.c
	xgets.c
)

add_library(${ISPELL_TARGET_NAME} SHARED ${ISPELL_SOURCES})

install(TARGETS ${ISPELL_TARGET_NAME}
  EXPORT ${ISPELL_TARGET_NAME}LibraryDepends
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
)
