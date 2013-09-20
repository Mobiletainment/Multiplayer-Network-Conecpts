#pragma once
#ifndef __SLMACROS_H__
#define __SLMACROS_H__

//!---------------------------------------------------------------------------
//! \file slMacros.h
//! \brief <brief-doc-todo>  
//! \details <details-doc-todo>
//! \author Cyrus Preuss
//!
//! This file is part of the Stateless Games SDK
//! Usage of the Stateless Games SDK is subject to an appropriate license agreement.
//!
//! Copyright (c) 2008-2013 Cyrus Preuss. All rights reserved.
//!----------------------------------------------------------------------------


#include "slConfig.h"

//-----------------------------------------------------------------------------
// define a convenient DEBUG macro for different OSes

//-----------------------------------------------------------------------------
// define a portable debug macro
// note: for GCC one has to define
// GCC_PREPROCESSOR_DEFINITIONS = DEBUG
//-----------------------------------------------------------------------------
#if defined (DEBUG) || defined (_DEBUG)
#  define SL_DEBUG 1
#else
#  if defined (NDEBUG)
#    define SL_DEBUG 0
#  else
#  endif
#endif

#ifndef SL_DEBUG
#  define SL_DEBUG 0
#endif

//-----------------------------------------------------------------------------
// memory allocation defines
#define SL_NEW new
#define SL_DELETE delete

#define SL_MALLOC malloc
#define SL_FREE free

#define SL_SAFE_DELETE( p ) { if( 0 != p ) { SL_DELETE p; p = 0; } }
#define SL_SAFE_DELETE_ARRAY( p ) { if( 0 != p ) { SL_DELETE[] p; p = 0; } }
#define SL_SAFE_FREE( p ) { if( 0 != p ) { SL_FREE( p ); p = 0; } }

//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Define _FILE_AND_LINE_ to "",0 if you want to strip out file and line info for memory tracking from the EXE
#ifndef SL_FILE_AND_LINE
#  if SL_DEBUG
#    define SL_FILE_AND_LINE __FILE__,__LINE__
#  else
#    define SL_FILE_AND_LINE "",0
#  endif
#endif
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// c function declaration define
// define __cdecl for non-Microsoft compilers
#if ( !defined(_MSC_VER) && !defined(__cdecl) )
#  define __cdecl
#endif

#define SL_CRTDECL __cdecl
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// defines to support compiler features regarding class and interface derivation
//-----------------------------------------------------------------------------
// classes
//-----------------------------------------------------------------------------
#ifndef _SL_HAVE_MS_EXTENSIONS
#  if defined(_MSC_VER)
#    define _SL_HAVE_MS_EXTENSIONS 1
#  else
#    define _SL_HAVE_MS_EXTENSIONS 0
#  endif
#endif	//_SL_HAVE_MS_EXTENSIONS

// The following macros are used together with virtual functions.
//	- SL_OVERRIDE: means that the function MUST override a base class function
//	- SL_SEALED: this function cannot be overridden in derived classes.
//	- SL_ABSTRACT: this function is abstract and must be overridden in derived classes
//	- SL_VIRTUAL_DECLARE: This virtual function is declared here the first time; but not abstract.
//						  Just to make it easier to see which functions are declared in a class.

#if _SL_HAVE_MS_EXTENSIONS
#	define SL_OVERRIDE override
#	define SL_SEALED sealed
#	define SL_ABSTRACT abstract
#   define SL_VIRTUAL_DECLARE
#else	//_SL_HAVE_MS_EXTENSIONS
#	define SL_OVERRIDE
#	define SL_SEALED
#	define SL_ABSTRACT = 0
#   define SL_VIRTUAL_DECLARE
#endif	//_SL_HAVE_MS_EXTENSIONS
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// include and support assert
// note: for final distribution we might decide to undef assert
#include <assert.h>
#define SL_ASSERT assert
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// a handy count macro
#define SL_COUNT_OF(x) ((sizeof(x)/sizeof(0[x])) / ((size_t)(!(sizeof(x) % sizeof(0[x])))))
// -1 as the string ends with '0'
#define SL_COUNT_OF_CHAR(x) (((sizeof(x)/sizeof(0[x])) / ((size_t)(!(sizeof(x) % sizeof(0[x]))))-1))


//-----------------------------------------------------------------------------
// a few macros dealing with common floating point problems
#define SL_FLOAT_EPSILON 0.0001f
#define SL_IS_FLOAT_EQUAL(f1, f2)   (fabsf((f1)-(f2)) < SL_FLOAT_EPSILON)

