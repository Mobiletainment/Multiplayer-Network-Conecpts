#pragma once
#ifndef __OPENSTEERTYPES_H__
#define __OPENSTEERTYPES_H__

//-----------------------------------------------------------------------------
//! Copyright (c) 2011, Jan Fietz, Cyrus Preuss
//! All rights reserved.
//! 
//! Redistribution and use in source and binary forms, with or without modification, 
//! are permitted provided that the following conditions are met:
//! 
//! * Redistributions of source code must retain the above copyright notice, 
//!   this list of conditions and the following disclaimer.
//! * Redistributions in binary form must reproduce the above copyright notice, 
//!   this list of conditions and the following disclaimer in the documentation 
//!   and/or other materials provided with the distribution.
//! * Neither the name of EduNetGames nor the names of its contributors
//!   may be used to endorse or promote products derived from this software
//!   without specific prior written permission.
//! 
//! THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
//! ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED 
//! WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. 
//! IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, 
//! INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES 
//! (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; 
//! LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON 
//! ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT 
//! (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, 
//! EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//-----------------------------------------------------------------------------
#ifdef WIN32
#ifdef _DEBUG
#include <crtdbg.h>
#endif
#endif

#ifndef __OSNATIVE_TYPES_H__
#define __OSNATIVE_TYPES_H__

#if (defined(__GNUC__)  || defined(__GCCXML__) || defined(__SNC__) || defined(__APPLE__) )
#include <stdint.h>
#endif

#if !defined(_STDINT_H) && !defined(_SN_STDINT_H) && !defined(_SYS_STDINT_H_) && !defined(_STDINT_H_)
typedef unsigned char         uint8_t;
typedef unsigned short        uint16_t;
typedef unsigned int          uint32_t;
typedef signed char         int8_t;
typedef signed short        int16_t;
typedef signed int          int32_t;
#if defined(_MSC_VER) && _MSC_VER < 1300
typedef unsigned __int64    uint64_t;
typedef signed __int64   	int64_t;
#else
typedef unsigned long long int    uint64_t;
typedef signed long long   	int64_t;
#endif
#endif


#endif //! __OSNATIVE_TYPES_H__


#if 0
//-----------------------------------------------------------------------------
namespace OpenSteer {

	typedef uint64_t NetworkId; 
	typedef uint64_t EntityClassId; 

	static const EntityClassId g_clasId_Unknown(0);	

}


#define OS_CID_UNKNOWN OpenSteer::g_clasId_Unknown
#endif

///The btScalar type abstracts floating point numbers, to easily switch between double and single floating point precision.
#if defined(OS_USE_DOUBLE_PRECISION)
typedef double osScalar;
//this number could be bigger in double precision
#define OS_LARGE_FLOAT 1e30
#define OS_SCALAR(x) (x) 
#else
typedef float osScalar;
//keep OS_LARGE_FLOAT * OS_LARGE_FLOAT < FLT_MAX
#define OS_LARGE_FLOAT 1e18f
#define OS_SCALAR(x) (x ## f) 
#endif


#endif //!  __OPENSTEERTYPES_H__
