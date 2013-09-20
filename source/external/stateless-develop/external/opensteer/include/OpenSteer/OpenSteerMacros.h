#ifndef __OPENSTEERMACROS_H__
#define __OPENSTEERMACROS_H__

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


//-----------------------------------------------------------------------------
//! some configuration macros
//! 

// usage of profiler iprof disabled right now
#ifdef WIN32
#  define OS_HAVE_PROFILE 0
#elif __APPLE__
#  define OS_HAVE_PROFILE 0
#else
#  define OS_HAVE_PROFILE 0
#endif

// for embedded use we don not want opensteer to use glut
#ifndef OS_HAVE_GLUT
#  define OS_HAVE_GLUT 0
#endif

// for embedded use we don not want opensteer to use glut
#ifndef OS_HAVE_BULLETLINEARMATH
#  define OS_HAVE_BULLETLINEARMATH 1
#endif

#ifndef OPENSTEER_Z_ISUP
#  define OPENSTEER_Z_ISUP 1
#endif

//-----------------------------------------------------------------------------
// memory
//-----------------------------------------------------------------------------
#ifdef _DEBUG
#include <crtdbg.h>
#define OS_NEW new ( _NORMAL_BLOCK, __FILE__, __LINE__ )
#else
#define OS_NEW new
#endif

#define OS_DELETE delete

#define OS_MALLOC malloc
#define OS_FREE free

#define OS_SAFE_DELETE( p ) { if( 0 != p ) { OS_DELETE p; p = 0; } }
#define OS_SAFE_DELETE_ARRAY( p ) { if( 0 != p ) { OS_DELETE[] p; p = 0; } }
#define OS_SAFE_FREE( p ) { if( 0 != p ) { OS_FREE( p ); p = 0; } }

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// defines to support compiler features regarding class and interface derivation
//-----------------------------------------------------------------------------
// classes
//-----------------------------------------------------------------------------
#ifndef _OS_HAVE_MS_EXTENSIONS
#  if defined(_MSC_VER)
#    define _OS_HAVE_MS_EXTENSIONS 1
#  else
#    define _OS_HAVE_MS_EXTENSIONS 0
#  endif
#endif	//_OS_HAVE_MS_EXTENSIONS

// The following macros are used together with virtual functions.
//	- OS_OVERRIDE: means that the function MUST override a base class function
//	- OS_SEALED: this function cannot be overridden in derived classes.
//	- OS_ABSTRACT: this function is abstract and must be overridden in derived classes
//	- OS_VIRTUAL_DECLARE: This virtual function is declared here the first time; but not abstract.
//						  Just to make it easier to see which functions are declared in a class.

#if _OS_HAVE_MS_EXTENSIONS
#	define OS_OVERRIDE override
#	define OS_SEALED sealed
#	define OS_ABSTRACT abstract
#   define OS_VIRTUAL_DECLARE
#else	//_SL_HAVE_MS_EXTENSIONS
#	define OS_OVERRIDE
#	define OS_SEALED
#	define OS_ABSTRACT = 0
#   define OS_VIRTUAL_DECLARE
#endif	//_OS_HAVE_MS_EXTENSIONS


#define OS_DECLARE_BASE( classname ) \
protected: \
	typedef classname BaseClass; \
public: \


#define OS_DECLARE_CLASSNAME virtual const char* getClassName( void ) const OS_ABSTRACT;

#define OS_IMPLEMENT_CLASSNAME( className ) virtual const char* getClassName( void ) const { return #className; }


// note: synthesize idea taken from cocos2d-x

/** OS_PROPERTY_READONLY is used to declare a protected variable.
 We can use getter to read the variable.
 @param varType : the type of variable.
 @param varName : variable name.
 @param funName : "get + funName" is the name of the getter.
 @warning : The getter is a public virtual function, you should rewrite it first.
 The variables and methods declared after OS_PROPERTY_READONLY are all public.
 If you need protected or private, please declare.
 */
#define OS_PROPERTY_READONLY(varType, varName, funName)\
protected: varType varName;\
public: virtual varType get##funName(void);

#define OS_PROPERTY_READONLY_PASS_BY_REF(varType, varName, funName)\
protected: varType varName;\
public: virtual const varType& get##funName(void);

/** OS_PROPERTY is used to declare a protected variable.
 We can use getter to read the variable, and use the setter to change the variable.
 @param varType : the type of variable.
 @param varName : variable name.
 @param funName : "get + funName" is the name of the getter.
 "set + funName" is the name of the setter.
 @warning : The getter and setter are public virtual functions, you should rewrite them first.
 The variables and methods declared after OS_PROPERTY are all public.
 If you need protected or private, please declare.
 */
#define OS_PROPERTY(varType, varName, funName)\
protected: varType varName;\
public: virtual varType get##funName(void);\
public: virtual void set##funName(varType var);

#define OS_PROPERTY_PASS_BY_REF(varType, varName, funName)\
protected: varType varName;\
public: virtual const varType& get##funName(void);\
public: virtual void set##funName(const varType& var);

/** OS_SYNTHESIZE_READONLY is used to declare a protected variable.
 We can use getter to read the variable.
 @param varType : the type of variable.
 @param varName : variable name.
 @param funName : "get + funName" is the name of the getter.
 @warning : The getter is a public inline function.
 The variables and methods declared after OS_SYNTHESIZE_READONLY are all public.
 If you need protected or private, please declare.
 */
#define OS_SYNTHESIZE_READONLY(varType, varName, funName)\
protected: varType varName;\
public: virtual varType get##funName(void) const { return varName; }

#define OS_SYNTHESIZE_READONLY_PASS_BY_REF(varType, varName, funName)\
protected: varType varName;\
public: virtual const varType& get##funName(void) const { return varName; }

/** OS_SYNTHESIZE is used to declare a protected variable.
 We can use getter to read the variable, and use the setter to change the variable.
 @param varType : the type of variable.
 @param varName : variable name.
 @param funName : "get + funName" is the name of the getter.
 "set + funName" is the name of the setter.
 @warning : The getter and setter are public  inline functions.
 The variables and methods declared after OS_SYNTHESIZE are all public.
 If you need protected or private, please declare.
 */
#define OS_SYNTHESIZE(varType, varName, funName)\
protected: varType varName;\
public: virtual varType get##funName(void) const { return varName; }\
public: virtual void set##funName(varType var){ varName = var; }

#define OS_SYNTHESIZE_PASS_BY_REF(varType, varName, funName)\
protected: varType varName;\
public: virtual const varType& get##funName(void) const { return varName; }\
public: virtual void set##funName(const varType& var){ varName = var; }




#endif //!  __OPENSTEERMACROS_H__
