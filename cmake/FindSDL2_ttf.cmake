# Locate SDL2_ttf library
# This module defines
# SDL2_TTF_LIBRARY, the name of the library to link against
# SDL2_TTF_FOUND, if false, do not try to link to SDL2_ttf
# SDL2_TTF_INCLUDE_DIR, where to find SDL_ttf.h
#
# This module is adapted from the FindSDL2.cmake module.

SET(SDL2_TTF_SEARCH_PATHS
	~/Library/Frameworks
	/Library/Frameworks
	/usr/local
	/usr
	/sw # Fink
	/opt/local # DarwinPorts
	/opt/csw # Blastwave
	/opt
	${SDL2_TTF_PATH}
)

FIND_PATH(SDL2_TTF_INCLUDE_DIR SDL_ttf.h
	HINTS
	$ENV{SDL2TTFDIR}
	PATH_SUFFIXES include/SDL2 include
	PATHS ${SDL2_TTF_SEARCH_PATHS}
)

if(CMAKE_SIZEOF_VOID_P EQUAL 8) 
	set(PATH_SUFFIXES lib64 lib/x64 lib)
else() 
	set(PATH_SUFFIXES lib/x86 lib)
endif() 

FIND_LIBRARY(SDL2_TTF_LIBRARY
	NAMES SDL2_ttf
	HINTS
	$ENV{SDL2TTFDIR}
	PATH_SUFFIXES ${PATH_SUFFIXES}
	PATHS ${SDL2_TTF_SEARCH_PATHS}
)

INCLUDE(FindPackageHandleStandardArgs)

FIND_PACKAGE_HANDLE_STANDARD_ARGS(SDL2_ttf REQUIRED_VARS SDL2_TTF_LIBRARY SDL2_TTF_INCLUDE_DIR) 