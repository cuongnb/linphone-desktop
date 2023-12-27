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

include(CheckFunctionExists)
include(CheckIncludeFile)
include(CheckSymbolExists)
include(CheckTypeSize)
include(CheckCSourceCompiles)
include(CheckLibraryExists)
include(CheckStructHasMember)
include(CMakePushCheckState)

# INCLUDES
check_include_file(arpa/inet.h HAVE_ARPA_INET_H)
check_include_file(arpa/nameser.h HAVE_ARPA_NAMESER_H)
check_include_file(bits/types.h HAVE_BITS_TYPES_H)
check_include_file(conio.h HAVE_CONIO_H)
check_include_file(crypt.h HAVE_CRYPT_H)
check_include_file(direct.h HAVE_DIRECT_H)
check_include_file(dirent.h HAVE_DIRENT_H)
check_include_file(dlfcn.h HAVE_DLFCN_H)
check_include_file(getopt.h HAVE_GETOPT_H)
check_include_file(grp.h HAVE_GRP_H)
check_include_file(io.h HAVE_IO_H)
check_include_file(malloc.h HAVE_MALLOC_H)
check_include_file(netinet/tcp.h HAVE_NETINET_TCP_H)
check_include_file(pwd.h HAVE_PWD_H)
check_include_file(process.h HAVE_PROCESS_H)
check_include_file(resolv.h HAVE_RESOLV_H)
check_include_file(sched.h HAVE_SCHED_H)
check_include_file(sgtty.h HAVE_SGTTY_H)
check_include_file(shadow.h HAVE_SHADOW_H)
check_include_file(sysexits.h HAVE_SYSEXITS_H)
check_include_file(sys/errno.h HAVE_SYS_ERRNO_H)
check_include_file(sys/event.h HAVE_SYS_EVENT_H)
check_include_file(sys/ioctl.h HAVE_SYS_IOCTL_H)
check_include_file(sys/filio.h HAVE_SYS_FILIO_H)
check_include_file(sys/param.h HAVE_SYS_PARAM_H)
check_include_file(sys/resource.h HAVE_SYS_RESOURCE_H)
check_include_file(sys/select.h HAVE_SYS_SELECT_H)
check_include_file(sys/socket.h HAVE_SYS_SOCKET_H)
check_include_file(sys/syslog.h HAVE_SYS_SYSLOG_H)
check_include_file(sys/time.h HAVE_SYS_TIME_H)
check_include_file(sys/ucred.h HAVE_SYS_UCRED_H)
check_include_file(sys/uio.h HAVE_SYS_UIO_H)
check_include_file(sys/un.h HAVE_SYS_UN_H)
check_include_file(syslog.h HAVE_SYSLOG_H)
check_include_file(termios.h HAVE_TERMIOS_H)
check_include_file(unistd.h HAVE_UNISTD_H)

check_include_file(sys/epoll.h HAVE_SYS_EPOLL_H)
if(HAVE_SYS_EPOLL_H)
    check_function_exists(epoll_create HAVE_EPOLL)
endif()

check_include_file(uuid/uuid.h HAVE_UUID_UUID_H)
if(HAVE_UUID_UUID_H)
    if(APPLE)
        check_function_exists(uuid_generate HAVE_UUID_GENERATE)
    else()
        check_library_exists(uuid uuid_generate "" HAVE_UUID_GENERATE)
    endif()
endif()

if(WIN32)
    check_include_file(winsock.h HAVE_WINSOCK_H)
    if(HAVE_WINSOCK_H)
        set(HAVE_WINSOCK 1)
    endif()
    check_include_file(winsock2.h HAVE_WINSOCK2_H)
    if(HAVE_WINSOCK2_H)
        set(HAVE_WINSOCK2 1)
    endif()
    check_include_file(wspiapi.h HAVE_WSPIAPI_H)
endif()

