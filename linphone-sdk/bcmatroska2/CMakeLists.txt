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

cmake_minimum_required(VERSION 3.1)

project(BCMatroska2
	VERSION 0.23
	LANGUAGES C
)

option(CONFIG_EBML_WRITING "Enable EBML file writing support" ON)
option(CONFIG_EBML_UNICODE "Enable Unicode support" ON)
option(CONFIG_DEBUGCHECKS "Enable internal checks" ON)
option(CONFIG_STDIO "Use stdio.h for writing" ON)
option(CONFIG_FILEPOS_64 "Encode filepos_t on 64bits" ON)
option(CONFIG_DEBUG_LEAKS "Enable internal menory leaks checking support" OFF)

include(GNUInstallDirs)

if(NOT CMAKE_INSTALL_RPATH AND CMAKE_INSTALL_PREFIX)
	set(CMAKE_INSTALL_RPATH ${CMAKE_INSTALL_FULL_LIBDIR})
	message(STATUS "Setting install rpath to ${CMAKE_INSTALL_RPATH}")
endif()

configure_file(config.h.cmake ${PROJECT_BINARY_DIR}/bcmatroska2_config.h)

find_package(BCToolbox 5.3.0 REQUIRED)

include_directories(
	${PROJECT_BINARY_DIR}
	corec
)

set(LINK_FLAGS )
if(APPLE)
	list(APPEND LINK_FLAGS "-framework Foundation")
	if(IOS)
		list(APPEND LINK_FLAGS "-framework CoreFoundation")
	endif()
endif()
string(REPLACE ";" " " LINK_FLAGS_STR "${LINK_FLAGS}")

if(WIN32)
	add_compile_definitions(
		"EBML2_EXPORTS"
		"MATROSKA2_EXPORTS"
		"STR_EXPORTS"
	)
endif()
add_compile_options(-w)

add_subdirectory(corec/corec)
add_subdirectory(libebml2)
add_subdirectory(libmatroska2)

install(FILES ${PROJECT_BINARY_DIR}/bcmatroska2_config.h
	DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/corec
)

add_subdirectory(build)

include(CMakePackageConfigHelpers)
set(CMAKE_MODULES_INSTALL_DIR "${CMAKE_INSTALL_DATADIR}/${PROJECT_NAME}/cmake")
configure_package_config_file("${PROJECT_NAME}Config.cmake.in" "${PROJECT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
	INSTALL_DESTINATION "${CMAKE_MODULES_INSTALL_DIR}"
	NO_SET_AND_CHECK_MACRO
)
write_basic_package_version_file("${PROJECT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake"
	VERSION ${PROJECT_VERSION}
	COMPATIBILITY AnyNewerVersion
)
install(FILES
	"${PROJECT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
	"${PROJECT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake"
	DESTINATION ${CMAKE_MODULES_INSTALL_DIR}
)

install(EXPORT ${PROJECT_NAME}Targets
	FILE "${PROJECT_NAME}Targets.cmake"
	DESTINATION ${CMAKE_MODULES_INSTALL_DIR}
)
