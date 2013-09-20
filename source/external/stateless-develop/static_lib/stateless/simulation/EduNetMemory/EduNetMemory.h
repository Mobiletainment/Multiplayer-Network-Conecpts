#ifndef __EDUNETMEMORY_H__
#define __EDUNETMEMORY_H__

//-----------------------------------------------------------------------------
// Copyright (c) 2011, Jan Fietz, Cyrus Preuss
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without modification,
// are permitted provided that the following conditions are met:
//
// * Redistributions of source code must retain the above copyright notice,
//   this list of conditions and the following disclaimer.
// * Redistributions in binary form must reproduce the above copyright notice,
//   this list of conditions and the following disclaimer in the documentation
//   and/or other materials provided with the distribution.
// * Neither the name of EduNetGames nor the names of its contributors
//   may be used to endorse or promote products derived from this software
//   without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
// ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
// WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
// IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
// INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
// (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
// LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
// ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
// EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//-----------------------------------------------------------------------------
#include "EduNetCore/EduNetConfig.h"
#include "EduNetCore/EduNetMacros.h"

// select a certain memory manager
#define ET_HAVE_MEMORYMANAGER_FLUID 0
//-----------------------------------------------------------------------------

// Standard includes MUST come first
// to get propper working memory managers
#include <stdio.h>
#include <stdlib.h>
#include <iostream>

#if ET_HAVE_MEMORYMANAGER_FLUID
# define ET_USE_FLUIDMEMORY_MGR // check for memory leaks...
#else
# ifdef  ET_USE_FLUIDMEMORY_MGR
#   undef ET_USE_FLUIDMEMORY_MGR
# endif
#endif


// NOTE: crtdbg is not working with fluid memory manager !!!
#ifdef  ET_USE_FLUIDMEMORY_MGR
  # pragma message ( "-- using fluid studios memory manager" )
  # pragma warning ( push )
  # pragma warning ( disable : 4290 ) // C++ exception specification ignored except to indicate a function is not __declspec(nothrow)
  # define __USEMMGR_MAKROS_ // how to make this work?
  # if FORCE_NO_FLUID_MEMORY_MAKROS
  #   undef __USEMMGR_MAKROS_
  # endif
# include "Fluid/mmgr.h"    // mmgr.h MUST come next
# pragma warning ( pop )
#endif // ET_USE_FLUIDMEMORY_MGR


#ifdef  ET_USE_FLUIDMEMORY_MGR // fluid memory manager is enabled
  #ifndef __USEMMGR_MAKROS_
  # pragma message ( "-- using fluid studios memory manager macros" )
    # define ET_MALLOC mmgr_malloc
    # define ET_CALLOC mmgr_calloc
    # define ET_FREE mmgr_free
    # define ET_MEMSET memset
    # define ET_MEMCPY memcpy
    # define ET_MEMMOVE memmove
    # define ET_REALLOC mmgr_realloc
  #else
  # pragma message ( "-- not using fluid studios memory manager macros" )
    # define ET_MALLOC malloc
    # define ET_CALLOC calloc
    # define ET_FREE free
    # define ET_MEMSET memset
    # define ET_MEMCPY memcpy
    # define ET_MEMMOVE memmove
    # define ET_REALLOC realloc
  #endif
#else
	#ifdef _DEBUG
	#include <crtdbg.h>
	#endif

    # define ET_MALLOC malloc
    # define ET_CALLOC calloc
    # define ET_FREE free
    # define ET_MEMSET memset
    # define ET_MEMCPY memcpy
    # define ET_MEMMOVE memmove
    # define ET_REALLOC realloc
#endif
#ifdef _MSC_VER
	#if ET_DEBUG
		#define ET_NEW new ( _NORMAL_BLOCK, __FILE__, __LINE__ )
	#else
		#define ET_NEW new
	#endif
#else
	#define ET_NEW new
#endif

#define ET_DELETE delete

#define ET_SAFE_DELETE( p ) { if( 0 != p ) { ET_DELETE p; p = 0; } }
#define ET_SAFE_DELETE_ARRAY( p ) { if( 0 != p ) { ET_DELETE[] p; p = 0; } }
#define ET_SAFE_FREE( p ) { if( 0 != p ) { ET_FREE( p ); p = 0; } }

//-----------------------------------------------------------------------------
class EduNetMemTracker
{
public:
	static void sdmInit( void );
	static void sdmShutdown( void );
};

#endif // __EDUNETMEMORY_H__