#define SL_DECIMAL_EPSILON 0.1f
#define SL_IS_DECIMAL_EQUAL(f1, f2)   (fabsf((f1)-(f2)) < SL_DECIMAL_EPSILON)

#define SL_FABS_DIFF(f1, f2)   (fabsf((f1)-(f2))) 

//-----------------------------------------------------------------------------
// inline macros
#define SL_INLINE inline

#if ( defined(_MSC_VER) || defined(_WIN32) || defined(WIN32) )
#  define SL_FORCEINLINE __forceinline
#else
#  define SL_FORCEINLINE inline
#endif


//-----------------------------------------------------------------------------
// a handy macro to have the base class typedef 
// reducing a lot of pain and errors while changing class hirarchies
#ifndef SL_DECLARE_BASE

#define SL_DECLARE_BASE( classname ) \
protected: \
typedef classname SLBaseClass; \
public: \


#endif

//-----------------------------------------------------------------------------
//! declare an empty private copy constructor and a private assignment
#define SL_DECLARE_CLASS_NO_COPY( classname ) \
private:\
	classname( const classname& );\
	classname& operator=( const classname& );

//-----------------------------------------------------------------------------
// implement a copy constructor and a assignment
#define SL_IMPLEMENT_CLASS_COPY( classname ) \
classname( const classname& other ){ copyFrom( other ); } \
classname& operator=( const classname& other ){ copyFrom( other ); return *this;} \



//-----------------------------------------------------------------------------
// a macro to have the possibility to deprecate code
#ifdef __GNUC__
#define DEPRECATED(func) func __attribute__ ((deprecated))
#elif defined(_MSC_VER)
#define DEPRECATED(func) __declspec(deprecated) func
#else
#pragma message("WARNING: You need to implement DEPRECATED for this compiler")
#define DEPRECATED(func) func
#endif

//-----------------------------------------------------------------------------
// a macro to declare and implement a dynamically allocated singleton
#define SL_DECLARE_SINGLETON( ClassName ) \
	public: \
	static ClassName* accessInstance( void ) { return ClassName::_instancePtr; } \
	static void createInstance( void ) { if( ClassName::accessInstance() == nullptr ) ClassName::_instance = SL_NEW ClassName();} \
	static void destroyInstance( void ) { SL_SAFE_DELETE(ClassName::_instance);} \
	private: \
	static ClassName* _instancePtr;

#define SL_IMPLEMENT_SINGLETON( ClassName ) ClassName* ClassName::ms_pInstance = NULL;


//-----------------------------------------------------------------------------
// log related macros
#ifdef WIN32
#  define SL_HAS_NSLOG 0
#else
#if TARGET_IPHONE_SIMULATOR
#  define SL_HAS_NSLOG 1
#else
#  define SL_HAS_NSLOG 0

#if SL_HAS_NSLOG
#  define SL_NSLOG(xx, ...)  NSLog(@"" xx, ##__VA_ARGS__)
#else
#  define SL_NSLOG(xx, ...)  //((void)0)
#endif

#endif


#endif


#define SL_DECLARE_ABSTRACT_CLASSNAME \
	virtual const char* getClassName( void ) const SL_ABSTRACT;

