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
project(XML2 VERSION "2.9.10" LANGUAGES C)


set(LIBXML_MAJOR_VERSION ${PROJECT_VERSION_MAJOR})
set(LIBXML_MINOR_VERSION ${PROJECT_VERSION_MINOR})
set(LIBXML_MICRO_VERSION ${PROJECT_VERSION_PATCH})
set(VERSION "${LIBXML_MAJOR_VERSION}.${LIBXML_MINOR_VERSION}.${LIBXML_MICRO_VERSION}")
set(LIBXML_VERSION_NUMBER 208000)


include(CMakeDependentOption)

option(ENABLE_C14N "Add the Canonicalization support." NO)
option(ENABLE_CATALOG "Add the Catalog support." NO)
option(ENABLE_DEBUG "Add the debugging module." NO)
option(ENABLE_DOCBOOK "Add Docbook SGML support." NO)
option(ENABLE_FEXCEPTIONS "Add GCC flag -fexceptions for C++ exceptions." NO)
option(ENABLE_FTP "Add the FTP support." NO)
option(ENABLE_HISTORY "Add history support to xmllint shell" NO)
option(ENABLE_HTML "Add the HTML support." NO)
option(ENABLE_HTTP "Add the HTTP support." NO)
option(ENABLE_ICONV "Add ICONV support." NO)
option(ENABLE_ICU "Add ICU support." NO)
option(ENABLE_ISO8859X "Add ISO8859X support if no iconv." NO)
option(ENABLE_LEGACY "Add deprecated APIs for compatibility." NO)
option(ENABLE_MEM_DEBUG "Add the memory debugging module." NO)
option(ENABLE_OUTPUT "Add the serialization support." YES)
option(ENABLE_PATTERN "Add the xmlPattern selection interface." YES)
option(ENABLE_PUSH "Add the PUSH parser interfaces." YES)
cmake_dependent_option(ENABLE_READER "Add the xmlReader parsing interface." YES "ENABLE_PUSH" NO)
option(ENABLE_REGEXPS "Add Regular Expressions support." YES)
option(ENABLE_RUN_DEBUG "Add the runtime debugging module." NO)
option(ENABLE_SAX1 "Add the older SAX1 interface." YES)
cmake_dependent_option(ENABLE_SCHEMAS "Add Relax-NG and Schemas support." YES "ENABLE_PATTERN;ENABLE_REGEXPS" NO)
cmake_dependent_option(ENABLE_SCHEMATRON "Add Schematron support." NO "ENABLE_PATTERN;ENABLE_XPATH" NO)
option(ENABLE_THREADS "Add multithread support." NO)
option(ENABLE_THREAD_ALLOC "Add per-thread memory." NO)
option(ENABLE_TREE "Add the DOM like tree manipulation APIs." YES)
option(ENABLE_VALID "Add the DTD validation support." YES)
cmake_dependent_option(ENABLE_WRITER "Add the xmlWriter saving interface." YES "ENABLE_PUSH;ENABLE_OUTPUT" NO)
option(ENABLE_XINCLUDE "Add the XInclude support." NO)
option(ENABLE_XPATH "Add the XPATH support." YES)
cmake_dependent_option(ENABLE_XPTR "Add the XPointer support." NO "ENABLE_XPATH" NO)
option(ENABLE_MODULES "Add the dynamic modules support." NO)


list(APPEND CMAKE_MODULE_PATH ${CMAKE_PREFIX_PATH}/share/cmake/Modules)


include(CheckIncludeFile)
include(CheckSymbolExists)

