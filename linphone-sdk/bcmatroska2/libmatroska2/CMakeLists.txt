############################################################################
# CMakeLists.txt
# Copyright (C) 2016-2023  Belledonne Communications, Grenoble France
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

set(MATROSKA2_SOURCE_FILES
	matroskamain.c
	matroskablock.c
	matroska_sem.c
)

set(LIBS )
if(WIN32 AND NOT CMAKE_SYSTEM_NAME STREQUAL "WindowsPhone" AND NOT CMAKE_SYSTEM_NAME STREQUAL "WindowsStore")
	list(APPEND LIBS Winmm)
endif()

string(REPLACE ";" " " LINK_FLAGS_STR "${LINK_FLAGS}")
add_library(bcmatroska2 $<TARGET_OBJECTS:bcmatroska2-corec-objects> $<TARGET_OBJECTS:bcmatroska2-ebml-objects> ${MATROSKA2_SOURCE_FILES})
set_target_properties(bcmatroska2 PROPERTIES VERSION 0.0.0 SOVERSION 0)
set_target_properties(bcmatroska2 PROPERTIES POSITION_INDEPENDENT_CODE TRUE)
if(NOT "${LINK_FLAGS_STR}" STREQUAL "")
	set_target_properties(bcmatroska2 PROPERTIES LINK_FLAGS "${LINK_FLAGS_STR}")
endif()
target_include_directories(bcmatroska2 PRIVATE ../libebml2 .)
target_include_directories(bcmatroska2 INTERFACE
	$<INSTALL_INTERFACE:include/>
	$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/libmatroska2>
	$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/libebml2>
	$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/corec>
	$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}>
	$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/corec/tools/coremake>
	$<BUILD_INTERFACE:${PROJECT_BINARY_DIR}>
	PRIVATE ../libebml2 .
)
target_link_libraries(bcmatroska2 PUBLIC ${BCToolbox_TARGET})
if(LIBS)
	target_link_libraries(bcmatroska2 PRIVATE ${LIBS})
endif()

if(MSVC AND BUILD_SHARED_LIBS)
	if(CMAKE_BUILD_TYPE STREQUAL "Debug" OR CMAKE_BUILD_TYPE STREQUAL "RelWithDebInfo")
		install(FILES $<TARGET_PDB_FILE:bcmatroska2>
			DESTINATION ${CMAKE_INSTALL_BINDIR}
			PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE
		)
	endif()
endif()

install(TARGETS bcmatroska2 EXPORT ${PROJECT_NAME}Targets
	RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
	LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
	ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
	PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE
)

install(DIRECTORY matroska
	DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)
