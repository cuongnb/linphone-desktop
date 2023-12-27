############################################################################
# CMakeLists.txt
# Copyright (C) 2010-2021  Belledonne Communications, Grenoble France
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

set(LBER_SOURCE_FILES
    assert.c
    bprint.c
    debug.c
    decode.c
    encode.c
    io.c
    memory.c
    options.c
    sockbuf.c
)

if(WIN32)
    list(APPEND LBER_SOURCE_FILES nt_err.c)
else()
    list(APPEND LBER_SOURCE_FILES stdio.c)
endif()

add_library(lber ${LBER_SOURCE_FILES})

if(WIN32)
    target_link_libraries(lber PRIVATE ws2_32)
endif()

if(BUILD_SHARED_LIBS)
    if(NOT WIN32)
        set_target_properties(lber PROPERTIES VERSION ${OPENLDAP_SO_VERSION} SOVERSION ${OPENLDAP_SO_SOVERSION})
    endif()

    add_compile_definitions(LDAP_LIBS_DYNAMIC)
endif()

target_include_directories(lber
    PUBLIC
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
        $<BUILD_INTERFACE:${PROJECT_BINARY_DIR}>
        $<INSTALL_INTERFACE:include>
    PRIVATE
        .
)

add_compile_definitions(LBER_LIBRARY)

list(APPEND OPENLDAP_EXTRA_TARGETS lber)
set(OPENLDAP_EXTRA_TARGETS ${OPENLDAP_EXTRA_TARGETS} PARENT_SCOPE)