check_include_file(inttypes.h HAVE_INTTYPES_H)
check_include_file(memory.h HAVE_MEMORY_H)
check_include_file(stdint.h HAVE_STDINT_H)
check_include_file(stdlib.h HAVE_STDLIB_H)
check_include_file(strings.h HAVE_STRINGS_H)
check_include_file(string.h HAVE_STRING_H)
check_include_file(sys/stat.h HAVE_SYS_STAT_H)
check_include_file(sys/types.h HAVE_SYS_TYPES_H)
check_include_file(poll.h HAVE_POLL_H)
check_include_file(sys/poll.h HAVE_SYS_POLL_H)
check_include_file(sys/uio.h HAVE_SYS_UIO_H)
check_include_file(fcntl.h HAVE_FCNTL_H)
check_include_file(sys/time.h HAVE_SYS_TIME_H)
check_include_file(unistd.h HAVE_UNISTD_H)
check_include_file(sys/shm.h HAVE_SYS_SHM_H)
check_include_file(windows.h HAVE_WINDOWS_H)
check_include_file(dlfcn.h HAVE_DLFCN_H)
check_include_file(alloca.h HAVE_ALLOCA_H)
check_include_file(ctype.h HAVE_CTYPE_H)
check_include_file(dirent.h HAVE_DIRENT_H)
check_include_file(errno.h HAVE_ERRNO_H)
check_include_file(malloc.h HAVE_MALLOC_H)
check_include_file(stdarg.h HAVE_STDARG_H)
check_include_file(time.h HAVE_TIME_H)
check_include_file(ansidecl.h HAVE_ANSIDECL_H)
check_include_file(ieeefp.h HAVE_IEEEFP_H)
check_include_file(nan.h HAVE_NAN_H)
check_include_file(math.h HAVE_MATH_H)
check_include_file(limits.h HAVE_LIMITS_H)
check_include_file(fp_class.h HAVE_FP_CLASS_H)
check_include_file(float.h HAVE_FLOAT_H)
check_include_file(sys/socket.h HAVE_SYS_SOCKET_H)
check_include_file(netinet/in.h HAVE_NETINET_IN_H)
check_include_file(arpa/inet.h HAVE_ARPA_INET_H)
check_include_file(netdb.h HAVE_NETDB_H)
check_include_file(sys/select.h HAVE_SYS_SELECT_H)
check_include_file(sys/mman.h HAVE_SYS_MMAN_H)
check_include_file(sys/timeb.h HAVE_SYS_TIMEB_H)
check_include_file(signal.h HAVE_SIGNAL_H)
check_include_file(arpa/nameser.h HAVE_ARPA_NAMESER_H)
check_include_file(resolv.h HAVE_RESOLV_H)
check_include_file(dl.h HAVE_DL_H)
if(HAVE_STDARG_H)
	check_symbol_exists(va_copy stdarg.h HAVE_VA_COPY)
	if(NOT HAVE_VA_COPY)
		check_symbol_exists(__va_copy stdarg.h HAVE___VA_COPY)
	endif()
endif()

configure_file(${PROJECT_SOURCE_DIR}/config.h.cmake ${PROJECT_BINARY_DIR}/config.h)
set_source_files_properties(${PROJECT_BINARY_DIR}/config.h PROPERTIES GENERATED ON)
add_compile_definitions("HAVE_CONFIG_H")

set(WITH_TRIO 0)
if(ENABLE_THREADS)
	set(WITH_THREADS 1)
else()
	set(WITH_THREADS 0)
endif()
if(ENABLE_THREAD_ALLOC)
	set(WITH_THREAD_ALLOC 1)
else()
	set(WITH_THREAD_ALLOC 0)
endif()
if(ENABLE_TREE)
	set(WITH_TREE 1)
else()
	set(WITH_TREE 0)
endif()
if(ENABLE_OUTPUT)
	set(WITH_OUTPUT 1)
else()
	set(WITH_OUTPUT 0)
endif()
if(ENABLE_PUSH)
	set(WITH_PUSH 1)
else()
	set(WITH_PUSH 0)
endif()
if(ENABLE_READER)
	set(WITH_READER 1)
else()
	set(WITH_READER 0)
endif()
if(ENABLE_PATTERN)
	set(WITH_PATTERN 1)
else()
	set(WITH_PATTERN 0)
endif()
if(ENABLE_WRITER)
	set(WITH_WRITER 1)
else()
	set(WITH_WRITER 0)
endif()
if(ENABLE_SAX1)
	set(WITH_SAX1 1)
else()
	set(WITH_SAX1 0)
endif()
if(ENABLE_FTP)
	set(WITH_FTP 1)
else()
	set(WITH_FTP 0)
endif()
if(ENABLE_HTTP)
	set(WITH_HTTP 1)
else()
	set(WITH_HTTP 0)
endif()
if(ENABLE_VALID)
	set(WITH_VALID 1)
else()
	set(WITH_VALID 0)
endif()
if(ENABLE_HTML)
	set(WITH_HTML 1)
else()
	set(WITH_HTML 0)
endif()
if(ENABLE_LEGACY)
	set(WITH_LEGACY 1)
else()
	set(WITH_LEGACY 0)
endif()
if(ENABLE_C14N)
	set(WITH_C14N 1)
else()
	set(WITH_C14N 0)
endif()
if(ENABLE_CATALOG)
	set(WITH_CATALOG 1)
else()
	set(WITH_CATALOG 0)
endif()
if(ENABLE_DOCBOOK)
	set(WITH_DOCB 1)
else()
	set(WITH_DOCB 0)
endif()
if(ENABLE_XPATH)
	set(WITH_XPATH 1)
else()
	set(WITH_XPATH 0)
endif()
if(ENABLE_XPTR)
	set(WITH_XPTR 1)
else()
	set(WITH_XPTR 0)
endif()
if(ENABLE_XINCLUDE)
	set(WITH_XINCLUDE 1)
else()
	set(WITH_XINCLUDE 0)
endif()
if(ENABLE_ICONV)
	set(WITH_ICONV 1)
