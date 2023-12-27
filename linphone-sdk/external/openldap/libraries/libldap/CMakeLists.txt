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

set(LDAP_SOURCE_FILES
    abandon.c
    account_usability.c
    add.c
    addentry.c
    assertion.c
    avl.c
    bind.c
    cancel.c
    charray.c
    compare.c
    controls.c
    cyrus.c
    dds.c
    delete.c
    deref.c
    dnssrv.c
    error.c
    extended.c
    fetch.c
    filter.c
    free.c
    getattr.c
    getdn.c
    getentry.c
    getvalues.c
    init.c
    lbase64.c
    ldap_sync.c
    ldif.c
    ldifutil.c
    messages.c
    modify.c
    modrdn.c
    msctrl.c
    open.c
    options.c
    os-ip.c
    os-local.c
    pagectrl.c
    passwd.c
    ppolicy.c
    print.c
    psearchctrl.c
    rdwr.c
    references.c
    request.c
    result.c
    rq.c
    sasl.c
    sbind.c
    schema.c
    search.c
    sort.c
    sortctrl.c
    stctrl.c
    string.c
    tavl.c
    thr_debug.c
    thr_nt.c
    thr_posix.c
    thr_pth.c
    thr_thr.c
    threads.c
    tls2.c
    tls_g.c
    tls_m.c
    tls_o.c
    tpool.c
    turn.c
    txn.c
    unbind.c
    url.c
    utf-8.c
    utf-8-conv.c
    util-int.c
    vc.c
    vlvctrl.c
    whoami.c
)

add_library(ldap ${LDAP_SOURCE_FILES})
target_link_libraries(ldap
    PRIVATE Threads::Threads ${MbedTLS_TARGET}
    PUBLIC ${OPENLDAP_EXTRA_TARGETS}
)

if(APPLE)
    target_link_libraries(ldap PRIVATE resolv)
elseif(WIN32)
    target_link_libraries(ldap PRIVATE ws2_32)
endif()

if(BUILD_SHARED_LIBS)
    if(NOT WIN32)
        set_target_properties(ldap PROPERTIES VERSION ${OPENLDAP_SO_VERSION} SOVERSION ${OPENLDAP_SO_SOVERSION})
    endif()

    add_compile_definitions(LDAP_LIBS_DYNAMIC)
endif()

target_include_directories(ldap
    PUBLIC
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
    PRIVATE
        .
        ${PROJECT_BINARY_DIR}
        ${MBEDTLS_INCLUDE_DIRS}
)

add_compile_definitions(LDAP_LIBRARY)
add_compile_definitions(LDAP_R_COMPILE)