# FUNCTIONS
check_function_exists(_spawnlp HAVE_SPAWNLP)
check_function_exists(bcopy HAVE_BCOPY)
check_function_exists(chroot HAVE_CHROOT)
check_function_exists(ctime_r HAVE_CTIME_R)
check_function_exists(endgrent HAVE_ENDGRENT)
check_function_exists(endpwent HAVE_ENDPWENT)
check_function_exists(fcntl HAVE_FCNTL)
check_function_exists(flock HAVE_FLOCK)
check_function_exists(fmemopen HAVE_FMEMOPEN)
check_function_exists(gai_strerror HAVE_GAI_STRERROR)
check_function_exists(getdtablesize HAVE_GETDTABLESIZE)
check_function_exists(geteuid HAVE_GETEUID)
check_function_exists(getgrgid HAVE_GETGRGID)
check_function_exists(gethostbyaddr_r HAVE_GETHOSTBYADDR_R)
check_function_exists(gethostbyname_r HAVE_GETHOSTBYNAME_R)
check_function_exists(getpeereid HAVE_GETPEEREID)
check_function_exists(getpwuid HAVE_GETPWUID)
check_function_exists(getpwnam HAVE_GETPWNAM)
check_function_exists(getspnam HAVE_GETSPNAM)
check_function_exists(gmtime_r HAVE_GMTIME_R)
check_function_exists(hstrerror HAVE_HSTRERROR)
check_function_exists(initgroups HAVE_INITGROUPS)
check_function_exists(ioctl HAVE_IOCTL)
check_function_exists(kqueue HAVE_KQUEUE)
check_function_exists(localtime_r HAVE_LOCALTIME_R)
check_function_exists(lockf HAVE_LOCKF)
check_function_exists(memrchr HAVE_MEMRCHR)
check_function_exists(pipe HAVE_PIPE)
check_function_exists(res_query HAVE_RES_QUERY)
check_function_exists(sched_yield HAVE_SCHED_YIELD)
check_function_exists(sendmsg HAVE_SENDMSG)
check_function_exists(setgid HAVE_SETGID)
check_function_exists(setegid HAVE_SETEGID)
check_function_exists(setsid HAVE_SETSID)
check_function_exists(setuid HAVE_SETUID)
check_function_exists(seteuid HAVE_SETEUID)
check_function_exists(sigaction HAVE_SIGACTION)
check_function_exists(sigset HAVE_SIGSET)
check_function_exists(strerror_r HAVE_STRERROR_R)
check_function_exists(strsep HAVE_STRSEP)
check_function_exists(strtoq HAVE_STRTOQ)
check_function_exists(strtouq HAVE_STRTOUQ)
check_function_exists(sysconf HAVE_SYSCONF)
check_function_exists(waitpid HAVE_WAITPID)
check_function_exists(wait4 HAVE_WAIT4)

check_function_exists(poll HAVE_POLL)
if(HAVE_POLL)
    check_include_file(poll.h HAVE_POLL_H)
    check_include_file(sys/poll.h HAVE_SYS_POLL_H)
endif()

if(NOT WIN32)
    check_function_exists(getaddrinfo HAVE_GETADDRINFO)
    check_function_exists(getnameinfo HAVE_GETNAMEINFO)
    check_function_exists(inet_ntop HAVE_INET_NTOP)
    check_function_exists(recv HAVE_RECV)
    check_function_exists(recvfrom HAVE_RECVFROM)
    check_function_exists(send HAVE_SEND)
    check_function_exists(sendto HAVE_SENDTO)
endif()

# SYMBOLS
check_symbol_exists(inet_aton "arpa/inet.h" HAVE_INET_ATON)

check_symbol_exists(snprintf "stdio.h" HAVE_SNPRINTF)
if(NOT HAVE_SNPRINTF)
    check_symbol_exists(_snprintf "stdio.h" HAVE__SNPRINTF)
    if(HAVE__SNPRINTF)
        set(snprintf "_snprintf")
    endif()
endif()

check_symbol_exists(vsnprintf "stdio.h;stdarg.h" HAVE_VSNPRINTF)
check_symbol_exists(_vsnprintf "stdio.h" HAVE__VSNPRINTF)
if(NOT HAVE_VSNPRINTF AND HAVE__VSNPRINTF)
    set(vsnprintf "_vsnprintf")
endif()

check_symbol_exists(TIOCGWINSZ "termios.h" GWINSZ_IN_TERMIOS)
if(NOT GWINSZ_IN_TERMIOS)
    check_symbol_exists(TIOCGWINSZ "sys/ioctl.h" GWINSZ_IN_SYS_IOCTL)
endif()

if(WIN32)
    cmake_push_check_state()
    set(CMAKE_REQUIRED_LIBRARIES ws2_32)
    check_symbol_exists(closesocket "winsock2.h;ws2tcpip.h" HAVE_CLOSESOCKET)
    check_symbol_exists(getaddrinfo "winsock2.h;ws2tcpip.h" HAVE_GETADDRINFO)
    check_symbol_exists(getnameinfo "winsock2.h;ws2tcpip.h" HAVE_GETNAMEINFO)
    check_symbol_exists(inet_ntop "winsock2.h;ws2tcpip.h" HAVE_INET_NTOP)
    check_symbol_exists(recv "winsock.h" HAVE_RECV)
    check_symbol_exists(recvfrom "winsock2.h" HAVE_RECVFROM)
    check_symbol_exists(send "winsock2.h" HAVE_SEND)
    check_symbol_exists(sendto "winsock.h" HAVE_SENDTO)
    cmake_pop_check_state()
endif()

