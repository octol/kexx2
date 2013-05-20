m4_include(m4/ax_cxx_compile_stdcxx_11.m4)
m4_include(m4/acx_pthread.m4)
m4_include(m4/boost.m4)

AC_PREREQ([2.69])
AC_INIT([Kexx2],[1.0.9],[jon@haeggblad.com],[kexx2],[http://www.haeggblad.com/kexx2])
AC_CONFIG_AUX_DIR([.])
AC_CONFIG_SRCDIR([README])
AC_CONFIG_MACRO_DIR([m4])
AC_CONFIG_HEADERS([config.h])
AM_INIT_AUTOMAKE
LT_INIT

AC_SUBST([KEXX2_VERSION],[1.0.9])

DEBUG_CXXFLAGS="-g -Og -std=gnu++11 \
-Wall -W -Wextra -Wpedantic -Wcast-qual -Wwrite-strings -Wconversion \
-fno-omit-frame-pointer -fno-inline -fdiagnostics-color=auto"
#DEBUG_CXXFLAGS="$DEBUG_CXXFLAGS -Weffc++"

#-------------------------------------------------------------------------------
# Checks for programs.
#-------------------------------------------------------------------------------
AC_PROG_CC
AC_PROG_CXX

#-------------------------------------------------------------------------------
# Check for C++11
#-------------------------------------------------------------------------------
AC_LANG_PUSH([C++])
AX_CXX_COMPILE_STDCXX_11
AC_LANG_POP([C++])

BOOST_REQUIRE([1.49])
BOOST_STRING_ALGO
BOOST_PROGRAM_OPTIONS
BOOST_FILESYSTEM

#-------------------------------------------------------------------------------
# Configure pthreads.
#-------------------------------------------------------------------------------
AC_ARG_WITH([pthreads],
            [AS_HELP_STRING([--with-pthreads],
               [use pthreads (default is yes)])],
            [with_pthreads=$withval],
            [with_pthreads=check])

have_pthreads=no
AS_IF([test "x$with_pthreads" != "xno"],
      [ACX_PTHREAD(
        [],
        [AS_IF([test "x$with_pthreads" != "xcheck"],
               [AC_MSG_FAILURE(
                 [--with-pthreads was specified, but unable to be used])])])
       have_pthreads="$acx_pthread_ok"])
AM_CONDITIONAL([HAVE_PTHREADS],[test "x$have_pthreads" == "xyes"])
AC_SUBST(PTHREAD_CFLAGS)
AC_SUBST(PTHREAD_LIBS)

#-------------------------------------------------------------------------------
# Check for SDL
#-------------------------------------------------------------------------------
SDL_VERSION=1.2.0
PKG_CHECK_MODULES(SDL,sdl >= $SDL_VERSION, [], 
                  AC_MSG_ERROR([SDL library >= $SDL_VERSION not found!]))

PKG_CHECK_MODULES(SDL_mixer, SDL_mixer >= $SDL_VERSION ,[],
	          AC_MSG_ERROR([SDL_mixer library >= $SDL_VERSION not found]))

PKG_CHECK_MODULES(SDL_image, SDL_image >= $SDL_VERSION ,[],
	          AC_MSG_ERROR([SDL_image library >= $SDL_VERSION not found]))

#-------------------------------------------------------------------------------
# Check for SDLc
#-------------------------------------------------------------------------------
PKG_CHECK_MODULES([LIBSDLC], [libsdlc-0.5.2 >= 0.5.2])

#-------------------------------------------------------------------------------
# Debug build switch
# from http://www.bioinf.uni-freiburg.de/~mmann/HowTo/automake.html#debug
#-------------------------------------------------------------------------------
AC_MSG_CHECKING([whether to enable the debug build])
AC_ARG_ENABLE([debug],
    [AS_HELP_STRING([--enable-debug],
        [enable debug build (def=no)])],
    [enable_debug="$enableval"],
    [enable_debug=no])
AC_MSG_RESULT([$enable_debug])

if test x"$enable_debug" = x"yes"; then
    AC_DEFINE([DEBUG],[],[Debug Mode])
    CXXFLAGS="$DEBUG_CXXFLAGS"
else
    AC_DEFINE([NDEBUG],[],[Release Mode])
fi

#-------------------------------------------------------------------------------
# Finally create all the generated files
# The configure script takes "file.in" and substitutes variables to produce
# "file". In this case we are just generating the Makefiles, but this could
# be used to generate any number of automatically generated files.
#-------------------------------------------------------------------------------
AC_CONFIG_FILES([Makefile
                 src/Makefile])
AC_OUTPUT
