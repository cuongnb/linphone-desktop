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
project(GSM C)


set(EXPORT_TARGETS_NAME "GSM")

set(GSM_SOURCE_FILES
	src/add.c
	src/code.c
	src/debug.c
	src/decode.c
	src/long_term.c
	src/lpc.c
	src/preprocess.c
	src/rpe.c
	src/gsm_destroy.c
	src/gsm_decode.c
	src/gsm_encode.c
	src/gsm_explode.c
	src/gsm_implode.c
	src/gsm_create.c
	src/gsm_print.c
	src/gsm_option.c
	src/short_term.c
	src/table.c
)
if(MSVC)
	list(APPEND GSM_SOURCE_FILES gsm.def)
endif(MSVC)

include(GNUInstallDirs)

add_library(gsm ${GSM_SOURCE_FILES})
target_compile_options(gsm PRIVATE "-w")
target_include_directories(gsm INTERFACE
	$<INSTALL_INTERFACE:include>
	$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/inc>
	PRIVATE $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/inc>
)

if(BUILD_SHARED_LIBS AND MSVC)
	if(CMAKE_BUILD_TYPE STREQUAL "Debug" OR CMAKE_BUILD_TYPE STREQUAL "RelWithDebInfo")
		install(FILES $<TARGET_PDB_FILE:gsm>
			DESTINATION ${CMAKE_INSTALL_BINDIR}
			PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE
		)
	endif()
endif()

install(TARGETS gsm EXPORT ${EXPORT_TARGETS_NAME}Targets
	RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
	LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
	ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
	FRAMEWORK DESTINATION Frameworks
	PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE
)


file(GLOB HEADER_FILES "inc/gsm.h")

install(FILES ${HEADER_FILES}
	DESTINATION include/gsm
	PERMISSIONS OWNER_READ OWNER_WRITE GROUP_READ WORLD_READ
)

install(EXPORT ${EXPORT_TARGETS_NAME}Targets
	FILE "${EXPORT_TARGETS_NAME}Targets.cmake"
	DESTINATION "${CMAKE_INSTALL_DATADIR}/${PROJECT_NAME}/cmake"
)
