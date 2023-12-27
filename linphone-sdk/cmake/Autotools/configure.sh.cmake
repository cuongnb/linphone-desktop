#!/bin/sh

if [ -n "@AUTOTOOLS_AS_COMPILER@" ]
then
	export AS="@AUTOTOOLS_AS_COMPILER@"
fi

export CC="@AUTOTOOLS_C_COMPILER@"
export CXX="@AUTOTOOLS_CXX_COMPILER@"
export OBJC="@AUTOTOOLS_OBJC_COMPILER@"
export LD="@AUTOTOOLS_LINKER@"
export AR="@AUTOTOOLS_AR@"
export RANLIB="@AUTOTOOLS_RANLIB@"
export STRIP="@AUTOTOOLS_STRIP@"
export NM="@AUTOTOOLS_NM@"
export CC_NO_LAUNCHER="@AUTOTOOLS_C_COMPILER_NO_LAUNCHER@"
export CXX_NO_LAUNCHER="@AUTOTOOLS_CXX_COMPILER_NO_LAUNCHER@"
export OBJC_NO_LAUNCHER="@AUTOTOOLS_OBJC_COMPILER_NO_LAUNCHER@"

export ASFLAGS="@EP_ASFLAGS@"
export CPPFLAGS="@EP_CPPFLAGS@"
export CFLAGS="@EP_CFLAGS@"
export CXXFLAGS="@EP_CXXFLAGS@"
export OBJCFLAGS="@EP_OBJCFLAGS@"
export LDFLAGS="@EP_LDFLAGS@"

export PATH="$PATH:@AUTOTOOLS_PROGRAM_PATH@"

mkdir -p "@EP_BUILD_DIR@"
cd "@EP_BUILD_DIR@"

if [ ! -f "@EP_CONFIG_H_FILE@" ]
then
	@EP_CONFIGURE_ENV@ "@EP_CONFIGURE_COMMAND@" @EP_CROSS_COMPILATION_OPTIONS@ @EP_LINKING_TYPE@ @EP_CONFIGURE_OPTIONS@
fi
