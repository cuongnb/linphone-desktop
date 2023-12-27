############################################################################
# CMakeLists.txt
# Copyright (C) 2014-2023  Belledonne Communications, Grenoble France
#
############################################################################
#
# This program is free software; you can redistribute it and/or
# modify it under the terms of the GNU General Public License
# as published by the Free Software Foundation; either version 2
# of the License, or (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
#
############################################################################

cmake_minimum_required(VERSION 3.1)

project(SQLITE3 LANGUAGES C)


set(SOURCE_FILES sqlite3.c)

if(CMAKE_SYSTEM_NAME STREQUAL "WindowsPhone" OR CMAKE_SYSTEM_NAME STREQUAL "WindowsStore")
	add_definitions(
		-DSQLITE_OS_WINRT=1
		-DSQLITE_WIN32_FILEMAPPING_API=1
		-DSQLITE_OMIT_LOAD_EXTENSION
	)
endif()

include(GNUInstallDirs)

add_library(sqlite3 ${SOURCE_FILES})
if(MSVC)
	add_definitions("-DSQLITE_API=__declspec(dllexport)")
	if(CMAKE_BUILD_TYPE STREQUAL "Debug" OR CMAKE_BUILD_TYPE STREQUAL "RelWithDebInfo")
		install(FILES $<TARGET_PDB_FILE:sqlite3>
			DESTINATION ${CMAKE_INSTALL_BINDIR}
			PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE
		)
	endif()
endif()

target_link_libraries(sqlite3 ${CMAKE_DL_LIBS})
target_include_directories(sqlite3 INTERFACE
	$<INSTALL_INTERFACE:include>
	$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}>
)

install(TARGETS sqlite3 EXPORT Sqlite3Targets
	RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
	LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
	ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
	PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE
)


file(GLOB HEADER_FILES "*.h")

install(FILES ${HEADER_FILES}
	DESTINATION include
	PERMISSIONS OWNER_READ OWNER_WRITE GROUP_READ WORLD_READ
)


set(CONFIG_PACKAGE_LOCATION "${CMAKE_INSTALL_DATADIR}/${PROJECT_NAME}/cmake")

install(EXPORT Sqlite3Targets
	FILE "Sqlite3Targets.cmake"
	DESTINATION "${CMAKE_INSTALL_DATADIR}/${PROJECT_NAME}/cmake"
)
