#pragma once
#ifndef __SLSTL_H__
#define __SLSTL_H__
//!---------------------------------------------------------------------------
//! \file "slSTL.h"
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
#include "slMacros.h"

#include <memory>
#include <algorithm>
#include <string>
#include <vector>
#include <map>

#include <sstream>
#include <iomanip>

// GCC includes
#if WIN32
//#include <hash_map>
#include <hash_set>
#else
#include <ext/hash_map>
#include <ext/hash_set>
#include <tr1/unordered_map>
#endif


#define GCC_VERSION (__GNUC__ * 10000 \
	+ __GNUC_MINOR__ * 100 \
	+ __GNUC_PATCHLEVEL__)

#if __APPLE__ && __MACH__
#else
#  if (defined(__GNUC__) )
#    if GCC_VERSION >= 40300
#      include <tr1/unordered_map>
#      include <tr1/unordered_set>
#    else
#       include <ext/hash_map>
#       include <ext/hash_set>
#    endif
#  else
#    include <hash_map>
#  endif
#endif



/// string typedef 
typedef std::string SLAString;

/// wstring typedef 
typedef std::wstring SLWString;

namespace sl	{
	//------------------------------------------------------------------------------
	SLAString& replaceString(SLAString& subject, const SLAString& search,
		const SLAString& replace);


	bool isEmptyString( const char* text );

	SL_INLINE 
		bool isEmptyString( const char* text )	{
			if((text == NULL) || (text[0] == 0))	{
				return true;
			}
			return false;
	}
}

#if WIN32
#else

//------------------------------------------------------------------------------
namespace __gnu_cxx
{ 
	/// std::string hash function 
	template<>
	struct hash< std::string >                                                       
	{                                                                                           
		size_t operator()( const std::string& x ) const                                           
		{                                                                                         
			return hash< const char* >()( x.c_str() );                                              
		}                                                                                         
	};                                                                                          
	
	/// wchar_t hash function 
	template<>
	struct hash<const wchar_t*> {
		size_t operator()(const wchar_t* s) const {
			return std::tr1::hash<const wchar_t*>()(s);
		}
	};
	
	/// std::wstring hash function 
	template<>
	struct hash<std::wstring> {
		size_t operator()(const std::wstring& s) const {
			const size_t hash_value = std::tr1::hash<std::wstring>()(s);
//			printf("hash_value %lu\n", hash_value);
			return hash_value;
			return std::tr1::hash<std::wstring>()(s);
		}
	};
	
	/// comparison structure for 2 wchar_t strings 
	struct eqWStr
	{
		bool operator()(const wchar_t* s1, const wchar_t* s2) const
		{
			return wcscmp(s1, s2) == 0;
		}
	};
	
	/// comparison structure for 2 std::wstring string objects 
	struct eqWString
	{
		bool operator()(const std::wstring& s1, const std::wstring& s2) const
		{
			return wcscmp(s1.c_str(), s2.c_str()) == 0;
		}
	};


	/// template function to allow long long hashmaps
	template<> struct hash< long long >
	{
		size_t operator()( const long long& x ) const
		{
			return static_cast<size_t>( x );
		}
	};


}          

#endif // WIN32


#endif // __SLSTL_H__
