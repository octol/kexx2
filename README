README
======

Kexx2 is a top down scrolling shoot'em up, similar to Raptor and Tyrian.

The project web page is at: http://www.haeggblad.com/kexx2
and the source code is hosted at: http://github.com/octol/kexx2

DISCLAIMER: Most of the code is from 2000-2001 and in an old C++ style (C with
classes).


Game controls
=============

Player 1:

    Movement:       Arrow keys.
    Main weapon:    Right Ctrl.
    Extra weapon:   Right Shift.

Player 2:

    Movement:       w, a, s, d keys
    Main weapon:    Left Shift.
    Extra weapon:   Left Ctrl.


Installation
============

Windows
-------

Just download one of the pre-compiled binaries from the web page.  These are in
zip format, containing a binary, data files and some documentation. After
uncompressing, the game is started by running kexx2.exe. There is no need to
run any installer.


Linux and OSX (and any other OS)
--------------------------------

For these platforms you need to build the game from source. See below.


Building from source
======================

Requirements
------------

Building the source code requires a C++11 compiler, such as gcc-4.8, and
depends on the following libraries:

- Boost::filesystem
- Boost::system
- Boost::program_options
- SDL 1.2
- SDL_mixer
- SDL_image


Building
--------

So far I've only tried building the source in a GNU/Linux environment.
Currently Kexx2 uses Autotools, and thus it should be fairly straightforward to
build.

    ./configure                     (check for compilation dependencies)
    make                            (compile)
    make install                    (optional)

Running `make' should produce an executable `kexx2' in the src/ sub directory,
which `make install' then copies to the system (/usr/local/bin/). Data files
are installed to /usr/local/share/kexx2. The location of these can be
configured by appending a suitable flag to the configure script.


Building the source from a git repository
-----------------------------------------

If the source code has been obtained from the git repository on GitHub an
additional step before `./configure' is needed to produce the necessary build
files. To generate these run

    autoreconf -ivf


Building for Windows
--------------------

Windows binaries can be obtained by cross compiling using the mingw compiler.
This is most easily done by setting up the build environment using MXE
(http://mxe.cc). From their intro it is as simple as

    git clone -b stable https://github.com/mxe/mxe.git
    cd mxe
    make gcc sdl sdl_mixer sdl_image 

After this it should be as simple as running ./cross-configure.sh and then
./cross-make.sh. Depending on where you setup the MXE environment you might
need to edit these to scripts to contain the correct $PATH.