else()
	set(WITH_ICONV 0)
endif()
if(ENABLE_ICU)
	set(WITH_ICU 1)
else()
	set(WITH_ICU 0)
endif()
if(ENABLE_ISO8859X)
	set(WITH_ISO8859X 1)
else()
	set(WITH_ISO8859X 0)
endif()
if(ENABLE_DEBUG)
	set(WITH_DEBUG 1)
else()
	set(WITH_DEBUG 0)
endif()
if(ENABLE_MEM_DEBUG)
	set(WITH_MEM_DEBUG 1)
else()
	set(WITH_MEM_DEBUG 0)
endif()
if(ENABLE_RUN_DEBUG)
	set(WITH_RUN_DEBUG 1)
else()
	set(WITH_RUN_DEBUG 0)
endif()
if(ENABLE_REGEXPS)
	set(WITH_REGEXPS 1)
else()
	set(WITH_REGEXPS 0)
endif()
if(ENABLE_SCHEMAS)
	set(WITH_SCHEMAS 1)
else()
	set(WITH_SCHEMAS 0)
endif()
if(ENABLE_SCHEMATRON)
	set(WITH_SCHEMATRON 1)
else()
	set(WITH_SCHEMATRON 0)
endif()
if(ENABLE_MODULES)
	set(WITH_MODULES 1)
else()
	set(WITH_MODULES 0)
endif()
set(WITH_ZLIB 0)
set(WITH_LZMA 0)

configure_file(${PROJECT_SOURCE_DIR}/include/libxml/xmlversion.h.in ${PROJECT_BINARY_DIR}/libxml/xmlversion.h @ONLY)
set_source_files_properties(${PROJECT_BINARY_DIR}/libxml/xmlversion.h PROPERTIES GENERATED ON)

include_directories(
	include
	${PROJECT_BINARY_DIR}
)


set(SOURCE_FILES
	buf.c
	c14n.c
	catalog.c
	chvalid.c
	debugXML.c
	dict.c
	DOCBparser.c
	encoding.c
	entities.c
	error.c
	globals.c
	hash.c
	HTMLparser.c
	HTMLtree.c
	legacy.c
	list.c
	nanoftp.c
	nanohttp.c
	parser.c
	parserInternals.c
	pattern.c
	relaxng.c
	SAX2.c
	SAX.c
	schematron.c
	threads.c
	tree.c
	uri.c
	valid.c
	xinclude.c
	xlink.c
	xmlIO.c
	xmlmemory.c
	xmlmodule.c
	xmlreader.c
	xmlregexp.c
	xmlsave.c
	xmlschemas.c
	xmlschemastypes.c
	xmlstring.c
	xmlunicode.c
	xmlwriter.c
	xpath.c
	xpointer.c
	xzlib.c
)

add_library(xml2 ${SOURCE_FILES})
set_target_properties(xml2 PROPERTIES VERSION 2 LINKER_LANGUAGE C)
target_include_directories(xml2 INTERFACE
	$<INSTALL_INTERFACE:include>
	$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
	$<BUILD_INTERFACE:${PROJECT_BINARY_DIR}>
)

if(MSVC)
	if(MSVC_VERSION LESS "1900")	# VS 14.0
		target_compile_definitions(xml2 PRIVATE "snprintf=_snprintf")
	endif()
else()
	target_link_libraries(xml2 m)
	if(NOT APPLE)
		set_target_properties(xml2 PROPERTIES LINK_FLAGS "-Wl,--version-script=${PROJECT_SOURCE_DIR}/libxml2.syms")
	endif()
endif()

if(CMAKE_SYSTEM_NAME STREQUAL "WindowsStore")
	target_compile_options(xml2 PRIVATE "/FIuniversal_windows_port.h")
endif()

include(GNUInstallDirs)

install(TARGETS xml2 EXPORT XML2Targets
	RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
	LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
	ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
	PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE
)

file(GLOB HEADER_FILES "include/libxml/*.h")

install(FILES ${HEADER_FILES} ${PROJECT_BINARY_DIR}/libxml/xmlversion.h
	DESTINATION include/libxml2/libxml
	PERMISSIONS OWNER_READ OWNER_WRITE GROUP_READ WORLD_READ
)

if(MSVC AND BUILD_SHARED_LIBS)
	if(CMAKE_BUILD_TYPE STREQUAL "Debug" OR CMAKE_BUILD_TYPE STREQUAL "RelWithDebInfo")
		install(FILES $<TARGET_PDB_FILE:xml2>
			DESTINATION ${CMAKE_INSTALL_BINDIR}
			PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE
		)
	endif()
endif()

install(EXPORT XML2Targets
	FILE "XML2Targets.cmake"
	DESTINATION "${CMAKE_INSTALL_DATADIR}/${PROJECT_NAME}/cmake"
)
