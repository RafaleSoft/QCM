// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#ifdef WIN32
	#include "targetver.h"

	#include <algorithm>
	#define UNLINK _unlink

	#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
	// Windows Header Files:
	#define max max
	#define min min
	#include <windows.h>
	#include "process.h"

	#include "tif_config.h"
#else
	// Linux and/or __arm__	: for raspberry / linux
#include <algorithm>
	//#ifndef max
	//	#define max(a,b)            (((a) > (b)) ? (a) : (b))
	//#endif

	//#ifndef min
	//	#define min(a,b)            (((a) < (b)) ? (a) : (b))
	//#endif

	#include <sys/types.h>	
	#include <sys/stat.h>
	#include <unistd.h>
	#include <cstddef>
	#include <cstring>
	#define UNLINK unlink
#endif

