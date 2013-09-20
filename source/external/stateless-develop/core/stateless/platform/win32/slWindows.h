//!---------------------------------------------------------------------------
//! \file "slWindows.h"
//! \brief <brief-doc-todo>  
//! \details <details-doc-todo>
//! \author Cyrus Preuss
//!
//! This file is part of the Stateless Games SDK
//! Usage of the Stateless Games SDK is subject to an appropriate license agreement.
//!
//! Copyright (c) 2012-2013 Cyrus Preuss. All rights reserved.
//!----------------------------------------------------------------------------


#pragma once
#ifndef __SLWINDOWS_H__
#define __SLWINDOWS_H__

//<license>
//-----------------------------------------------------------------------------
// Copyright (c) 2005, Cyrus Preuss
// All rights reserved.
//-----------------------------------------------------------------------------
//</license>

//-----------------------------------------------------------------------------
#if defined(WIN32) || defined(_WIN32)
#   if SL_SHOW_CONFIG
#      pragma message( " - windows build" )
#   endif
#   define __SL_WINDOWS_SPEEDUPS__ 1
// some windows speedups
#	if __SL_WINDOWS_SPEEDUPS__

#		define WIN32_LEAN_AND_MEAN
#		define WIN32_EXTRA_LEAN

#		define NOGDICAPMASKS
#		define OEMRESOURCE
#		define NOATOM
#ifndef __SL_WINDOWS_USECLIPBOARD__
#		define NOCLIPBOARD
#endif
//#		define NOCTLMGR // windows utility functions like DialogBox,
#		define NOMEMMGR
#		define NOMETAFILE
#		define NOOPENFILE
#		define NOSERVICE
#		define NOSOUND
#		define NOCOMM
#		define NOKANJI
#		define NOHELP
#		define NOPROFILER
#		define NODEFERWINDOWPOS
#		define NOMCX
// #define NONLS
// #define NOMB

#	endif // __SL_WINDOWS_SPEEDUPS__


# ifndef _WIN32_WINNT
#  define _WIN32_WINNT 0x0502 // Windows XP SP1
# endif
//# endif


#if _MSC_VER >= 1400

#endif

#  pragma message( "--------------------------" )
#  pragma message( " Windows Configuration: including windows" )
#  pragma message( "--------------------------" )

#include <windows.h>
#include <shellapi.h>



// we do not want pollution trough windows with strange macros
#ifdef max
#undef max
#endif

#ifdef min
#undef min
#endif


#endif  // WIN32

#include "slWindowsMacros.h"

#endif  // __SLWINDOWS_H__
