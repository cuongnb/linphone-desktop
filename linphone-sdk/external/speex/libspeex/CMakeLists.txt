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

if(WIN32)
	set(LIBS ws2_32)
endif()

if(ENABLE_SPEEX_CODEC)
	set(LIBSPEEX_SOURCE_FILES
		bits.c
		cb_search.c
		exc_10_16_table.c
		exc_10_32_table.c
		exc_20_32_table.c
		exc_5_256_table.c
		exc_5_64_table.c
		exc_8_128_table.c
		filters.c
		gain_table.c
		gain_table_lbr.c
		hexc_10_32_table.c
		hexc_table.c
		high_lsp_tables.c
		lpc.c
		lsp.c
		lsp_tables_nb.c
		ltp.c
		modes.c
		modes_wb.c
		nb_celp.c
		quant_lsp.c
		sb_celp.c
		speex.c
		speex_callbacks.c
		speex_header.c
		stereo.c
		vbr.c
		vq.c
		window.c
	)

	if(MSVC)
		list(APPEND LIBSPEEX_SOURCE_FILES libspeex.def)
	endif()

	add_library(speex ${LIBSPEEX_SOURCE_FILES})
	set_target_properties(speex PROPERTIES VERSION 6)
	target_link_libraries(speex ${LIBS})
	target_include_directories(speex PUBLIC
		$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
		$<BUILD_INTERFACE:${PROJECT_BINARY_DIR}>
		$<INSTALL_INTERFACE:include>
	)
	if(MSVC)
		if(CMAKE_BUILD_TYPE STREQUAL "Debug" OR CMAKE_BUILD_TYPE STREQUAL "RelWithDebInfo")
			install(FILES $<TARGET_PDB_FILE:speex>
				DESTINATION ${CMAKE_INSTALL_BINDIR}
				PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE
			)
		endif()
	endif()
	target_compile_options(speex PRIVATE "-w")
	install(TARGETS speex EXPORT SpeexTargets
		RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
		LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
		ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
		PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE
	)
endif()


if(ENABLE_SPEEX_DSP)
	set(LIBSPEEXDSP_SOURCE_FILES
		buffer.c
		fftwrap.c
		filterbank.c
		jitter.c
		kiss_fft.c
		kiss_fftr.c
		mdf.c
		preprocess.c
		resample.c
		resample_neon.c
		scal.c
		smallft.c
		speexdsp.c
		_kiss_fft_guts.h
		kiss_fft.h
		kiss_fftr.h
	)

	if(MSVC)
		list(APPEND LIBSPEEXDSP_SOURCE_FILES libspeexdsp.def)
	endif()

	add_library(speexdsp ${LIBSPEEXDSP_SOURCE_FILES})
	set_target_properties(speexdsp PROPERTIES VERSION 6)
	target_link_libraries(speexdsp ${LIBS})
	target_include_directories(speexdsp PUBLIC
		$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
		$<BUILD_INTERFACE:${PROJECT_BINARY_DIR}>
		$<INSTALL_INTERFACE:include>
	)
	if(MSVC)
		if(CMAKE_BUILD_TYPE STREQUAL "Debug" OR CMAKE_BUILD_TYPE STREQUAL "RelWithDebInfo")
			install(FILES $<TARGET_PDB_FILE:speexdsp>
				DESTINATION ${CMAKE_INSTALL_BINDIR}
				PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE
			)
		endif()
	endif()
	install(TARGETS speexdsp EXPORT SpeexDspTargets
		RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
		LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
		ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
		PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE
	)
endif()