#define SL_IMPLEMENT_CLASSNAME( className ) \
	static const char* staticClassName( void ) { return #className; }  \
	virtual const char* getClassName( void ) const { return className::staticClassName(); }



#define SL_SET_FUNC( x, y, prefix )       SL_FORCEINLINE void set##y( x t )  { _##y = t; };
#define SL_GET_FUNC( x, y, prefix )       SL_FORCEINLINE x get##y() const { return _##y; };
#define SL_GET_SET_FUNC( x, y, prefix )   SL_GET_FUNC( x, y, prefix ) SL_SET_FUNC( x, y, prefix )

// note: synthesize idea taken from cocos2d-x

#define SL_SAFE_RELEASE(p)            do { if(p) { (p)->release(); } } while(0)
#define SL_SAFE_RELEASE_NULL(p)        do { if(p) { (p)->release(); (p) = nullptr; } } while(0)
#define SL_SAFE_RETAIN(p)            do { if(p) { (p)->retain(); } } while(0)


/** SL_PROPERTY_READONLY is used to declare a protected variable.
 We can use getter to read the variable.
 @param varType : the type of variable.
 @param varName : variable name.
 @param funName : "get + funName" is the name of the getter.
 @warning : The getter is a public virtual function, you should rewrite it first.
 The variables and methods declared after SL_PROPERTY_READONLY are all public.
 If you need protected or private, please declare.
 */
#define SL_PROPERTY_READONLY(varType, varName, funName)\
protected: varType varName;\
public: virtual varType get##funName(void);

#define SL_PROPERTY_READONLY_PASS_BY_REF(varType, varName, funName)\
protected: varType varName;\
public: virtual const varType& get##funName(void);

/** SL_PROPERTY is used to declare a protected variable.
 We can use getter to read the variable, and use the setter to change the variable.
 @param varType : the type of variable.
 @param varName : variable name.
 @param funName : "get + funName" is the name of the getter.
 "set + funName" is the name of the setter.
 @warning : The getter and setter are public virtual functions, you should rewrite them first.
 The variables and methods declared after SL_PROPERTY are all public.
 If you need protected or private, please declare.
 */
#define SL_PROPERTY(varType, varName, funName)\
protected: varType varName;\
public: virtual varType get##funName(void);\
public: virtual void set##funName(varType var);

#define SL_PROPERTY_PASS_BY_REF(varType, varName, funName)\
protected: varType varName;\
public: virtual const varType& get##funName(void);\
public: virtual void set##funName(const varType& var);

/** SL_SYNTHESIZE_READONLY is used to declare a protected variable.
 We can use getter to read the variable.
 @param varType : the type of variable.
 @param varName : variable name.
 @param funName : "get + funName" is the name of the getter.
 @warning : The getter is a public inline function.
 The variables and methods declared after SL_SYNTHESIZE_READONLY are all public.
 If you need protected or private, please declare.
 */
#define SL_SYNTHESIZE_READONLY(varType, varName, funName)\
protected: varType varName;\
public: virtual varType get##funName(void) const { return varName; }

#define SL_SYNTHESIZE_READONLY_PASS_BY_REF(varType, varName, funName)\
private: varType varName;\
public: virtual const varType& get##funName(void) const { return varName; }

#define SL_SYNTHESIZE_IS(varType, varName, funNameGet, funNameSet)\
protected: varType varName;\
public: virtual varType is##funNameGet(void) const { return varName; }\
public: virtual void set##funNameSet(varType var){ varName = var; }

/** SL_SYNTHESIZE is used to declare a protected variable.
 We can use getter to read the variable, and use the setter to change the variable.
 @param varType : the type of variable.
 @param varName : variable name.
 @param funName : "get + funName" is the name of the getter.
 "set + funName" is the name of the setter.
 @warning : The getter and setter are public  inline functions.
 The variables and methods declared after SL_SYNTHESIZE are all public.
 If you need protected or private, please declare.
 */

#define SL_SYNTHESIZE_STATIC(varType, varName, funName)\
protected: static varType varName;\
public: static varType get##funName(void) { return varName; }\
public: static void set##funName(varType var){ varName = var; }

#define SL_SYNTHESIZE(varType, varName, funName)\
protected: varType varName;\
public: virtual varType get##funName(void) const { return varName; }\
public: virtual void set##funName(varType var){ varName = var; }

#define SL_SYNTHESIZE_PASS_BY_REF(varType, varName, funName)\
protected: varType varName;\
public: virtual const varType& get##funName(void) const { return varName; }\
public: virtual void set##funName(const varType& var){ varName = var; }

#define SL_SYNTHESIZE_GET_BY_REF(varType, varName, funName)\
protected: varType varName;\
public: virtual varType& get##funName(void) { return varName; }\

#define SL_SYNTHESIZE_GET(varType, varName, funName)\
protected: varType varName;\
public: virtual varType get##funName(void) const { return varName; }\

#define SL_SYNTHESIZE_RETAIN(varType, varName, funName)    \
private: varType varName; \
public: virtual varType get##funName(void) const { return varName; } \
public: virtual void set##funName(varType var)   \
{ \
    if (varName != var) \
    { \
        SL_SAFE_RETAIN(var); \
        SL_SAFE_RELEASE(varName); \
        varName = var; \
    } \
} 



// special macros for special coders
// included here to have the option to customize log behaviour
#include "slMacrosForCoders.h"


#ifdef WIN32
#else
#define SL_NSLOG_ALWAYS(xx, ...)  NSLog(@"" xx, ##__VA_ARGS__)
#endif







#endif // __SLMACROS_H__
