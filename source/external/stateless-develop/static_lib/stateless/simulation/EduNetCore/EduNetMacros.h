#ifndef __EDUNETMACROS_H__
#define __EDUNETMACROS_H__

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


#if defined(_MSC_VER)
#define EF_FORCEINLINE __forceinline
#else
#define EF_FORCEINLINE inline
#endif

//-----------------------------------------------------------------------------
// classes
//-----------------------------------------------------------------------------
#define ET_ABSTRACT = 0

#define ET_DECLARE_BASE( classname ) \
protected: \
	typedef classname BaseClass; \
public: \

//-----------------------------------------------------------------------------
//! implement an empty private copy constructor and a private assignment
#define ET_IMPLEMENT_CLASS_NO_COPY( classname ) private:\
        classname( const classname& );\
        classname& operator=( const classname& );

//-----------------------------------------------------------------------------
//  macros spit out 'clickable' file and line number
//-----------------------------------------------------------------------------
// with line number
#ifndef ET_STRING
# define ET_STRING2(x) #x
# define ET_STRING(x) ET_STRING2(x)
#endif //  KK_STRING

// sample 1
//#pragma message (__FILE__ "(" ET_STRING(__LINE__) "): USE Macros ")
#define ET_SOURCE_MESSAGE __FILE__ "(" ET_STRING(__LINE__) "):"
// sample 2
//#pragma message( ET_SOURCE_MESSAGE "Warning: Unicode disabled! Check your project settings!")

// Macro to supress warning that parameters aren't used.
#define ET_UNUSED_PARAMETER(expr) (void)expr




#ifndef linux
#ifdef _DEBUG
#define ET_DEBUG 1
#endif
#else
#ifndef NDEBUG
#define ET_DEBUG 1
#endif
#endif

#ifdef ET_DEBUG
#define ET_ASSERT( expr ) assert(expr);
#else
  #ifdef WIN32
    #define ET_ASSERT( expr ) __noop;
  #else
    #define ET_ASSERT( expr )
  #endif
#endif


// no profiling right now
#define ET_PROFILE(profileTag)

#endif // __EDUNETMACROS_H__
