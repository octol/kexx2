m4_include(m4/ax_cxx_compile_stdcxx_11.m4)
m4_include(m4/acx_pthread.m4)

AC_PREREQ([2.69])
AC_INIT([Kexx2],
        [1.0.6],
        [jon@haeggblad.com],
        [kexx2],
        [http://github.com/octol/kexx2])
AC_CONFIG_AUX_DIR([.])
AC_CONFIG_SRCDIR([README])
AC_CONFIG_MACRO_DIR([m4])
AC_CONFIG_HEADERS([config.h])
AM_INIT_AUTOMAKE
LT_INIT

AC_SUBST([KEXX2_VERSION],[1.0.6])

#-------------------------------------------------------------------------------
# Checks for programs.
#-------------------------------------------------------------------------------
AC_PROG_CC
AC_PROG_CXX

#-------------------------------------------------------------------------------
# Check for C++11
#-------------------------------------------------------------------------------
#AC_LANG([C++])
AC_LANG_PUSH([C++])
AX_CXX_COMPILE_STDCXX_11
AC_LANG_POP([C++])

CXXFLAGS="$CXXFLAGS -pedantic -Wall -W -Wextra"
CXXFLAGS="$CXXFLAGS -Wcast-qual -Wwrite-strings"
#CXXFLAGS="$CXXFLAGS -Wshadow"
#CXXFLAGS="$CXXFLAGS -Weffc++"
#CXXFLAGS="$CXXFLAGS -Wconversion" 
#CXXFLAGS="$CXXFLAGS -Werror"

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
AM_PATH_SDL($SDL_VERSION,:,
	    AC_MSG_ERROR([*** SDL version $SDL_VERSION not found!])
)
CFLAGS="$CFLAGS $SDL_CFLAGS"
CXXFLAGS="$CXXFLAGS $SDL_CFLAGS"
LIBS="$LIBS $SDL_LIBS"

#-------------------------------------------------------------------------------
# Check for SDL_mixer
#-------------------------------------------------------------------------------
have_SDLmixer=no
AC_CHECK_LIB(SDL_mixer, Mix_OpenAudio, have_SDLmixer=yes)
if test x$have_SDLmixer != xyes; then
    AC_MSG_ERROR([*** Can not find the SDL_mixer library
Try: http://www.libsdl.org/projects/SDL_mixer/
])
fi
LIBS="$LIBS -lSDL_mixer"

#-------------------------------------------------------------------------------
# Check for SDL_image
#-------------------------------------------------------------------------------
have_SDLimage=no
AC_CHECK_LIB(SDL_image, IMG_Load_RW, have_SDLimage=yes)
if test x$have_SDLimage != xyes; then
    AC_MSG_ERROR([*** Can not find the SDL_image library
Try: http://www.libsdl.org/projects/SDL_image/
])
fi
LIBS="$LIBS -lSDL_image"

#-------------------------------------------------------------------------------
# Check for SDLc
#-------------------------------------------------------------------------------
PKG_CHECK_MODULES([LIBSDLC], [libsdlc-0.4 >= 0.4])

#-------------------------------------------------------------------------------
# Finally create all the generated files
# The configure script takes "file.in" and substitutes variables to produce
# "file". In this case we are just generating the Makefiles, but this could
# be used to generate any number of automatically generated files.
#-------------------------------------------------------------------------------
AC_CONFIG_FILES([Makefile
                 src/Makefile
                 ])
                 #tests/gtest/Makefile
                 #tests/Makefile
#AC_CONFIG_SUBDIRS([libsdlc])
AC_OUTPUT