# MEMBERS
check_struct_has_member("struct msghdr" "msg_control" "sys/types.h;sys/socket.h" HAVE_STRUCT_MSGHDR_MSG_CONTROL)
check_struct_has_member("struct passwd" "pw_gecos" "pwd.h" HAVE_STRUCT_PASSWD_PW_GECOS)
check_struct_has_member("struct passwd" "pw_passwd" "pwd.h" HAVE_STRUCT_PASSWD_PW_PASSWD)
check_struct_has_member("struct stat" "st_blksize" "sys/type.h;sys/stat.h" HAVE_STRUCT_STAT_ST_BLKSIZE)

# COMPILES
check_c_source_compiles(
    "#include <stdio.h>\n
    #include <sys/types.h>\n
    #include <errno.h>\n
    #ifdef _WIN32\n
    #include <stdlib.h>\n
    #endif\n
    int main(void) {\n
        char *c = (char *) *sys_errlist;\n
        return 0;\n
    }"
    HAVE_SYS_ERRLIST
)
if(NOT HAVE_SYS_ERRLIST)
    set(DECL_SYS_ERRLIST 1)
endif()

# TYPE SIZE
check_type_size(int SIZEOF_INT)
check_type_size(long SIZEOF_LONG)
check_type_size("long long" SIZEOF_LONG_LONG)
check_type_size(short SIZEOF_SHORT)
check_type_size(wchar_t SIZEOF_WCHAR_T)

if(SIZEOF_INT LESS 4)
    set(LBER_INT_T "long")
else()
    set(LBER_INT_T "int")
endif()

check_type_size(gid_t SIZEOF_GID_T)
if(NOT HAVE_SIZEOF_GID_T)
    set(gid_t "int")
endif()

check_type_size(uid_t SIZEOF_UID_T)
if(NOT HAVE_SIZEOF_UID_T)
    set(uid_t "int")
endif()

check_type_size(caddr_t SIZEOF_CADDR_T)
if(NOT HAVE_SIZEOF_CADDR_T)
    set(caddr_t "char *")
endif()

cmake_push_check_state()
if(WIN32)
    set(CMAKE_EXTRA_INCLUDE_FILES ws2tcpip.h)
else()
    set(CMAKE_EXTRA_INCLUDE_FILES sys/socket.h)
endif()

check_type_size(socklen_t SOCKLEN_T)

if(NOT HAVE_SOCKLEN_T)
    set(socklen_t "int")
endif()

cmake_pop_check_state()

# OTHER
if(Threads_FOUND)
    if(CMAKE_USE_WIN32_THREADS_INIT)
        set(HAVE_NT_EVENT_LOG 1)
        set(HAVE_NT_SERVICE_MANAGER 1)
        set(HAVE_NT_THREADS 1)
    else()
        set(REENTRANT 1)
        set(_REENTRANT 1)
        set(THREAD_SAFE 1)
        set(_THREAD_SAFE 1)
        set(THREADSAFE 1)
        set(_THREADSAFE 1)
        set(_SGI_MP_SOURCE 1)

        if(CMAKE_USE_PTHREADS_INIT)
            set(HAVE_PTHREADS 10)

            cmake_push_check_state()
            set(CMAKE_REQUIRED_LIBRARIES Threads::Threads)
            check_include_file(pthread.h HAVE_PTHREAD_H)
            check_symbol_exists(pthread_detach "pthread.h" HAVE_PTHREAD_DETACH)
            check_symbol_exists(pthread_getconcurrency "pthread.h" HAVE_PTHREAD_GETCONCURRENCY)
            check_symbol_exists(pthread_kill "pthread.h" HAVE_PTHREAD_KILL)
            check_symbol_exists(pthread_rwlock_destroy "pthread.h" HAVE_PTHREAD_RWLOCK_DESTROY)
            check_symbol_exists(pthread_setconcurrency "pthread.h" HAVE_PTHREAD_SETCONCURRENCY)
            check_symbol_exists(pthread_yield "pthread.h" HAVE_PTHREAD_YIELD)
            cmake_pop_check_state()
        endif()
    endif()
endif()

set(EXEEXT "")
if(WIN32)
   set(EXEEXT ".exe")
endif()

if(NOT WIN32)
    set(CTIME_R_NARGS 2)
    set(GETHOSTBYADDR_R_NARGS 8)
    set(GETHOSTBYNAME_R_NARGS 6)
    set(SELECT_TYPE_ARG1 "int")
    set(SELECT_TYPE_ARG234 "(fd_set *)")
    set(SELECT_TYPE_ARG5 "(struct timeval *)")
    set(URANDOM_DEVICE "/dev/urandom")
    set(LDAP_PF_INET6 1)
    set(LDAP_PF_LOCAL 1)
    set(LDAP_SYSLOG 1)
endif()