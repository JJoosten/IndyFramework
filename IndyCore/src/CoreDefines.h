// Juul Joosten 2013


#pragma once

#include <assert.h>
#include <iostream>


// UTILITY
#ifndef NULL
	#define NULL 0
#endif


// DEBUG  break define only enabled with MSC compiler using x86 (32 bit) in debug mode
#pragma warning ( disable : 4005)							// disable warning for redefinition of define for this define
#ifndef BREAKPOINT(debugMessage)
#ifdef _DEBUG
#define BREAKPOINT(debugMessage) assert(false && #debugMessage);
#ifdef _MSC_VER												// check if visual studio compiler is used
#ifndef _WIN64												// check if on 32 bit
#ifdef _DEBUG												// BREAK define only enabled in debug
#define BREAKPOINT(debugMessage) { printf(#debugMessage); \
								   __asm { int 3 }; }		// nasty but gets the job done!
#endif
#endif
#endif
#else														// ifdef NDEBUG
#define BREAKPOINT(debugMessage) { printf(#debugMessage); }
#endif
#endif
#pragma warning ( default : 4005 ) // reset warning for redefinition of define to default


// MEMORY
#ifndef MB_TO_BYTE
	#define MB_TO_BYTE(v) (v << 20)
#endif

#ifndef BYTE_TO_MB
	#define BYTE_TO_MB(v) (v >> 20)
#endif

#ifndef MB_TO_KB
	#define MB_TO_KB(v) (v << 10)
#endif

#ifndef KB_TO_MB
	#define KB_TO_MB(v) (v >> 10)
#endif

#ifndef KB_TO_BYTE
	#define KB_TO_BYTE(v) (v << 10)
#endif

#ifndef BYTE_TO_KB 
	#define BYTE_TO_KB(v) (v >> 10)
#endif

#ifndef OFFSET_OF
	#define OFFSET_OF(klass, var) (unsigned int)&(((klass*)0)->var)
#endif


// TIME
#ifndef MILISECONDS_TO_SECONDS
	#define MILISECONDS_TO_SECONDS(v) ((double)v / 1000.0)
#endif

#ifndef MILISECONDS_TO_MICROSECONDS
	#define MILISECONDS_TO_MICROSECONDS(v) ((double)v * 1000.0)
#endif

#ifndef SECONDS_TO_MILISECONDS
	#define SECONDS_TO_MILISECONDS(v) ((double)v * 1000.0)
#endif 

#ifndef SECONDS_TO_MICROSECONDS
	#define SECONDS_TO_MICROSECONDS(v) ((double)v * 1000000.0)
#endif

#ifndef MICROSECONDS_TO_SECONDS
	#define MICROSECONDS_TO_SECONDS(v) ((double)v / 1000000.0)
#endif

#ifndef MICROSECONDS_TO_MILISECONDS
	#define MICROSECONDS_TO_MILISECONDS(v) ((double)v / 1000.0)
#endif