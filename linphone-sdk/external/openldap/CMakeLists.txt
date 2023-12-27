############################################################################
# CMakeLists.txt
# Copyright (C) 2010-2023  Belledonne Communications, Grenoble France
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

cmake_minimum_required(VERSION 3.12)

project(OpenLDAP VERSION 2.6.1 LANGUAGES C)

string(REPLACE "." "0" OPENLDAP_VERSION "${PROJECT_VERSION}")
set(OPENLDAP_VERSION_MAJOR ${PROJECT_VERSION_MAJOR})
set(OPENLDAP_VERSION_MINOR ${PROJECT_VERSION_MINOR})
set(OPENLDAP_VERSION_PATCH X)

set(OPENLDAP_SO_VERSION 2.0.200)
set(OPENLDAP_SO_SOVERSION 2)

if(BUILD_SHARED_LIBS)
    if(WIN32)
        set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS TRUE)
    endif()
endif()

if(APPLE)
    set(CMAKE_FIND_ROOT_PATH "/")
endif()

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

set(CMAKE_THREAD_PREFER_PTHREAD TRUE)
set(THREADS_PREFER_PTHREAD_FLAG TRUE)
find_package(Threads REQUIRED)
find_package(MbedTLS REQUIRED)

include(GNUInstallDirs)
include(cmake/CheckFunctionsAndLibs.cmake)

configure_file(include/lber_types.h.in lber_types.h)
configure_file(include/ldap_config.h.in ldap_config.h)
configure_file(include/ldap_features.h.in ldap_features.h)
configure_file(include/portable.h.in portable.h)

set(OPENLDAP_EXTRA_TARGETS )

add_subdirectory(include)
add_subdirectory(libraries)

list(APPEND OPENLDAP_HEADER_FILES
    ${CMAKE_CURRENT_BINARY_DIR}/lber_types.h
    ${CMAKE_CURRENT_BINARY_DIR}/ldap_features.h
)
set_target_properties(ldap PROPERTIES
    PUBLIC_HEADER "${OPENLDAP_HEADER_FILES}"
)

install(TARGETS ldap ${OPENLDAP_EXTRA_TARGETS} EXPORT OpenLDAPTargets
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    FRAMEWORK DESTINATION Frameworks
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/openldap
)

set(CONFIG_PACKAGE_LOCATION "${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}")

install(EXPORT OpenLDAPTargets
    FILE "OpenLDAPTargets.cmake"
    DESTINATION ${CONFIG_PACKAGE_LOCATION}
)

include(CMakePackageConfigHelpers)
configure_package_config_file(${CMAKE_CURRENT_SOURCE_DIR}/cmake/Config.cmake.in
    "${CMAKE_CURRENT_BINARY_DIR}/OpenLDAPConfig.cmake"
    INSTALL_DESTINATION ${CONFIG_PACKAGE_LOCATION}
    NO_SET_AND_CHECK_MACRO
    NO_CHECK_REQUIRED_COMPONENTS_MACRO
)

write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/OpenLDAPConfigVersion.cmake"
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY AnyNewerVersion
)

install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/OpenLDAPConfig.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/OpenLDAPConfigVersion.cmake
    DESTINATION ${CONFIG_PACKAGE_LOCATION}
)
